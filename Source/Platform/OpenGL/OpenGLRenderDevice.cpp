#include "OpenGLRenderDevice.h"
#include "EngineCore/EngineUtils.h"
#include "DataTypes/MArray.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

static bool addShader(GLuint shaderProgram, const String& text, GLenum type,
		Array<GLuint>* shaders);
static void addAllAttributes(GLuint program, const String& vertexShaderText, uint32 version);
static bool checkShaderError(GLuint shader, int flag,
		bool isProgram, const String& errorMessage);
static void addShaderUniforms(GLuint shaderProgram, const String& shaderText,
		Map<String, GLint>& uniformMap, Map<String, GLint>& samplerMap);

bool OpenGLRenderDevice::isInitialized = false;

bool OpenGLRenderDevice::GlobalInit()
{
	if(isInitialized) {
		return isInitialized;
	}
	int32 major = 3;
	int32 minor = 2;
	
	isInitialized = true;
	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_WARNING,
				"Could not set core OpenGL profile");
		isInitialized = false;
	}
	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major) != 0) {
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR,
				"Could not set major OpenGL version to %d: %s",
				major, SDL_GetError());
		isInitialized = false;
	}
	if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor) != 0) {
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR,
				"Could not set minor OpenGL version to %d: %s",
				minor, SDL_GetError());
		isInitialized = false;
	}
	
	return isInitialized;
}

OpenGLRenderDevice::OpenGLRenderDevice(Window& window) :
	shaderVersion(""), version(0),
	boundFBO(0),
	viewportFBO(0),
	boundVAO(0),
	boundShader(0),
	currentFaceCulling(FACE_CULL_NONE),
	currentDepthFunc(DRAW_FUNC_ALWAYS),
	currentSourceBlend(BLEND_FUNC_NONE),
	currentDestBlend(BLEND_FUNC_NONE),
	currentStencilFunc(DRAW_FUNC_ALWAYS),
	currentStencilTestMask((uint32)0xFFFFFFFF),
	currentStencilWriteMask((uint32)0xFFFFFFFF),
	currentStencilComparisonVal(0),
	currentStencilFail(STENCIL_KEEP),
	currentStencilPassButDepthFail(STENCIL_KEEP),
	currentStencilPass(STENCIL_KEEP),
	blendingEnabled(false),
	shouldWriteDepth(false),
	stencilTestEnabled(false),
	scissorTestEnabled(false)
{
	context = SDL_GL_CreateContext(window.getWindowHandle());
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if(res != GLEW_OK) {
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "%s", glewGetErrorString(res));
		throw std::runtime_error("Render device could not be initialized");
	}

	struct FBOData fboWindowData;
	fboWindowData.Width = window.getWidth();
	fboWindowData.Height = window.getHeight();
	fboMap[0] = fboWindowData;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(DRAW_FUNC_ALWAYS);
	glDepthMask(GL_FALSE);
//	glEnable(GL_FRAMEBUFFER_SRGB);
	glFrontFace(GL_CW);
}

OpenGLRenderDevice::~OpenGLRenderDevice()
{
	SDL_GL_DeleteContext(context);
}

void OpenGLRenderDevice::clear(uint32 fbo, bool shouldClearColor, bool shouldClearDepth,
		bool shouldClearStencil, const Color& color, uint32 stencil)
{
	setFBO(fbo);
	uint32 flags = 0;
	if(shouldClearColor) {
		flags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color[0], color[1], color[2], color[3]);
	}
	if(shouldClearDepth) {
		flags |= GL_DEPTH_BUFFER_BIT;
	}
	if(shouldClearStencil) {
		flags |= GL_STENCIL_BUFFER_BIT;
		setStencilWriteMask(stencil);
	}

	glClear(flags);
}


/*
 * Complete drawing process:
 * + Ensure appropriate render targets are bound
 * + Ensure appropriate sized rendering viewport
 * + Ensure appropriate blend mode for each render target
 * + Ensure appropriate scissor rect, if any
 * + Ensure appropriate polygon and culling modes
 * + Ensure appropriate depth modes
 * - Ensure appropriate stencil modes
 * + Ensure appropriate shader programs are bound
 * = Update appropriate uniform buffers
 * = Bind appropriate textures/samplers
 * = Update shader with appropriate samplers and uniform buffers
 * + Bind the element array buffer and vertex attributes
 * + If only 1 instance is being rendered, issue a draw call to render
 * the indexed primitive array
 * + Otherwise, perform an instanced draw call for the number of desired
 * instances
 */
void OpenGLRenderDevice::draw(uint32 fbo, uint32 shader, uint32 vao,
		const DrawParams& drawParams,
		uint32 numInstances, uint32 numElements)
{
	if(numInstances == 0) {
		return;
	}
	setFBO(fbo);
	setViewport(fbo);
	setBlending(drawParams.SourceBlend, drawParams.DestinationBlend);
	setScissorTest(drawParams.UseScissorTest,
			drawParams.ScissorStartX, drawParams.ScissorStartY,
			drawParams.ScissorWidth, drawParams.ScissorHeight);
	setFaceCulling(drawParams.FaceCulling);
	setDepthTest(drawParams.ShouldWriteDepth, drawParams.DepthFunc);
	setShader(shader);
	setVAO(vao);

	if(numInstances == 1) {
		glDrawElements(drawParams.PrimitiveType, (GLsizei)numElements, GL_UNSIGNED_INT, 0);
	} else {
		glDrawElementsInstanced(drawParams.PrimitiveType, (GLsizei)numElements, GL_UNSIGNED_INT, 0,
				numInstances);
	}
}

void OpenGLRenderDevice::setFBO(uint32 fbo)
{
	if(fbo == boundFBO) {
		return;
	}
//	if(fbo == 0) {
//		glEnable(GL_FRAMEBUFFER_SRGB);
//	} else if(boundFBO == 0) {
//		glDisable(GL_FRAMEBUFFER_SRGB);
//	}
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	boundFBO = fbo;
}

void OpenGLRenderDevice::setViewport(uint32 fbo)
{
	if(fbo == viewportFBO) {
		return;
	}
	glViewport(0, 0, fboMap[fbo].Width, fboMap[fbo].Height);
	viewportFBO = fbo;
}

void OpenGLRenderDevice::setShader(uint32 shader)
{
	if(shader == boundShader) {
		return;
	}
	glUseProgram(shader);
	boundShader = shader;
}

void OpenGLRenderDevice::setVAO(uint32 vao)
{
	if(vao == boundVAO) {
		return;
	}
	glBindVertexArray(vao);
	boundVAO = vao;
}

void OpenGLRenderDevice::setBlending(enum BlendFunc sourceBlend, enum BlendFunc destBlend)
{
	if(sourceBlend == currentSourceBlend && destBlend == currentDestBlend) {
		return;
	} else if(sourceBlend == BLEND_FUNC_NONE || destBlend == BLEND_FUNC_NONE) {
		glDisable(GL_BLEND);
	} else if(currentSourceBlend == BLEND_FUNC_NONE || currentDestBlend == BLEND_FUNC_NONE) {
		glEnable(GL_BLEND);
		glBlendFunc(sourceBlend, destBlend);
	} else {
		glBlendFunc(sourceBlend, destBlend);
	}

	currentSourceBlend = sourceBlend;
	currentDestBlend = destBlend;
}


void OpenGLRenderDevice::setScissorTest(bool enable, uint32 startX, uint32 startY,
			uint32 Width, uint32 Height)
{
	if(!enable) {
		if(!scissorTestEnabled) {
			return;
		} else {
			glDisable(GL_SCISSOR_TEST);
			scissorTestEnabled = false;
			return;
		}
	}
	if(!scissorTestEnabled) {
		glEnable(GL_SCISSOR_TEST);
	}
	glScissor(startX, startY, Width, Height);
	scissorTestEnabled = true;
}

void OpenGLRenderDevice::setFaceCulling(enum FaceCulling cullingMode)
{
	if(cullingMode == currentFaceCulling) {
		return;
	}
	
	if(cullingMode == FACE_CULL_NONE) { // Face culling is enabled, but needs to be disabled
		glDisable(GL_CULL_FACE);
	} else if(currentFaceCulling == FACE_CULL_NONE) { // Face culling is disabled but needs to be enabled
		glEnable(GL_CULL_FACE);
		glCullFace(cullingMode);
	} else { // Only need to change culling state
		glCullFace(cullingMode);
	}
	currentFaceCulling = cullingMode;
}

void OpenGLRenderDevice::setDepthTest(bool shouldWrite, enum DrawFunc depthFunc)
{
	if(shouldWrite != shouldWriteDepth) {
		glDepthMask(shouldWrite ? GL_TRUE : GL_FALSE);
		shouldWriteDepth = shouldWrite;
	}

	if(depthFunc == currentDepthFunc) {
		return;
	}
	glDepthFunc(depthFunc);
	currentDepthFunc = depthFunc;
}

void OpenGLRenderDevice::setStencilTest(bool enable, enum DrawFunc stencilFunc,
		uint32 stencilTestMask, uint32 stencilWriteMask, int32 stencilComparisonVal,
		enum StencilOp stencilFail, enum StencilOp stencilPassButDepthFail,
		enum StencilOp stencilPass)
{
	if(enable != stencilTestEnabled) {
		if(enable) {
			glEnable(GL_STENCIL_TEST);
		} else {
			glDisable(GL_STENCIL_TEST);
		}
		stencilTestEnabled = enable;
	}

	if(stencilFunc != currentStencilFunc || stencilTestMask != currentStencilTestMask
			|| stencilComparisonVal != currentStencilComparisonVal) {
		glStencilFunc(stencilFunc, stencilTestMask, stencilComparisonVal);
		currentStencilComparisonVal = stencilComparisonVal;
		currentStencilTestMask = stencilTestMask;
		currentStencilFunc = stencilFunc;
	}

	if (stencilFail != currentStencilFail || stencilPass != currentStencilPass || stencilPassButDepthFail != currentStencilPassButDepthFail) 
	{
		glStencilOp(stencilFail, stencilPassButDepthFail, stencilPass);
		currentStencilFail = stencilFail;
		currentStencilPass = stencilPass;
		currentStencilPassButDepthFail = stencilPassButDepthFail;
	}

	setStencilWriteMask(stencilWriteMask);
}

void OpenGLRenderDevice::setStencilWriteMask(uint32 mask)
{
	if(currentStencilWriteMask == mask) {
		return;
	}
	glStencilMask(mask);
	currentStencilWriteMask = mask;
}

uint32 OpenGLRenderDevice::CreateRenderTarget(uint32 Texture, int32 Width, int32 Height, enum FramebufferAttachment Attachment, uint32 AttachmentNumber, uint32 MipLevel)
{
	uint32 FBO;
	glGenFramebuffers(1, &FBO);
	setFBO(FBO);

	GLenum attachmentTypeGL = Attachment + AttachmentNumber;
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentTypeGL, GL_TEXTURE_2D, Texture, MipLevel);

	struct FBOData data;
	data.Width = Width;
	data.Height = Height;
	fboMap[FBO] = data;

	return FBO;
}

uint32 OpenGLRenderDevice::ReleaseRenderTarget(uint32 fbo)
{
	if (fbo == 0) { return 0; }

	Map<uint32, FBOData>::iterator it = fboMap.find(fbo);
	if(it == fboMap.end()) {
		return 0;
	}

	glDeleteFramebuffers(1, &fbo);
	fboMap.erase(it);
	return 0;
}

uint32 OpenGLRenderDevice::CreateVertexArray(const float** VertexData, const uint32* VertexElementSizes, uint32 NumVertexComponents, uint32 NumInstanceComponents, uint32 NumVertices, const uint32* Indices, uint32 NumIndices, enum BufferUsage Usage)
{
	unsigned int numBuffers = NumVertexComponents + NumInstanceComponents + 1;

	GLuint VAO;
	auto* buffers = new GLuint[numBuffers];
	auto* bufferSizes = new uintptr[numBuffers];

	glGenVertexArrays(1, &VAO);
	setVAO(VAO);

	glGenBuffers(numBuffers, buffers);
	for(uint32 i = 0, _Attribute = 0; i < numBuffers-1; i++) 
	{
		enum BufferUsage attribUsage = Usage;
		bool inInstancedMode = false;
		if (i >= NumVertexComponents) 
		{
			attribUsage = USAGE_DYNAMIC_DRAW;
			inInstancedMode = true;
		}

		uint32 _ElementSize = VertexElementSizes[i];
		const void* bufferData = inInstancedMode ? nullptr : VertexData[i];
		uintptr dataSize = inInstancedMode ? _ElementSize * sizeof(float) : _ElementSize * sizeof(float) * NumVertices;
		
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, dataSize, bufferData, attribUsage);
		bufferSizes[i] = dataSize;

		// Because OpenGL doesn't support attributes with more than 4
		// elements, each set of 4 elements gets its own attribute.
		uint32 elementSizeDiv = _ElementSize/4;
		uint32 _ElementSizeRem = _ElementSize%4;
		for (uint32 j = 0; j < elementSizeDiv; j++) 
		{
			glEnableVertexAttribArray(_Attribute);
			glVertexAttribPointer(_Attribute, 4, GL_FLOAT, GL_FALSE, _ElementSize * sizeof(GLfloat), (const GLvoid*)(sizeof(GLfloat) * j * 4));
			if (inInstancedMode) 
			{
				glVertexAttribDivisor(_Attribute, 1);
			}

			_Attribute++;
		}
		if (_ElementSizeRem != 0) 
		{
			glEnableVertexAttribArray(_Attribute);
			glVertexAttribPointer(_Attribute, _ElementSize, GL_FLOAT, GL_FALSE,	_ElementSize * sizeof(GLfloat), (const GLvoid*)(sizeof(GLfloat) * elementSizeDiv * 4));
			if (inInstancedMode) 
			{
				glVertexAttribDivisor(_Attribute, 1);
			}
			_Attribute++;
		}
	}

	uintptr indicesSize = NumIndices * sizeof(uint32);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[numBuffers-1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, Indices, Usage);
	bufferSizes[numBuffers-1] = indicesSize;

	struct VertexArray vaoData;
	vaoData.buffers = buffers;
	vaoData.bufferSizes = bufferSizes;
	vaoData.numBuffers = numBuffers;
	vaoData.numElements = NumIndices;
	vaoData.usage = Usage;
	vaoData.instanceComponentsStartIndex = NumVertexComponents;
	vaoMap[VAO] = vaoData;
	return VAO;
}

void OpenGLRenderDevice::UpdateVertexArrayBuffer(uint32 vao, uint32 bufferIndex,
			const void* data, uintptr dataSize)
{
	if(vao == 0) {
		return;
	}

	Map<uint32, VertexArray>::iterator it = vaoMap.find(vao);
	if(it == vaoMap.end()) {
		return;
	}
	const struct VertexArray* vaoData = &it->second;
	enum BufferUsage usage;
	if(bufferIndex >= vaoData->instanceComponentsStartIndex) {
		usage = USAGE_DYNAMIC_DRAW;
	} else {
		usage = vaoData->usage;
	}

	setVAO(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vaoData->buffers[bufferIndex]);
	if(vaoData->bufferSizes[bufferIndex] >= dataSize) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
	} else {
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage);
		vaoData->bufferSizes[bufferIndex] = dataSize;
	}	
}

uint32 OpenGLRenderDevice::ReleaseVertexArray(uint32 vao)
{
	if(vao == 0) {
		return 0;
	}
	Map<uint32, VertexArray>::iterator it = vaoMap.find(vao);
	if(it == vaoMap.end()) {
		return 0;
	}
	const struct VertexArray* vaoData = &it->second;
	
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(vaoData->numBuffers, vaoData->buffers);
	delete[] vaoData->buffers;
	delete[] vaoData->bufferSizes;
	vaoMap.erase(it);
	return 0;
}

uint32 OpenGLRenderDevice::CreateSampler(enum SamplerFilter minFilter, enum SamplerFilter magFilter,
			enum SamplerWrapMode wrapU, enum SamplerWrapMode wrapV, float anisotropy)
{
	uint32 result = 0;
	glGenSamplers(1, &result);
	glSamplerParameteri(result, GL_TEXTURE_WRAP_S, wrapU);
	glSamplerParameteri(result, GL_TEXTURE_WRAP_T, wrapV);
	glSamplerParameteri(result, GL_TEXTURE_MAG_FILTER, magFilter);
	glSamplerParameteri(result, GL_TEXTURE_MIN_FILTER, minFilter);
	if(anisotropy != 0.0f && minFilter != FILTER_NEAREST && minFilter != FILTER_LINEAR) {
		glSamplerParameterf(result, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	}
	return result;
}

uint32 OpenGLRenderDevice::ReleaseSampler(uint32 sampler)
{
	if(sampler == 0) {
		return 0;
	}
	glDeleteSamplers(1, &sampler);
	return 0;
}

static GLint getOpenGLFormat(enum OpenGLRenderDevice::PixelFormat format)
{
	switch(format) {
	case OpenGLRenderDevice::FORMAT_R: return GL_RED;
	case OpenGLRenderDevice::FORMAT_RG: return GL_RG;
	case OpenGLRenderDevice::FORMAT_RGB: return GL_RGB;
	case OpenGLRenderDevice::FORMAT_RGBA: return GL_RGBA;
	case OpenGLRenderDevice::FORMAT_DEPTH: return GL_DEPTH_COMPONENT;
	case OpenGLRenderDevice::FORMAT_DEPTH_AND_STENCIL: return GL_DEPTH_STENCIL;
	default:
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "PixelFormat %d is not a valid PixelFormat.",
				format);
		return 0;
	};
}

static GLint getOpenGLInternalFormat(enum OpenGLRenderDevice::PixelFormat format, bool compress)
{
	switch(format) {
	case OpenGLRenderDevice::FORMAT_R: return GL_RED;
	case OpenGLRenderDevice::FORMAT_RG: return GL_RG;
	case OpenGLRenderDevice::FORMAT_RGB: 
		if(compress) {
			return GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
		} else {
			return GL_RGB;
			//return GL_SRGB;
		}
	case OpenGLRenderDevice::FORMAT_RGBA:
		if(compress) {
			// TODO: Decide on the default RGBA compression scheme
//			return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
		} else {
			return GL_RGBA;
			//return GL_SRGB_ALPHA;
		}
	case OpenGLRenderDevice::FORMAT_DEPTH: return GL_DEPTH_COMPONENT;
	case OpenGLRenderDevice::FORMAT_DEPTH_AND_STENCIL: return GL_DEPTH_STENCIL;
	default:
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "PixelFormat %d is not a valid PixelFormat.",
				format);
		return 0;
	};
}


uint32 OpenGLRenderDevice::CreateTexture2D(int32 Width, int32 Height, const void* data,
			enum PixelFormat dataFormat, enum PixelFormat internalFormatIn,
			bool generateMipmaps, bool compress)
{
	GLint format = getOpenGLFormat(dataFormat);
	GLint internalFormat = getOpenGLInternalFormat(internalFormatIn, compress);
	GLenum textureTarget = GL_TEXTURE_2D;
	GLuint textureHandle;

	glGenTextures(1, &textureHandle);
	glBindTexture(textureTarget, textureHandle);
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(textureTarget, 0, internalFormat, Width, Height, 0, format,
			GL_UNSIGNED_BYTE, data);

	if(generateMipmaps) {
		glGenerateMipmap(textureTarget);
	} else {
		glTexParameteri(textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
	}

	return textureHandle;
}

uint32 OpenGLRenderDevice::CreateDDSTexture2D(uint32 Width, uint32 Height, const unsigned char* buffer, uint32 fourCC, uint32 mipMapCount)
{
	GLint format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Invalid compression format for DDS texture\n");
		return 0;
	}
	
    GLuint textureID;
    glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	for (unsigned int level = 0; level < mipMapCount && (Width || Height); ++level)
	{
		unsigned int size = ((Width+3)/4)*((Height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, Width, Height, 
			0, size, buffer + offset);

		offset += size;
		Width  /= 2;
		Height /= 2;
	}

	return textureID;
}

uint32 OpenGLRenderDevice::ReleaseTexture2D(uint32 texture2D)
{
	if(texture2D == 0) {
		return 0;
	}
	glDeleteTextures(1, &texture2D);
	return 0;
}

uint32 OpenGLRenderDevice::createUniformBuffer(const void* data, uintptr dataSize,
		enum BufferUsage usage)
{
	uint32 ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, dataSize, data, usage);
	return ubo;
}

void OpenGLRenderDevice::updateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	void* dest = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	Memory::memcpy(dest, data, dataSize);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

uint32 OpenGLRenderDevice::releaseUniformBuffer(uint32 buffer)
{
	if(buffer == 0) {
		return 0;
	}
	glDeleteBuffers(1, &buffer);
	return 0;
}

uint32 OpenGLRenderDevice::createShaderProgram(const String& shaderText)
{
	GLuint shaderProgram = glCreateProgram();

	if(shaderProgram == 0) 
	{
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Error creating shader program\n");
        return (uint32)-1;
    }

	String version = getShaderVersion();
	String vertexShaderText = "#version " + version +
		"\n#define VS_BUILD\n#define GLSL_VERSION " + version + "\n" + shaderText;
	String fragmentShaderText = "#version " + version +
		"\n#define FS_BUILD\n#define GLSL_VERSION " + version + "\n" + shaderText;

	ShaderProgram programData;
	if(!addShader(shaderProgram, vertexShaderText, GL_VERTEX_SHADER,
				&programData.shaders)) {
		return (uint32)-1; 
	}
	if(!addShader(shaderProgram, fragmentShaderText, GL_FRAGMENT_SHADER,
				&programData.shaders)) {
		return (uint32)-1;
	}
	
	glLinkProgram(shaderProgram);
	if(checkShaderError(shaderProgram, GL_LINK_STATUS,
				true, "Error linking shader program")) {
		return (uint32)-1;
	}

    glValidateProgram(shaderProgram);
	if(checkShaderError(shaderProgram, GL_VALIDATE_STATUS,
				true, "Invalid shader program")) {
		return (uint32)-1;
	}

	addAllAttributes(shaderProgram, vertexShaderText, getVersion());
	addShaderUniforms(shaderProgram, shaderText, programData.uniformMap,
			programData.samplerMap);

	shaderProgramMap[shaderProgram] = programData;
	return shaderProgram;
}

void OpenGLRenderDevice::setShaderUniformBuffer(uint32 shader, const String& uniformBufferName,
			uint32 buffer)
{
	setShader(shader);
	glBindBufferBase(GL_UNIFORM_BUFFER, 
			shaderProgramMap[shader].uniformMap[uniformBufferName],
			buffer);
}

void OpenGLRenderDevice::setShaderSampler(uint32 shader, const String& samplerName,
		uint32 texture, uint32 sampler, uint32 unit)
{
	setShader(shader);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindSampler(unit, sampler);
	glUniform1i(shaderProgramMap[shader].samplerMap[samplerName], unit);
}


uint32 OpenGLRenderDevice::releaseShaderProgram(uint32 shader)
{
	if(shader == 0) {
		return 0;
	}
	Map<uint32, ShaderProgram>::iterator programIt = shaderProgramMap.find(shader);
	if(programIt == shaderProgramMap.end()) {
		return 0;
	}
	const struct ShaderProgram* shaderProgram = &programIt->second; 

	for(Array<uint32>::const_iterator it = shaderProgram->shaders.begin();
			it != shaderProgram->shaders.end(); ++it) 
	{
		glDetachShader(shader, *it);
		glDeleteShader(*it);
	}
	glDeleteProgram(shader);
	shaderProgramMap.erase(programIt);
	return 0;
}
uint32 OpenGLRenderDevice::getVersion()
{
	if(version != 0) {
		return version;
	}

	int32 majorVersion;
	int32 minorVersion;
		
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion); 
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
		
	version = (uint32)(majorVersion * 100 + minorVersion * 10);
	return version;
}

String OpenGLRenderDevice::getShaderVersion()
{
	if(!shaderVersion.empty()) {
		return shaderVersion;
	}
	
	uint32 version = getVersion();

	if(version >= 330) {
		shaderVersion = FString::toString(version);
	}
	else if(version >= 320) {
		shaderVersion = "150";
	}
	else if(version >= 310) {
		shaderVersion = "140";
	}
	else if(version >= 300) {
		shaderVersion = "130";
	}
	else if(version >= 210) {
		shaderVersion = "120";
	}
	else if(version >= 200) {
		shaderVersion = "110";
	}
	else {
		int32 majorVersion = version / 100;
		int32 minorVersion = (version / 10) % 10;
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR,
				"Error: OpenGL Version %d.%d does not support shaders.\n",
				majorVersion, minorVersion);
		return "";
	}

	return shaderVersion;
}

static bool addShader(GLuint shaderProgram, const String& text, GLenum type,
		Array<GLuint>* shaders)
{
	GLuint shader = glCreateShader(type);

	if(shader == 0)
	{
		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Error creating shader type %d", type);
		return false;
	}

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = (GLint)text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
	{
        GLchar InfoLog[1024];

        glGetShaderInfoLog(shader, 1024, NULL, InfoLog);
        DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "Error compiling shader type %d: '%s'\n",
				shader, InfoLog);	
		return false;
    }

	glAttachShader(shaderProgram, shader);
	shaders->push_back(shader);
	return true;
}

static bool checkShaderError(GLuint shader, int flag,
		bool isProgram, const String& errorMessage)
{
	GLint success = 0;
    GLchar error[1024] = { 0 };

	if(isProgram) {
		glGetProgramiv(shader, flag, &success);
	} else {
		glGetShaderiv(shader, flag, &success);
	}

	if(!success) {
		if(isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		} else {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR, "%s: '%s'\n", errorMessage.c_str(), error);
		return true;
	}
	return false;
}

static void addAllAttributes(GLuint program, const String& vertexShaderText, uint32 version)
{
	if(version >= 320) {
		// Layout is enabled. Return.
		return;
	}

	// FIXME: This code assumes attributes are listed in order, which isn't
	// true for all compilers. It's safe to ignore for now because OpenGL versions
	// requiring this aren't being used.
	GLint numActiveAttribs = 0;
	GLint maxAttribNameLength = 0;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);

//	DEBUG_LOG_TEMP2("Adding attributes!");
//	DEBUG_LOG_TEMP("%i %i", numActiveAttribs, maxAttribNameLength);
	Array<GLchar> nameData(maxAttribNameLength);
	for(GLint attrib = 0; attrib < numActiveAttribs; ++attrib) {
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;

		glGetActiveAttrib(program, attrib, nameData.size(),
				&actualLength, &arraySize, &type, &nameData[0]);
		glBindAttribLocation(program, attrib, (char*)&nameData[0]);
//		DEBUG_LOG_TEMP2("Adding attribute!");
//		DEBUG_LOG_TEMP("%s: %d", (char*)&nameData[0], attrib);
	}
}

static void addShaderUniforms(GLuint shaderProgram, const String& shaderText,
		Map<String, GLint>& uniformMap, Map<String, GLint>& samplerMap)
{
	GLint numBlocks;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
	for(int32 block = 0; block < numBlocks; ++block) {
		GLint nameLen;
		glGetActiveUniformBlockiv(shaderProgram, block,
				GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);

		Array<GLchar> name(nameLen);
		glGetActiveUniformBlockName(shaderProgram, block, nameLen, NULL, &name[0]);
		String uniformBlockName((char*)&name[0], nameLen-1);
		uniformMap[uniformBlockName] = glGetUniformBlockIndex(shaderProgram, &name[0]);
	}

	GLint numUniforms = 0;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &numBlocks);
	
	// Would get GL_ACTIVE_UNIFORM_MAX_LENGTH, but buggy on some drivers
	Array<GLchar> uniformName(256); 
	for(int32 uniform = 0; uniform < numUniforms; ++uniform) {
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(shaderProgram, uniform, uniformName.size(),
				&actualLength, &arraySize, &type, &uniformName[0]);
		if(type != GL_SAMPLER_2D) {
			DEBUG_LOG(LOG_TYPE_RENDERER, LOG_ERROR,
					"Non-sampler2d uniforms currently unsupported!");
			continue;
		}
		String name((char*)&uniformName[0], actualLength - 1);
		samplerMap[name] = glGetUniformLocation(shaderProgram, (char*)&uniformName[0]);
	}
}

