#include "InputSystem.h"
#include "CommandLabel.h"

namespace ss {

    CommandLabel::CommandLabel(InputSystem* inputSystem) : CommandBase(inputSystem) {
        type = Type::label;
    }

    CommandLabel::~CommandLabel() {
    }

    void CommandLabel::Process() {
        // ���x���͑����ɏI��
        isFinish = true;
    }

} // namespace ss
