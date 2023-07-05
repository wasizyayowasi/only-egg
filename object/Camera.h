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
//	void update(VECTOR playerPos);

public:

	// プレイヤーの向いている方向
	float sideAngle_ = 0.0f;
	float verAngle_ = 0.0f;

	// カメラの向いている方向
	float cameraAngle_ = 0.0f;
	float cameraAngle2_ = 0.0f;
	//カメラの位置
	VECTOR cameraPos_ = { 0.0f,600.0f,1000.0f };





//	float radius_ = 0.0f;
//	float angle_ = 0.0f;
//	float verAngle_ = 0.0f;
//	VECTOR pos_;

	
};

