#include "Interim.h"
#include"../util/Model.h"

namespace {
	//�t�@�C���p�X
	const char* const interim_file_name = "data/object/interim.mv1";
	//�g�嗦
	constexpr float interim_scale = 28.0f;

	const VECTOR start_pos = { 2820,2415,795 };
}

Interim::Interim():ItemBase(interim_file_name, start_pos,interim_scale)
{
}
