#include "InputSystem.h"
#include "CommandClear.h"

namespace ss {

    CommandClear::CommandClear(InputSystem* inputSystem) : CommandLabel(inputSystem) {
        type = Type::clear;
    }

    CommandClear::~CommandClear() {
    }

} // namespace ss