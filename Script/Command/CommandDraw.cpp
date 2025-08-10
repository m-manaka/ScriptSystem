#include "DxLib.h"
#include "CommandDraw.h"
#include "InputSystem.h"

namespace ss {

    CommandDraw::CommandDraw(InputSystem* inputSystem) : CommandLabel(inputSystem) {
        type = Type::draw;
        handle = -1;
        x = 0;
        y = 0;
    }

    CommandDraw::~CommandDraw() {
    }

    void CommandDraw::Process() {
        isFinish = true;
    }

    void CommandDraw::Draw() const {
        if (handle == -1) {
            return;
        }

        DrawGraph(x, y, handle, TRUE);
    }

} // namespace ss
