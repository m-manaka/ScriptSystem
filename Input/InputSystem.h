#pragma once
#include <vector>

//!
//! InputSystem �̍ő�̖ړI��
//! GetJoypadInputState �Ȃǂ̏����� 1 �x������������Ηǂ��悤�ɂ��邱��
//! 1 �x������������A�ȍ~�� InputSystem ���v���W�F�N�g�ŋ��ʂŎg���悤�ɂ���
//!
class InputSystem {
public:
    InputSystem();
    virtual ~InputSystem();

    bool Init();
    bool Process();

    // public ���\�b�h�Ƀv���W�F�N�g�ŕK�v�ȓ��͏�����ǉ�����
    // ������v���W�F�N�g�ŋ��ʂŎg���悤�ɂ���

    // ����̃T���v���v���W�F�N�g�p�̓��͏���
    // �{���̓Q�[���̌����L�[���͂ɂȂǂɂ���
    bool IsScriptUp() const;
    bool IsScriptDown() const;
    bool IsScriptDecided() const;

 private:
     void CheckPadNum();
     bool IsEscape() const;

     std::vector<int> stateList;
     std::vector<int> triggerList;
};
