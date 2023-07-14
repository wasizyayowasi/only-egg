#include "Arrow.h"
#include "../util/Model.h"
#include "../util/game.h"

namespace {
	//ファイルパス
	const char* const arrow_file_name = "data/object/arrow.mv1";

	//矢印のサイズ
	constexpr float arrow_scale = 0.1f;

	//矢印の初期ベクトル
	const VECTOR start_arrow_vec = { 0,0,-1 };

	//矢印の配置場所(スクリーン座標)
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
	{//矢印の処理
		//ゴールがどこにあるかを知らせるための矢印を作る予定
		VECTOR temp;
		MATRIX rotMtx;
		//ゴールとプレイヤーのベクトルを取得
		temp = VSub(fryPanPos, playerPos);
		//初期ベクトル(初期向き)とゴールとプレイヤーのベクトル(ゴールへ向き)を回転行列として取得する
		rotMtx = MGetRotVec2(start_arrow_vec, temp);
		//矢印の拡大率を行列を使って変更する
		MATRIX scaleMtx = MGetScale(VGet(arrow_scale, arrow_scale, arrow_scale));
		//回転行列と拡大率の行列をかける
		MATRIX mtx = MMult(rotMtx, scaleMtx);
		//平行移動マトリクスを上で作った拡大マトリクスと回転マトリクスにかける
		mtx = MMult(mtx, MGetTranslate(ConvScreenPosToWorldPos(arrow_pos)));
		//それをモデルにかける
		MV1SetMatrix(arrow_->getModelHandle(), mtx);
	}
}

void Arrow::draw()
{
	arrow_->draw();
}
