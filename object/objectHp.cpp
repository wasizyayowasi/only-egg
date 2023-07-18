#include "objectHp.h"
#include "DxLib.h"
#include <algorithm>

objectHp::objectHp(int hp)
{
	maxHp_ = hp;
	hp_ = maxHp_;
}

objectHp::~objectHp()
{
}

void objectHp::update()
{
}

void objectHp::draw()
{
	DrawBoxAA(pos_.x + 20, pos_.y + 20, pos_.x + maxHp_ + 20, pos_.y + 70, 0xffffff, false);
	DrawBoxAA(pos_.x + 21, pos_.y + 21, pos_.x + hp_ + 19, pos_.y + 69, 0xff0000, true);
}

void objectHp::onDamage(int hp)
{
	hp_ -= hp;
	hp_ = (std::max)(hp_, 0);
}

void objectHp::cure(int recoveryValue)
{
	hp_ = (std::min)(hp_ + recoveryValue,maxHp_);
}
