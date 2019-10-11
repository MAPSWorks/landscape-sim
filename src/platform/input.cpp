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

const Input::KeyData& Input::GetKeyData() const {
    return key_data_;
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