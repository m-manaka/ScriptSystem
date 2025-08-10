#include <stdexcept>
#include <algorithm>
#include <functional>
#include "Script.h"
#include "CommandBase.h"
#include "CommandSelect.h"
#include "CommandLabel.h"
#include "CommandJump.h"
#include "CommandDraw.h"
#include "CommandClear.h"

namespace {
    bool CompareRemoveMessageCommand(ss::CommandBase* target) {
        return target->GetType() == ss::CommandBase::Type::message;
    }

    bool CompareRemoveSelectCommand(ss::CommandBase* target) {
        return target->GetType() == ss::CommandBase::Type::select;
    }
}

namespace ss {

    Script::Script() {
        currentCommand = nullptr;
        currentCommandIndex = 0;
    }

    Script::~Script() {
        labelList.clear();
        drawList.clear();

        for (auto& command : commandList) {
            delete command;
        }

        commandList.clear();
    }

    bool Script::Init(const std::vector<CommandBase*>& commandList) {
        this->commandList = commandList;

        return InitCommand();
    }

    bool Script::InitCommand() {
        for (auto i = 0; i < commandList.size(); ++i) {
            auto command = commandList[i];
            auto type = command->GetType();

            switch (type) {
            case CommandBase::Type::message:
            case CommandBase::Type::select:
            case CommandBase::Type::draw:
            case CommandBase::Type::wait:
                if (currentCommand == nullptr) {
                    currentCommand = command;
                    currentCommand->Finish();
                    currentCommandIndex = i;

                    SetDrawCommand();
                }
                break;

            case CommandBase::Type::label:
                auto label = dynamic_cast<CommandLabel*>(command);
                auto& labelString = label->GetLabel();

                // ���ꂪ�{���� emplace(_back) �̎g����
                labelList.emplace(labelString, i);
                break;
            }
        }

        return true;
    }

    bool Script::CheclLabelCommand() {
        if (currentCommand == nullptr) {
            return false;
        }

        auto type = currentCommand->GetType();

        if (type == CommandBase::Type::select) {
            auto label = dynamic_cast<CommandSelect*>(currentCommand);

            if (!label->IsFinish()) {
                return false;
            }

            try {
                const auto& labelString = label->GetSelectLabel();

                SetLabelCommand(labelString);

                return true;
            }
            catch (const std::out_of_range& e) {
                return false;
            }
        }
        else if (type == CommandBase::Type::jump) {
            auto jump = dynamic_cast<CommandJump*>(currentCommand);
            auto& labelString = jump->GetLabel();

            SetLabelCommand(labelString);

            return true;
        }

        return false;
    }

    bool Script::SetLabelCommand(const std::string& label) {
        if (labelList.find(label) == labelList.end()) {
            return false;
        }

        // ���x���̏ꍇ�͕`��͑S�ăN���A
        drawList.clear();

        auto index = labelList[label];

        currentCommand = commandList[index];
        currentCommand->Finish();
        currentCommandIndex = index;

        return true;
    }

    bool Script::SetCurrentCommand() {
        if (currentCommandIndex < 0 || currentCommandIndex >= commandList.size()) {
            return false;
        }

        if (CheclLabelCommand()) {
            return true;
        }

        for (auto i = currentCommandIndex + 1; i < commandList.size(); ++i) {
            auto command = commandList[i];
            auto type = command->GetType();

            switch (type) {
            case CommandBase::Type::message:
            case CommandBase::Type::select:
            case CommandBase::Type::draw:
            case CommandBase::Type::wait:
                CheckDrawCommand(command);

                currentCommand = command;
                currentCommand->Finish();
                currentCommandIndex = i;

                SetDrawCommand();
                return true;

            case CommandBase::Type::clear:
                CheckDrawCommand(command);
                break;

            case CommandBase::Type::jump:
                auto commandJump = dynamic_cast<CommandJump*>(command);
                auto& label = commandJump->GetLabel();

                if (labelList.find(label) == labelList.end()) {
                    return false;
                }

                auto index = labelList[label];
                auto current = commandList[index];

                CheckDrawCommand(current);

                currentCommand = current;
                currentCommand->Finish();
                currentCommandIndex = index;
                return true;
            }
        }

        return false;
    }

    void Script::SetDrawCommand() {
        if (currentCommand == nullptr) {
            return;
        }

        auto type = currentCommand->GetType();

        switch (type) {
        case CommandBase::Type::message:
        case CommandBase::Type::select:
        case CommandBase::Type::draw:
            drawList.emplace_back(currentCommand);
            break;
        }
    }

    void Script::CheckDrawCommand(CommandBase* currentTarget) {
        if (currentTarget == nullptr) {
            return;
        }

        auto type = currentTarget->GetType();
        auto removeBegin = drawList.end(); // �C�e���[�^�[�̍Ō���w���Ă���
        std::function<bool(CommandBase*)> compareFunc = nullptr;

        switch (type) {
        case CommandBase::Type::message:
            compareFunc = &CompareRemoveMessageCommand;
            // remove_if �͏����𖞂����v�f�� vector �̍Ō�Ɉړ�������
            removeBegin = std::remove_if(drawList.begin(), drawList.end(), compareFunc);

            // remove_if �ňړ��������v�f�� erase �ō폜����
            if (removeBegin != drawList.end()) {
                drawList.erase(removeBegin, drawList.end());
            }
            break;

        case CommandBase::Type::select:
            compareFunc = &CompareRemoveSelectCommand;
            // remove_if �͏����𖞂����v�f�� vector �̍Ō�Ɉړ�������
            removeBegin = std::remove_if(drawList.begin(), drawList.end(), compareFunc);

            // remove_if �ňړ��������v�f�� erase �ō폜����
            if (removeBegin != drawList.end()) {
                drawList.erase(removeBegin, drawList.end());
            }
            break;

        case CommandBase::Type::clear:
            // �����_���ŏ������w�肷��
            // �����_���Ƃ͕ϐ��̂悤�Ɉ�����֐��̂���(�ϐ��ɑ�����\)
            // �����_���̌`
            // [�L���v�`�����X�g](�������X�g) -> �߂�l�̌^ { �֐��̏��� }
            compareFunc = [&currentTarget](CommandBase* target) {
                if (target->GetType() == CommandBase::Type::draw) {
                    auto commandDraw = dynamic_cast<CommandDraw*>(target);
                    auto commandClear = dynamic_cast<CommandClear*>(currentTarget);

                    if (commandDraw->GetLabel() == commandClear->GetLabel()) {
                        return true;
                    }
                }

                return false;
            };

            // remove_if �͏����𖞂����v�f�� vector �̍Ō�Ɉړ�������
            removeBegin = std::remove_if(drawList.begin(), drawList.end(), compareFunc);

            // remove_if �ňړ��������v�f�� erase �ō폜����
            if (removeBegin != drawList.end()) {
                drawList.erase(removeBegin, drawList.end());
            }
            break;

        case CommandBase::Type::label:
        case CommandBase::Type::jump:
            drawList.clear();
            break;
        }
    }

    void Script::Process() {
        if (currentCommand == nullptr) {
            return;
        }

        currentCommand->Process();

        if (currentCommand->IsFinish()) {
            SetCurrentCommand();
        }
    }

    void Script::Draw() const {
        if (drawList.size() <= 0) {
            return;
        }

        for (auto& command : drawList) {
            command->Draw();
        }
    }

} // namespace ss
