#include "imgui/ImGui_windowcontrol.h"
#include "imgui/ImGui_themes.h"
#include "utils/stringconverter.h"
#include <cassert>
#include <fstream>
#include <imgui.h>
#include <string>
#include <utility>

void WindowControl::RegisterWindow(const std::string& name, bool open,
                                   Function function,
                                   int flags) {
  auto it = mRegistry.find(name);
  if (it != mRegistry.end()) {
    it->second.function = function;
    return;
  }
  WindowInformation wi;
  wi.open = open;
  wi.function = function;
  wi.flags = flags;
  mRegistry[name] = std::move(wi);
}

void WindowControl::RegisterMenu(const std::string& name,
                                 Function function) {
  auto it = mMenuBarRegistry.find(name);
  assert(it == mMenuBarRegistry.end() &&
         "There is already a Menu registered with the same name!");
  mMenuBarRegistry[name] = function;
}

void WindowControl::RegisterPopup (const std::string& name, bool modal, Function function) {
  assert(function && "nullptr Function for popup is not allowed");
  assert(!name.empty() && "Empty name is not allowed");
  auto it = mPopupRegistry.find(name);
  assert(it == mPopupRegistry.end() && "There is already a Popup registered with the same name!");
  mPopupRegistry[name] = PopupInformation(modal, function);
}

void WindowControl::ToggleWindow(const std::string& name) {
  auto it = mRegistry.find(name);
  if (it == mRegistry.end())
    return;
  it->second.open = !it->second.open;
}

void WindowControl::SetWindowState(const std::string& name, bool open) {
  auto it = mRegistry.find(name);
  if (it == mRegistry.end())
    return;
  it->second.open = open;
}

WindowInformation& WindowControl::GetWindowInfo(const std::string& name) {
  static WindowInformation wi;
  auto it = mRegistry.find(name);
  if (it == mRegistry.end())
    return wi;
  return it->second;
}

void WindowControl::DrawWindows() {
  DrawMainMenu();
  for (auto& [name, wi] : mRegistry) {
    if (!wi)
      continue;
    if (ImGui::Begin (name.c_str (), &wi.open, wi.flags)) {
      wi.function();
			for (auto& [name, pi] : mPopupRegistry) {
				if (!pi)
					continue;
				if (pi.modal) {
					if (ImGui::BeginPopupModal (name.c_str ())) {
						pi.function();
						ImGui::EndPopup();
					}
				} else {
					if (ImGui::BeginPopup (name.c_str ())) {
						pi.function();
						ImGui::EndPopup();
					}
				}
			}
    }
    ImGui::End();
  }
}

void WindowControl::DrawMainMenu() {
  ImGui::BeginMainMenuBar();
  for (const auto& [name, function] : mMenuBarRegistry) {
    if (ImGui::BeginMenu(name.c_str())) {
      function();
      for (auto& [name, pi] : mPopupRegistry) {
        if (!pi)
          continue;
        if (pi.modal) {
          if (ImGui::BeginPopupModal (name.c_str ())) {
            pi.function();
            ImGui::EndPopup();
          }
        } else {
          if (ImGui::BeginPopup (name.c_str ())) {
            pi.function();
            ImGui::EndPopup();
          }
        }
      }
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

void WindowControl::ToggleWindow() {
  for (auto& [name, wi] : mRegistry) {
    if (!wi.function)
      return;
    ImGui::Checkbox(name.c_str(), &wi.open);
  }
}

void WindowControl::ThemeSelector() {
  if (ImGui::BeginMenu("Light Themes")) {
    if (ImGui::Button("Normal")) {
      SetTheme(LIGHT);
      mTheme = LIGHT;
    }
    if (ImGui::Button("Gold")) {
      SetTheme(GOLD_LIGHT);
      mTheme = GOLD_LIGHT;
    }
    if (ImGui::Button("Lila")) {
      SetTheme(PURPLE_LIGHT);
      mTheme = PURPLE_LIGHT;
    }
    if (ImGui::Button("Braun")) {
      SetTheme(NOCTUA_LIGHT);
      mTheme = NOCTUA_LIGHT;
    }
    if (ImGui::Button("Rose")) {
      SetTheme(ROSEPINE_LIGHT);
      mTheme = ROSEPINE_LIGHT;
    }
    ImGui::EndMenu();
  }
  if (ImGui::BeginMenu("Dark Themes")) {
    if (ImGui::Button("Normal")) {
      SetTheme(DARK);
      mTheme = DARK;
    }
    if (ImGui::Button("Gold")) {
      SetTheme(GOLD_DARK);
      mTheme = GOLD_DARK;
    }
    if (ImGui::Button("Lila")) {
      SetTheme(PURPLE_DARK);
      mTheme = PURPLE_DARK;
    }
    if (ImGui::Button("Braun")) {
      SetTheme(NOCTUA_DARK);
      mTheme = NOCTUA_DARK;
    }
    if (ImGui::Button("Rose")) {
      SetTheme(ROSEPINE_DARK);
      mTheme = ROSEPINE_DARK;
    }
    ImGui::EndMenu();
  }
}

void WindowControl::EditWindowFlags() {
  if (ImGui::BeginCombo("Fenster Auswahl", mSelectedWindow.c_str())) {
    for (const auto& [name, wi] : mRegistry) {
      if (!wi.function || !wi.open)
        continue;
      bool selected = name == mSelectedWindow;
      if (ImGui::Selectable(name.c_str(), selected))
        mSelectedWindow = name;
    }
    ImGui::EndCombo();
  }
  auto it = mRegistry.find(mSelectedWindow);
  if (it == mRegistry.end()) {
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

void WindowControl::CheckboxFlags(const char* label, int* flags,
                                  int flag) {
  bool v = (*flags & flag) != 0;
  if (ImGui::Checkbox(label, &v)) {
    if (v)
      *flags |= flag;
    else
      *flags &= ~flag;
  }
}

void WindowControl::SaveSettings() {
  std::ofstream file;
  file.open("windowcontrol.ini", std::ios::binary);
  if (!file)
    return;
  // Saving behavior settings
  file << "[Settings]\n";
  file << "Theme=" << std::to_string(mTheme) << "\n";
  file << "\n";
  // Saving each window settings
  for (const auto& [name, wi] : mRegistry) {
    if (!wi.function)
      continue;
    file << "[Window][" << name << "]\n";
    file << "Flags=" << std::to_string(wi.flags) << "\n";
    file << "Open=" << std::to_string(wi.open) << "\n";
    file << "\n";
  }
  file.close();
}

void WindowControl::LoadSettings() {
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
          mTheme = static_cast<unsigned char>(std::stoi(value));
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
  SetTheme(mTheme);
}

void WindowControl::ToggleThemeSelect() {
  mThemeSelectOpen = !mThemeSelectOpen;
}

void WindowControl::ToggleEditFlags() { mEditFlagsOpen = !mEditFlagsOpen; }

void WindowControl::ToggleWindowToggle() {
  mWindowToggleOpen = !mWindowToggleOpen;
}
