#pragma once
#include <map>
#include <string>
#include <unordered_map>

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

class WindowControl {
public:
  static WindowControl& Get() {
    static WindowControl instance;
    return instance;
  }

  void RegisterWindow(const std::string& name, bool open,
                      Function function, int flags = 0);
  void RegisterMenu(const std::string& name,
                    Function function);
  void RegisterPopup(const std::string &name, bool modal, Function function);
  void ToggleWindow(const std::string& name);
  void SetWindowState(const std::string& name, bool open);
  WindowInformation& GetWindowInfo(const std::string& name);
  void DrawWindows();
  void DrawMainMenu();
  void ToggleWindow();
  void ThemeSelector();
  void EditWindowFlags();
  void SaveSettings();
  void LoadSettings();
  void ToggleThemeSelect();
  void ToggleEditFlags();
  void ToggleWindowToggle();

  const size_t size() const { return mRegistry.size(); }

private:
  std::unordered_map<std::string, WindowInformation> mRegistry;
  std::unordered_map<std::string, PopupInformation> mPopupRegistry;
  std::map<std::string, Function> mMenuBarRegistry;
  std::string mSelectedWindow;
  unsigned char mTheme = 0;
  bool mEditFlagsOpen = false;
  bool mThemeSelectOpen = false;
  bool mWindowToggleOpen = false;
  void CheckboxFlags(const char* label, int* flags, int flag);

public:
  WindowControl(const WindowControl&) = delete;
  WindowControl& operator=(const WindowControl&) = delete;
  WindowControl(WindowControl&&) = delete;
  WindowControl& operator=(WindowControl&&) = delete;

private:
  WindowControl() = default;
  ~WindowControl() = default;
};
