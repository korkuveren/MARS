#pragma once

#include "DataTypes/MMap.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

struct MaterialSpec
{
	Map<String, String> textureNames;
	Map<String, float> floats;
	Map<String, Vector3D> vectors;
	Map<String, Matrix> matrices;
};
