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
        // �w��̎��Ԃ��o�߂�����I��
        if (waitTime > 0) {
            CommandTime::Process();

            if (elapsedTime >= waitTime) {
                isFinish = true;
            }
        }
        else {
            // ���Ԃ̎w�肪�Ȃ��ꍇ�́A���肳���܂őҋ@
            if (inputSystem->IsScriptDecided()) {
                isFinish = true;
            }
        }
    }

} // namespace ss
