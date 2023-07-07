#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class FryPan
{
public:
	FryPan();
	virtual ~FryPan();

	/// <summary>
	/// �`��
	/// </summary>
	void draw();

	/// <summary>
	/// ���f���n���h���̎擾
	/// </summary>
	/// <returns>���f���n���h����Ԃ�</returns>
	int getModel();

	/// <summary>
	/// �t���[���̎擾
	/// </summary>
	/// <returns>�t���[���̒l��Ԃ�</returns>
	int getFrameIndex();

	/// <summary>
	/// �|�W�V�����̎擾
	/// </summary>
	VECTOR getPos();

	float getRadius();

private:

	std::shared_ptr<Model> fryPan_;
	VECTOR pos_;

};

