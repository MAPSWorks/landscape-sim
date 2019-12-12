#pragma once
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <base/types.h>

// Stores current state of the mouse-keyboard input
namespace platform {
class Input {
public:
    enum class Key : t::I32 {
        kUnknown = GLFW_KEY_UNKNOWN,
        kA = GLFW_KEY_A,
        kD = GLFW_KEY_D,
        kS = GLFW_KEY_S,
        kW = GLFW_KEY_W,
        kU = GLFW_KEY_U,
        kI = GLFW_KEY_I,
        kO = GLFW_KEY_O,
        kJ = GLFW_KEY_J,
        kK = GLFW_KEY_K,
        kL = GLFW_KEY_L,
        kR = GLFW_KEY_R,
        kF = GLFW_KEY_F,
        kUp = GLFW_KEY_UP,
        kDown = GLFW_KEY_DOWN,
        kLeft = GLFW_KEY_LEFT,
        kRight = GLFW_KEY_RIGHT,
        kPageUp = GLFW_KEY_PAGE_UP,
        kPageDown = GLFW_KEY_PAGE_DOWN,
        kOnePastLast = GLFW_KEY_LAST+1
    };
    // Action of a key (mouse or keyboard)
    enum class Action : t::I32 {
        kPress = GLFW_PRESS,
        kRelease = GLFW_RELEASE
    };
    // Current state of the keyboard
    struct KeyData {
        // current data
        Key key = Key::kUnknown;
        Action action = Action::kRelease;
        // Keys that are currently being held by keyboard user
        std::unordered_set<Key> active;
    };
    // Current state of the mouse
    struct MouseData {
        // Current x,y
        t::Vec2 position = t::Vec2(0.0);
        // To calculate offset of mouse movement
        t::Vec2 last_position = t::Vec2(0.0);
        // How much mouse is moved
        t::Vec2 offset = t::Vec2(0.0);
        // When enabled, mouse data isn't calculated
        bool cursor_disabled = true;
        // Mouse movement sensitivity
        t::F32 sensitivity = 0.05f;
        // To determine first mouse touch
        bool first_move = true;
    };
    // Mouse buttons
    enum class MouseButton : t::I32 {
        kLeftButton = GLFW_MOUSE_BUTTON_LEFT,
        kRightButton = GLFW_MOUSE_BUTTON_RIGHT
    };
    struct MouseButtonData {
        MouseButton button;
        Action action = Action::kRelease;;
        t::I32 mods;
    };
    Input() = default;
    // store current key and action and change active key set
    void UpdateKeyData(Key key, Action action);
    void UpdateMouse(t::F32 xpos, t::F32 ypos);
    void UpdateMouseButton(MouseButton button, Action action, t::I32 mods);
    const KeyData& GetKeyData() const;
    MouseData& GetMouseData();
    const MouseButtonData& GetMouseButtonData() const;
    // Print current key state 
    void PrintKeyData() const;
private:
    KeyData key_data_;
    MouseData mouse_data_;
    MouseButtonData mouse_button_data_;
};
}; // platform