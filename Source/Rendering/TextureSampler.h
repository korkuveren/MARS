#pragma once

#include "RenderDevice.h"

class Sampler
{
public:
	inline Sampler(RenderDevice& deviceIn,
			enum RenderDevice::SamplerFilter minFilter
				= RenderDevice::FILTER_NEAREST_MIPMAP_LINEAR,
			enum RenderDevice::SamplerFilter magFilter = RenderDevice::FILTER_LINEAR,
			enum RenderDevice::SamplerWrapMode wrapU = RenderDevice::WRAP_CLAMP,
			enum RenderDevice::SamplerWrapMode wrapV = RenderDevice::WRAP_CLAMP,
			float anisotropy = 0.0f) :
		device(&deviceIn),
		deviceId(device->CreateSampler(minFilter, magFilter,
				wrapU, wrapV, anisotropy)) {}
	inline ~Sampler()
	{
		deviceId = device->ReleaseSampler(deviceId);
	}

	inline uint32 getId();
private:
	RenderDevice* device;
	uint32 deviceId;

	NULL_COPY_AND_ASSIGN(Sampler);
};

inline uint32 Sampler::getId()
{
	return deviceId;
}

