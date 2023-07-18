#pragma once
#include"../util/Vec2.h"

class objectHp
{
public:

	objectHp(int hp);
	virtual ~objectHp();

	void update();
	/// <summary>
	/// �`��
	/// </summary>
	void draw();

	/// <summary>
	/// hp�̎擾
	/// </summary>
	/// <returns>hp_�̒l��Ԃ�</returns>
	int getHp() { return hp_; }
	/// <summary>
	/// �����Ŏ󂯎�����l��hp_�����炷
	/// </summary>
	/// <param name="hp">�_���[�W</param>
	void onDamage(int hp);

	void cure(int recoveryValue);

private:

	int maxHp_ = 0;
	int hp_ = 0;

	Vec2 pos_;

};

