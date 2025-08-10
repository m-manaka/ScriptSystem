#include "InputSystem.h"
#include "CommandJump.h"

namespace ss {

    CommandJump::CommandJump(InputSystem* inputSystem) : CommandLabel(inputSystem) {
        type = Type::jump;
    }

    CommandJump::~CommandJump() {
    }

    void CommandJump::Process() {
        // �W�����v�͑����ɏI��
        isFinish = true;
    }

} // namespace ss
