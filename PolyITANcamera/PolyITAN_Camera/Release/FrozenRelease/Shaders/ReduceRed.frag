#version 130

uniform sampler2D tex0;

uniform int iTargetWidth;
uniform int iTargetHeight;

void main()
{
	// int (resulting in floor) maps the whole texture coordinate range of a single pixel down to integer index
	int iX = int(gl_TexCoord[0].s*iTargetWidth);  //
	int iY = int(gl_TexCoord[0].t*iTargetHeight); //

	// Minification
	//  _________________________
	// | | | | | | | | | | | | | |  <- texels
	// 
	//  _________________________
	// |                         |  <- the pixel
	//              ^ 
	//            texture coordinate is sent from the center of the pixel

	ivec2 iSamplerSize = textureSize(tex0, 0);

	int iBinWidth  = iSamplerSize.x/iTargetWidth;
	int iBinHeight = iSamplerSize.y/iTargetHeight;

	iX *= iBinWidth;
	iY *= iBinHeight;

    // Accumulate only single component
	float fAccum = 0.0f;
	for (int iRow=0; iRow<iBinHeight; iRow++)
	{
		for (int iCol=0; iCol<iBinWidth; iCol++)
		{
			fAccum += texelFetch(tex0, ivec2(iX + iCol, iY + iRow), 0).r;
		}
	}

    fAccum /= float(iBinHeight*iBinWidth);

	gl_FragColor = vec4(fAccum, 0.0, 0.0, 0.0);
}

