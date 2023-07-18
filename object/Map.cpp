#include "Map.h"
#include "../util/Model.h"

namespace {
	//�t�@�C����
	const char* const egg_name = "data/mapObject/cube.mv1";
	const char* const stage_file_name = "data/mapObject/Yroom.mv1";
	const char* const stage3_file_name = "data/mapObject/Nroom.mv1";
	const char* const stage2_file_name = "data/mapObject/Mroom.mv1";

	//�T�C�Y
	VECTOR groundScale = { 20.0f, 10.0f, 10.0f };
//	VECTOR scale = { 10.0f, 10.0f, 10.0f };
	VECTOR scale = { 100.0f, 100.0f, 100.0f };
	//VECTOR scale = { 5.0f, 5.0f, 5.0f };

	constexpr int box_num = 3;
	constexpr int cabinet2_num = 4;
}

Map::Map(int stageNum)
{
	{//�I�����ꂽ�X�e�[�W��stage_�ɑ������
		switch (stageNum) {
		case 0:
			stage_ = std::make_shared<Model>(stage_file_name);
			break;
		case 1:
			stage_ = std::make_shared<Model>(stage2_file_name);
			break;
		case 2:
			stage_ = std::make_shared<Model>(stage3_file_name);
			break;
		case 3:
			stage_ = std::make_shared<Model>(stage_file_name);
			break;
		}
	}
	
	

	//scale�ݒ�
	{
		stage_->setScale(scale);
		stage_->setPos({0,-2,0});
	}
	
	//�|�W�V�����ݒ�
	{
	}

	//��]�ݒ�
	{
	}


	//����
	ground_ = std::make_shared<Model>(egg_name);

	ground_->setScale(groundScale);
	ground_->setPos({ 0.0f,-2000.0f, 0.0f });

	float x = 0.0f;
}

Map::~Map()
{
}

void Map::update()
{
}

void Map::draw()
{
	SetUseLighting(false);

	stage_->draw();

	SetUseLighting(true);

}
