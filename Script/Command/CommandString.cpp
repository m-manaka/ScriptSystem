#include <string.h>
#include <algorithm>
#include "CommandString.h"

namespace ss {

    CommandString::CommandString() {
        // string + CommandString::MAX_STRING_MAX �̓|�C���^���Z
        // �|�C���^�̌^�̃T�C�Y�������|�C���^��i�߂�
        // `\0` �� C ����̕�����̏I�[������
        std::fill(string, string + CommandString::MAX_STRING_MAX, '\0');
    }

    CommandString::~CommandString() {
    }

    // �A�X�L�[(���p)�����̏������Ή�����ׂ̏���
    const TCHAR* CommandString::GetString(const std::string& message) {
        auto messageSize = message.size();
        auto copySize = messageSize < MAX_STRING_MAX ? messageSize : MAX_STRING_MAX;

        // string.h ���K�v
        // strncpy �� C ����̊֐��ŁA������̃R�s�[���s��
        // strncpy_s �� strncpy �̈��S�łŁA�o�b�t�@�I�[�o�[������h��
        // ��{��I�� _s ���t���Ă���֐����g�p����ׂ�
        strncpy_s(string, message.c_str(), copySize);

        return string;
    }

} // namespace ss
