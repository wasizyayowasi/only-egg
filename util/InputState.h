#pragma once

#include <vector>
#include <map>
#include <string>

enum class InputType
{
	next,			/// 次へ行くボタン
	prev,			/// 前に戻るボタン
	pause,			/// ポーズボタン
	up,				///上
	down,			///下
	left,			///左
	right,			///右
	tempUp,				///上
	tempDown,			///下
	tempLeft,			///左
	tempRight,			///右
	checkpoint,
	space,
	rightTrigger,
	max,			///最大入力インデックス
};

enum class InputCategory {
	keybd,		//キーボード
	pad,		//ゲームパッド
	mouse,		//マウス
};

struct InputInfo {
	InputCategory cat;
	int id;
};

class KeyConfigScene;

class InputState
{
	friend KeyConfigScene;
public:
	InputState();

	bool isTriggered(InputType type) const;


	bool isPressed(InputType type) const;


	void update();


	void rewriteInputInfo(InputType type, InputCategory cat, int id);


	void commitChangedInputInfo();


	void rollbackChangedInputInfo();


	void resetInputInfo();


	void savekeyInfo()const;

	void loadKeyInfo();
private:

	using InputMap_t = std::map<InputType, std::vector<InputInfo> >;
	InputMap_t inputMapTable;

	InputMap_t tempMapTable;

	InputMap_t defaultMapTable;

	std::map<InputType, std::string> inputNameTable;

	std::vector<bool> currentInput;
	std::vector<bool> lastInput;
};
