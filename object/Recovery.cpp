#include "Recovery.h"

namespace {
	//�t�@�C���p�X
	const char* const recovery_file_name = "data/object/recovery.mv1";
	//�g�嗦
	constexpr float recovery_scale = 28.0f;

	const VECTOR start_pos = { 1000,0,0 };
}

Recovery::Recovery() : ItemBase(recovery_file_name,start_pos,recovery_scale)
{
}
