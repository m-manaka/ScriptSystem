#pragma once
#include "CommandLabel.h"

class InputSystem;

namespace ss {

    class CommandClear : public CommandLabel {
    public:
        CommandClear(InputSystem* InputSystem);
        virtual ~CommandClear();
    };

} // namespace ss
