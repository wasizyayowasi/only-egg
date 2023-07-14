#include "Model.h"
#include <cassert>

namespace {
	const char* const collision_frame_name = "Collision";
}

Model::Model(const char* fileName)
{
	modelHandle_ = MV1LoadModel(fileName);
	assert(modelHandle_ != -1);

	isEnable_ = true;
}

Model::Model(int orgModel)
{
	assert(modelHandle_ != -1);
	modelHandle_ = MV1DuplicateModel(orgModel);

	isEnable_ = true;
}

Model::~Model()
{
	MV1DeleteModel(modelHandle_);
}

void Model::draw()
{
	MV1DrawModel(modelHandle_);
}

void Model::setPos(VECTOR pos)
{
	MV1SetPosition(modelHandle_, pos);
}

void Model::setRot(VECTOR rot)
{
	MV1SetRotationXYZ(modelHandle_, rot);
}

void Model::setScale(VECTOR scale)
{
	MV1SetScale(modelHandle_, scale);
}

