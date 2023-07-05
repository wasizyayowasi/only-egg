#include "Camera.h"
#include "../object/Player.h"
#include<math.h>
#include <cmath>

namespace{
	//パッドの右スティックの傾き
	constexpr float slope = 400.0f;
	//傾くスピード
	constexpr float rot_speed = 0.03f;
}

Camera::Camera()
{
}

Camera::~Camera()
{

}

void Camera::update(const InputState& input, std::shared_ptr<Player> player, bool pictographFlag, DINPUT_JOYSTATE& stick)
{
	{//カメラの角度調整
		// 左右キーで旋回する
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
	
	DrawFormatString(0, 90, 0xffffff, "%f", sideAngle_);

	player->setAngle(sideAngle_);
	updateCamera(player->GetPos());

}

void Camera::updateCamera(VECTOR playerPos)
{
	cameraAngle_ = (cameraAngle_ * 0.8f) + (sideAngle_ * 0.2f);
	cameraAngle2_ = (cameraAngle2_ * 0.8f) + (verAngle_ * 0.2f);

	MATRIX cameraRotMtx = MGetRotY(cameraAngle_);
	MATRIX cameraRotMtx2 = MGetRotX(cameraAngle2_);

	// Y軸のカメラの追従
	// ジャンプ時は単純にプレイヤーに追従するのではなく
	// プレイヤーの立っていた位置を見るようにする
	VECTOR cameraTrans = playerPos;
	cameraTrans.y = playerPos.y * 0.65f;
	MATRIX playerTransMtx = MGetTranslate(cameraTrans);


	// プレイヤーの回転に合わせてカメラの位置、注視点を回転させる
	// プレイヤーの回転情報と平行移動情報を合成
	MATRIX cameraMtx = MMult(cameraRotMtx2, cameraRotMtx);
	cameraMtx = MMult(cameraMtx, playerTransMtx);

	VECTOR cameraPos = VTransform(cameraPos_, cameraMtx);

	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(5.0f, 2800.0f);

	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);

	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(cameraPos, playerPos);
}

//void Camera::update(VECTOR playerPos)
//{
//	float addX = 0.0f, addY = 0.0f,addZ = 0.0f;
//
//	radius_ = angle_ * 3.14f / 180.0f;
//
//	addX = sin(radius_) * 300.0f;
//	addZ = cos(radius_) * 300.0f;
//
//	//addY = cos(verAngle_ * 3.14f / 180.0f) * ((addX * addX + addZ * addZ) * 300.0f);
//
//	pos_.x = playerPos.x + addX;
//	pos_.y = 300.0f;
//	//pos_.y = playerPos.y + addY;
//	pos_.z = playerPos.z + addZ;
//
//	GetJoypadDirectInputState(DX_INPUT_PAD1, &stick);
//
//	{//カメラの角度調整
//		// 左右キーで旋回する
//		if (stick.Rx < -slope)
//		{
//			angle_ -= rot_speed;
//		}
//		if (stick.Rx > slope)
//		{
//			angle_ += rot_speed;
//		}
//		if (stick.Ry > slope)
//		{
//			verAngle_ += rot_speed;
//		}
//		if (stick.Ry < -slope)
//		{
//			verAngle_ -= rot_speed;
//		}
//	}
//
//	DrawSphere3D(pos_, 20, 32, 0xffff00, 0xffff00, true);
//	//DrawCircleAA(pos_.x, pos_.z, 20, 32, 0xff0000, true);
//	SetCameraPositionAndTarget_UpVecY(pos_, playerPos);
//
//	DrawFormatString(0, 260, 0xffffff, "%f", angle_);
//	DrawFormatString(0, 275, 0xffffff, "%f", verAngle_);
//	DrawFormatString(0, 290, 0xffffff, "%f,%f,%f", pos_.x, pos_.y, pos_.z);
//
//}

