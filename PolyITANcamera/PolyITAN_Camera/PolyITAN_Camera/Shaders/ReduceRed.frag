#version 300 es

precision highp float;


uniform sampler2D tex0;

uniform int iTargetWidth;
uniform int iTargetHeight;

// Called from Gradient calculation
// iTargetWidth x iTargetHeight are the final dimensions specifically resulting in some integer binning
void main()
{
	// int (resulting in floor) maps the whole texture coordinate range of a single pixel down to integer index
	float fX = gl_TexCoord[0].s*float(iTargetWidth);
	float fY = gl_TexCoord[0].t*float(iTargetHeight);

	// Minification
	//  _________________________
	// | | | | | | | | | | | | | |  <- texels
	// 
	//  _________________________
	// |                         |  <- the pixel
	//              ^ 
	//            texture coordinate is sent from the center of the pixel

	ivec2 ivSamplerSize = textureSize(tex0, 0);

	float fBinWidth  = float(ivSamplerSize.x/iTargetWidth);
	float fBinHeight = float(ivSamplerSize.y/iTargetHeight);

	fX *= fBinWidth;
	fY *= fBinHeight;

    // Accumulate only single component
	float fAccum = 0.0f;
	for (float fRow=0.0; fRow<fBinHeight; fRow+=1.0)
	{
		for (float fCol=0.0; fCol<fBinWidth; fCol+=1.0)
		{
			fAccum += texelFetch(tex0, ivec2(fX + fCol, fY + fRow), 0).r;
		}
	}

    fAccum /= fBinHeight*fBinWidth;

	gl_FragColor = vec4(fAccum, 0.0, 0.0, 0.0);
}

