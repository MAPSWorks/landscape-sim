#include "input.h"
#include <base/log.h>

namespace platform {
void Input::UpdateKeyData(Key key, Action action) {
    key_data_.key = key;
    key_data_.action = action;
    // Keys that are currently being held by user, used outside keypress function
    // Usually in keypress function single hit or release is beinged checked
    if (key > Key::kUnknown && key < Key::kOnePastLast) {
        if (action == Action::kPress) {
            key_data_.active.insert(key);
        }
        else if (action == Action::kRelease) {
            if (!key_data_.active.erase(key)) {
                base::Log::Error("'Platform: tried releasing key that was not in active set");
            }
        }
    }
}

void Input::UpdateMouse(t::F32 xpos, t::F32 ypos) {
    mouse_data_.position = t::Vec2(xpos, ypos);
    // When mouse input is calculated for free-look
    if (mouse_data_.cursor_disabled) {
        // When mouse is touched for the first time make sure it does not jump
        if (mouse_data_.first_move) {
            mouse_data_.last_position = t::Vec2(xpos, ypos);
            mouse_data_.first_move = false;
        }
        // How much data has have moved
        mouse_data_.offset.x = xpos - mouse_data_.last_position.x;
        // Reversed since y-coordinates range from bottom to top
        mouse_data_.offset.y = mouse_data_.last_position.y - ypos;
        mouse_data_.last_position.x = xpos;
        mouse_data_.last_position.y = ypos;
        // Scale
        mouse_data_.offset *= mouse_data_.sensitivity;
    }
}

void Input::UpdateMouseButton(MouseButton button, Action action, t::I32 mods) {
    mouse_button_data_.button = button;
    mouse_button_data_.action = action;
    mouse_button_data_.mods = mods;
}

const Input::KeyData& Input::GetKeyData() const {
    return key_data_;
}

Input::MouseData& Input::GetMouseData() {
    return mouse_data_;
}

const Input::MouseButtonData& Input::GetMouseButtonData() const {
    return mouse_button_data_;
}

void Input::PrintKeyData() const {
    base::Log::Info("'Platform: current key state:");
    base::Log::Info("'key - ", (t::I32)key_data_.key);
    base::Log::Info("'action - ", (t::I32)key_data_.action);
    base::Log::Info("'Active keys (currently pressed): ");
    for (const auto& key : key_data_.active) {
        base::Log::Info("'", (t::I32)key);
    }
}
}; // platform