#include <tchar.h>
#include <stdexcept>
#include "DxLib.h"
#include "InputSystem.h"
#include "CommandSelect.h"
#include "ScriptSystem.h"

namespace {
    // ä÷êîÇÃñﬂÇËílÇÕ constexpr Ç…èoóàÇ»Ç¢ÇÃÇ≈ const Ç…Ç∑ÇÈ
    const auto messageColor = GetColor(255, 255, 255);
    const auto selectColor = GetColor(255, 255, 0);
}

namespace ss {

    CommandSelect::CommandSelect(InputSystem* inputSystem) : CommandBase(inputSystem) {
        type = Type::select;
        selectIndex = 0;
        commandString = nullptr;
    }

    CommandSelect::~CommandSelect() {
        for (auto& select : selectList) {
            delete select;
        }

        selectList.clear();

        if (commandString != nullptr) {
            delete commandString;
            commandString = nullptr;
        }
    }

    const std::string& CommandSelect::GetSelectLabel() const {
        if (selectIndex < 0 || selectIndex >= selectList.size()) {
            // ó·äOÇÃèàóùÇÃó·
            throw std::out_of_range(_T("CommandSelect : selectIndex out of range"));
        }

        auto select = selectList[selectIndex];

        return select->GetLabel();
    }

    bool CommandSelect::Init() {
        commandString = new CommandString();

        return true;
    }

    void CommandSelect::Finish() {
        CommandBase::Finish();

        selectIndex = 0;
    }

    void CommandSelect::Process() {
        auto selectSize = static_cast<int>(selectList.size());

        if (inputSystem->IsScriptUp()) {
            selectIndex--;

            if (selectIndex < 0) {
                selectIndex = selectSize - 1;
            }
        }
        else if (inputSystem->IsScriptDown()) {
            selectIndex++;

            if (selectIndex >= selectSize) {
                selectIndex = 0;
            }
        }
        else if (inputSystem->IsScriptDecided()) {
            isFinish = true;
        }
    }

    unsigned int CommandSelect::GetMessageColor(const int index) const {
        if (index == selectIndex) {
            return selectColor;
        }

        return messageColor;
    }

    void CommandSelect::Draw() const {
        if (commandString == nullptr) {
            return;
        }

        auto x = ScriptSystem::SELECT_START.x;
        auto y = ScriptSystem::SELECT_START.y;

        for (auto i = 0; i < selectList.size(); ++i) {
            auto select = selectList[i];
            auto& drawMessage = select->GetMessage();

            if (drawMessage.empty()) {
                continue;
            }

            auto message = commandString->GetString(drawMessage);

            DrawString(x, y, message, GetMessageColor(i));

            y += ScriptSystem::STRING_HEIGHT;
        }
    }

} // namespace ss
