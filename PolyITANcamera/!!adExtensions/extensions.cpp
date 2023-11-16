#include <GL/gl.h>
#include <GL/glext.h>
//#include <GL/wglext.h>
#include <cstddef>
#include <string.h>
#include <GL/freeglut.h>
#include <stdio.h>

PFNGLDRAWBUFFERSPROC              glDrawBuffersARB = NULL;
PFNGLDELETEOBJECTARBPROC          glDeleteObjectARB  = NULL;
//PFNGLGETHANDLEARBPROC			  glGetHandleARB  = NULL;
//PFNGLDETACHOBJECTARBPROC		  glDetachObjectARB  = NULL;
PFNGLCREATESHADEROBJECTARBPROC	  glCreateShaderObjectARB  = NULL;
PFNGLSHADERSOURCEARBPROC		  glShaderSourceARB  = NULL;
PFNGLCOMPILESHADERARBPROC		  glCompileShaderARB  = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC	  glCreateProgramObjectARB  = NULL;
PFNGLATTACHOBJECTARBPROC		  glAttachObjectARB  = NULL;
PFNGLLINKPROGRAMARBPROC			  glLinkProgramARB  = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC	  glUseProgramObjectARB  = NULL;
PFNGLVALIDATEPROGRAMARBPROC		  glValidateProgramARB  = NULL;
PFNGLUNIFORM1FARBPROC			  glUniform1fARB  = NULL;
//PFNGLUNIFORM2FARBPROC			  glUniform2fARB  = NULL;
PFNGLUNIFORM3FARBPROC			  glUniform3fARB  = NULL;
PFNGLUNIFORM4FARBPROC			  glUniform4fARB  = NULL;
PFNGLUNIFORM1IARBPROC			  glUniform1iARB  = NULL;
PFNGLUNIFORM2IARBPROC			  glUniform2iARB  = NULL;
//PFNGLUNIFORM3IARBPROC			  glUniform3iARB  = NULL;
//PFNGLUNIFORM4IARBPROC			  glUniform4iARB  = NULL;
//PFNGLUNIFORM1FVARBPROC			  glUniform1fvARB  = NULL;
PFNGLUNIFORM2FVARBPROC			  glUniform2fvARB  = NULL;
//PFNGLUNIFORM3FVARBPROC			  glUniform3fvARB  = NULL;
//PFNGLUNIFORM4FVARBPROC			  glUniform4fvARB  = NULL;
//PFNGLUNIFORM1IVARBPROC			  glUniform1ivARB  = NULL;
//PFNGLUNIFORM2IVARBPROC			  glUniform2ivARB  = NULL;
//PFNGLUNIFORM3IVARBPROC			  glUniform3ivARB  = NULL;
//PFNGLUNIFORM4IVARBPROC			  glUniform4ivARB  = NULL;
//PFNGLUNIFORMMATRIX2FVARBPROC	  glUniformMatrix2fvARB  = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC	  glUniformMatrix3fvARB  = NULL;
//PFNGLUNIFORMMATRIX4FVARBPROC	  glUniformMatrix4fvARB  = NULL;
//PFNGLGETOBJECTPARAMETERFVARBPROC  glGetObjectParameterfvARB  = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC  glGetObjectParameterivARB  = NULL;
PFNGLGETINFOLOGARBPROC			  glGetInfoLogARB  = NULL;
//PFNGLGETATTACHEDOBJECTSARBPROC	  glGetAttachedObjectsARB  = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC	  glGetUniformLocationARB  = NULL;
//PFNGLGETACTIVEUNIFORMARBPROC	  glGetActiveUniformARB  = NULL;
//PFNGLGETUNIFORMFVARBPROC		  glGetUniformfvARB  = NULL;
//PFNGLGETUNIFORMIVARBPROC		  glGetUniformivARB  = NULL;
//PFNGLGETSHADERSOURCEARBPROC	  glGetShaderSourceARB  = NULL;

PFNGLGETATTRIBLOCATIONARBPROC    glGetAttribLocationARB  = NULL;
PFNGLVERTEXATTRIB2FVARBPROC		 glVertexAttrib2fvARB = NULL;

// ARB_vertex_buffer_object
PFNGLBINDBUFFERARBPROC           glBindBufferARB           = NULL;
PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB        = NULL;
PFNGLGENBUFFERSARBPROC           glGenBuffersARB           = NULL;
PFNGLISBUFFERARBPROC             glIsBufferARB             = NULL;
PFNGLBUFFERDATAARBPROC           glBufferDataARB           = NULL;
PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB        = NULL;
PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB     = NULL;
PFNGLMAPBUFFERARBPROC            glMapBufferARB            = NULL;
PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB          = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB    = NULL;

PFNGLPOINTPARAMETERFARBPROC					   glPointParameterfARB   = NULL;
PFNGLPOINTPARAMETERFVARBPROC				   glPointParameterfvARB  = NULL;

// FrameBufferObject
PFNGLISRENDERBUFFERPROC                        glIsRenderbufferEXT;
PFNGLBINDRENDERBUFFERPROC                      glBindRenderbufferEXT;
PFNGLDELETERENDERBUFFERSPROC                   glDeleteRenderbuffersEXT;
PFNGLGENRENDERBUFFERSPROC                      glGenRenderbuffersEXT;
PFNGLRENDERBUFFERSTORAGEPROC                   glRenderbufferStorageEXT;
PFNGLGETRENDERBUFFERPARAMETERIVPROC            glGetRenderbufferParameterivEXT;
PFNGLISFRAMEBUFFERPROC                         glIsFramebufferEXT;
PFNGLBINDFRAMEBUFFERPROC                       glBindFramebufferEXT;
PFNGLDELETEFRAMEBUFFERSPROC                    glDeleteFramebuffersEXT;
PFNGLGENFRAMEBUFFERSPROC                       glGenFramebuffersEXT;
PFNGLCHECKFRAMEBUFFERSTATUSPROC                glCheckFramebufferStatusEXT;
PFNGLFRAMEBUFFERTEXTURE1DPROC                  glFramebufferTexture1DEXT;
PFNGLFRAMEBUFFERTEXTURE2DPROC                  glFramebufferTexture2DEXT;
PFNGLFRAMEBUFFERTEXTURE3DPROC                  glFramebufferTexture3DEXT;
PFNGLFRAMEBUFFERRENDERBUFFERPROC               glFramebufferRenderbufferEXT;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC   glGetFramebufferAttachmentParameterivEXT;
PFNGLGENERATEMIPMAPPROC                        glGenerateMipmapEXT;

//UniformBufferObject
PFNGLGENBUFFERSPROC                            glGenBuffers;
PFNGLGETUNIFORMBLOCKINDEXPROC                  glGetUniformBlockIndex;


bool isExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');

	if (where || *extension == '\0') return 0;

	extensions=glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings, etc. */

	start=extensions;
	for (;;) 
	{
		where=(GLubyte *) strstr((const char *) start, extension);
		if (!where) break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0') return true;
		start = terminator;
	}

	return false;
}


bool isExtensionSupportedWGL(const char *extension)
{
	const GLubyte *wgl_extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');

	if (where || *extension == '\0') return 0;

	//  extensions=glGetString(GL_EXTENSIONS);
#ifdef _WIN32
	{
		PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = 0;
		wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
		if(wglGetExtensionsStringARB)
			wgl_extensions = (const GLubyte *) wglGetExtensionsStringARB(wglGetCurrentDC());
	}
#endif

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings, etc. */

	start=wgl_extensions;
	for (;;) 
	{
		where=(GLubyte *) strstr((const char *) start, extension);
		if (!where) break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0') return true;
		start = terminator;
	}

	return false;
}

void linkExtensions()
{
	glDrawBuffersARB =          (PFNGLDRAWBUFFERSPROC)              glutGetProcAddress("glDrawBuffersARB");
	glGetInfoLogARB =           (PFNGLGETINFOLOGARBPROC)            glutGetProcAddress("glGetInfoLogARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)  glutGetProcAddress("glGetObjectParameterivARB");
	glDeleteObjectARB=          (PFNGLDELETEOBJECTARBPROC)          glutGetProcAddress("glDeleteObjectARB");
	glCompileShaderARB=         (PFNGLCOMPILESHADERARBPROC)         glutGetProcAddress("glCompileShaderARB");
	glShaderSourceARB=          (PFNGLSHADERSOURCEARBPROC)          glutGetProcAddress("glShaderSourceARB");
	glCreateShaderObjectARB=    (PFNGLCREATESHADEROBJECTARBPROC)    glutGetProcAddress("glCreateShaderObjectARB");
	glUseProgramObjectARB=      (PFNGLUSEPROGRAMOBJECTARBPROC)      glutGetProcAddress("glUseProgramObjectARB");
	glValidateProgramARB=       (PFNGLVALIDATEPROGRAMARBPROC)       glutGetProcAddress("glValidateProgramARB");
	glCreateProgramObjectARB=   (PFNGLCREATEPROGRAMOBJECTARBPROC)   glutGetProcAddress("glCreateProgramObjectARB");
	glLinkProgramARB=           (PFNGLLINKPROGRAMARBPROC)           glutGetProcAddress("glLinkProgramARB");
	glAttachObjectARB=          (PFNGLATTACHOBJECTARBPROC)          glutGetProcAddress("glAttachObjectARB");
	glUniform1iARB=             (PFNGLUNIFORM1IARBPROC)             glutGetProcAddress("glUniform1iARB");
	glUniform2iARB=             (PFNGLUNIFORM2IARBPROC)             glutGetProcAddress("glUniform2iARB");
	glGetUniformLocationARB=    (PFNGLGETUNIFORMLOCATIONARBPROC)    glutGetProcAddress("glGetUniformLocationARB");
	glUniform1fARB=             (PFNGLUNIFORM1FARBPROC)             glutGetProcAddress("glUniform1fARB");
	glUniform3fARB=             (PFNGLUNIFORM3FARBPROC)             glutGetProcAddress("glUniform3fARB");
	glUniform2fvARB=            (PFNGLUNIFORM2FVARBPROC)            glutGetProcAddress("glUniform2fvARB");
	glUniformMatrix3fvARB=      (PFNGLUNIFORMMATRIX3FVARBPROC)      glutGetProcAddress("glUniformMatrix3fvARB");
	glUniform4fARB=             (PFNGLUNIFORM4FARBPROC)             glutGetProcAddress("glUniform4fARB");

	glGetAttribLocationARB=		(PFNGLGETATTRIBLOCATIONARBPROC)     glutGetProcAddress("glGetAttribLocationARB");
	glVertexAttrib2fvARB=       (PFNGLVERTEXATTRIB2FVARBPROC)       glutGetProcAddress("glVertexAttrib2fvARB");

	glBindBufferARB           = (PFNGLBINDBUFFERARBPROC)            glutGetProcAddress("glBindBufferARB");
	glDeleteBuffersARB        = (PFNGLDELETEBUFFERSARBPROC)         glutGetProcAddress("glDeleteBuffersARB");
	glGenBuffersARB           = (PFNGLGENBUFFERSARBPROC)            glutGetProcAddress("glGenBuffersARB");
	glIsBufferARB             = (PFNGLISBUFFERARBPROC)              glutGetProcAddress("glIsBufferARB");
	glBufferDataARB           = (PFNGLBUFFERDATAARBPROC)            glutGetProcAddress("glBufferDataARB");
	glBufferSubDataARB        = (PFNGLBUFFERSUBDATAARBPROC)         glutGetProcAddress("glBufferSubDataARB");
	glGetBufferSubDataARB     = (PFNGLGETBUFFERSUBDATAARBPROC)      glutGetProcAddress("glGetBufferSubDataARB");
	glMapBufferARB            = (PFNGLMAPBUFFERARBPROC)             glutGetProcAddress("glMapBufferARB");
	glUnmapBufferARB          = (PFNGLUNMAPBUFFERARBPROC)           glutGetProcAddress("glUnmapBufferARB");
	glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)  glutGetProcAddress("glGetBufferParameterivARB");
	glGetBufferPointervARB    = (PFNGLGETBUFFERPOINTERVARBPROC)     glutGetProcAddress("glGetBufferPointervARB");

	glPointParameterfARB  =     (PFNGLPOINTPARAMETERFARBPROC)		glutGetProcAddress("glPointParameterfARB");
	glPointParameterfvARB =     (PFNGLPOINTPARAMETERFVARBPROC)		glutGetProcAddress("glPointParameterfvARB");


	// FrameBufferObject
	glIsRenderbufferEXT             = (PFNGLISRENDERBUFFERPROC)             glutGetProcAddress("glIsRenderbufferEXT");                     
	glBindRenderbufferEXT			= (PFNGLBINDRENDERBUFFERPROC)			glutGetProcAddress("glBindRenderbufferEXT");					
	glDeleteRenderbuffersEXT		= (PFNGLDELETERENDERBUFFERSPROC)		glutGetProcAddress("glDeleteRenderbuffersEXT");
	glGenRenderbuffersEXT			= (PFNGLGENRENDERBUFFERSPROC)			glutGetProcAddress("glGenRenderbuffersEXT");
	glRenderbufferStorageEXT		= (PFNGLRENDERBUFFERSTORAGEPROC)		glutGetProcAddress("glRenderbufferStorageEXT");
	glGetRenderbufferParameterivEXT	= (PFNGLGETRENDERBUFFERPARAMETERIVPROC)	glutGetProcAddress("glGetRenderbufferParameterivEXT");
	glIsFramebufferEXT				= (PFNGLISFRAMEBUFFERPROC)				glutGetProcAddress("glIsFramebufferEXT");
	glBindFramebufferEXT			= (PFNGLBINDFRAMEBUFFERPROC)			glutGetProcAddress("glBindFramebufferEXT");
	glDeleteFramebuffersEXT			= (PFNGLDELETEFRAMEBUFFERSPROC)			glutGetProcAddress("glDeleteFramebuffersEXT");
	glGenFramebuffersEXT			= (PFNGLGENFRAMEBUFFERSPROC)			glutGetProcAddress("glGenFramebuffersEXT");
	glCheckFramebufferStatusEXT		= (PFNGLCHECKFRAMEBUFFERSTATUSPROC)		glutGetProcAddress("glCheckFramebufferStatusEXT");
	glFramebufferTexture1DEXT		= (PFNGLFRAMEBUFFERTEXTURE1DPROC)		glutGetProcAddress("glFramebufferTexture1DEXT");
	glFramebufferTexture2DEXT		= (PFNGLFRAMEBUFFERTEXTURE2DPROC)		glutGetProcAddress("glFramebufferTexture2DEXT");
	glFramebufferTexture3DEXT		= (PFNGLFRAMEBUFFERTEXTURE3DPROC)		glutGetProcAddress("glFramebufferTexture3DEXT");
	glFramebufferRenderbufferEXT	= (PFNGLFRAMEBUFFERRENDERBUFFERPROC)	glutGetProcAddress("glFramebufferRenderbufferEXT");
	glGetFramebufferAttachmentParameterivEXT= (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)   glutGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
	glGenerateMipmapEXT				= (PFNGLGENERATEMIPMAPPROC)	            glutGetProcAddress("glGenerateMipmapEXT");
}


bool CheckExtensions()
{
	printf("\n");

	printf("Is GL_ARB_shading_language_100 supported...");
	if(isExtensionSupported("GL_ARB_shading_language_100"))
		printf("yes\n");
	else
	{
		printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_shader_objects supported...");
	if(isExtensionSupported("GL_ARB_shader_objects"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_vertex_shader supported...");
	if(isExtensionSupported("GL_ARB_vertex_shader"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_fragment_shader supported...");
	if(isExtensionSupported("GL_ARB_fragment_shader"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_multitexture supported...");
	if(isExtensionSupported("GL_ARB_multitexture"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_vertex_buffer_object supported...");
	if(isExtensionSupported("GL_ARB_vertex_buffer_object"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_point_parameters supported...");
	if(isExtensionSupported("GL_ARB_point_parameters"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_point_sprite supported...");
	if(isExtensionSupported("GL_ARB_point_sprite"))
		printf("yes\n");
	else
	{	printf("no\n");		//by default FALSE
		return false;
	}

	//printf("Is GL_EXT_texture_filter_anisotropic supported...");
	//if(isExtensionSupported("GL_EXT_texture_filter_anisotropic"))
	//{	printf("yes\n");
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.f);
	//}
	//else
	//{	printf("no\n");		//by default FALSE
		//return false;
	//}

	printf("Is GL_EXT_framebuffer_object...");
	if (isExtensionSupported("GL_EXT_framebuffer_object"))
		printf("yes\n");
	else
	{
		printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_texture_non_power_of_two...");
	if (isExtensionSupported("GL_ARB_texture_non_power_of_two"))
		printf("yes\n");
	else
	{
		printf("no\n");		//by default FALSE
		return false;
	}

	printf("Is GL_ARB_draw_buffers...");
	if (isExtensionSupported("GL_ARB_draw_buffers"))
		printf("yes\n");
	else
	{
		printf("no\n");		//by default FALSE
		return false;
	}

return true;
}
