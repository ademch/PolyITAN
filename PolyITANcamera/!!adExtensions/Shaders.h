
#ifndef SHADERS_H
#define SHADERS_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <map>
#include <assert.h>
#include "../!!adGlobals/TextureDescriptor.h"

// list of indices every Application must support
#define FFP        0
#define OS_FBuffer 0


class GPUProgram
{
public:
	GPUProgram(GLhandleARB _id)
	{
		programObj = _id;
	}

	GLhandleARB programObj;
	std::map<std::string, int> Uniforms;

	int GetUniform(std::string name)
	{
		std::map<std::string, int>::iterator iter;

		iter = Uniforms.find(name);
		assert(iter != Uniforms.end());
		if (iter == Uniforms.end())	return -1;

		return iter->second;
	}

	void addShader(const GLcharARB *shaderSource, GLenum shaderType);
	void link_program();

protected:
	void printInfoLog(GLhandleARB object);
};


class GLSL_AbstractPipeline
{
public:
	GLSL_AbstractPipeline(): m_CurShader(FFP)
	{	_this = this;
	    bUseShaderPipeline = false;
	}

	virtual ~GLSL_AbstractPipeline()
	{
		destroy_shaders();
	}

	std::map<std::string, GPUProgram*> GPUPrograms;

	GPUProgram* operator[](std::string name)
	{
		std::map<std::string, GPUProgram*>::iterator iter;

		iter = GPUPrograms.find(name);
		if (iter == GPUPrograms.end())	return NULL;

		return iter->second;
	}

	virtual bool init_shaders() = 0;
	void destroy_shaders();

	GLhandleARB m_CurShader;
	bool bUseShaderPipeline;

	static GLSL_AbstractPipeline* getPtr()
	{	return _this;
	}

protected:
	bool CreateShader(const char* name, const char* nameVertexProgram, const char* nameFragmentProgram);
	bool CreateShaderR(const char* name, const char* sourceVertexProgram, const char* sourceFragmentProgram);
	GLint glGetUniformLocationARBassert(GLhandleARB programObj, const GLcharARB *name);

private:
	static GLSL_AbstractPipeline* _this;
};



class FrameBufferObject
{
public:
	FrameBufferObject(int iWidth, int iHeight);
	~FrameBufferObject()
	{
		free(m_dataCPUmem);
	}

	GLuint m_uiTextureID;
	GLuint m_uiTextureID2;
	GLuint m_uiFramebufferID;

	int m_iWidth;
	int m_iHeight;

	void Activate();
	void Deactivate();

	TextureDescriptor*  Init(bool bDepth = false);
	TextureDescriptor** Init2();
	void Init(TextureDescriptor* texDesc, bool bDepth = false);

	unsigned char* DownloadTexture(GLenum format, unsigned int nrChannels = 1);
	void Resize(bool bDepth);

	void SetupGraphicsPipeline();

protected:
	unsigned char *m_dataCPUmem;

};



#endif
