#pragma once
#include <vector>
#include <string>
#include "CommandTime.h"
#include "CommandString.h"

class InputSystem;

namespace ss {

    class CommandMessage : public CommandTime {
    public:
        CommandMessage(InputSystem* inputSystem);
        virtual ~CommandMessage();

        const std::vector<std::string>& GetMessageList() { return messageList; }
        void AddMessage(const std::string& message) { messageList.emplace_back(message); }

        bool Init() override;
        void Finish() override;
        void Process() override;
        void Draw() const override;

    private:
        std::vector<std::string> messageList;
        std::vector<std::string> drawMessageList;

        CommandString* commandString;
    };

} // namespace ss
