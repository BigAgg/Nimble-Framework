#include "app/app.h"
#include "app/eventmanager.h"
#include <raylib.h>
#include <filesystem>
#include <fstream>

#include <imgui.h>
#include "imgui/ImGui_windowcontrol.h"
#include <rlImGui.h>
#include <rlgl.h>


namespace fs = std::filesystem;

/*
* App:
*   - Singleton
*   - Storing window management
*   - Storing structure
*/
class App {
public:
  static App& Get () {
    static App instance;
    return instance;
  }

  struct {
    int w = 1080;
    int h = 720;
    int fps = 30;
    int device = 0;
    int posx = 0;
    int posy = 0;
    bool borderless = false;
    bool vsync = false;
    bool maximized = false;
  } settings;

  struct {
    bool hasDroppedFile = false;
    std::vector<std::string> files;

    std::string getFirstFile(std::string endswith = "") {
      if (!hasDroppedFile || files.size() == 0) {
        hasDroppedFile = false;
        return "";
      }
      hasDroppedFile = false;
      if (endswith == "") {
        std::string file = files[0];
        files.clear();
        return file;
      }
      for (std::string file : files) {
        if (file.ends_with(endswith)) {
          files.clear();
          return file;
        }
      }
      files.clear();
      return "";
    }

    std::vector<std::string> getFiles(std::string endswith = "") {
      std::vector<std::string> outfiles;
      if (!hasDroppedFile)
        return outfiles;
      hasDroppedFile = false;
      for (std::string file : files) {
        if (file.ends_with(endswith)) {
          outfiles.push_back(file);
        }
      }
      files.clear();
      return outfiles;
    }
  } dragDrop;

  bool initialized = false;
  bool close = true;
  std::string name = "App";
  std::string workingdir = "App/";

  // Draw handles
  bool drawing = false;

  // Deleting copy/move
  App(const App &) = delete;
  App &operator=(const App &) = delete;
  App(App &&) = delete;
  App &operator=(App &&) = delete;

private:
  App() = default;
  ~App() = default;
};

static void HandleDropfiles () {
  auto &app = App::Get();
  if (IsFileDropped ()) {
		if (!IsWindowFocused())
			SetWindowFocused();
		app.dragDrop.files.clear();
		FilePathList dropped = LoadDroppedFiles();
		app.dragDrop.hasDroppedFile = true;
		if (dropped.count > 0) {
			for (unsigned int i = 0; i < dropped.count; i++) {
				app.dragDrop.files.push_back(dropped.paths[i]);
			}
		}
		UnloadDroppedFiles(dropped);
  }
}

static void ClearDropfiles () {
  auto &app = App::Get();
  app.dragDrop.files.clear();
  app.dragDrop.hasDroppedFile = false;
}

// Initializing window and generating basic structure
void app::Init (int width, int height, const std::string& name) {
  auto &app = App::Get();
  if (app.initialized)
    return;
  app.name = name;
  // Checking for paths
  if (!fs::exists(app.workingdir))
    fs::create_directories(app.workingdir);
  // Load settings if available
  if (fs::exists (app.workingdir + "settings.bin")) {
    std::ifstream file;
    file.open(app.workingdir + "settings.bin", std::ios::binary);
    if (file) {
      file.read((char *)&app.settings, sizeof(app.settings));
    }
  } else {
    app.settings.w = width;
    app.settings.h = height;
  }
  // Initialize Raylib (Window Management)
  SetConfigFlags(
      FLAG_WINDOW_RESIZABLE |
      FLAG_VSYNC_HINT |
      FLAG_MSAA_4X_HINT);
  InitWindow(app.settings.w, app.settings.h, name.c_str());
  SetExitKey(KEY_NULL);
  SetTargetFPS(app.settings.fps);
  if (!app.settings.vsync) {
    ClearWindowState(FLAG_VSYNC_HINT);
  }
  SetWindowMonitor(app.settings.device);
  if (app.settings.posy > 4)
    SetWindowPosition(app.settings.posx, app.settings.posy);
  else {
    const int device = GetCurrentMonitor();
    const int width = GetMonitorWidth(device);
    const int height = GetMonitorHeight(device);
    SetWindowPosition(
      width / 2 - app.settings.w / 2,
      height / 2 - app.settings.h / 2
    );
  }
  if (app.settings.maximized)
    MaximizeWindow();
  InitAudioDevice();
  app.close = false;
  app.initialized = true;
  rlImGuiSetup(false);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  auto &wc = WindowControl::Get();
  wc.LoadSettings();
}

void app::Close () {
  auto &app = App::Get();
  app.close = true;
  if (!app.initialized)
    return;
  // Check if folder exists
  if (!fs::exists(app.workingdir))
    fs::create_directories(app.workingdir);
  // Retrieve current app state
  app.settings.device = GetCurrentMonitor();
  if (IsWindowMinimized ()) {
    RestoreWindow();
  }
  app.settings.maximized = IsWindowMaximized();
  if (app.settings.maximized)
    RestoreWindow();
  Vector2 pos = GetWindowPosition();
  app.settings.posx = static_cast<int>(pos.x);
  app.settings.posy = static_cast<int>(pos.y);
  app.settings.w = GetScreenWidth();
  app.settings.h = GetScreenHeight();
  app.settings.vsync = IsWindowState(FLAG_VSYNC_HINT);
  // Save app settings
  std::ofstream file;
  file.open(app.workingdir + "settings.bin", std::ios::binary);
  if (file) {
    file.write((char *)&app.settings, sizeof(app.settings));
  }
  // Closing window control
  auto &wc = WindowControl::Get();
  wc.SaveSettings();
  CloseWindow();
}

void app::BeginDrawing () {
  HandleDropfiles();
  auto &app = App::Get();
  if (!app.initialized || app.drawing)
    return;
  app.drawing = true;
  ::BeginDrawing();
  ::ClearBackground(BLACK);
  rlDrawRenderBatchActive();
  rlImGuiBegin();
  // Setting up doc host functionality
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                  ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_HorizontalScrollbar;

  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

  ImGui::Begin("DockSpaceHost", nullptr, window_flags);
  ImGui::PopStyleVar(2);

  // Important: creates the docking node
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0, 0));
  ImGui::End();
  // Drawing all windows
  auto &wc = WindowControl::Get();
  wc.DrawWindows ();
}

void app::EndDrawing () {
  auto &app = App::Get();
  if (!app.drawing)
    return;
  app.drawing = false;
  rlImGuiEnd();
  ::EndDrawing();
  auto &em = EventRegistry::Get();
  em.TriggerEvents();
  ClearDropfiles();
}

void app::SetIcon (const std::string& filepath) {
  SetWindowIcon(LoadImage(filepath.c_str()));
}

void app::SetFont (const std::string& filepath) {
  ImGuiIO &io = ImGui::GetIO();
  ImFont *font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
  if (font)
    io.FontDefault = font;
}

bool app::Running () {
  auto &app = App::Get();
  return !app.close && !WindowShouldClose();
}

void app::DrawStartup (const std::string& filepath) {
  auto &app = App::Get();
  if (!app.initialized || app.drawing)
    return;
  app.drawing = true;
  Texture2D t = LoadTexture(filepath.c_str());
  ::BeginDrawing();
  ::ClearBackground(BLACK);
  ::DrawTexturePro(t,
                   {0, 0, static_cast<float>(t.width), static_cast<float>(t.height)},
                   {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
                   {0, 0}, 0.0f, WHITE);
  ::EndDrawing();
  app.drawing = false;
  UnloadTexture(t);
}

bool app::HasDropfiles () {
  auto &app = App::Get();
  return app.dragDrop.hasDroppedFile;
}

std::vector<std::string> app::GetDropfiles (const std::string& endswith) {
  auto &app = App::Get();
  return app.dragDrop.getFiles(endswith);
}
