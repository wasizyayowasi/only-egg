#pragma once
#include<memory>
#include "DxLib.h"

class Model;

class ItemBase
{
public:

	ItemBase(const char* const fileName,VECTOR pos,float scale);
	ItemBase(int handle,VECTOR pos,float scale);
	virtual ~ItemBase();

	virtual void update();
	void draw();

	int getModelHandle();

	int getFrameIndex();

	bool isEnabled();
	void setEnabled();

	VECTOR getPos() { return pos_; }
	void setPos(VECTOR pos);

private:

	VECTOR pos_;

	VECTOR rotation_ = { 0,0,0 };

	std::shared_ptr<Model> model_;

};

