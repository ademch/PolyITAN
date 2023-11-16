#version 130

uniform sampler2D tex0;

uniform vec3 vGain;

void main()
{
	// int (resulting in floor) maps the whole texture coordinate range of a single pixel down to integer index
	int iX = int(gl_TexCoord[0].s*4096.0f); //
	int iY = int(gl_TexCoord[0].t*3000.0f); //
	
	// Minification
	//  _________________________
	// | | | | | | | | | | | | | |  <- texels
	// 
	//  _________________________
	// |                         |  <- pixel
	//              ^ 
	//            texture coordinate is sent from the center of the pixel

	// The texel here is random, what matters is either it is even or odd

	// t01 o------o t11
	//     |      |
	//     |      |
	// t00 o------o t10

	if (mod(iX,2.0) > 0.1f) // odd column searches backward
	{	
		if (mod(iY,2.0) > 0.1f)
		{
			vec4 t00 = texelFetch(tex0, ivec2(iX-1, iY-1), 0);
			vec4 t10 = texelFetch(tex0, ivec2(iX,   iY-1), 0);
			vec4 t11 = texelFetch(tex0, ivec2(iX,   iY),   0);
			vec4 t01 = texelFetch(tex0, ivec2(iX-1, iY),   0);

			// upper right corner
			gl_FragColor = vec4(vGain.r*t00.r, vGain.g*(t10.r+t01.r)/2.0f, vGain.b*t11.r, 1.0);
		}
		else
		{
			vec4 t00 = texelFetch(tex0, ivec2(iX-1, iY),    0);
			vec4 t10 = texelFetch(tex0, ivec2(iX,   iY),    0);
			vec4 t11 = texelFetch(tex0, ivec2(iX,   iY+1),  0);
			//vec4 t01 = texelFetch(tex0, ivec2(iX-1, iY+1), 0);

			// lower right corner
			gl_FragColor = vec4(vGain.r*t00.r, vGain.g*t10.r, vGain.b*t11.r, 1.0);
		}
	}
	else // even column searches forward
	{
		if (mod(iY,2.0) > 0.1f)
		{
			vec4 t00 = texelFetch(tex0, ivec2(iX,   iY-1), 0);
			//vec4 t10 = texelFetch(tex0, ivec2(iX+1,   iY-1), 0);
			vec4 t11 = texelFetch(tex0, ivec2(iX+1,  iY),  0);
			vec4 t01 = texelFetch(tex0, ivec2(iX,    iY),  0);

			// upper left corner
			gl_FragColor = vec4(vGain.r*t00.r, vGain.g*t01.r, vGain.b*t11.r, 1.0);
		}
		else
		{
			vec4 t00 = texelFetch(tex0, ivec2(iX,   iY),   0);
			vec4 t10 = texelFetch(tex0, ivec2(iX+1, iY),   0);
			vec4 t11 = texelFetch(tex0, ivec2(iX+1, iY+1), 0);
			vec4 t01 = texelFetch(tex0, ivec2(iX,   iY+1), 0);

			// lower left corner
			gl_FragColor = vec4(vGain.r*t00.r, vGain.g*(t01.r+t10.r)/2.0f, vGain.b*t11.r, 1.0);
		}
	}

}
