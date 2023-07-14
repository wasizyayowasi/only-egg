#pragma once
#include "SceneBase.h"

class GameEnd : public SceneBase
{
public:
	GameEnd(SceneManager& manager,int stageNum,int resultHandle,int figureHandle[], int secondTime, int minuteTime,float remainingHp,int baconNum);
	virtual ~GameEnd();

	void update(const InputState& input);
	void draw();

	void externalFileExport();

private:

	int stageNum_ = 0;
	int resultHandle_ = 0;
	int figureHandle_[16];
	int cursorNum_ = 0;
	int minuteTime_ = 0;
	int secondTime_ = 0;
	int baconNum_ = 0;

	float remainingHp_ = 0.0f;

	bool questRewards[3];

	
};

