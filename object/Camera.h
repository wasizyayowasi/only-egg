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

	// プレイヤーの向いている方向
	float sideAngle_ = 0.0f;
	float verAngle_ = 0.0f;

	// カメラの向いている方向
	float cameraSideAngle_ = 0.0f;
	float cameraVerAngle_ = 0.0f;
	//カメラの位置
	VECTOR cameraPos_ = { 0.0f,600.0f,1500.0f };

};

