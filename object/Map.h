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
	/// 更新
	/// </summary>
	void update();

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	/// <summary>
	/// ステージモデルの取得
	/// </summary>
	/// <returns>stage_に入るモデル情報を返す</returns>
	std::shared_ptr<Model> getStage()	const { return stage_; }

private:

	int tempModel = -1;


	std::shared_ptr<Model> stage_;
};

