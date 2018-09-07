#include <iostream>
#include "EngineCore/Application.h"
#include "EngineCore/Window.h"
#include "EngineCore/MemoryManager.h"
#include "EngineCore/EngineUtils.h"
#include "Rendering/RenderContext.h"
#include "Rendering/AssetLoader.h"

#include "EngineCore/TimerManager.h"
#include "tests.hpp"

#include "Math/Transform.h"
#include "Math/Sphere.h"
#include "Math/aabb.h"
#include "Math/Plane.h"
#include "Math/Intersects.h"

// NOTE: Profiling reveals that in the current instanced rendering system:
// - Updating the buffer takes more time than
// - Calculating the transforms which takes more time than
// - Performing the instanced draw
static int RunApp(Application* App)
{
	Tests::RunTests();
	Window _Window(*App, 1500, 1000, "MARS Engine");

	// Begin scene creation
	RenderDevice _Device(_Window);
	RenderTarget _Target(_Device);
	RenderContext _Context(_Device, _Target);

	Array<IndexedModel> _Models;
	Array<uint32> _ModelMaterialIndices;
	Array<MaterialSpec> _ModelMaterials;
	AssetLoader::LoadAsset("./Resources/Models/sphere.obj", _Models, _ModelMaterialIndices, _ModelMaterials);

	VertexArray _VertexArray(_Device, _Models[0], RenderDevice::USAGE_STATIC_DRAW);
	Sampler _Sampler(_Device, RenderDevice::FILTER_LINEAR_MIPMAP_LINEAR);

	DDSTexture _DDSTexture;
	if (!_DDSTexture.Load("./Resources/Textures/bricks.dds")) 
	{
		DEBUG_LOG("Main", LOG_ERROR, "Could not load texture!");
		return 1;
	}
	Texture _Texture(_Device, _DDSTexture);

	String _ShaderText;
	FString::loadTextFileWithIncludes(_ShaderText, "./Resources/Shaders/basicShader.glsl", "#include");
	Shader _Shader(_Device, _ShaderText);
	_Shader.setSampler("diffuse", _Texture, _Sampler, 0);
	
	Matrix _Perspective(Matrix::Perspective(Math::ToRad(70.0f/2.0f),	4.0f/3.0f, 0.1f, 1000.0f));
	float _Amount = 0.0f;
	Color _Color(0.0f, 0.5f, 0.5f);
	float _RandZ = 20.0f;
	float _RandScaleX = _RandZ * _Window.getWidth()/(float)_Window.getHeight();
	float _RandScaleY = _RandZ;
	
	uint32 _NumInstances = 100;
	Matrix _TransformMatrix(Matrix::Identity());
	Transform _Transform;
	Array<Matrix> _TransformMatrixArray;
	Array<Matrix> _TransformMatrixBaseArray;
	for (uint32 Index = 0; Index < _NumInstances; Index++) 
	{
		_TransformMatrixArray.push_back(Matrix::Identity());
		_Transform.SetTranslation(Cartesian3D((Math::Randf() * _RandScaleX)-_RandScaleX/2.0f,	(Math::Randf() * _RandScaleY)-_RandScaleY/2.0f, _RandZ));
		_TransformMatrixBaseArray.push_back(_Transform.ToMatrix());
	}

	_Transform.SetTranslation(Cartesian3D(0.0f,0.0f,0.0f));
	
	RenderDevice::DrawParams drawParams;
	drawParams.PrimitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
	drawParams.FaceCulling = RenderDevice::FACE_CULL_BACK;
	drawParams.ShouldWriteDepth = true;
	drawParams.DepthFunc = RenderDevice::DRAW_FUNC_LESS;
	// End scene creation

	uint32 fps = 0;
	double lastTime = Time::getTime();
	double fpsTimeCounter = 0.0;
	double updateTimer = 1.0;
	float frameTime = 1.f/60.f;
	while(App->GetIsRunning()) {
		double currentTime = Time::getTime();
		double passedTime = currentTime - lastTime;
		lastTime = currentTime;

		fpsTimeCounter += passedTime;
		updateTimer += passedTime;

		if(fpsTimeCounter >= 1.0) {
			double msPerFrame = 1000.0/(double)fps;
			DEBUG_LOG("FPS", "NONE", "%f ms (%d fps)", msPerFrame, fps);
			fpsTimeCounter = 0;
			fps = 0;
		}
		
		bool shouldRender = false;
		while(updateTimer >= frameTime) {
			App->HandleMessage(frameTime);
			// Begin scene update
			_Transform.SetRotation(Quaternion(Spatial3D(Cartesian3D(1.f)).Normalized().Inner(), _Amount*10.0f/11.0f));
			for(uint32 i = 0; i < _TransformMatrixArray.size(); i++) {
				_TransformMatrixArray[i] = (_Perspective * _TransformMatrixBaseArray[i] * _Transform.ToMatrix());
			}
			_VertexArray.updateBuffer(4, &_TransformMatrixArray[0],
					_TransformMatrixArray.size() * sizeof(Matrix));
			_Amount += (float)frameTime/2.0f;
			// End scene update

			updateTimer -= frameTime;
			shouldRender = true;
		}
		
		if(shouldRender) {
			// Begin scene render
			_Context.clear(_Color, true);
			_Context.draw(_Shader, _VertexArray, drawParams, _NumInstances);
			// End scene render
			
			_Window.present();
			fps++;
		} else {
			Time::sleep(1);
		}
	}
	return 0;
}

#ifdef main
#undef main
#endif

#define USE_PARAMATER(x) x;

int main(int argc, char** argv)
{
	/** @todo(devlinw, @any): pull arguments from the command line */

	USE_PARAMATER(argc) USE_PARAMATER(argv)
	Application* _App = Application::StartApplication();
	int32 _Results = RunApp(_App);
	delete _App;
	return _Results;
}

#undef USE_PARAMATER