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
#include <vector>

struct DockContainer {
  std::unordered_map<std::string, WindowInformation> registry;
  ImGuiWindowFlags flags;
};

class WindowControl {
public:
  static WindowControl& Get() {
    static WindowControl instance;
    return instance;
  }
  std::map<std::string, DockContainer> dockregistry;
  std::string activedockspace;
  std::vector<std::string> docknames;
  std::unordered_map<std::string, PopupInformation> popupRegistry;
  std::pair<std::string, PopupInformation> openedPopup;
  //std::map<std::string, Function> menuBarRegistry;
  std::map<std::string, std::map<std::string, std::vector<Function>>> menuBarRegistry;
  std::string selectedWindow;
  unsigned char theme = NIMBLE_LIGHT;
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

void RegisterDock(const std::string& name, int flags){
  auto& wc = WindowControl::Get();
  auto it = wc.dockregistry.find(name);
  if (it != wc.dockregistry.end()) {
    LOG_WARNING("Dock does already exist %s", name.c_str());
    return;
  }
  wc.dockregistry[name] = { {}, flags };
  wc.docknames.push_back(name);
  LOG_INFO("Registered Dock %s with flags %d", name.c_str(), flags);
}

void SetDockspace(const std::string& name) {
  auto& wc = WindowControl::Get();
  auto it = wc.dockregistry.find(name);
  if (it == wc.dockregistry.end()) {
    LOG_WARNING("Dock is not registered %s", name.c_str());
    return;
  }
  wc.activedockspace = name;
}

std::vector<std::string> DockNames(){
  auto& wc = WindowControl::Get();
  return wc.docknames;
}

void RegisterWindow(const std::string& name, bool open,
                                   Function function,
                                   int flags) {
  /*
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
  */
  auto& wc = WindowControl::Get();
  if (wc.activedockspace.empty())
    return;
  auto it = wc.dockregistry.find(wc.activedockspace);
  if (it == wc.dockregistry.end()) {
    LOG_WARNING("Active dockspace does not exist %s", wc.activedockspace.c_str());
    return;
  }
  auto it2 = it->second.registry.find(name);
  if (it2 != it->second.registry.end()) {
    it2->second.function = function;
    return;
  }
  WindowInformation wi;
  wi.open = open;
  wi.function = function;
  wi.flags = flags;
  it->second.registry[name] = std::move(wi);
}

void RegisterWindow(const std::string& dockspace, const std::string& name, bool open, Function function, int flags){
  auto& wc = WindowControl::Get();
  const std::string active = wc.activedockspace;
  auto it = wc.dockregistry.find(dockspace);
  if (it == wc.dockregistry.end())
    RegisterDock(dockspace);
  SetDockspace(dockspace);
  RegisterWindow(name, open, function, flags);
  SetDockspace(active);
}

void RegisterMenu(const std::string& name) {
  auto& wc = WindowControl::Get();
  auto it = wc.menuBarRegistry.find(name);
  if (it != wc.menuBarRegistry.end()) {
    LOG_THROW("There is already a menu registered with the same name: %s", name.c_str());
  }
  wc.menuBarRegistry[name] = {};
}

void RegisterSubmenu(const std::string menu, const std::string& name, Function function){
  auto& wc = WindowControl::Get();
  auto it = wc.menuBarRegistry.find(menu);
  if (it == wc.menuBarRegistry.end())
    RegisterMenu(menu);
  wc.menuBarRegistry[menu][name].push_back(function);
}

void RegisterPopup (const std::string& name, bool modal, Function function) {
  auto& wc = WindowControl::Get();
  if (!function) {
    LOG_THROW("nullptr Function for popup is not allowed: %s", name.c_str());
  }
  if (name.empty()) {
    LOG_THROW("Empty name is not allowed!");
  }
  auto it = wc.popupRegistry.find(name);
  if (it != wc.popupRegistry.end()) {
    LOG_THROW("There is already a Popup registered with the same name: %s", name.c_str());
  }
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
  auto it = wc.dockregistry[wc.activedockspace].registry.find(name);
  if (it == wc.dockregistry[wc.activedockspace].registry.end())
    return;
  it->second.open = !it->second.open;
}

void SetWindowState(const std::string& name, bool open) {
  auto& wc = WindowControl::Get();
  auto it = wc.dockregistry[wc.activedockspace].registry.find(name);
  if (it == wc.dockregistry[wc.activedockspace].registry.end())
    return;
  it->second.open = open;
}

WindowInformation& GetWindowInfo(const std::string& name) {
  auto& wc = WindowControl::Get();
  static WindowInformation wi;
  auto it = wc.dockregistry[wc.activedockspace].registry.find(name);
  if (it == wc.dockregistry[wc.activedockspace].registry.end())
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
  auto it = wc.dockregistry.find(wc.activedockspace);
  if (it == wc.dockregistry.end())
    return;
  std::string dockname = wc.activedockspace;
  DockContainer dockinfo = wc.dockregistry[dockname];
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin(dockname.c_str(), nullptr, dockinfo.flags);
	ImGui::PopStyleVar(2);

	// Important: creates th docking node
	ImGuiID dockspace_id = ImGui::GetID(dockname.c_str());
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0));
	ImGui::End();
	// Drawing windows related to this dockspace
	for (auto& [name, wi] : dockinfo.registry) {
		if (!wi)
			continue;
		if (ImGui::Begin(name.c_str(), &wi.open, wi.flags | ImGuiWindowFlags_NoFocusOnAppearing)) {
			wi.function();
		}
		ImGui::End();
	}
}

void DrawMainMenu() {
  auto& wc = WindowControl::Get();
  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("Menü")) {
    for (const auto& name : DockNames()) {
      if (name == wc.activedockspace)
        continue;
      if (ImGui::Button(name.c_str(), ImVec2(300.0f, 0)))
        SetDockspace(name);
    }
    ImGui::EndMenu();
  }
  for (const auto& [menu, submenus] : wc.menuBarRegistry) {
    if (ImGui::BeginMenu(menu.c_str())) {
      for (const auto& [name, submenu] : submenus) {
        if (name.empty()) {
          for (const auto& menu : submenu) {
						menu();
						ImGui::Separator();
          }
          continue;
        }
        if (ImGui::BeginMenu(name.c_str())) {
          for (const auto& menu : submenu) {
            menu();
            ImGui::Separator();
          }
          ImGui::EndMenu();
        }
      }
      ImGui::EndMenu();
    }
  }
  if (ImGui::BeginMenu("Fenster")) {
    if (ImGui::BeginMenu("Verhalten")) {
      EditWindowFlags();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Ein/-Ausschalten")) {
      ToggleWindow();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Farbschema")) {
      ThemeSelector();
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
}

void ToggleWindow() {
  auto& wc = WindowControl::Get();
  for (auto& [name, wi] : wc.dockregistry[wc.activedockspace].registry) {
    if (!wi.function)
      return;
    ImGui::Checkbox(name.c_str(), &wi.open);
  }
}

void ThemeSelector() {
  auto& wc = WindowControl::Get();
  if (ImGui::BeginMenu("Hell")) {
    if (ImGui::Button("Blau")) {
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
    if (ImGui::Button("Modern")) {
      SetTheme(MODERN_LIGHT);
      wc.theme = MODERN_LIGHT;
    }
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Dunkel")) {
    if (ImGui::Button("Blau")) {
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
    if (ImGui::Button("Modern")) {
      SetTheme(MODERN_DARK);
      wc.theme = MODERN_DARK;
    }
    ImGui::EndMenu();
  }
}

void EditWindowFlags() {
  auto& wc = WindowControl::Get();
  if (ImGui::BeginCombo("Fensterauswahl", wc.selectedWindow.c_str())) {
    for (const auto& [dockname, dockinfo] : wc.dockregistry) {
			for (const auto& [name, wi] : dockinfo.registry) {
				if (!wi.function || !wi.open)
					continue;
				bool selected = name == wc.selectedWindow;
				if (ImGui::Selectable(name.c_str(), selected))
					wc.selectedWindow = name;
			}
    }
    ImGui::EndCombo();
  }
  auto it = wc.dockregistry[wc.activedockspace].registry.find(wc.selectedWindow);
  if (it == wc.dockregistry[wc.activedockspace].registry.end()) {
    return;
  }

  auto& wi = it->second;
  ImGui::SeparatorText("Fenstereigenschaften");
  CheckboxFlags("Keine Titelbar", &wi.flags, ImGuiWindowFlags_NoTitleBar);
  CheckboxFlags("Feste Größe", &wi.flags, ImGuiWindowFlags_NoResize);
  CheckboxFlags("Feste Position", &wi.flags, ImGuiWindowFlags_NoMove);
  CheckboxFlags("Keine Scrollbar", &wi.flags, ImGuiWindowFlags_NoScrollbar);
  CheckboxFlags("Kein Scrollen mit Maus", &wi.flags,
                ImGuiWindowFlags_NoScrollWithMouse);
  CheckboxFlags("Kein verkleinern", &wi.flags, ImGuiWindowFlags_NoCollapse);
  CheckboxFlags("Automatische Größe", &wi.flags,
                ImGuiWindowFlags_AlwaysAutoResize);
  CheckboxFlags("Kein Hintergrund", &wi.flags, ImGuiWindowFlags_NoBackground);
  CheckboxFlags("Keine gespeicherten Einstellungen", &wi.flags,
                ImGuiWindowFlags_NoSavedSettings);
  CheckboxFlags("Keine Mauseingaben", &wi.flags,
                ImGuiWindowFlags_NoMouseInputs);
  CheckboxFlags("Menu Bar", &wi.flags, ImGuiWindowFlags_MenuBar);
  CheckboxFlags("Horizontale Scrollbar", &wi.flags,
                ImGuiWindowFlags_HorizontalScrollbar);
  CheckboxFlags("Kein Fokus beim Erscheinen", &wi.flags,
                ImGuiWindowFlags_NoFocusOnAppearing);
  CheckboxFlags("Kein nach vorne bringen beim Fokusieren", &wi.flags,
                ImGuiWindowFlags_NoBringToFrontOnFocus);
  CheckboxFlags("Immer vertikale Scrollbar", &wi.flags,
                ImGuiWindowFlags_AlwaysVerticalScrollbar);
  CheckboxFlags("Immer Horizontale Scrollbar", &wi.flags,
                ImGuiWindowFlags_AlwaysHorizontalScrollbar);
  CheckboxFlags("Keine Navigationseingaben", &wi.flags, ImGuiWindowFlags_NoNavInputs);
  CheckboxFlags("Kein Navigationsfokus", &wi.flags, ImGuiWindowFlags_NoNavFocus);
  ImGui::BeginDisabled();
  CheckboxFlags("Ungespeicherted Dokument", &wi.flags,
                ImGuiWindowFlags_UnsavedDocument);
  ImGui::EndDisabled();
  CheckboxFlags("Kein Docking/Snapping", &wi.flags, ImGuiWindowFlags_NoDocking);
  CheckboxFlags("Keine Navigation", &wi.flags, ImGuiWindowFlags_NoNav);
  CheckboxFlags("Keine Dekoration", &wi.flags, ImGuiWindowFlags_NoDecoration);
  ImGui::BeginDisabled();
  CheckboxFlags("Keine Eingaben", &wi.flags, ImGuiWindowFlags_NoInputs);
  ImGui::EndDisabled();
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
  for (const auto& [dockname, dockinfo] : wc.dockregistry) {
		for (const auto& [name, wi] : dockinfo.registry) {
			if (!wi.function)
				continue;
			file << "[Window][" << name << "]\n";
			file << "Flags=" << std::to_string(wi.flags) << "\n";
			file << "Open=" << std::to_string(wi.open) << "\n";
      file << "Dockspace=" << dockname << "\n";
			file << "\n";
		}
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
      std::string dockspace = "";
      while (!windowname.empty() && std::getline(file, line) &&
             !line.empty()) {
        if (line.back() == '\r')
          line.pop_back();
        const auto& [name, value] = split_at(line, "=");
        if (name == "Open")
          open = static_cast<bool>(std::stoi(value));
        else if (name == "Flags")
          flags = std::stoi(value);
        else if (name == "Dockspace")
          dockspace = value;
      }
      if (!windowname.empty())
        RegisterWindow(dockspace, windowname, open, nullptr, flags);
    }
  }
  file.close();
  SetTheme(wc.theme);
}
