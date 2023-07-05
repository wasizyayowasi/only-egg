#pragma once
#include<memory>
#include"DxLib.h"

class objectHp;
class Model;
class InputState;
class Map;

class Player
{
public:

	Player();
	virtual ~Player();

	void tempUpdate(VECTOR moveVector);

	void update(const InputState& input);
	void draw();

	void setMapData(std::shared_ptr<Map> map);

	//FIXME:���ʂȊ֐������炷

	// �V���b�g�̒��S�ʒu�擾
	void setPos(VECTOR pos);
	VECTOR GetPos() const { return pos_; };
	VECTOR GetlastPos() const { return lastPos_; };

	int getModel();

	void stateChange();
	bool getState();
	void setAngle(float angle);
	void setInitialPosition();

	float getRadius()const;

	int getHp()const;

private:

	int landingCount_ = 0;

	bool jump_ = true;
	bool sunnyEggFlag_ = false;
	bool deadFlag_ = false;

	// �v���C���[�̌����Ă������
	float sideAngle_ = 0.0f;
	float verAngle_ = 0.0f;

	// �J�����̌����Ă������
	float cameraAngle_ = 0.0f;
	float cameraAngle2_ = 0.0f;

	float verticalRotation_ = 0.0f;
	float sideRotation_ = 0.0f;

	//�P�t���[���O�̈ʒu
	VECTOR lastPos_;
	VECTOR cameraPos_;

	std::shared_ptr<Model> eggModel_;
	std::shared_ptr<Model> sunnyEggModel_;
	std::shared_ptr<objectHp> hp_;
	
	VECTOR pos_;
	float jumpVec_ = 0.0f;

	std::shared_ptr<Map> map_;
};

