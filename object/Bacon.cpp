#include "Bacon.h"
#include "../util/Model.h"

/// <summary>
/// 複製元、オリジナルベーコンのコンストラクタ
/// </summary>
/// <param name="fileName">ベーコンモデルのファイルパス</param>
Bacon::Bacon(const char* const fileName) : ItemBase(fileName,{0,0,0},1.0f)
{
}

/// <summary>
/// オリジナルを元にした複製ベーコンのコンストラクタ
/// </summary>
/// <param name="handle">オリジナルベーコンのモデル</param>
Bacon::Bacon(int handle) : ItemBase(handle, { 200,0,0 }, 1.0f)
{
}