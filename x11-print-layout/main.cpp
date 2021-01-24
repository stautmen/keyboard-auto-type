#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <iostream>

Display *display;

std::string atom_str(Atom atom) {
    if (!atom) {
        return "";
    }
    auto name = XGetAtomName(display, atom);
    std::string str(name);
    XFree(name);
    return str;
}

int main() {
    std::cout << "X11 keyboard layout map" << std::endl;

    display = XOpenDisplay(nullptr);
    auto kbd = XkbGetKeyboard(display, XkbAllComponentsMask, XkbUseCoreKbd);

    XkbStateRec kbs;
    XkbGetState(display, XkbUseCoreKbd, &kbs);

    std::cout << "Keyboard symbols: " << atom_str(kbd->names->symbols) << std::endl;
    std::cout << "Groups:" << std::endl;
    for (auto gr = 0; gr < sizeof(kbd->names->groups) / sizeof(kbd->names->groups[0]); gr++) {
        std::cout << "  " << static_cast<int>(gr) << ": " << atom_str(kbd->names->groups[gr]);
        if (gr == kbs.group) {
            std::cout << " (active)";
        }
        std::cout << std::endl;
    }

    for (uint16_t key_code = kbd->min_key_code; key_code <= kbd->max_key_code; key_code++) {
        auto key_groups_num = XkbKeyNumGroups(kbd, key_code);
        std::cout << "Key 0x" << std::hex << static_cast<int>(key_code) << std::endl;
        for (auto group = 0; group < key_groups_num; group++) {
            auto shift_levels_count = XkbKeyGroupWidth(kbd, key_code, group);
            auto key_type = XkbKeyKeyType(kbd, key_code, group);
            std::cout << "  Group " << static_cast<int>(group) << ": " << atom_str(key_type->name)
                      << std::endl;
            for (auto shift_level = 0; shift_level < shift_levels_count; shift_level++) {
                auto sym = XkbKeySymEntry(kbd, key_code, shift_level, group);
                std::cout << "    Shift level " << static_cast<int>(shift_level) << ": 0x"
                          << static_cast<int>(sym);
                if (sym) {
                    std::cout << " (XK_" << XKeysymToString(sym) << ")";
                }
                std::cout << std::endl;
            }
        }
    }

    XkbFreeKeyboard(kbd, XkbAllCompatMask, True);
    XCloseDisplay(display);
}
