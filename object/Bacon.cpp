#include "Bacon.h"
#include "../util/Model.h"

namespace {
	
}

/// <summary>
/// 複製元、オリジナルベーコンのコンストラクタ
/// </summary>
/// <param name="fileName">ベーコンモデルのファイルパス</param>
Bacon::Bacon(const char* const fileName)
{
	bacon_ = std::make_shared<Model>(fileName);

	pos_ = { 0,0,0 };

	bacon_->setPos(pos_);

	bacon_->setRot({1.55f,rotation_,0});

}

/// <summary>
/// オリジナルを元にした複製ベーコンのコンストラクタ
/// </summary>
/// <param name="handle">オリジナルベーコンのモデル</param>
Bacon::Bacon(int handle)
{
	bacon_ = std::make_shared<Model>(handle);

	pos_ = { 200,0,0 };

	bacon_->setPos(pos_);

	bacon_->setRot({ 1.55f,rotation_,0 });
}

Bacon::~Bacon()
{
}

/// <summary>
/// 更新
/// </summary>
void Bacon::update()
{
	rotation_ += 0.01f;

	bacon_->setRot({ 1.55f,rotation_,0 });
}

/// <summary>
/// 描画
/// </summary>
void Bacon::draw()
{
	if (bacon_->isEnabled()) {
		bacon_->draw();
	}
}

/// <summary>
/// モデルハンドルの取得
/// </summary>
/// <returns>モデルハンドルを返す</returns>
int Bacon::getModel()
{
	return bacon_->getModelHandle();
}

/// <summary>
/// フレームの取得
/// </summary>
/// <returns>フレームを返す</returns>
int Bacon::getFrameIndex()
{
	return bacon_->getColFrameIndex();
}

/// <summary>
/// ベーコンがフィールドに存在しているか
/// </summary>
/// <returns>ベーコンの有無を返す</returns>
bool Bacon::isEnabled()
{
	return bacon_->isEnabled();
}

/// <summary>
/// ベーコンがフィールドに存在しているかどうかを変更する
/// </summary>
void Bacon::setIsEnabled()
{
	bacon_->setIsEnable();
}
