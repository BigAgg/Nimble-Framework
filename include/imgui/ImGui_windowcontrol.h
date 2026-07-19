#pragma once
#include <string>

using Function = void(*)();

struct PopupInformation {
  bool modal = true;
  Function function = nullptr;

  explicit operator bool() { return function; }
};

struct WindowInformation {
  bool open = false;
  int flags = 0;
  Function function = nullptr;

  explicit operator bool() { return open && function; }

  bool is_flag(int flag) const {
    return (flags & flag) != 0;
  }

  void set_flag(int flag) {
    flags |= flag;
  }

  void clear_flag(int flag) {
    flags &= ~flag;
  }

  void toggle_flag(int flag) {
    flags ^= flag;
  }
};

void RegisterWindow(const std::string& name, bool open,
  Function function, int flags = 0);
void RegisterMenu(const std::string& name,
  Function function);
void RegisterPopup(const std::string& name, bool modal, Function function);
void ToggleWindow(const std::string& name);
void SetWindowState(const std::string& name, bool open);
WindowInformation& GetWindowInfo(const std::string& name);
void DrawWindows();
void DrawMainMenu();
void ToggleWindow();
void ThemeSelector();
void EditWindowFlags();
void SaveWindowControlSettings();
void LoadWindowControlSettings();
