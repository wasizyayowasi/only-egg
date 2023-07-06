#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class Bacon
{
public:

	Bacon(const char* const fileName);
	Bacon(int handle);
	virtual ~Bacon();

	void update();
	void draw();

	int getModel();

	int getFrameIndex();

	bool isEnabled();
	void setIsEnabled();

private:

	float rotation_ = 0.0f;

	VECTOR pos_;
	std::shared_ptr<Model> bacon_;
};

