#pragma once
#include "DxLib.h"

class Model
{
public:
	Model(const char* fileName);
	Model(int orgModel);

	virtual ~Model();

	void draw();

	int getModelHandle() const { return modelHandle_; }
	int getColFrameIndex() const { return -1; }

	void setPos(VECTOR pos);
	void setRot(VECTOR rot);
	void setScale(VECTOR scale);

	bool isEnabled() { return isEnable_; }
	void setEnable() { isEnable_ = false; }

private:
	int modelHandle_;

	bool isEnable_ = false;
};

