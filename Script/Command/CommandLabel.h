#pragma once
#include "CommandBase.h"
#include <string>

class InputSystem;

namespace ss {

    class CommandLabel : public CommandBase {
    public:
        CommandLabel(InputSystem* inputSystem);
        virtual ~CommandLabel();

        const std::string& GetLabel() { return label; }
        void SetLabel(const std::string& label) { this->label = label; }

        void Process() override;

    private:
        std::string label;
    };

} // namespace ss
