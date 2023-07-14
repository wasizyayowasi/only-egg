#include "Arrow.h"
#include "../util/Model.h"
#include "../util/game.h"

namespace {
	//�t�@�C���p�X
	const char* const arrow_file_name = "data/object/arrow.mv1";

	//���̃T�C�Y
	constexpr float arrow_scale = 0.1f;

	//���̏����x�N�g��
	const VECTOR start_arrow_vec = { 0,0,-1 };

	//���̔z�u�ꏊ(�X�N���[�����W)
	const VECTOR arrow_pos = { Game::kScreenWidth / 2,80,0.2f };
}

Arrow::Arrow()
{
	arrow_ = std::make_shared<Model>(arrow_file_name);

	MATRIX scaleMtx = MGetScale(VGet(arrow_scale, arrow_scale, arrow_scale));
	MV1SetMatrix(arrow_->getModelHandle(), scaleMtx);
}

Arrow::~Arrow()
{
}

void Arrow::update(VECTOR fryPanPos, VECTOR playerPos)
{
	{//���̏���
		//�S�[�����ǂ��ɂ��邩��m�点�邽�߂̖������\��
		VECTOR temp;
		MATRIX rotMtx;
		//�S�[���ƃv���C���[�̃x�N�g�����擾
		temp = VSub(fryPanPos, playerPos);
		//�����x�N�g��(��������)�ƃS�[���ƃv���C���[�̃x�N�g��(�S�[���֌���)����]�s��Ƃ��Ď擾����
		rotMtx = MGetRotVec2(start_arrow_vec, temp);
		//���̊g�嗦���s����g���ĕύX����
		MATRIX scaleMtx = MGetScale(VGet(arrow_scale, arrow_scale, arrow_scale));
		//��]�s��Ɗg�嗦�̍s���������
		MATRIX mtx = MMult(rotMtx, scaleMtx);
		//���s�ړ��}�g���N�X����ō�����g��}�g���N�X�Ɖ�]�}�g���N�X�ɂ�����
		mtx = MMult(mtx, MGetTranslate(ConvScreenPosToWorldPos(arrow_pos)));
		//��������f���ɂ�����
		MV1SetMatrix(arrow_->getModelHandle(), mtx);
	}
}

void Arrow::draw()
{
	arrow_->draw();
}
