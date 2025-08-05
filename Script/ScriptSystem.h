#pragma once
#include <string>
#include <vector>
#include <map>

class InputSystem;

namespace ss {
    class CommandBase;
    class Script;

    class ScriptSystem {
    public:
        struct Point {
            int x;
            int y;
        };

        static constexpr Point MESSAGE_START = { 400, 400 };
        static constexpr Point SELECT_START = { 500, 200 };
        static constexpr auto STRING_HEIGHT = 32;
        static constexpr auto STRING_SPEED = 25;

        ScriptSystem();
        virtual ~ScriptSystem();

        bool Init(InputSystem* inputSystem);
        int LoadScript(const std::string& filePath);
        Script* GetScript(const int index) const;

    private:
        Script* CreateScript(const std::vector<CommandBase*>& commandList) const;

        std::vector<Script*> scriptList;
        std::map<std::string, int> handleList;

        InputSystem* inputSystem;
    };

} // namespace ss
