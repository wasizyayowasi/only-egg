#pragma once
#include "DxLib.h"
#include <memory>

class Model;

class FryPan
{
public:
	FryPan();
	virtual ~FryPan();

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// モデルハンドルの取得
	/// </summary>
	/// <returns>モデルハンドルを返す</returns>
	int getModel();

	/// <summary>
	/// フレームの取得
	/// </summary>
	/// <returns>フレームの値を返す</returns>
	int getFrameIndex();

	/// <summary>
	/// ポジションの取得
	/// </summary>
	VECTOR getPos();

	float getRadius();

private:

	std::shared_ptr<Model> fryPan_;
	VECTOR pos_;

};

