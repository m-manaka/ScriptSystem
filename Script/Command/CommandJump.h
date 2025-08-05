#pragma once
#include "CommandLabel.h"
#include <string>

class InputSystem;

namespace ss {

    class CommandJump : public CommandLabel {
        public:
        CommandJump(InputSystem* InputSystem);
        virtual ~CommandJump();

        void Process() override;
    };

} // namespace ss
