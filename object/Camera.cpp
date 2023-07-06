#include "Camera.h"
#include "../object/Player.h"
#include<math.h>
#include <cmath>

namespace{
	//�p�b�h�̉E�X�e�B�b�N�̌X��
	constexpr float slope = 400.0f;
	//�X���X�s�[�h
	constexpr float rot_speed = 0.03f;
}

Camera::Camera()
{
}

Camera::~Camera()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input">key�̓��͏��</param>
/// <param name="player">�v���C���[�̃|�C���^</param>
/// <param name="pictographFlag">�G�����𑀍삵�Ă��邩�ǂ���</param>
/// <param name="stick">pad�̉E�X�e�B�b�N�̓��͏��</param>
void Camera::update(const InputState& input, std::shared_ptr<Player> player, bool pictographFlag, DINPUT_JOYSTATE& stick)
{
	{//�J�����̊p�x����
		// ���E�L�[�Ő��񂷂�
		if (!pictographFlag) {
			if (stick.Rx < -slope || input.isPressed(InputType::tempLeft))
			{
				sideAngle_ -= rot_speed;
			}
			if (stick.Rx > slope || input.isPressed(InputType::tempRight))
			{
				sideAngle_ += rot_speed;
			}
			if (stick.Ry > slope || input.isPressed(InputType::tempUp))
			{
				verAngle_ -= rot_speed;
				if (verAngle_ < -1) {
					verAngle_ = -1;
				}
			}
			if (stick.Ry < -slope || input.isPressed(InputType::tempDown))
			{
				verAngle_ += rot_speed;
				if (verAngle_ > 0.9f) {
					verAngle_ = 0.9f;
				}
			}
		}
	}
	
	//�v���C���[�ɍ��E�ɓ����������̃A���O���𑗂�
	player->setAngle(sideAngle_);
	//�J�����̍��W�X�V
	updateCamera(player->GetPos());

}

/// <summary>
/// �J�����̍��W�X�V
/// </summary>
/// <param name="playerPos">�v���C���[�̍��W</param>
void Camera::updateCamera(VECTOR playerPos)
{
	//�㉺���E�̊p�x���擾����
	cameraSideAngle_ = (cameraSideAngle_ * 0.8f) + (sideAngle_ * 0.2f);
	cameraVerAngle_ = (cameraVerAngle_ * 0.8f) + (verAngle_ * 0.2f);
	//Y�s���]�̎擾
	MATRIX cameraRotMtx = MGetRotY(cameraSideAngle_);
	//X�s���]�̎擾
	MATRIX cameraRotMtx2 = MGetRotX(cameraVerAngle_);

	// Y���̃J�����̒Ǐ]
	// �W�����v���͒P���Ƀv���C���[�ɒǏ]����̂ł͂Ȃ�
	// �v���C���[�̗����Ă����ʒu������悤�ɂ���
	VECTOR cameraTrans = playerPos;
	cameraTrans.y = playerPos.y * 0.65f;
	MATRIX playerTransMtx = MGetTranslate(cameraTrans);


	// �v���C���[�̉�]�ɍ��킹�ăJ�����̈ʒu�A�����_����]������
	// �v���C���[�̉�]���ƕ��s�ړ���������
	MATRIX cameraMtx = MMult(cameraRotMtx2, cameraRotMtx);
	cameraMtx = MMult(cameraMtx, playerTransMtx);

	VECTOR cameraPos = VTransform(cameraPos_, cameraMtx);

	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(5.0f, 2800.0f);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTarget_UpVecY(cameraPos, playerPos);
}

