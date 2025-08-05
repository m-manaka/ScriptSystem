#include "DxLib.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "CommandMessage.h"
#include "CommandSelect.h"
#include "CommandLabel.h"
#include "CommandJump.h"
#include "CommandDraw.h"
#include "CommandClear.h"
#include "CommandWait.h"
#include "Script.h"
#include "ScriptSystem.h"
#include "InputSystem.h"

namespace {
    constexpr auto EMPTY_STR = _T("");
    constexpr auto EMPTY_WSTR = L"";

    constexpr auto COMMAND = _T("command");
    constexpr auto PARAMETER_BASE = _T("parameter0");
    constexpr auto PARAMETER_00 = _T("parameter00");
    constexpr auto PARAMETER_01 = _T("parameter01");
    constexpr auto PARAMETER_02 = _T("parameter02");

    constexpr auto COMMAND_HANDLE = _T("handle");
    constexpr auto COMMAND_MESSAGE = _T("message");
    constexpr auto COMMAND_SELECT = _T("select");
    constexpr auto COMMAND_LABEL = _T("label");
    constexpr auto COMMAND_JUMP = _T("jump");
    constexpr auto COMMAND_DRAW = _T("draw");
    constexpr auto COMMAND_WAIT = _T("wait");
    constexpr auto COMMAND_CLEAR = _T("clear");

    //!
    //! @fn std::wstring ScriptsData::ConvertUTF8ToWide(const std::string& utf8)
    //! @brief UTF-8 文字コードの std::string を std::wstring(UTF-16) に変換する
    //! @param[in] utf8 UTF-8 文字コードの std::string
    //! @return UTF-16 文字コードの std::wstring
    //!
    std::wstring ConvertUTF8ToWide(const std::string& utf8) {
        if (utf8.empty()) {
            return EMPTY_WSTR;
        }

        const auto in_length = static_cast<int>(utf8.length());
        const auto out_length = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), in_length, 0, 0);

        if (out_length <= 0) {
            return EMPTY_WSTR;
        }

        std::vector<wchar_t> buffer(out_length);

        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), in_length, &(buffer[0]), out_length);

        std::wstring utf16(buffer.begin(), buffer.end());

        return utf16;
    }

    //!
    //! @fn std::string ScriptsData::ConvertWideToMultiByte(const std::wstring& utf16)
    //! @brief std::wstring(UTF-16) をマルチバイト文字コードの std::string に変換する
    //! @param[in] utf16 UTF-16 文字コードの std::wstring
    //! @return マルチバイト文字コードの std::string
    //!
    std::string ConvertWideToMultiByte(const std::wstring& utf16) {
        if (utf16.empty()) {
            return EMPTY_STR;
        }

        const auto in_length = static_cast<int>(utf16.length());
        const auto out_length = WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), in_length, 0, 0, 0, 0);

        if (out_length <= 0) {
            return EMPTY_STR;
        }

        std::vector<char> buffer(out_length);

        WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), in_length, &(buffer[0]), out_length, 0, 0);

        std::string mbs(buffer.begin(), buffer.end());

        return mbs;
    }

    //!
    //! UTF-8 文字列をマルチバイト文字列に変換して DX ライブラリで扱えるようにする
    //!
    std::string ConvertString(const std::string& utf8) {
        const auto utf16 = ConvertUTF8ToWide(utf8);
        return ConvertWideToMultiByte(utf16);
    }

    //!
    //! 指定したパラメータ名を std::string に取得する
    //!
    void GetParameter(const nlohmann::json& json, const std::string& parameter, std::string& setString) {
        std::string getString;

        json.at(parameter).get_to(getString);

        if (!getString.empty()) {
            setString = ConvertString(getString);
        }
    }
}

namespace ss {

        ScriptSystem::ScriptSystem() {
            inputSystem = nullptr;
        }

        ScriptSystem::~ScriptSystem() {
            for (auto& script : scriptList) {
                delete script;
            }

            scriptList.clear();
        }

        bool ScriptSystem::Init(InputSystem* inputSystem) {
            this->inputSystem = inputSystem;

            return true;
        }

        int ScriptSystem::LoadScript(const std::string& filePath) {
            if (inputSystem == nullptr || filePath.empty()) {
                return -1;
            }

            std::ifstream file(filePath);
            nlohmann::json json;

            file >> json;

            std::vector<CommandBase*> commandList;

            for (auto& data : json) {
                std::string getCommand;
                GetParameter(data, COMMAND, getCommand);

                // スプレッドシート側の空行をスキップ
                if (getCommand.empty()) {
                    continue;
                }

                if (getCommand == COMMAND_HANDLE) {
                    std::string getFilePath;
                    std::string getHandleLabel;

                    GetParameter(data, PARAMETER_00, getFilePath);
                    GetParameter(data, PARAMETER_01, getHandleLabel);

                    if (!getFilePath.empty() && !getHandleLabel.empty()) {
                        // すでに読み込んでいるハンドルは読み込まない
                        if (handleList.find(getHandleLabel) == handleList.end()) {
                            auto handle = LoadGraph(getFilePath.c_str());

                            if (handle != -1) {
                                handleList[getHandleLabel] = handle;
                            }
                        }
                    }
                }
                else if (getCommand == COMMAND_MESSAGE) {
                    auto commandMessage = new CommandMessage(inputSystem);

                    for (int i = 0; i < 4; i++) {
                        std::string getMessage;
                        auto parameter = PARAMETER_BASE + std::to_string(i);

                        GetParameter(data, parameter, getMessage);

                        if (!getMessage.empty()) {
                            commandMessage->AddMessage(getMessage);
                        }
                    }

                    commandMessage->Init(); 
                    commandList.emplace_back(commandMessage);
                }
                else if (getCommand == COMMAND_SELECT) {
                    auto commandSelect = new CommandSelect(inputSystem);

                    for (int i = 0; i < 4; i += 2) {
                        std::string getMessage;
                        std::string getLabel;
                        auto parameter0 = PARAMETER_BASE + std::to_string(i);
                        auto parameter1 = PARAMETER_BASE + std::to_string(i + 1);

                        GetParameter(data, parameter0, getMessage);
                        GetParameter(data, parameter1, getLabel);

                        if (!getMessage.empty() && !getLabel.empty()) {
                            auto select = new CommandSelect::Select();

                            select->SetMessage(getMessage);
                            select->SetLabel(getLabel);

                            commandSelect->GetSelectList().emplace_back(select);
                        }
                    }

                    commandSelect->Init();
                    commandList.emplace_back(commandSelect);
                }
                else if (getCommand == COMMAND_LABEL) {
                    auto commandLabel = new CommandLabel(inputSystem);
                    std::string getLabel;

                    GetParameter(data, PARAMETER_00, getLabel);

                    commandLabel->SetLabel(getLabel);
                    commandLabel->Init();
                    commandList.emplace_back(commandLabel);
                }
                else if (getCommand == COMMAND_JUMP) {
                    auto commandJump = new CommandJump(inputSystem);
                    std::string getLabel;

                    GetParameter(data, PARAMETER_00, getLabel);

                    commandJump->SetLabel(getLabel);
                    commandJump->Init();
                    commandList.emplace_back(commandJump);
                }
                else if (getCommand == COMMAND_DRAW) {
                    std::string getDrawLabel;

                    GetParameter(data, PARAMETER_00, getDrawLabel);

                    if (!getDrawLabel.empty()) {
                        if (handleList.find(getDrawLabel) != handleList.end()) {
                            auto commandDraw = new CommandDraw(inputSystem);

                            commandDraw->SetLabel(getDrawLabel);
                            commandDraw->SetHandle(handleList[getDrawLabel]);

                            int x, y;

                            data.at(PARAMETER_01).get_to(x);
                            data.at(PARAMETER_02).get_to(y);

                            commandDraw->SetX(x);
                            commandDraw->SetY(y);
                            commandDraw->Init();
                            commandList.emplace_back(commandDraw);
                        }
                    }
                }
                else if (getCommand == COMMAND_CLEAR) {
                    auto commandClear = new CommandClear(inputSystem);
                    std::string getLabel;

                    GetParameter(data, PARAMETER_00, getLabel);

                    commandClear->SetLabel(getLabel);
                    commandClear->Init();
                    commandList.emplace_back(commandClear);
                }
                else if (getCommand == COMMAND_WAIT) {
                    auto commandWait = new CommandWait(inputSystem);
                    int getWaitTime;

                    data.at(PARAMETER_00).get_to(getWaitTime);

                    commandWait->SetWaitTime(getWaitTime);
                    commandWait->Init();
                    commandList.emplace_back(commandWait);
                }
            }

            auto script = CreateScript(commandList);

            if (script == nullptr) {
                return -1;
            }

            // static_cast は C++ のキャスト演算子
            auto index = static_cast<int>(scriptList.size());

            scriptList.emplace_back(script);

            return index;
        }

        Script* ScriptSystem::GetScript(const int index) const {
            if (index < 0 || index >= scriptList.size()) {
                return nullptr;
            }

            return scriptList[index];
        }

        Script* ScriptSystem::CreateScript(const std::vector<CommandBase*>& commandList) const {
            if (commandList.empty()) {
                return nullptr;
            }

            auto script = new Script();

            if (!script->Init(commandList)) {
                delete script;
                return nullptr;
            }

            return script;
        }

} // namespace ss
