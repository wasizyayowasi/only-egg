#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Map;
class Player;
class Camera;
class Model;
class FryPan;
class Arrow;
class Model;

class ItemBase;

class GameMain : public SceneBase
{
public:
	GameMain(SceneManager& manager,int stageNum);
	virtual ~GameMain();

	void update(const InputState& input);
	void draw();

	void pictographUpdate(const InputState& input);

	void itemSetPosition();

	void checkCollision();

	void effectUpdate();

private:

	//アイテムの数
	int itemNum = 0;

	//画像関係
	int pictographHandle_[8];
	int pictographDiskHandle_ = -1;
	int menugraphHandle_ = -1;
	int resultgraphHandle_ = -1;
	int clockgraphHandle_[16];
	int gameOverHandle_ = -1;
	int optiongraphHandle_ = -1;
	//エフェクシア関係
	int effectHandle_ = -1;
	int playEffectHandle_[4];
	int effectTime_ = 0;
	bool effectIsEnable_[4];
	//シーン遷移関係
	bool deadFlag_ = false;
	//絵文字円盤の描画判断
	bool pictographFlag_ = false;
	//ステージ番号
	int stageNum_ = 0;
	//絵文字番号
	int pictographNum_ = -1;
	//表示時間の計測
	int pictographDisplayCount_ = 0;

	int tempTime_ = 0;
	int clearTimeSecond_ = 0;
	int clearTimeMinute_ = 0;

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	//フェード関係の変数
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	VECTOR cameraPos_ = VGet(0, 300, -600);
	VECTOR cameraTarget = VGet(0.0f, 0.0f, 0.0f);
	VECTOR stickRot = {0.0f,0.0f,0.0f};

	void (GameMain::* updateFunc_)(const InputState& input);

	std::shared_ptr<Map> map_;
	std::shared_ptr<FryPan> fryPan_;
	std::shared_ptr<Player> player_;
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Arrow> arrow_;
	std::vector<std::shared_ptr<Model>> bane_;

	std::vector<std::shared_ptr<ItemBase>> item_;

	DINPUT_JOYSTATE stick_;
};

