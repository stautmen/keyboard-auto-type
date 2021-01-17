# Keyboard Auto Type

Cross-platform library for simulating keyboard input events.

<kbd>K</kbd><kbd>E</kbd><kbd>Y</kbd><kbd>B</kbd><kbd>O</kbd><kbd>A</kbd><kbd>R</kbd><kbd>D</kbd><kbd>-</kbd><kbd>A</kbd><kbd>U</kbd><kbd>T</kbd><kbd>O</kbd><kbd>-</kbd><kbd>T</kbd><kbd>Y</kbd><kbd>P</kbd><kbd>E</kbd>

## Status

WIP, don't use it yet.

## Installation

TODO

## Usage

Include the library headers and create an `AutoType` object:
```cpp
#include "keyboard-auto-type.h"

namespace kbd = keyboard_auto_type;

kbd::AutoType typer;
```

Perform some auto-typing using a high-level API:
```cpp
typer.text(U"Hello, world!");
```

Alternatively, you can simulate one key stroke:
```cpp
typer.key_press(U'a');
```

You can also use [modifiers](Modifiers) to perform different actions, for example, this will send <kbd>⌘</kbd><kbd>A</kbd> to select all text:
```cpp
typer.key_press(U'a', kbd::KeyCode::A, typer.shortcut_modifier());
```

Another example of a word deletion using <kbd>⌥</kbd><kbd>⌫</kbd>, in this case you pass `code`, but not `character` (also see more about [shortcuts](#shortcuts)):
```cpp
typer.key_press(0, kbd::KeyCode::BackwardDelete, kbd::Modifier::Option);
```

There's also a method to run combinations like <kbd>⌘</kbd><kbd>A</kbd> / <kbd>⌃</kbd><kbd>A</kbd>:
```cpp
typer.shortcut(kbd::KeyCode::C); // copy
typer.shortcut(kbd::KeyCode::V); // paste
```

## Low-level API

If you need access to a low-level API, there's a method `key_move` that can trigger specific individual key events, for example, using this your can trigger only `keyUp` or simulate a keypress with right Ctrl.

Other methods (`key_press`, `text`) will also press the modifier key for you, while `key_move` won't do it. However it accepts `modifier` parameter because you may need to pass it to the key event. For example, an event emitted when <kbd>A</kbd> is moved down in <kbd>⌘</kbd><kbd>A</kbd> combination, contains a flag that allows to understand that Command is now pressed.

```cpp
// make sure modifiers keys are not hold by the user
typer.ensure_modifier_not_pressed();
// press Alt
typer.key_move(kbd::Direction::Down,
               kbd::Modifier::Alt);
// press the A key
typer.key_move(kbd::Direction::Down,
               U'a',
               kbd::KeyCode::A,
               kbd::Modifier::Alt);
```

## Modifiers

The library supports keyboard modifiers in most of methods. The modifiers are:
```cpp
kbd::Modifiers::Ctrl
kbd::Modifiers::Alt
kbd::Modifiers::Shift
kbd::Modifiers::Meta
```

If you like these names more, there are aliases:
```cpp
kbd::Modifiers::Control // alias for Ctrl
kbd::Modifiers::Option  // alias for Alt
kbd::Modifiers::Command // alias for Meta
kbd::Modifiers::Win     // alias for Meta
```

Since the "command" shortcut is a very common one, the library provides a convenience method that returns <kbd>⌘</kbd> on macOS and <kbd>Ctrl</kbd> on other OS:
```cpp
kbd::AutoType::shortcut_modifier()
```

which is also exposed as `shortcut` method:
```cpp
typer.shortcut(kbd::KeyCode::A); // select all
```

## Key codes

Where applicable, you can pass different key codes, for example:
```cpp
kbd::KeyCode::A
```

Key codes are mapped depending on operating system. If the key doesn't exist, an error is returned.

## Shortcuts

There are a lot of keyboard shortcuts available in operating systems and applications. While you can trigger them using this library, please keep in mind that the library doesn't provide a standard way of triggering them. For example, <kbd>⌘</kbd><kbd>A</kbd> will select text on macOS, but not on Windows. It's not a goal of this library to standartize this.

## Errors

By default, if exception support is enabled, auto-type methods can throw an exception. If exception support is disabled, they will just return an error code described below. You can also disable exceptions using `KEYBOARD_AUTO_TYPE_NO_EXCEPTIONS` flag.

All functions return `AutoTypeResult`. If it's not `AutoTypeResult::Ok`, ther are following errors possible:

- `AutoTypeResult::BadArg`: bad argument
- `AutoTypeResult::ModifierNotReleased`: the user is holding a modifier key, simulating keystrokes in this state can have unexpected consequences
- `AutoTypeResult::NotSupported`: the given key code is not supported on this operating system
- `AutoTypeResult::OsError`: there was an error during simulating keyboard input

## Window management

The library also contains window management functions that you may need for simulating keyboard input.

Get information about the active / frontmost window:

```cpp
kbd::AutoType::active_window()
```

There's an option to get window title and url from browsers, this will display prompts about managing other apps. You can pass these options to get window title and url respectively:
```cpp
window_info = kbd::AutoType::active_window({
    .get_window_title = true,
    .get_browser_url = true,
})
```

This function returns just a pid of the active / frontmost process:
```cpp
kbd::AutoType::active_pid()
```

To activate the window found using `active_window`:
```cpp
kbd::AutoType::show_window(window_info)
```

## Strings

The library accepts 32-bit platform-independent wide characters in form of `std::u32string` or `char32_t`, the conversion is up to you. In some places, such as window information, it will return `std::string`, these strings are in UTF-8.

## Thread safety

The library is not thread safe. Moreover, it's not a good idea to manipulate the keyboard from different threads at the same time, don't do it.

## C++ standard

This project requires C++17 or above.

## License

[MIT](LICENSE.md)
