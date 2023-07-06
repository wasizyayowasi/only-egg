#pragma once
#include "SceneBase.h"
#include <memory>

class Player;

class PauseScene : public SceneBase
{
public:
	PauseScene(SceneManager& manager, std::shared_ptr<Player> player,int handle,int gameoverHandle);
	virtual ~PauseScene();

	void update(const InputState& input);
	void draw();
private:

	int handle_ = 0;
	int gameoverHandle_ = 0;
	int cursorNum_ = 0;

	std::shared_ptr<Player> player_;
};

