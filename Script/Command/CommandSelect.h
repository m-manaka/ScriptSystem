#pragma once
#include <vector>
#include <string>
#include "CommandBase.h"
#include "CommandString.h"

class InputSystem;

namespace ss {

    class CommandSelect : public CommandBase {
    public:
        class Select {
        public:
            Select() {}
            virtual ~Select() {}

            const std::string& GetMessage() { return message; }
            const std::string& GetLabel() { return label; }

            void SetMessage(const std::string& message) { this->message = message; }
            void SetLabel(const std::string& label) { this->label = label; }

        private:
            std::string message;
            std::string label;
        };

        CommandSelect(InputSystem* inputSystem);
        virtual ~CommandSelect();

        std::vector<Select*>& GetSelectList() { return selectList; }
        // noexcept(false) ‚Í—áŠO‚ğ“Š‚°‚é‰Â”\«‚ª‚ ‚é‚±‚Æ‚ğ¦‚·
        const std::string& GetSelectLabel() const noexcept(false);

        bool Init() override;
        void Finish() override;
        void Process() override;
        void Draw() const override;

    private:
        unsigned int GetMessageColor(const int index) const;

        std::vector<Select*> selectList;
        int selectIndex;

        CommandString* commandString;
    };
} // namespace ss
