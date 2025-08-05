#pragma once
#include "CommandTime.h"

class InputSystem;

namespace ss {

    class CommandWait : public CommandTime {
    public:
        CommandWait(InputSystem* inputSystem);
        virtual ~CommandWait();

        int GetWaitTime() { return waitTime; }
        void SetWaitTime(const int waitTime) { this->waitTime = waitTime; }

        void Finish() override;
        void Process() override;

    private:
        int waitTime;
    };

} // namespace ss
