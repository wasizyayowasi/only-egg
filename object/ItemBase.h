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

private:

	VECTOR rotation_ = { 0,0,0 };

	std::shared_ptr<Model> model_;

};

