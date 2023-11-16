#include "GLSL_Pipeline.h"
#include <assert.h>


bool GLSL_Pipeline::init_shaders()
{
bool bSuccess;

	//--Shader--
	bSuccess = CreateShader("BayerR", "Shaders/Simple.vert", "Shaders/Bayer2rgbRandom.frag");
	if (!bSuccess) return false;

	// get uniform locations
	GPUPrograms["BayerR"]->Uniforms["tex0"]  = glGetUniformLocationARBassert(GPUPrograms["BayerR"]->programObj, "tex0");
	GPUPrograms["BayerR"]->Uniforms["vGain"] = glGetUniformLocationARBassert(GPUPrograms["BayerR"]->programObj, "vGain");


	//--Shader--
	bSuccess = CreateShader("BayerF", "Shaders/Simple.vert", "Shaders/Bayer2rgb.frag");
	if (!bSuccess) return false;

	// get uniform locations
	GPUPrograms["BayerF"]->Uniforms["tex0"] = glGetUniformLocationARBassert(GPUPrograms["BayerF"]->programObj, "tex0");

	//--Shader--
	bSuccess = CreateShader("ReduceRed", "Shaders/Simple.vert", "Shaders/ReduceRed.frag");
	if (!bSuccess) return false;

	// get uniform locations
	GPUPrograms["ReduceRed"]->Uniforms["tex0"] = glGetUniformLocationARBassert(GPUPrograms["ReduceRed"]->programObj, "tex0");
	GPUPrograms["ReduceRed"]->Uniforms["iTargetWidth"]  = glGetUniformLocationARBassert(GPUPrograms["ReduceRed"]->programObj, "iTargetWidth");
	GPUPrograms["ReduceRed"]->Uniforms["iTargetHeight"] = glGetUniformLocationARBassert(GPUPrograms["ReduceRed"]->programObj, "iTargetHeight");


	//--Shader--
	bSuccess = CreateShader("ReduceY", "Shaders/Simple.vert", "Shaders/ReduceY.frag");
	if (!bSuccess) return false;

	// get uniform locations
	GPUPrograms["ReduceY"]->Uniforms["tex0"] = glGetUniformLocationARBassert(GPUPrograms["ReduceY"]->programObj, "tex0");
	GPUPrograms["ReduceY"]->Uniforms["iTargetWidth"]  = glGetUniformLocationARBassert(GPUPrograms["ReduceY"]->programObj, "iTargetWidth");
	GPUPrograms["ReduceY"]->Uniforms["iTargetHeight"] = glGetUniformLocationARBassert(GPUPrograms["ReduceY"]->programObj, "iTargetHeight");


	//--Shader--
	bSuccess = CreateShader("Sobel", "Shaders/Simple.vert", "Shaders/Sobel.frag");
	if (!bSuccess) return false;

	// get uniform locations
	GPUPrograms["Sobel"]->Uniforms["tex0"] = glGetUniformLocationARBassert(GPUPrograms["Sobel"]->programObj, "tex0");


	//--Shader--
	bSuccess = CreateShader("rgb2yuv", "Shaders/Simple.vert", "Shaders/rgb2yuv.frag");
	if (!bSuccess) return false;
	
	// get uniform locations
	GPUPrograms["rgb2yuv"]->Uniforms["tex0"] = glGetUniformLocationARBassert(GPUPrograms["rgb2yuv"]->programObj, "tex0");


	//--Shader--
	bSuccess = CreateShader("bilateral", "Shaders/Simple.vert", "Shaders/Bilateral.frag");
	if (!bSuccess) return false;
	
	// get uniform locations
	GPUPrograms["bilateral"]->Uniforms["tex0"]      = glGetUniformLocationARBassert(GPUPrograms["bilateral"]->programObj, "tex0");
	GPUPrograms["bilateral"]->Uniforms["vGain"]     = glGetUniformLocationARBassert(GPUPrograms["bilateral"]->programObj, "vGain");
	GPUPrograms["bilateral"]->Uniforms["fSigmaSqr"] = glGetUniformLocationARBassert(GPUPrograms["bilateral"]->programObj, "fSigmaSqr");

	return true;
}
