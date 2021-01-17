#ifndef KEYBOARD_AUTO_TYPE_H
#define KEYBOARD_AUTO_TYPE_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#elif __APPLE__
#include <Carbon/Carbon.h>
#include <sys/types.h>
#endif

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "key-code.h"

namespace keyboard_auto_type {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
using pid_t = DWORD;
using window_handle_t = HWND;
using os_key_code_t = BYTE;
#elif __APPLE__
using window_handle_t = intptr_t;
using os_key_code_t = CGKeyCode;
#endif

enum class Modifier : uint8_t {
    None = 0b0000,

    Ctrl = 0b0001,
    Control = Ctrl,

    Alt = 0b0010,
    Option = Alt,

    Shift = 0b0100,

    Meta = 0b1000,
    Command = Meta,
    Win = Meta,
};

Modifier operator+(Modifier lhs, Modifier rhs);
Modifier operator|(Modifier lhs, Modifier rhs);
Modifier operator&(Modifier lhs, Modifier rhs);

enum class Direction { Up, Down };

enum class AutoTypeResult {
    Ok,
    BadArg,
    ModifierNotReleased,
    NotSupported,
};

struct AppWindowInfo {
    pid_t pid = 0;
    window_handle_t window_id = 0;
    std::string app_name;
    std::string title;
    std::string url;
};

struct ActiveWindowArgs {
    bool get_window_title = false;
    bool get_browser_url = false;
};

class AutoType {
  private:
    class AutoTypeImpl;
    std::unique_ptr<AutoTypeImpl> impl_;

  public:
    AutoType();
    ~AutoType();

    AutoType(const AutoType &) = delete;
    AutoType &operator=(const AutoType &) = delete;
    AutoType(AutoType &&) = delete;
    AutoType &operator=(AutoType &&) = delete;

    AutoTypeResult text(std::u32string_view text, Modifier modifier = Modifier::None);

    AutoTypeResult key_press(char32_t character, Modifier modifier = Modifier::None);
    AutoTypeResult key_press(char32_t character, KeyCode code, Modifier modifier = Modifier::None);

    AutoTypeResult shortcut(KeyCode code);
    static Modifier shortcut_modifier();

    AutoTypeResult ensure_modifier_not_pressed();
    Modifier get_pressed_modifiers();
    AutoTypeResult key_move(Direction direction, char32_t character,
                            Modifier modifier = Modifier::None);
    AutoTypeResult key_move(Direction direction, char32_t character, KeyCode code,
                            Modifier modifier = Modifier::None);
    AutoTypeResult key_move(Direction direction, KeyCode code, Modifier modifier = Modifier::None);
    AutoTypeResult key_move(Direction direction, Modifier modifier);
    AutoTypeResult key_move(Direction direction, char32_t character, os_key_code_t code,
                            Modifier modifier = Modifier::None);
    std::optional<os_key_code_t> os_key_code(KeyCode code);
    os_key_code_t os_key_code_for_char(char32_t character);
    std::vector<os_key_code_t> os_key_codes_for_chars(std::u32string_view text);

    static pid_t active_pid();
    static AppWindowInfo active_window(const ActiveWindowArgs &args = {});
    static bool show_window(const AppWindowInfo &window);
};

} // namespace keyboard_auto_type

#endif
