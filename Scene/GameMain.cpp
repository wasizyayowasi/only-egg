#include "GameMain.h"

#include "../object/Map.h"
#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/FryPan.h"
#include "../object/Bacon.h"

#include "GameEnd.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include "GameOver.h"

#include "../util/Model.h"
#include "../util/game.h"
#include "../util/InputState.h"

#include <cmath>

namespace {
	//�t�@�C���p�X
	const char* const pictograph_file_name = "data/graph/pictoGraph2.png";
	const char* const pictographDisk_file_name = "data/graph/pictoGraphDisk.png";
	const char* const menu_graph = "data/graph/menuGraph.png";
	const char* const result_graph = "data/graph/result.png";
	const char* const clock_graph = "data/graph/clock.png";
	const char* const bacon_file_name = "data/object/bacon.mv1";
	const char* const gameOver_file_name = "data/graph/gameover.png";
	const char* const arrow_file_name = "data/object/arrow.mv1";
	//�����x�[�R����
	constexpr int duplication_bacon_num = 1;
	//�p�b�h�̉E�X�e�B�b�N�̌X��
	constexpr float slope = 400.0f;
	//�X���X�s�[�h
	constexpr float rot_speed = 0.05f;
	//�G�����̃T�C�Y
	constexpr int pictograph_Billborad_size = 80;
	constexpr int pictograph_size = 100;
	//�G�����̊p�x
	constexpr float pictograph_angle = 0.0f;
	//�G�����\���^�C��
	constexpr int display_time = 300;
	//�����̉摜��
	constexpr int clockGraphNum_ = 16;
	//���̃T�C�Y
	constexpr float arrow_scale = 50.0f;
}

GameMain::GameMain(SceneManager& manager, int stageNum) : SceneBase(manager),updateFunc_(&GameMain::fadeInUpdate),stageNum_(stageNum)
{
	//�J�����֘A
	{
		/////////////// 3D�֘A�̐ݒ� /////////////
		// Z�o�b�t�@���g�p����
		SetUseZBuffer3D(true);
		// Z�o�b�t�@�ւ̏������݂��s��
		SetWriteZBuffer3D(true);
		// �|���S���̗��ʂ�`�悵�Ȃ�
		SetUseBackCulling(true);

		//////////////// �J�����̐ݒ� //////////////////
		// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
		SetCameraNearFar(5.0f, 2800.0f);
		// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
		SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
		// �J�����̎���p��ݒ�(���W�A��)
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	}
	
	{//����
		map_ = std::make_shared<Map>(stageNum_);
		player_ = std::make_shared<Player>();
		camera_ = std::make_shared<Camera>();
		fryPan_ = std::make_shared<FryPan>();
		arrow_ = std::make_shared<Model>(arrow_file_name);
		bacon_.push_back(std::make_shared<Bacon>(bacon_file_name));
		for (int i = 0; i < duplication_bacon_num; i++) {
			bacon_.push_back(std::make_shared<Bacon>(bacon_[0]->getModel()));
		}
	}
	
	{//�摜���[�h
		pictographDiskHandle_ = LoadGraph(pictographDisk_file_name);
		menugraphHandle_ = LoadGraph(menu_graph);
		resultgraphHandle_ = LoadGraph(result_graph);
		gameOverHandle_ = LoadGraph(gameOver_file_name);
		LoadDivGraph(pictograph_file_name, 8, 8, 1, pictograph_size, pictograph_size, pictographHandle_);
		LoadDivGraph(clock_graph, clockGraphNum_, clockGraphNum_, 1, pictograph_Billborad_size, pictograph_Billborad_size, clockgraphHandle_);
	}

	player_->setMapData(map_);
	arrow_->setScale({ arrow_scale,arrow_scale,arrow_scale });

	MV1SetupCollInfo(player_->getModel(), -1, 8, 8, 8);
	MV1SetupCollInfo(fryPan_->getModel(), -1, 8, 8, 8);

	SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	ChangeLightTypePoint({ 0,500,0 }, 2000.0f, 0.0f, 0.006f, 0.0f);

}

GameMain::GameMain(SceneManager& manager) : SceneBase(manager), updateFunc_(&GameMain::fadeInUpdate)
{
	//�J�����֘A
	{
		/////////////// 3D�֘A�̐ݒ� /////////////
		// Z�o�b�t�@���g�p����
		SetUseZBuffer3D(true);
		// Z�o�b�t�@�ւ̏������݂��s��
		SetWriteZBuffer3D(true);
		// �|���S���̗��ʂ�`�悵�Ȃ�
		SetUseBackCulling(true);

		//////////////// �J�����̐ݒ� //////////////////
		// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
		SetCameraNearFar(5.0f, 2800.0f);
		// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
		SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
		// �J�����̎���p��ݒ�(���W�A��)
		SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	}

	{//����
		map_ = std::make_shared<Map>(stageNum_);
		player_ = std::make_shared<Player>();
		camera_ = std::make_shared<Camera>();
		fryPan_ = std::make_shared<FryPan>();
		arrow_ = std::make_shared<Model>(arrow_file_name);
		bacon_.push_back(std::make_shared<Bacon>(bacon_file_name));
		for (int i = 0; i < duplication_bacon_num; i++) {
			bacon_.push_back(std::make_shared<Bacon>(bacon_[0]->getModel()));
		}
	}

	{//�摜���[�h
		pictographDiskHandle_ = LoadGraph(pictographDisk_file_name);
		menugraphHandle_ = LoadGraph(menu_graph);
		resultgraphHandle_ = LoadGraph(result_graph);
		gameOverHandle_ = LoadGraph(gameOver_file_name);
		LoadDivGraph(pictograph_file_name, 8, 8, 1, pictograph_size, pictograph_size, pictographHandle_);
		LoadDivGraph(clock_graph, clockGraphNum_, clockGraphNum_, 1, pictograph_Billborad_size, pictograph_Billborad_size, clockgraphHandle_);
	}

	player_->setMapData(map_);
	arrow_->setScale({ arrow_scale,arrow_scale,arrow_scale });

	MV1SetupCollInfo(player_->getModel(), -1, 8, 8, 8);
	MV1SetupCollInfo(fryPan_->getModel(), -1, 8, 8, 8);

	SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.0f));

	ChangeLightTypePoint({ 0,500,0 }, 2000.0f, 0.0f, 0.006f, 0.0f);

}

GameMain::~GameMain()
{
	MV1TerminateCollInfo(player_->getModel(), -1);
	MV1TerminateCollInfo(fryPan_->getModel(), -1);
	for (auto& bacon : bacon_) {
		MV1TerminateCollInfo(bacon->getModel(), -1);
	}
	DeleteGraph(pictographDiskHandle_);
	DeleteGraph(menugraphHandle_);
	DeleteGraph(resultgraphHandle_);
	DeleteGraph(gameOverHandle_);
	for (auto& graph : pictographHandle_) {
		DeleteGraph(graph);
	}
	for (auto& graph : clockgraphHandle_) {
		DeleteGraph(graph);
	}
}

/// <summary>
/// ���C���V�[���̍X�V
/// </summary>
/// <param name="input"></param>
void GameMain::update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

/// <summary>
/// �`��
/// </summary>
void GameMain::draw()
{
	//DrawString(0, 0, "GameMain", 0xffffff);

	{//�O���b�h���\���@�������\��
		for (float x = -500.0f; x <= 500.0f; x += 100.0f)
		{
			VECTOR start = VGet(x, 0.0f, -500.0f);
			VECTOR end = VGet(x, 0.0f, 500.0f);
			DrawLine3D(start, end, GetColor(0, 255, 0));
		}
		for (float z = -500.0f; z <= 500.0f; z += 100.0f)
		{
			VECTOR start = VGet(-500.0f, 0.0f, z);
			VECTOR end = VGet(500.0f, 0.0f, z);
			DrawLine3D(start, end, GetColor(255, 255, 0));
		}
	}

	{//�e�I�u�W�F�N�g�̕`��
		map_->draw();
		player_->draw();
		fryPan_->draw();
		for (auto& bacon : bacon_) {
			bacon->draw();
		}
	}
	

	//�G�����~�Ղ̕`��
	if (pictographFlag_) {
		DrawRotaGraphF(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 3.0f, 0.0f, pictographDiskHandle_, true);
		DrawCircleAA(Game::kScreenWidth / 2 + stickRot.x * 200, Game::kScreenHeight / 2 + stickRot.y * 200, 20, 32, 0xff0000, true);
	}

	{//�G�����̍��W�A�`��
		VECTOR pictographPos;
		pictographPos = { player_->GetPos().x - 60,player_->GetPos().y + 70,player_->GetPos().z };

		DrawBillboard3D(pictographPos, 0.5f, 0.5f, pictograph_Billborad_size, pictograph_angle, pictographHandle_[pictographNum_], true);
	}

	{//���v
		//���v�̔w�i
		DrawBox(1700, 0, Game::kScreenWidth, 100, 0xffffff, true);
		//�����̕`��
		DrawGraph(1700, 10, clockgraphHandle_[clearTimeMinute_ / 10], true);
		DrawGraph(1730, 10, clockgraphHandle_[clearTimeMinute_ % 10], true);
		DrawGraph(1755, 10, clockgraphHandle_[10], true);
		DrawGraph(1780, 10, clockgraphHandle_[clearTimeSecond_ / 10], true);
		DrawGraph(1810, 10, clockgraphHandle_[clearTimeSecond_ % 10], true);
	}
	
	arrow_->draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// �G�����I���X�V
/// </summary>
/// <param name="input">key����</param>
void GameMain::pictographUpdate(const InputState& input)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &stick_);

	if (input.isPressed(InputType::rightTrigger)) {

		pictographFlag_ = true;

		stickRot.x = static_cast<float>(stick_.Rx);
		stickRot.y = static_cast<float>(stick_.Ry);
		stickRot.z = static_cast<float>(stick_.Rz);

		float size = 0.0f;
		size = VSize(stickRot);
		if (size > 0.0f) {
			stickRot = VNorm(stickRot);
		}
		else {
			pictographNum_ = -1;
		}

		size = std::atan2(stickRot.y, stickRot.x);
		size = size * 180 / 3.14f;

		//FIXME:�����Ƃ�������������͂�
		if (size > 0 && size < 45) {
			pictographNum_ = 0;
			pictographDisplayCount_ = 0;
		}
		else if (size > 45 && size < 90) {
			pictographNum_ = 1;
			pictographDisplayCount_ = 0;
		}
		else if (size > 90 && size < 135) {
			pictographNum_ = 2;
			pictographDisplayCount_ = 0;
		}
		else if (size > 135 && size < 180) {
			pictographNum_ = 3;
			pictographDisplayCount_ = 0;
		}
		else if (size < 0 && size > -45) {
			pictographNum_ = 4;
			pictographDisplayCount_ = 0;
		}
		else if (size < -45 && size > -90) {
			pictographNum_ = 5;
			pictographDisplayCount_ = 0;
		}
		else if (size < -90 && size > -135) {
			pictographNum_ = 6;
			pictographDisplayCount_ = 0;
		}
		else if (size < -135 && size > -180) {
			pictographNum_ = 7;
			pictographDisplayCount_ = 0;
		}
	}
}

/// <summary>
/// �t�F�[�h�C������
/// </summary>
/// <param name="input"></param>
void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="input"></param>
void GameMain::normalUpdate(const InputState& input)
{
	deadFlag_ = false;

	//�G�����\������
	{
		pictographDisplayCount_++;
		if (pictographDisplayCount_ > display_time) {
			pictographNum_ = -1;
			pictographDisplayCount_ = (std::min)(pictographDisplayCount_, display_time);
		}
	}
	

	pictographFlag_ = false;

	//�G�����X�V
	pictographUpdate(input);

	{//�e�I�u�W�F�N�g�̍X�V
		if (!deadFlag_) {
			player_->update(input);
			camera_->update(input, player_, pictographFlag_, stick_);
			for (auto& bacon : bacon_) {
				bacon->update();
			}
		}
	}
	
	//�v���C���[�̏�Ԃ��擾��true�ł���΃Q�[����
	//���f������t���O�𗧂Ă�
	if (player_->getState()) {
		deadFlag_ = true;
	}

	//�����蔻��p�ϐ�
	MV1_COLL_RESULT_POLY_DIM fryPanResult;
	MV1_COLL_RESULT_POLY_DIM baconResult[duplication_bacon_num + 1];
	//�֐����g�p���������Ă��邩�̏���ϐ��ɑ������
	fryPanResult = MV1CollCheck_Capsule(fryPan_->getModel(), fryPan_->getFrameIndex(), player_->GetPos(), player_->GetlastPos(),player_->getRadius());
	for (int i = 0; i < duplication_bacon_num + 1;i++) {
		baconResult[i] = MV1CollCheck_Capsule(bacon_[i]->getModel(), bacon_[i]->getFrameIndex(), player_->GetPos(), player_->GetlastPos(), player_->getRadius());
	}
	
	//�t���C�p���ƃv���C���[�̓����蔻��
	if (fryPanResult.HitNum > 0) {
		if (fryPanResult.Dim->Normal.y > 0.000001) {
			player_->stateChange();
			player_->setPos(fryPan_->getPos());
			updateFunc_ = &GameMain::fadeOutUpdate;
		}
	}

	//�x�[�R���ƃv���C���[�̓����蔻��
	for (int i = 0; i < duplication_bacon_num + 1; i++) {
		if (baconResult[i].HitNum > 0) {
			bacon_[i]->setIsEnabled();
		}
	}
	
	//���C���������I��������
	if (deadFlag_) {
		updateFunc_ = &GameMain::fadeOutUpdate;
	}

	//�|�[�Y��ʂɈړ�
	if (input.isTriggered(InputType::pause)) {
		manager_.pushScene(new PauseScene(manager_,player_, menugraphHandle_,gameOverHandle_));
	}

	//����Ȃ�
	if (input.isTriggered(InputType::pause)) {
		updateFunc_ = &GameMain::fadeOutUpdate;
	}

	//TODO:���Ԃ̏����̕ύX
	{
		tempTime_++;
		if (tempTime_ > 59) {
			clearTimeSecond_++;
			tempTime_ = 0;
		}
		if (clearTimeSecond_ > 59) {
			clearTimeMinute_++;
			clearTimeSecond_ = 0;
		}
	}
	
	{//�S�[�����ǂ��ɂ��邩��m�点�邽�߂̖������\��
		VECTOR temp;
		temp =  VSub(player_->GetPos(), fryPan_->getPos());
		float size = VSize(temp);
		if (size > 0.0f) {
			temp = VNorm(temp);
		}
		arrow_->setRot(temp);
	}
	

	MV1CollResultPolyDimTerminate(fryPanResult);

	for (auto& result : baconResult) {
		MV1CollResultPolyDimTerminate(result);
	}

	SetLightPosition({ player_->GetPos().x,player_->GetPos().y + 200,player_->GetPos().z });
}

/// <summary>
/// �t�F�[�h�A�E�g����
/// </summary>
/// <param name="input"></param>
void GameMain::fadeOutUpdate(const InputState& input)
{
	float remainingHp_ = static_cast<float>(player_->getHp()) / 400.0f;
	int invalidBaconNum = 0;
	for (auto& bacon : bacon_) {
		if (!bacon->isEnabled()) {
			invalidBaconNum++;
		}
	}

	if (deadFlag_) {
		manager_.pushScene(new GameOver(manager_, gameOverHandle_));
	}
	else {
		manager_.pushScene(new GameEnd(manager_,resultgraphHandle_, clockgraphHandle_,clearTimeSecond_,clearTimeMinute_, remainingHp_, invalidBaconNum));
	}

	/*fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.changeScene(new GameEnd(manager_));
		return;
	}*/
}
