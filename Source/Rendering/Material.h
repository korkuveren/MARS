#pragma once

#include "DataTypes/MMap.h"
#include "Math/Cartesian.h"
#include "Math/Matrix.h"

struct MaterialSpec
{
	Map<String, String> textureNames;
	Map<String, float> floats;
	Map<String, Cartesian3D> vectors;
	Map<String, Matrix> matrices;
};
