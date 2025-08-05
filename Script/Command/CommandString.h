#pragma once
#include <tchar.h>
#include <string>

namespace ss {

    class CommandString {
    public:
        static constexpr auto MAX_STRING_MAX = 512;

        CommandString();
        virtual ~CommandString();

        const TCHAR* GetString(const std::string& message);

    private:
        TCHAR string[MAX_STRING_MAX];
    };

} // namespace ss
