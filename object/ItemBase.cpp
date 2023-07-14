#include "ItemBase.h"
#include "../util/Model.h"

ItemBase::ItemBase(const char* const fileName, VECTOR pos, float scale)
{
	model_ = std::make_shared<Model>(fileName);
	model_->setPos(pos);
	model_->setRot(rotation_);
	model_->setScale({ scale,scale ,scale });
}

ItemBase::ItemBase(int handle, VECTOR pos, float scale)
{
	model_ = std::make_shared<Model>(handle);
	model_->setPos(pos);
	model_->setRot(rotation_);
	model_->setScale({ scale,scale ,scale });
}

ItemBase::~ItemBase()
{
}

void ItemBase::update()
{
	rotation_.y += 0.01f;
	model_->setRot(rotation_);
}

void ItemBase::draw()
{
	if (isEnabled()) {
		model_->draw();
	}
}

int ItemBase::getModelHandle()
{
	return model_->getModelHandle();
}

int ItemBase::getFrameIndex()
{
	return model_->getColFrameIndex();
}

bool ItemBase::isEnabled()
{
	return model_->isEnabled();
}

void ItemBase::setEnabled()
{
	model_->setEnable();
}
