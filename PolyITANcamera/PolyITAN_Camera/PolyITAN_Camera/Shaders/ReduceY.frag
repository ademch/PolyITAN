#version 300 es

precision highp float;


uniform sampler2D tex0;

uniform int iTargetWidth;
uniform int iTargetHeight;

// Called from Histogram calculation
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

	vec3 vAccum = vec3(0,0,0);
	for (float fRow=0.0; fRow<fBinHeight; fRow+=1.0)
	{
		for (float fCol=0.0; fCol<fBinWidth; fCol+=1.0)
		{
			vAccum += texelFetch(tex0, ivec2(fX + fCol, fY + fRow), 0).rgb;
		}
	}

    vAccum /= fBinHeight*fBinWidth;
    vec3 vCoef = vec3(0.299f, 0.587f, 0.114f);

	gl_FragColor = vec4(vAccum, dot(vAccum, vCoef));
}

