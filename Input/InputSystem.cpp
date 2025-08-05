#include <array>
#include "DxLib.h"
#include "InputSystem.h"

namespace {
    constexpr std::array<int, 16> padList = {
        DX_INPUT_PAD1,
        DX_INPUT_PAD2,
        DX_INPUT_PAD3,
        DX_INPUT_PAD4,
        DX_INPUT_PAD5,
        DX_INPUT_PAD6,
        DX_INPUT_PAD7,
        DX_INPUT_PAD8,
        DX_INPUT_PAD9,
        DX_INPUT_PAD10,
        DX_INPUT_PAD11,
        DX_INPUT_PAD12,
        DX_INPUT_PAD13,
        DX_INPUT_PAD14,
        DX_INPUT_PAD15,
        DX_INPUT_PAD16
    };

    // 今回の ScriptSystem のサンプルプロジェクト用に用意した
    // キー入力のトリガー判定用の変数なので、本来は InputSystem には不要のはず
    auto lastKeyUp = false;
    auto lastKeyDown = false;
    auto lastKeySpace = false;
}

InputSystem::InputSystem() {
}

InputSystem::~InputSystem() {
}

bool InputSystem::Init() {
    CheckPadNum();

    return true;
}

// 自動で接続されたパッドの数を取得して、stateList と triggerList をリサイズする
void InputSystem::CheckPadNum() {
    auto padNum = GetJoypadNum();

    if (stateList.size() != padNum) {
        stateList.resize(padNum);
        triggerList.resize(padNum);
    }
}

bool InputSystem::Process() {
    CheckPadNum();

    for (auto i = 0; i < stateList.size(); ++i) {
        auto pad = padList[i];
        auto state = GetJoypadInputState(pad);
        auto lastState = stateList[i];

        stateList[i] = state;
        triggerList[i] = (state ^ lastState) & state;
    }

    // エスケープキーが押されたら終了
    if (IsEscape()) {
        return false;
    }

    return true;
}

bool InputSystem::IsEscape() const {
    // メインループを抜けたい好きな入力判定にすれば良い
    return CheckHitKey(KEY_INPUT_ESCAPE) != 0;
}

bool InputSystem::IsScriptUp() const {
    // スクリプトの上移動を好きな入力判定にすれば良い
    auto nowKeyUp = CheckHitKey(KEY_INPUT_UP) != 0;
    auto isUp = nowKeyUp && !lastKeyUp;

    lastKeyUp = nowKeyUp;

    return isUp;
}

bool InputSystem::IsScriptDown() const {
    // スクリプトの下移動を好きな入力判定にすれば良い
    auto nowKeyDown = CheckHitKey(KEY_INPUT_DOWN) != 0;
    auto isDown = nowKeyDown && !lastKeyDown;

    lastKeyDown = nowKeyDown;

    return isDown;
}

bool InputSystem::IsScriptDecided() const {
    // スクリプトの決定を好きな入力判定にすれば良い
    auto nowKeySpace = CheckHitKey(KEY_INPUT_SPACE) != 0;
    auto isSpace = nowKeySpace && !lastKeySpace;

    lastKeySpace = nowKeySpace;

    return isSpace;
}
