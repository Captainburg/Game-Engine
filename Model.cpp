#include "Model.h"

Model::Model(char filename[])
	:fname(filename)
{
}

Model::~Model()
{
}

char * Model::GetFileName()
{
	return fname;
}
