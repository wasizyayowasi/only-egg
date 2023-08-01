#pragma once
#include <memory>
#include <vector>

class Model;

class Map
{
public:
	Map(int stageNum);
	virtual ~Map();

	/// <summary>
	/// �X�V
	/// </summary>
	void update();

	/// <summary>
	/// �`��
	/// </summary>
	void draw();

	/// <summary>
	/// �X�e�[�W���f���̎擾
	/// </summary>
	/// <returns>stage_�ɓ��郂�f������Ԃ�</returns>
	std::shared_ptr<Model> getStage()	const { return stage_; }

private:

	int tempModel = -1;


	std::shared_ptr<Model> stage_;
};

