#include "imgui/ImGui_windowcontrol.h"
#include "imgui/ImGui_themes.h"
#include "utils/logging.h"
#include "utils/stringconverter.h"
#include <cassert>
#include <fstream>
#include <imgui.h>
#include <string>
#include <utility>
#include <unordered_map>
#include <map>

class WindowControl {
public:
  static WindowControl& Get() {
    static WindowControl instance;
    return instance;
  }
  std::unordered_map<std::string, WindowInformation> registry;
  std::unordered_map<std::string, PopupInformation> popupRegistry;
  std::pair<std::string, PopupInformation> openedPopup;
  std::map<std::string, Function> menuBarRegistry;
  std::string selectedWindow;
  unsigned char theme = 0;
  bool editFlagsOpen = false;
  bool themeSelectOpen = false;
  bool windowToggleOpen = false;

public:
  WindowControl(const WindowControl&) = delete;
  WindowControl& operator=(const WindowControl&) = delete;
  WindowControl(WindowControl&&) = delete;
  WindowControl& operator=(WindowControl&&) = delete;

private:
  WindowControl() = default;
  ~WindowControl() = default;
};

static void CheckboxFlags(const char* label, int* flags, int flag);

void RegisterWindow(const std::string& name, bool open,
                                   Function function,
                                   int flags) {
  auto& wc = WindowControl::Get();
  auto it = wc.registry.find(name);
  if (it != wc.registry.end()) {
    it->second.function = function;
    return;
  }
  WindowInformation wi;
  wi.open = open;
  wi.function = function;
  wi.flags = flags;
  wc.registry[name] = std::move(wi);
}

void RegisterMenu(const std::string& name,
                                 Function function) {
  auto& wc = WindowControl::Get();
  auto it = wc.menuBarRegistry.find(name);
  assert(it == wc.menuBarRegistry.end() &&
         "There is already a Menu registered with the same name!");
  wc.menuBarRegistry[name] = function;
}

void RegisterPopup (const std::string& name, bool modal, Function function) {
  auto& wc = WindowControl::Get();
  assert(function && "nullptr Function for popup is not allowed");
  assert(!name.empty() && "Empty name is not allowed");
  auto it = wc.popupRegistry.find(name);
  assert(it == wc.popupRegistry.end() && "There is already a Popup registered with the same name!");
  wc.popupRegistry[name] = PopupInformation(modal, function);
}

void OpenPopup(const std::string& name) {
  auto& wc = WindowControl::Get();
  auto it = wc.popupRegistry.find(name);
  if (it == wc.popupRegistry.end()) {
    LOG_WARNING("Desired popup is not registered: %s", name.c_str());
    return;
  }
  wc.openedPopup = std::make_pair(it->first, it->second);
}

void ClosePopup() {
  auto& wc = WindowControl::Get();
  wc.openedPopup = {};
  ImGui::CloseCurrentPopup();
}

void ToggleWindow(const std::string& name) {
  auto& wc = WindowControl::Get();
  auto it = wc.registry.find(name);
  if (it == wc.registry.end())
    return;
  it->second.open = !it->second.open;
}

void SetWindowState(const std::string& name, bool open) {
  auto& wc = WindowControl::Get();
  auto it = wc.registry.find(name);
  if (it == wc.registry.end())
    return;
  it->second.open = open;
}

WindowInformation& GetWindowInfo(const std::string& name) {
  auto& wc = WindowControl::Get();
  static WindowInformation wi;
  auto it = wc.registry.find(name);
  if (it == wc.registry.end())
    return wi;
  return it->second;
}

void DrawWindows() {
  auto& wc = WindowControl::Get();
  DrawMainMenu();
  // Handle popup
  if (!wc.openedPopup.first.empty()) {
    if (!ImGui::IsPopupOpen(wc.openedPopup.first.c_str())) {
      ImGui::OpenPopup(wc.openedPopup.first.c_str());
    }
    bool modal = wc.openedPopup.second.modal;
    if (modal) {
      if (ImGui::BeginPopupModal(wc.openedPopup.first.c_str())) {
        wc.openedPopup.second.function();
        ImGui::EndPopup();
      }
    }
    else {
      if (ImGui::BeginPopup(wc.openedPopup.first.c_str())) {
        wc.openedPopup.second.function();
        ImGui::EndPopup();
      }
    }
  }
  // Handle windows
  for (auto& [name, wi] : wc.registry) {
    if (!wi)
      continue;
    if (ImGui::Begin (name.c_str (), &wi.open, wi.flags | ImGuiWindowFlags_NoFocusOnAppearing)) {
      wi.function();
    }
    ImGui::End();
  }
}

void DrawMainMenu() {
  auto& wc = WindowControl::Get();
  ImGui::BeginMainMenuBar();
  for (const auto& [name, function] : wc.menuBarRegistry) {
    if (ImGui::BeginMenu(name.c_str())) {
      function();
      ImGui::EndMenu();
    }
  }
  if (ImGui::BeginMenu("Windows")) {
    if (ImGui::BeginMenu("Edit Window Flags")) {
      EditWindowFlags();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Toggle Window")) {
      ToggleWindow();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Select Theem")) {
      ThemeSelector();
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
}

void ToggleWindow() {
  auto& wc = WindowControl::Get();
  for (auto& [name, wi] : wc.registry) {
    if (!wi.function)
      return;
    ImGui::Checkbox(name.c_str(), &wi.open);
  }
}

void ThemeSelector() {
  auto& wc = WindowControl::Get();
  if (ImGui::BeginMenu("Light Themes")) {
    if (ImGui::Button("Normal")) {
      SetTheme(LIGHT);
      wc.theme = LIGHT;
    }
    if (ImGui::Button("Gold")) {
      SetTheme(GOLD_LIGHT);
      wc.theme = GOLD_LIGHT;
    }
    if (ImGui::Button("Lila")) {
      SetTheme(PURPLE_LIGHT);
      wc.theme = PURPLE_LIGHT;
    }
    if (ImGui::Button("Braun")) {
      SetTheme(NOCTUA_LIGHT);
      wc.theme = NOCTUA_LIGHT;
    }
    if (ImGui::Button("Rose")) {
      SetTheme(ROSEPINE_LIGHT);
      wc.theme = ROSEPINE_LIGHT;
    }
    if (ImGui::Button("Nimble")) {
      SetTheme(NIMBLE_LIGHT);
      wc.theme = NIMBLE_LIGHT;
    }
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Dark Themes")) {
    if (ImGui::Button("Normal")) {
      SetTheme(DARK);
      wc.theme = DARK;
    }
    if (ImGui::Button("Gold")) {
      SetTheme(GOLD_DARK);
      wc.theme = GOLD_DARK;
    }
    if (ImGui::Button("Lila")) {
      SetTheme(PURPLE_DARK);
      wc.theme = PURPLE_DARK;
    }
    if (ImGui::Button("Braun")) {
      SetTheme(NOCTUA_DARK);
      wc.theme = NOCTUA_DARK;
    }
    if (ImGui::Button("Rose")) {
      SetTheme(ROSEPINE_DARK);
      wc.theme = ROSEPINE_DARK;
    }
    if (ImGui::Button("Nimble")) {
      SetTheme(NIMBLE_DARK);
      wc.theme = NIMBLE_DARK;
    }
    ImGui::EndMenu();
  }
}

void EditWindowFlags() {
  auto& wc = WindowControl::Get();
  if (ImGui::BeginCombo("Fenster Auswahl", wc.selectedWindow.c_str())) {
    for (const auto& [name, wi] : wc.registry) {
      if (!wi.function || !wi.open)
        continue;
      bool selected = name == wc.selectedWindow;
      if (ImGui::Selectable(name.c_str(), selected))
        wc.selectedWindow = name;
    }
    ImGui::EndCombo();
  }
  auto it = wc.registry.find(wc.selectedWindow);
  if (it == wc.registry.end()) {
    return;
  }

  auto& wi = it->second;
  ImGui::SeparatorText("Window Flags");
  CheckboxFlags("No Title Bar", &wi.flags, ImGuiWindowFlags_NoTitleBar);
  CheckboxFlags("No Resize", &wi.flags, ImGuiWindowFlags_NoResize);
  CheckboxFlags("No Move", &wi.flags, ImGuiWindowFlags_NoMove);
  CheckboxFlags("No Scrollbar", &wi.flags, ImGuiWindowFlags_NoScrollbar);
  CheckboxFlags("No Scroll With Mouse", &wi.flags,
                ImGuiWindowFlags_NoScrollWithMouse);
  CheckboxFlags("No Collapse", &wi.flags, ImGuiWindowFlags_NoCollapse);
  CheckboxFlags("Always Auto Resize", &wi.flags,
                ImGuiWindowFlags_AlwaysAutoResize);
  CheckboxFlags("No Background", &wi.flags, ImGuiWindowFlags_NoBackground);
  CheckboxFlags("No Saved Settings", &wi.flags,
                ImGuiWindowFlags_NoSavedSettings);
  CheckboxFlags("No Mouse Inputs", &wi.flags,
                ImGuiWindowFlags_NoMouseInputs);
  CheckboxFlags("Menu Bar", &wi.flags, ImGuiWindowFlags_MenuBar);
  CheckboxFlags("Horizontal Scrollbar", &wi.flags,
                ImGuiWindowFlags_HorizontalScrollbar);
  CheckboxFlags("No Focus On Appearing", &wi.flags,
                ImGuiWindowFlags_NoFocusOnAppearing);
  CheckboxFlags("No Bring to Front On Focus", &wi.flags,
                ImGuiWindowFlags_NoBringToFrontOnFocus);
  CheckboxFlags("Always Vertical Scrollbar", &wi.flags,
                ImGuiWindowFlags_AlwaysVerticalScrollbar);
  CheckboxFlags("Always Horizontal Scrollbar", &wi.flags,
                ImGuiWindowFlags_AlwaysHorizontalScrollbar);
  CheckboxFlags("No Nav Inputs", &wi.flags, ImGuiWindowFlags_NoNavInputs);
  CheckboxFlags("No Nav Focus", &wi.flags, ImGuiWindowFlags_NoNavFocus);
  CheckboxFlags("Unsaved Document", &wi.flags,
                ImGuiWindowFlags_UnsavedDocument);
  CheckboxFlags("No Docking", &wi.flags, ImGuiWindowFlags_NoDocking);
  CheckboxFlags("No Nav", &wi.flags, ImGuiWindowFlags_NoNav);
  CheckboxFlags("No Decoration", &wi.flags, ImGuiWindowFlags_NoDecoration);
  CheckboxFlags("No Inputs", &wi.flags, ImGuiWindowFlags_NoInputs);
}

static void CheckboxFlags(const char* label, int* flags,
                                  int flag) {
  bool v = (*flags & flag) != 0;
  if (ImGui::Checkbox(label, &v)) {
    if (v)
      *flags |= flag;
    else
      *flags &= ~flag;
  }
}

void SaveWindowControlSettings() {
  auto& wc = WindowControl::Get();
  std::ofstream file;
  file.open("windowcontrol.ini", std::ios::binary);
  if (!file)
    return;
  // Saving behavior settings
  file << "[Settings]\n";
  file << "Theme=" << std::to_string(wc.theme) << "\n";
  file << "\n";
  // Saving each window settings
  for (const auto& [name, wi] : wc.registry) {
    if (!wi.function)
      continue;
    file << "[Window][" << name << "]\n";
    file << "Flags=" << std::to_string(wi.flags) << "\n";
    file << "Open=" << std::to_string(wi.open) << "\n";
    file << "\n";
  }
  file.close();
}

void LoadWindowControlSettings() {
  auto& wc = WindowControl::Get();
  std::ifstream file;
  file.open("windowcontrol.ini", std::ios::binary);
  if (!file)
    return;
  // Loading line by line
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    // Loading Settings
    if (line == "[Settings]") {
      while (std::getline(file, line) && !line.empty()) {
        if (line.back() == '\r')
          line.pop_back();
        const auto& [name, value] = split_at(line, "=");
        if (name == "Theme") {
          wc.theme = static_cast<unsigned char>(std::stoi(value));
        }
      }
    }
    // Loading Window
    if (line.starts_with("[Window][")) {
      // Retrieve name
      auto [type, windowname] = split_at(line, "][");
      windowname.pop_back();
      bool open = false;
      int flags = 0;
      while (!windowname.empty() && std::getline(file, line) &&
             !line.empty()) {
        if (line.back() == '\r')
          line.pop_back();
        const auto& [name, value] = split_at(line, "=");
        if (name == "Open")
          open = static_cast<bool>(std::stoi(value));
        else if (name == "Flags")
          flags = std::stoi(value);
      }
      if (!windowname.empty())
        RegisterWindow(windowname, open, nullptr, flags);
    }
  }
  file.close();
  SetTheme(wc.theme);
}
