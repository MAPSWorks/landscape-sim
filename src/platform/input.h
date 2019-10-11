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
        Key key;
        Action action;
        // Keys that are currently being held by keyboard user
        std::unordered_set<Key> active;
    };
    Input() = default;
    // store current key and action and change active key set
    void UpdateKeyData(Key key, Action action);
    const KeyData& GetKeyData() const;
    // Print current key state 
    void PrintKeyData() const;
private:
    KeyData key_data_;
};
}; // platform