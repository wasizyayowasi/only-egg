#pragma once
#include"../util/Vec2.h"

class objectHp
{
public:

	objectHp(int hp);
	virtual ~objectHp();

	void update();
	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// hpの取得
	/// </summary>
	/// <returns>hp_の値を返す</returns>
	int getHp() { return hp_; }
	/// <summary>
	/// 引数で受け取った値分hp_を減らす
	/// </summary>
	/// <param name="hp">ダメージ</param>
	void onDamage(int hp);

	void cure(int recoveryValue);

private:

	int maxHp_ = 0;
	int hp_ = 0;

	Vec2 pos_;

};

