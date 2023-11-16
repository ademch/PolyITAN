#include <assert.h>
#include <stdlib.h>

#include "extensions.h"
#include "read_shader_file.h"
#include <GL/glu.h>
#include "Shaders.h"
#include "../!!adGlobals/wdir.h"
#include <iostream>
#include <string.h>


using namespace std;

GLSL_AbstractPipeline* GLSL_AbstractPipeline::_this = NULL;

void GPUProgram::printInfoLog(GLhandleARB object)
{
	int maxLength = 0;
	glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);

	if (maxLength)
	{
		char *infoLog = new char[maxLength];
		glGetInfoLogARB(object, maxLength, &maxLength, infoLog);

		if (maxLength)
			printf("%s\n", infoLog);

		delete [] infoLog;
	}
}


void GPUProgram::addShader(const GLcharARB *shaderSource, GLenum shaderType)
{
	assert(programObj != 0);
	assert(shaderSource != 0);
	assert(shaderType != 0);

	GLhandleARB object = glCreateShaderObjectARB(shaderType);
	if (object == 0)
	{
		printf("Error creating shader object\n");
		exit(-1);
	}

	glShaderSourceARB(object, 1, &shaderSource, NULL);

	// compile vertex shader object
	glCompileShaderARB(object);

	// check if shader compiled
	GLint bCompiled = 0;
	glGetObjectParameterivARB(object, GL_OBJECT_COMPILE_STATUS_ARB, &bCompiled);

	printInfoLog(object);
	if (!bCompiled)
	{	
		string str;
		cin >> str;
		exit(-1);
	}

	// attach vertex shader to program object
	glAttachObjectARB(programObj, object);

	// delete vertex object, no longer needed
	glDeleteObjectARB(object);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		cout << "OpenGL error: " << gluErrorString(err) << endl;

	delete [] shaderSource;
}


void GPUProgram::link_program()
{
	glLinkProgramARB(programObj);

	GLint linked = false;
	glGetObjectParameterivARB(programObj, GL_OBJECT_LINK_STATUS_ARB, &linked);
	printInfoLog(programObj);
	if (!linked)
	{
		cout << "Shaders failed to link, exiting..." << endl;
		string str;
		cin >> str;
		exit(-1);
	}

	glValidateProgramARB(programObj);

	GLint validated = false;
	glGetObjectParameterivARB(programObj, GL_OBJECT_VALIDATE_STATUS_ARB, &validated);
	printInfoLog(programObj);
	if (!validated)
	{
		cout << "Shaders failed to validate, exiting..." << endl;
		exit(-1);
	}
}

GLint GLSL_AbstractPipeline::glGetUniformLocationARBassert(GLhandleARB programObj, const GLcharARB *name)
{
	GLint iLocation = glGetUniformLocationARB(programObj, name);
	assert(iLocation >= 0);

	return iLocation;
}


bool GLSL_AbstractPipeline::CreateShader(const char* name, const char* nameVertexProgram, const char* nameFragmentProgram)
{
GLcharARB* shaderData;

	GPUPrograms[name] = new GPUProgram(glCreateProgramObjectARB());

	shaderData = read_shader_file(FullPathToFile(nameVertexProgram));
	if (shaderData == NULL)	return false;
	GPUPrograms[name]->addShader(shaderData, GL_VERTEX_SHADER_ARB);

	shaderData = read_shader_file(FullPathToFile(nameFragmentProgram));
	if (shaderData == NULL)	return false;
	GPUPrograms[name]->addShader(shaderData, GL_FRAGMENT_SHADER_ARB);

	GPUPrograms[name]->link_program();

	return true;
}


bool GLSL_AbstractPipeline::CreateShaderR(const char* name, const char* sourceVertexProgram, const char* sourceFragmentProgram)
{
	GLcharARB* shaderData;

	GPUPrograms[name] = new GPUProgram(glCreateProgramObjectARB());

	shaderData = new char[strlen(sourceVertexProgram)+1];
	strcpy(shaderData, sourceVertexProgram);
	GPUPrograms[name]->addShader(shaderData, GL_VERTEX_SHADER_ARB);

	shaderData = new char[strlen(sourceFragmentProgram) + 1];
	strcpy(shaderData, sourceFragmentProgram);
	GPUPrograms[name]->addShader(shaderData, GL_FRAGMENT_SHADER_ARB);

	GPUPrograms[name]->link_program();

	return true;
}

void GLSL_AbstractPipeline::destroy_shaders()
{
	std::map<string, GPUProgram*>::iterator iter;
	
	//When a container object is deleted, it will detach each attached object as part of the deletion process. 
	for (iter = GPUPrograms.begin(); iter != GPUPrograms.end(); ++iter)
	{
		glDeleteObjectARB(iter->second->programObj);
		delete iter->second;
	}

	GPUPrograms.clear();
}

FrameBufferObject::FrameBufferObject(int iWidth, int iHeight)
{
	m_iWidth  = iWidth;
	m_iHeight = iHeight;

	m_uiTextureID     = 0;
	m_uiTextureID2    = 0;
	m_uiFramebufferID = 0;

	m_dataCPUmem = NULL;
}


TextureDescriptor* FrameBufferObject::Init(bool bDepth)
{
	// The framebuffer holding renderbuffers (color, depth or stencil) or textures as a color buffer
	glGenFramebuffersEXT(1, &m_uiFramebufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_uiFramebufferID);

	glGenTextures(1, &m_uiTextureID);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

	// Reserve texture space
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// attach m_uiTextureID as GL_COLOR_ATTACHMENT0
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_uiTextureID, 0);

	if (bDepth) {
		GLuint depth_rb;
		glGenRenderbuffersEXT(1, &depth_rb);

		// initialize depth renderbuffer
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
	}

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, OS_FBuffer);

	return new TextureDescriptor(m_uiTextureID, m_iWidth, m_iHeight);
}

TextureDescriptor** FrameBufferObject::Init2()
{
	// The framebuffer holding renderbuffers (color, depth or stencil) or textures as a color buffer
	glGenFramebuffersEXT(1, &m_uiFramebufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_uiFramebufferID);

	glGenTextures(1, &m_uiTextureID);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

	// Provide empty texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// attach m_uiTextureID as GL_COLOR_ATTACHMENT0
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_uiTextureID, 0);

	glGenTextures(1, &m_uiTextureID2);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID2);

	// Provide empty texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// attach m_uiTextureID as GL_COLOR_ATTACHMENT1
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, m_uiTextureID2, 0);

	// the buffer selection in glDrawBuffers is part of the framebuffer object state.
	// Spec: The intial value of DRAW_BUFFER for application-created framebuffer objects is COLOR_ATTACHMENT0_EXT.
	// If a fragment shader writes to "gl_FragColor", DrawBuffers specifies a set of draw buffers into which
	// the single fragment color defined by "gl_FragColor" is written.

    GLuint drawbuffers[2] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};
    glDrawBuffersARB(2, drawbuffers);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, OS_FBuffer);

	TextureDescriptor **descriptors = (TextureDescriptor **)malloc(sizeof(TextureDescriptor *)*2);
	descriptors[0] = new TextureDescriptor(m_uiTextureID, m_iWidth, m_iHeight);
	descriptors[1] = new TextureDescriptor(m_uiTextureID2, m_iWidth, m_iHeight);

	return descriptors;
}


// Attach existing texture
void FrameBufferObject::Init(TextureDescriptor* texDesc, bool bDepth)
{
	assert(texDesc->m_uiTextureID > 0);
	assert(texDesc->m_width == m_iWidth);
	assert(texDesc->m_height == m_iHeight);
	
	// The framebuffer holding renderbuffers (color, depth or stencil) or textures as a color buffer
	glGenFramebuffersEXT(1, &m_uiFramebufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_uiFramebufferID);

	m_uiTextureID = texDesc->m_uiTextureID;

	// attach m_uiTextureID as GL_COLOR_ATTACHMENT0
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_uiTextureID, 0);

	if (bDepth) {
		GLuint depth_rb;
		glGenRenderbuffersEXT(1, &depth_rb);

		// initialize depth renderbuffer
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
	}

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, OS_FBuffer);
}


void FrameBufferObject::Activate()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_uiFramebufferID);
}

void FrameBufferObject::Deactivate()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, OS_FBuffer);
}


unsigned char* FrameBufferObject::DownloadTexture(GLenum format, unsigned int nrChannels)
{
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

	if (m_dataCPUmem == NULL)
		m_dataCPUmem = (unsigned char *)malloc(m_iWidth*m_iHeight*nrChannels);
	memset(m_dataCPUmem, 0, m_iWidth*m_iHeight*nrChannels);

	glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, m_dataCPUmem);

	return m_dataCPUmem;
}

void FrameBufferObject::Resize(bool bDepth)
{
}

void FrameBufferObject::SetupGraphicsPipeline()
{
	glViewport(0, 0, m_iWidth, m_iHeight);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);

		glLoadIdentity();
		glOrtho(-m_iWidth/2.0f,m_iWidth/2.0f, -m_iHeight/2.0f,m_iHeight/2.0f, -3000,3000);

	glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
}
