#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include <filesystem>
#include "utils/filedialog.h"
#include <cstdarg>

namespace ImGui {
inline void HelpMarker (const char* fmt, ...) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered (ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_AllowWhenDisabled)) {
		ImGui::BeginTooltip();
		va_list args;
		va_start(args, fmt);
		ImGui::TextV(fmt, args);
		va_end(args);
		ImGui::EndTooltip();
		}
}

inline void HelpMarkerColored (bool hightlight, const char* fmt, ...) {
  ImVec4 color = hightlight
                     ? ImVec4(1.0f, 0.2f, 0.2f, 1.0f) // red
                     : ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
  ImGui::PushStyleColor(ImGuiCol_Text, color);
  ImGui::TextUnformatted("(?)");
  ImGui::PopStyleColor();

	if (ImGui::IsItemHovered (ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_AllowWhenDisabled)) {
    ImGui::BeginTooltip();
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
    ImGui::EndTooltip();
	}
}

bool IsCurrentWindowHovered() {
  // Get the current child window
  ImGuiWindow *child = ImGui::GetCurrentWindow();
  // Child window rectangle in absolute coordinates:
  ImVec2 min = child->Pos;
  ImVec2 max = ImVec2(child->Pos.x + child->Size.x,
                      child->Pos.y + child->Size.y);
  return ImGui::IsMouseHoveringRect(min, max);
}

void Filewalker (std::string& path, std::vector<std::string>& files) {
  namespace fs = std::filesystem;
  if (path.empty())
    return;
  fs::path p(path);
  if (ImGui::ArrowButton ("Back", ImGuiDir_Left) && p.has_parent_path()) {
    path = p.parent_path().string();
    p = fs::path(path);
    files.clear();
    for (const auto& entry : fs::directory_iterator (p)) {
      files.push_back(entry.path().string());
    }
  }
  ImGui::SameLine();
  ImGui::Text("%s", p.filename().string().c_str());
  if (ImGui::BeginListBox ("##files")) {
    for (const auto& file : files) {
      fs::path filepath(file);
      if (ImGui::Selectable (filepath.filename ().string ().c_str ())) {
        if (fs::is_directory (filepath)) {
          path = file;
          files.clear();
          for (const auto& entry : fs::directory_iterator (filepath)) {
            files.push_back(entry.path().string());
          }
          break;
        }
        else
          OpenPath(file);
      }
      ImGui::SetItemTooltip("%s", file.c_str());
    }
    ImGui::EndListBox();
  }
  if (ImGui::Button("Ordner öffnen"))
    OpenPath(path);
}
}

