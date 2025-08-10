#include "DxLib.h"
#include "InputSystem.h"
#include "CommandTime.h"
#include "ScriptSystem.h"

namespace ss {

    CommandTime::CommandTime(InputSystem* inputSystem) : CommandBase(inputSystem) {
        startTime = 0;
        elapsedTime = 0;
    }

    CommandTime::~CommandTime() {
    }

    void CommandTime::Finish() {
        CommandBase::Finish();

        startTime = 0;
        elapsedTime = 0;
    }

    void CommandTime::Process() {
        if (startTime <= 0) {
            startTime = GetNowCount();
        }

        elapsedTime = GetNowCount() - startTime;
    }

} // namespace ss
