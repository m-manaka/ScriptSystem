#pragma once
#include "CommandBase.h"

class InputSystem;

namespace ss {

    class CommandTime : public CommandBase {
    public:
        CommandTime(InputSystem* inputSystem);
        virtual ~CommandTime();

        void Finish() override;
        void Process() override;

    protected:
        int startTime;
        int elapsedTime;
    };
} // namespace ss
