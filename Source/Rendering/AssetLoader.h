#pragma once

#include "IndexedModel.h"
#include "Material.h"

namespace AssetLoader
{
	bool LoadAsset(const String& fileName, Array<IndexedModel>& models, Array<uint32>& modelMaterialIndices, Array<MaterialSpec>& materials);
}
