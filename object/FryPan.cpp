#include "FryPan.h"
#include "../util/Model.h"

namespace {
	const char* const file_name = "data/object/fryPan.mv1";
	constexpr float radius = 60.0f;
	const VECTOR startPos = { 4945.0f,1025.0f,1345.0f };
}

FryPan::FryPan()
{
	pos_ = startPos;

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
//	DrawSphere3D(pos_, radius, 32, 0xffffff, 0xffffff, true);
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
