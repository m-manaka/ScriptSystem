#include "InputSystem.h"
#include "CommandJump.h"

namespace ss {

    CommandJump::CommandJump(InputSystem* inputSystem) : CommandLabel(inputSystem) {
        type = Type::jump;
    }

    CommandJump::~CommandJump() {
    }

    void CommandJump::Process() {
        // ƒWƒƒƒ“ƒv‚Í‘¦À‚ÉI—¹
        isFinish = true;
    }

} // namespace ss
