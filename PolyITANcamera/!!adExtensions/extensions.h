//

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <GL/gl.h>
#include <GL/glext.h>

// This macro helps us to pass an integer value via the "pointer" argument of 
// functions like glTexCoordPointer, glNormalPointer, and glVertexPointer.
// Normally, we would use the "pointer" argument to pass the vertex data in,
// but when we use the glBindBufferARB function of ARB_vertex_buffer_object,
// this argument is used as an offset instead
#define GRAPHICS_BUFFER_OFFSET(i) ((char *)NULL + (i))


bool isExtensionSupported(const char *extension);
bool isExtensionSupportedWGL(const char *extension);
void linkExtensions();
bool CheckExtensions();

extern PFNGLDRAWBUFFERSPROC                glDrawBuffersARB;
extern PFNGLDELETEOBJECTARBPROC            glDeleteObjectARB;  
extern PFNGLGETHANDLEARBPROC			   glGetHandleARB;  
extern PFNGLDETACHOBJECTARBPROC		       glDetachObjectARB;  
extern PFNGLCREATESHADEROBJECTARBPROC	   glCreateShaderObjectARB;  
extern PFNGLSHADERSOURCEARBPROC			   glShaderSourceARB;  
extern PFNGLCOMPILESHADERARBPROC		   glCompileShaderARB;  
extern PFNGLCREATEPROGRAMOBJECTARBPROC	   glCreateProgramObjectARB;  
extern PFNGLATTACHOBJECTARBPROC		       glAttachObjectARB;  
extern PFNGLLINKPROGRAMARBPROC			   glLinkProgramARB;  
extern PFNGLUSEPROGRAMOBJECTARBPROC	       glUseProgramObjectARB;  
extern PFNGLVALIDATEPROGRAMARBPROC		   glValidateProgramARB;  
extern PFNGLUNIFORM1FARBPROC			   glUniform1fARB;  
extern PFNGLUNIFORM2FARBPROC			   glUniform2fARB;  
extern PFNGLUNIFORM3FARBPROC			   glUniform3fARB;  
extern PFNGLUNIFORM4FARBPROC			   glUniform4fARB;  
extern PFNGLUNIFORM1IARBPROC			   glUniform1iARB;  
extern PFNGLUNIFORM2IARBPROC			   glUniform2iARB;  
extern PFNGLUNIFORM3IARBPROC			   glUniform3iARB;  
extern PFNGLUNIFORM4IARBPROC			   glUniform4iARB;  
extern PFNGLUNIFORM1FVARBPROC			   glUniform1fvARB;  
extern PFNGLUNIFORM2FVARBPROC			   glUniform2fvARB;  
extern PFNGLUNIFORM3FVARBPROC			   glUniform3fvARB;  
extern PFNGLUNIFORM4FVARBPROC			   glUniform4fvARB;  
extern PFNGLUNIFORM1IVARBPROC			   glUniform1ivARB;  
extern PFNGLUNIFORM2IVARBPROC			   glUniform2ivARB;  
extern PFNGLUNIFORM3IVARBPROC			   glUniform3ivARB;  
extern PFNGLUNIFORM4IVARBPROC			   glUniform4ivARB;  
extern PFNGLUNIFORMMATRIX2FVARBPROC		   glUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC		   glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC		   glUniformMatrix4fvARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC    glGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC    glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC			   glGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC	   glGetAttachedObjectsARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC	   glGetUniformLocationARB;
extern PFNGLGETACTIVEUNIFORMARBPROC		   glGetActiveUniformARB;
extern PFNGLGETUNIFORMFVARBPROC			   glGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC			   glGetUniformivARB;
extern PFNGLGETSHADERSOURCEARBPROC		   glGetShaderSourceARB;
										   
extern PFNGLGETATTRIBLOCATIONARBPROC       glGetAttribLocationARB;
extern PFNGLVERTEXATTRIB2FVARBPROC		   glVertexAttrib2fvARB;
										   
//extern PFNGLACTIVETEXTUREARBPROC           glActiveTextureARB;
//extern PFNGLMULTITEXCOORD2FARBPROC	     glMultiTexCoord2fARB;
										   
extern PFNGLBINDBUFFERARBPROC              glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC           glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC              glGenBuffersARB;
extern PFNGLISBUFFERARBPROC                glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC              glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC           glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC        glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC               glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC             glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC    glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC       glGetBufferPointervARB;
										   
//extern PFNWGLCREATEPBUFFERARBPROC          wglCreatePbufferARB;
//extern PFNWGLGETPBUFFERDCARBPROC           wglGetPbufferDCARB;
//extern PFNWGLRELEASEPBUFFERDCARBPROC       wglReleasePbufferDCARB;
//extern PFNWGLDESTROYPBUFFERARBPROC         wglDestroyPbufferARB;
//extern PFNWGLQUERYPBUFFERARBPROC           wglQueryPbufferARB;

//extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
//extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
//extern PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;

extern PFNGLPOINTPARAMETERFARBPROC		   glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC		   glPointParameterfvARB;

// FrameBufferObject
extern PFNGLISRENDERBUFFERPROC                        glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFERPROC           			  glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSPROC        			  glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSPROC           			  glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEPROC        			  glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC 			  glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFERPROC              			  glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFERPROC            			  glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSPROC         			  glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSPROC            			  glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC     			  glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC       			  glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC       			  glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC       			  glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC    			  glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC	  glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPPROC						  glGenerateMipmapEXT;

extern PFNGLGENBUFFERSPROC                            glGenBuffers;
extern PFNGLGETUNIFORMBLOCKINDEXPROC                  glGetUniformBlockIndex;


#endif
