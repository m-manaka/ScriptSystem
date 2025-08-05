#include "DxLib.h"
#include "Script.h"
#include "ScriptSystem.h"
#include "InputSystem.h"

namespace {
    constexpr auto WINDOW_TITLE = _T("Script System Sample");
    constexpr auto SCREEN_WIDTH = 1200;
    constexpr auto SCREEN_HEIGHT = 720;
    constexpr auto SCREEN_DEPTH = 32;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    auto window_mode = FALSE;

#ifdef _DEBUG
    window_mode = TRUE;
#endif

    SetMainWindowText(WINDOW_TITLE);
    ChangeWindowMode(window_mode);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);

    if (DxLib_Init() == -1) {
        return -1;
    }

    auto inputSystem = new InputSystem();
    auto scriptSystem = new ss::ScriptSystem();

    inputSystem->Init();
    scriptSystem->Init(inputSystem);

    auto handle = scriptSystem->LoadScript(_T("json/Scene00.json"));

    if (handle == -1) {
        DxLib_End();
        return -1;
    }

    auto script = scriptSystem->GetScript(handle);

    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() != -1) {
        if (!inputSystem->Process()) {
            break;
        }

        script->Process();

        ClearDrawScreen();
        script->Draw();
        ScreenFlip();
    }

    delete scriptSystem;

    DxLib_End();

    return 0;
}
