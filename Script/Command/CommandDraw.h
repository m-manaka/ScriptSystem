#pragma once
#include "CommandLabel.h"

class InputSystem;

namespace ss {

    class CommandDraw : public CommandLabel {
    public:
        CommandDraw(InputSystem* inputSystem);
        virtual ~CommandDraw();

        void SetHandle(const int handle) { this->handle = handle; }

        int GetX() { return x; }
        int GetY() { return y; }
        void SetX(const int x) { this->x = x; }
        void SetY(const int y) { this->y = y; }

        void Process() override;
        void Draw() const override;

    private:
        int handle;
        int x;
        int y;
    };

} // namespace ss
