#include <string.h>
#include <algorithm>
#include "CommandString.h"

namespace ss {

    CommandString::CommandString() {
        // string + CommandString::MAX_STRING_MAX はポインタ演算
        // ポインタの型のサイズ分だけポインタを進める
        // `\0` は C 言語の文字列の終端を示す
        std::fill(string, string + CommandString::MAX_STRING_MAX, '\0');
    }

    CommandString::~CommandString() {
    }

    // アスキー(半角)文字の処理も対応する為の処理
    const TCHAR* CommandString::GetString(const std::string& message) {
        auto messageSize = message.size();
        auto copySize = messageSize < MAX_STRING_MAX ? messageSize : MAX_STRING_MAX;

        // string.h が必要
        // strncpy は C 言語の関数で、文字列のコピーを行う
        // strncpy_s は strncpy の安全版で、バッファオーバーランを防ぐ
        // 基本手的に _s が付いている関数を使用するべき
        strncpy_s(string, message.c_str(), copySize);

        return string;
    }

} // namespace ss
