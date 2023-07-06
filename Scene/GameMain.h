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
class Bacon;

class GameMain : public SceneBase
{
public:
	GameMain(SceneManager& manager,int stageNum);
	GameMain(SceneManager& manager);
	virtual ~GameMain();

	void update(const InputState& input);
	void draw();
	void pictographUpdate(const InputState& input);

private:

	//‰æ‘œŠÖŒW
	int pictographHandle_[8];
	int pictographDiskHandle_ = -1;
	int menugraphHandle_ = -1;
	int resultgraphHandle_ = -1;
	int clockgraphHandle_[16];
	int gameOverHandle_ = -1;
	//ƒV[ƒ“‘JˆÚŠÖŒW
	bool deadFlag_ = false;
	//ŠG•¶š‰~”Õ‚Ì•`‰æ”»’f
	bool pictographFlag_ = false;
	//ƒXƒe[ƒW”Ô†
	int stageNum_ = 0;
	//ŠG•¶š”Ô†
	int pictographNum_ = -1;
	//•\¦ŠÔ‚ÌŒv‘ª
	int pictographDisplayCount_ = 0;

	int tempTime_ = 0;
	int clearTimeSecond_ = 0;
	int clearTimeMinute_ = 0;

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	//ƒtƒF[ƒhŠÖŒW‚Ì•Ï”
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
	std::vector<std::shared_ptr<Bacon>> bacon_;
	std::shared_ptr<Model> arrow_;

	DINPUT_JOYSTATE stick_;
};

