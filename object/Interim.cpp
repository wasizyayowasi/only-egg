#include "Interim.h"
#include"../util/Model.h"

namespace {
	//ファイルパス
	const char* const interim_file_name = "data/object/interim.mv1";
	//拡大率
	constexpr float interim_scale = 28.0f;

	const VECTOR start_pos = { 2820,2415,795 };
}

Interim::Interim():ItemBase(interim_file_name, start_pos,interim_scale)
{
}
