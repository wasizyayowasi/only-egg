#include "FryPan.h"
#include "../util/Model.h"

namespace {
	const char* const file_name = "data/object/fryPan.mv1";
	constexpr float radius = 60.0f;
	const VECTOR stage1_start_pos = { 5990.0f,972.0f,762.0f };
	const VECTOR stage2_start_pos = { 410.0f,3200.0f,-3547.0f };
}

FryPan::FryPan(int stageNum)
{
	switch (stageNum) {
	case 0:
		pos_ = stage1_start_pos;
		break;
	case 1:
		pos_ = stage2_start_pos;
		break;
	case 2:
		pos_ = stage1_start_pos;
		break;
	case 3:
		pos_ = stage1_start_pos;
		break;
	}

	fryPan_ = std::make_shared<Model>(file_name);
	fryPan_->setScale({ 10.0f,10.0f ,10.0f });
	fryPan_->setPos(pos_);
}

FryPan::~FryPan()
{
}

void FryPan::draw()
{
	fryPan_->draw();
}

int FryPan::getModel()
{
	return fryPan_->getModelHandle();
}

int FryPan::getFrameIndex()
{
	return fryPan_->getColFrameIndex();
}

VECTOR FryPan::getPos()
{
	return pos_;
}

float FryPan::getRadius()
{
	return radius;
}
