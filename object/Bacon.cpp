#include "Bacon.h"
#include "../util/Model.h"

/// <summary>
/// �������A�I���W�i���x�[�R���̃R���X�g���N�^
/// </summary>
/// <param name="fileName">�x�[�R�����f���̃t�@�C���p�X</param>
Bacon::Bacon(const char* const fileName) : ItemBase(fileName,{0,0,0},1.0f)
{
}

/// <summary>
/// �I���W�i�������ɂ��������x�[�R���̃R���X�g���N�^
/// </summary>
/// <param name="handle">�I���W�i���x�[�R���̃��f��</param>
Bacon::Bacon(int handle) : ItemBase(handle, { 200,0,0 }, 1.0f)
{
}