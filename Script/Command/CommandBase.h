#pragma once

class InputSystem;

namespace ss {

    class CommandBase {
    public:
        enum class Type : int {
            none,
            message,
            select,
            label,
            jump,
            draw,
            clear,
            wait
        };

        CommandBase(InputSystem* InputSystem) {
            type = Type::none;
            isFinish = false;
            this->inputSystem = InputSystem;
        }
        virtual ~CommandBase() {}

        Type GetType() const { return type; }
        bool IsFinish() const { return isFinish; }

        virtual bool Init() { return true; }
        virtual void Finish() { isFinish = false; }
        virtual void Process() {}
        virtual void Draw() const {}

    protected:
        Type type;
        bool isFinish;

        InputSystem* inputSystem;
    };

} // namespace ss
