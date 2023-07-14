#pragma once
#include "DxLib.h"
#include "../util/InputState.h"
#include <memory>

class Player;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void update(const InputState& input, std::shared_ptr<Player> player,bool pictographFlag, DINPUT_JOYSTATE& stick);
	void updateCamera(VECTOR playerPos);

public:

	// �v���C���[�̌����Ă������
	float sideAngle_ = 0.0f;
	float verAngle_ = 0.0f;

	// �J�����̌����Ă������
	float cameraSideAngle_ = 0.0f;
	float cameraVerAngle_ = 0.0f;
	//�J�����̈ʒu
	VECTOR cameraPos_ = { 0.0f,600.0f,1500.0f };

};

