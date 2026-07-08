#pragma once
#include <string>
#include <vector>

namespace app {
void Init(int width, int height, const std::string &name);
void Close();
void BeginDrawing();
void EndDrawing();
void SetIcon(const std::string &filepath);
void SetFont(const std::string &filepath);
void DrawStartup(const std::string& filepath);

bool HasDropfiles();
std::vector<std::string> GetDropfiles(const std::string &endswith = "");

bool Running();
}; // namespace app
