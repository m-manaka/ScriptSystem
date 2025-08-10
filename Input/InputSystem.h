#pragma once
#include <vector>

//!
//! InputSystem の最大の目的は
//! GetJoypadInputState などの処理を 1 度だけ処理すれば良いようにすること
//! 1 度処理をしたら、以降は InputSystem をプロジェクトで共通で使うようにする
//!
class InputSystem {
public:
    InputSystem();
    virtual ~InputSystem();

    bool Init();
    bool Process();

    // public メソッドにプロジェクトで必要な入力処理を追加する
    // それをプロジェクトで共通で使うようにする

    // 今回のサンプルプロジェクト用の入力処理
    // 本来はゲームの決定やキー入力になどにする
    bool IsScriptUp() const;
    bool IsScriptDown() const;
    bool IsScriptDecided() const;

 private:
     void CheckPadNum();
     bool IsEscape() const;

     std::vector<int> stateList;
     std::vector<int> triggerList;
};
