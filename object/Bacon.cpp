#include "Bacon.h"
#include "../util/Model.h"

namespace {
	
}

/// <summary>
/// �������A�I���W�i���x�[�R���̃R���X�g���N�^
/// </summary>
/// <param name="fileName">�x�[�R�����f���̃t�@�C���p�X</param>
Bacon::Bacon(const char* const fileName)
{
	bacon_ = std::make_shared<Model>(fileName);

	pos_ = { 0,0,0 };

	bacon_->setPos(pos_);

	bacon_->setRot({1.55f,rotation_,0});

}

/// <summary>
/// �I���W�i�������ɂ��������x�[�R���̃R���X�g���N�^
/// </summary>
/// <param name="handle">�I���W�i���x�[�R���̃��f��</param>
Bacon::Bacon(int handle)
{
	bacon_ = std::make_shared<Model>(handle);

	pos_ = { 200,0,0 };

	bacon_->setPos(pos_);

	bacon_->setRot({ 1.55f,rotation_,0 });
}

Bacon::~Bacon()
{
}

/// <summary>
/// �X�V
/// </summary>
void Bacon::update()
{
	rotation_ += 0.01f;

	bacon_->setRot({ 1.55f,rotation_,0 });
}

/// <summary>
/// �`��
/// </summary>
void Bacon::draw()
{
	if (bacon_->isEnabled()) {
		bacon_->draw();
	}
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
/// <returns>���f���n���h����Ԃ�</returns>
int Bacon::getModel()
{
	return bacon_->getModelHandle();
}

/// <summary>
/// �t���[���̎擾
/// </summary>
/// <returns>�t���[����Ԃ�</returns>
int Bacon::getFrameIndex()
{
	return bacon_->getColFrameIndex();
}

/// <summary>
/// �x�[�R�����t�B�[���h�ɑ��݂��Ă��邩
/// </summary>
/// <returns>�x�[�R���̗L����Ԃ�</returns>
bool Bacon::isEnabled()
{
	return bacon_->isEnabled();
}

/// <summary>
/// �x�[�R�����t�B�[���h�ɑ��݂��Ă��邩�ǂ�����ύX����
/// </summary>
void Bacon::setIsEnabled()
{
	bacon_->setIsEnable();
}
