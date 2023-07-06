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

/// <summary>
/// 更新
/// </summary>
/// <param name="input">keyの入力情報</param>
/// <param name="player">プレイヤーのポインタ</param>
/// <param name="pictographFlag">絵文字を操作しているかどうか</param>
/// <param name="stick">padの右スティックの入力情報</param>
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
	
	//プレイヤーに左右に動かした分のアングルを送る
	player->setAngle(sideAngle_);
	//カメラの座標更新
	updateCamera(player->GetPos());

}

/// <summary>
/// カメラの座標更新
/// </summary>
/// <param name="playerPos">プレイヤーの座標</param>
void Camera::updateCamera(VECTOR playerPos)
{
	//上下左右の角度を取得する
	cameraSideAngle_ = (cameraSideAngle_ * 0.8f) + (sideAngle_ * 0.2f);
	cameraVerAngle_ = (cameraVerAngle_ * 0.8f) + (verAngle_ * 0.2f);
	//Y行列回転の取得
	MATRIX cameraRotMtx = MGetRotY(cameraSideAngle_);
	//X行列回転の取得
	MATRIX cameraRotMtx2 = MGetRotX(cameraVerAngle_);

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

