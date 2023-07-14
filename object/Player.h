#pragma once
#include<memory>
#include"DxLib.h"

class objectHp;
class Model;
class InputState;
class Map;

//�����蔻��̍ő�q�b�g��
constexpr int max_hit_coll = 2048;

class Player
{
public:

	Player();
	virtual ~Player();

	void checkCollisionStage(VECTOR moveVector);

	void update(const InputState& input);
	void draw();

	void setMapData(std::shared_ptr<Map> map);

	//FIXME:���ʂȊ֐������炷

	void setPos(VECTOR pos);
	VECTOR getPos() const { return pos_; };
	VECTOR getlastPos() const { return lastPos_; };

	int getModel();

	void stateChange();
	bool getState();
	void setAngle(float angle);
	void resetInitial();

	float getRadius()const;

	int getHp()const;

	void FlyAway();

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

	MV1_COLL_RESULT_POLY_DIM HitDim;
	MV1_COLL_RESULT_POLY* kabe[max_hit_coll];
	MV1_COLL_RESULT_POLY* yuka[max_hit_coll];
	MV1_COLL_RESULT_POLY* poly;
	HITRESULT_LINE lineRes;

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

