#include "Player.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "Map.h"
#include "objectHp.h"
#include <algorithm>

namespace {
	//ファイルパス
	const char* const egg_name = "data/object/egg01.mv1";
	const char* const sunny_egg_name = "data/object/sunnySideUp.mv1";
	//モデルのサイズ
	VECTOR eggScale = { 0.5f, 0.5f, 0.5f };
	//重力
	constexpr float gravity = -1.0f;
	//ジャンプ力
	constexpr float jump_power = 36.0f;
	//速さ
	constexpr float speed = 5.0f;
	//半径
	constexpr float radius = 15.0f;
	constexpr float collition_radius = 100.0f;

	// カメラの初期位置
	constexpr VECTOR camera_target{ 0, 800, -120 };

	// 旋回速度
	constexpr float rot_speed = 0.03f;
	constexpr float slope = 400.0f;

	// プレイヤーの移動量
	constexpr VECTOR player_vec{ 0, 0, speed };
	constexpr VECTOR player_vec2{ speed, 0,0 };
	//ジャンプ中の移動量
	constexpr VECTOR jump_vec2{ 2.0f, 0,0 };
	
	//プレイヤーの高さ
	constexpr float player_height = 30.0f;

	//初期位置
	constexpr VECTOR start_pos = { 0,30,200 };
	//ダメージ
	constexpr int damege = 20;
}

Player::Player()
{
	eggModel_ = std::make_shared<Model>(egg_name);
	sunnyEggModel_ = std::make_shared<Model>(sunny_egg_name);

	pos_ = start_pos;

	eggModel_->setPos(pos_);
	eggModel_->setScale(eggScale);

	hp_ = std::make_shared<objectHp>(400);

	cameraPos_ = { 0.0f,600.0f,1000.0f };

}

Player::~Player()
{
	MV1TerminateCollInfo(eggModel_->getModelHandle(), -1);
}

//Dxlib3Dアクション基本の当たり判定処理をそのまま持ってきたため
//詳しくはわからない
//要解析
void Player::checkCollisionStage(VECTOR moveVector)
{
	VECTOR oldPos;
	VECTOR nowPos;
	int i, j, k;
	int kabeNum;
	int yukaNum;
	bool moveFlag = false;
	bool hitFlag = false;

	//更新前のポジションを取得する
	oldPos = pos_;
	//更新後のポジションを取得する
	nowPos = VAdd(pos_, moveVector);
	//モデルと球の当たり判定
	HitDim = MV1CollCheck_Sphere(map_->getStage()->getModelHandle(), -1, oldPos, collition_radius + VSize(moveVector));

	//ベクトルの絶対値を取得し、動いたか動いていないかのフラグをとる
	if (fabs(moveVector.x) > 0.01f || fabs(moveVector.z) > 0.01f) {
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	kabeNum = 0;
	yukaNum = 0;

	//前にとったモデルと球の当たり判定処理
	for (i = 0; i < HitDim.HitNum; i++) {
		//モデルの法線ベクトル
		if (HitDim.Dim[i].Normal.y < 0.000001f && HitDim.Dim[i].Normal.y > -0.000001f) {
			if (HitDim.Dim[i].Position[0].y > oldPos.y + 1.0f ||
				HitDim.Dim[i].Position[1].y > oldPos.y + 1.0f ||
				HitDim.Dim[i].Position[2].y > oldPos.y + 1.0f) 
			{
				//壁ポリゴン情報の取得？
				if (kabeNum < max_hit_coll) {
					kabe[kabeNum] = &HitDim.Dim[i];
					kabeNum++;
				}
			
			}
		}
		else {
			//床ポリゴン情報の取得？
			if (yukaNum < max_hit_coll) {
				yuka[yukaNum] = &HitDim.Dim[i];
				yukaNum++;
			}
		}
	}

	//壁の処理
	if (kabeNum != 0) {
		hitFlag = false;
		//動いていたら
		if (moveFlag == true) {
			for (i = 0; i < kabeNum; i++) {
				poly = kabe[i];
				//プレイヤーを元にしたカプセルと壁ポリゴンの判定　　当たっていなかったらcontinue
				if (HitCheck_Capsule_Triangle(nowPos,VAdd(nowPos,VGet(0.0f,player_height,0.0f)),20.0f,poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE) continue;

				hitFlag = true;

				VECTOR slideVec;
				//プレイヤーのベクトルとポリゴンの法線ベクトルの外積を取得
				slideVec = VCross(moveVector, poly->Normal);
				//プレイヤーのベクトルとポリゴンの法線ベクトルの外積とポリゴンの外積の法線ベクトルの外積を取得
				slideVec = VCross(poly->Normal, slideVec);
				//更新前のプレイヤーのポジションと上記の外積を取得
				nowPos = VAdd(oldPos, slideVec);

				//また当たり判定？
				for (j = 0; j < kabeNum; j++) {
					poly = kabe[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, player_height, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) break;
				}

				//当たっていなかったらフラグを折る
				if (j == kabeNum) {
					hitFlag = false;
					break;
				}
			}
		}
	}
	else {
		//一つも壁とのhit情報がなかった場合
		for (i = 0; i < kabeNum; i++) {
			poly = kabe[i];
			if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, player_height, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) {
				hitFlag = false;
				break;
			}
		}

	}

	// 壁に当たっていたら壁から押し出す処理を行う
	if (hitFlag) {
		for (k = 0; k < 16; k++) {
			for (i = 0; i < kabeNum; i++) {
				poly = kabe[i];
				if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, player_height, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == FALSE) continue;
				nowPos = VAdd(nowPos, VScale(poly->Normal, 5.0f));
				for (int j = 0; j < kabeNum; j++) {
					poly = kabe[j];
					if (HitCheck_Capsule_Triangle(nowPos, VAdd(nowPos, VGet(0.0f, player_height, 0.0f)), 20.0f, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE) break;
				}
				if(j == kabeNum)break;
			}
			if (i != kabeNum)break;
		}
	}

	//床との当たり判定
	if (yukaNum != 0) {
	
		if (jump_ && jumpVec_ > 0.0f) {
			float minY = 0.0f;
			hitFlag = false;
			for (i = 0; i < yukaNum; i++) {
				poly = yuka[i];
				lineRes = HitCheck_Line_Triangle(nowPos, VAdd(nowPos, VGet(nowPos.x, nowPos.y + player_height, nowPos.z)), poly->Position[0], poly->Position[1], poly->Position[2]);
				if (lineRes.HitFlag == false)continue;
				if (hitFlag == 1 && minY < lineRes.Position.y)continue;
				hitFlag = true;
				minY = lineRes.Position.y;
			}
			if (hitFlag) {
				nowPos.y = minY - player_height;
			}
		}
		else {
			float maxY = 0.0f;
			hitFlag = false;
			for (i = 0; i < yukaNum; i++) {
				poly = yuka[i];
				if (jump_) {
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, player_height, 0.0f)), VAdd(nowPos, VGet(0.0f, -1.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				else {
					lineRes = HitCheck_Line_Triangle(VAdd(nowPos, VGet(0.0f, player_height, 0.0f)), VAdd(nowPos, VGet(0.0f, -40.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
				}
				if (lineRes.HitFlag == false) continue;
				if (hitFlag && maxY > lineRes.Position.y)continue;
				hitFlag = true;
				maxY = lineRes.Position.y;
			}
			if (hitFlag) {
				nowPos.y = maxY;
				jumpVec_ = -1.0f;
				if (jump_) {
					jump_ = false;
					hp_->onDamage(damege);
				}
			}
			else {
				jump_ = true;
			}
		}
	}

	pos_ = nowPos;
	eggModel_->setPos(pos_);
	sunnyEggModel_->setPos(pos_);
	MV1CollResultPolyDimTerminate(HitDim);

}

/// <summary>
/// プレイヤー情報の更新
/// </summary>
/// <param name="input">key入力情報</param>
void Player::update(const InputState& input)
{
//	MV1RefreshCollInfo(model_->getModelHandle(), -1);

	{//ジャンプ処理
		if (jump_) {
			jumpVec_ += gravity;
			pos_.y += jumpVec_;
			if (pos_.y <= 0.0f) {
				pos_.y = 0.0f;
				jumpVec_ = 0.0f;
				jump_ = false;
				if (landingCount_ > 0) {
					hp_->onDamage(damege);
					landingCount_--;
				}
			}
		}
	}


	lastPos_ = { pos_.x,pos_.y + 15,pos_.z };

	// プレイヤーが向いている方向によって進む方向を決める
	MATRIX playerRotMtx = MGetRotY(sideAngle_);
	VECTOR verMove = VTransform(player_vec, playerRotMtx);
	VECTOR sideMove;
	VECTOR moveVec = { 0.0f,0.0f,0.0f };

	{//移動処理

		if (input.isPressed(InputType::up)) {
			moveVec = VSub(moveVec,verMove);
		}
		if (input.isPressed(InputType::down)) {
			moveVec = verMove;
		}

		if (!jump_) {
			sideMove = VTransform(player_vec2, playerRotMtx);
		}
		else {
			sideMove = VTransform(jump_vec2, playerRotMtx);
		}

		if (input.isPressed(InputType::left)) {
			moveVec = VAdd(moveVec, sideMove);
		}
		if (input.isPressed(InputType::right)) {
			moveVec = VSub(moveVec, sideMove);
		}

	}

	//ジャンプ開始処理
	if (!jump_) {
		DrawFormatString(0, 45, 0xffffff, "%d", jump_);
		if (input.isPressed(InputType::space)) {
			jumpVec_ = jump_power;
			jump_ = true;
			landingCount_ = 1;
		}
	}

	//死亡処理
	if (hp_->getHp() <= 0.0f) {
		sunnyEggFlag_ = true;
	}

	if (!(moveVec.x == 0.00000f && moveVec.y == 0.00000f && moveVec.z == 0.00000f)) {
		moveVec = VScale(VNorm(moveVec),10.0f);
	}

	checkCollisionStage(moveVec);
}

/// <summary>
/// 描画
/// </summary>
void Player::draw()
{
	if (!sunnyEggFlag_ && !deadFlag_) {
		eggModel_->draw();
	}
	else {
		sunnyEggModel_->draw();
	}

	DrawFormatString(0, 100, 0xff0000, "%f :%f :%f", pos_.x, pos_.y, pos_.z);
	DrawLine3D(pos_, { pos_.x,pos_.y + player_height,pos_.z }, 0xff0000);

	hp_->draw();
	DrawFormatString(200, 40, 0xffffff, "%d", hp_->getHp());
}

/// <summary>
/// ステージ情報を受け取る
/// </summary>
/// <param name="map">ステージの情報</param>
void Player::setMapData(std::shared_ptr<Map> map)
{
	map_ = map;
}

/// <summary>
/// プレイヤーのポジションをセットする
/// </summary>
/// <param name="pos">引数で受け取ったポジション</param>
void Player::setPos(VECTOR pos)
{
	pos_ = pos;
	eggModel_->setPos(pos);
}

/// <summary>
/// 卵のモデルを取得する
/// </summary>
/// <returns>卵のモデル情報を返す</returns>
int Player::getModel()
{
	return eggModel_->getModelHandle();
}

/// <summary>
/// 卵の状態を変化させる
/// </summary>
void Player::stateChange()
{
	sunnyEggFlag_ = true;
}

/// <summary>
/// 卵の状態を取得する
/// </summary>
/// <returns>卵の状態を返す</returns>
bool Player::getState()
{
	return sunnyEggFlag_;
}

void Player::setAngle(float angle)
{
	sideAngle_ = angle;
}

void Player::resetInitial()
{
	jumpVec_ = 0.0f;
	jump_ = true;
	pos_ = start_pos;
	eggModel_->setPos(pos_);
}

/// <summary>
/// 卵の半径の取得
/// </summary>
/// <returns>卵の半径を返す</returns>
float Player::getRadius() const
{
	return radius;
}

int Player::getHp() const
{
	return hp_->getHp();
}

void Player::FlyAway()
{
	jumpVec_ = jump_power * 1.2f;
	jump_ = true;
}
