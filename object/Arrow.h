#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class Arrow
{
public:

	Arrow();
	virtual ~Arrow();

	void update(VECTOR fryPanPos, VECTOR playerPos);
	void draw();

private:

	std::shared_ptr<Model> arrow_;

};

