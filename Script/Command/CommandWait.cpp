#include "InputSystem.h"
#include "CommandWait.h"

namespace ss {

    CommandWait::CommandWait(InputSystem* inputSystem) : CommandTime(inputSystem) {
        type = Type::wait;
        waitTime = 0;
        elapsedTime = 0;
    }

    CommandWait::~CommandWait() {
    }

    void CommandWait::Finish() {
        CommandTime::Finish();

        elapsedTime = 0;
    }

    void CommandWait::Process() {
        // 指定の時間が経過したら終了
        if (waitTime > 0) {
            CommandTime::Process();

            if (elapsedTime >= waitTime) {
                isFinish = true;
            }
        }
        else {
            // 時間の指定がない場合は、決定されるまで待機
            if (inputSystem->IsScriptDecided()) {
                isFinish = true;
            }
        }
    }

} // namespace ss
