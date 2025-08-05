#pragma once
#include <vector>
#include <string>
#include <map>

namespace ss {
    class CommandBase;

    class Script {
    public:
        Script();
        virtual ~Script();

        bool Init(const std::vector<CommandBase*>& commandList);
        void Process();
        void Draw() const;

    private:
        bool InitCommand();
        
        bool SetCurrentCommand();
        bool CheclLabelCommand();
        bool SetLabelCommand(const std::string& label);

        void SetDrawCommand();
        void CheckDrawCommand(CommandBase* currentTarget);

        std::vector<CommandBase*> commandList;
        std::vector<CommandBase*> drawList;
        std::map<std::string, int> labelList;

        CommandBase* currentCommand;
        int currentCommandIndex;
    };

} // namespace ss