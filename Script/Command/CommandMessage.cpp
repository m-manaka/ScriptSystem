#include <tchar.h>
#include <string.h>
#include "DxLib.h"
#include "InputSystem.h"
#include "CommandMessage.h"
#include "ScriptSystem.h"

namespace {
    // ä÷êîÇÃñﬂÇËílÇÕ constexpr Ç…èoóàÇ»Ç¢ÇÃÇ≈ const Ç…Ç∑ÇÈ
    const auto messageColor = GetColor(255, 255, 255);
}

namespace ss {

    CommandMessage::CommandMessage(InputSystem* inputSystem) : CommandTime(inputSystem) {
        type = Type::message;
        commandString = nullptr;
    }

    CommandMessage::~CommandMessage() {
        messageList.clear();

        if (commandString != nullptr) {
            delete commandString;
            commandString = nullptr;
        }
    }

    bool CommandMessage::Init() {
        auto size = messageList.size();

        if (size == 0) {
            return false;
        }

        drawMessageList.resize(size);

        commandString = new CommandString();

        return true;
    }

    void CommandMessage::Finish() {
        CommandTime::Finish();

        for (auto& message : drawMessageList) {
            message.clear();
        }
    }

    void CommandMessage::Process() {
        CommandTime::Process();

        auto messageSize = static_cast<int>(messageList.size());
        auto drawCount = elapsedTime / ScriptSystem::STRING_SPEED;

        for (auto i = 0; i < messageSize; ++i) {
            auto& message = messageList[i];
            auto& drawMessage = drawMessageList[i];
            auto size = static_cast<int>(message.size());

            if (drawCount <= 0) {
                drawMessage.clear();
            }
            else if (drawCount < size) {
                drawMessage = message.substr(0, drawCount);
            }
            else if (drawCount >= size) {
                drawMessage = message;

                if (i >= messageSize - 1) {
                    isFinish = true;
                }
            }

            drawCount -= size;
        }
    }

    void CommandMessage::Draw() const {
        if (commandString == nullptr) {
            return;
        }

        auto x = ScriptSystem::MESSAGE_START.x;
        auto y = ScriptSystem::MESSAGE_START.y;

        for (auto& drawMessage : drawMessageList) {
            if (drawMessage.empty()) {
                break;
            }

            auto message = commandString->GetString(drawMessage);

            DrawString(x, y, message, messageColor);

            y += ScriptSystem::STRING_HEIGHT;
        }
    }

} // namespace ss
