#version 130

uniform sampler2D tex0;


void main()
{
	// OpenGL window space is defined such that pixel centers are on half-integer boundaries.
	// So the center of the lower-left pixel is (0.5, 0.5). int results in floor
	int iX = int(gl_FragCoord.x);
	int iY = int(gl_FragCoord.y);
	
	// Minification
	//  _________________________
	// |            |            |  <- texels
	// 
	//  _________________________
	// |                         |  <- pixel
	//              ^ 
	//            texture coordinate is sent from the center of the pixel

	iX = iX * 2;
	iY = iY * 2;

	// G            B
	// t01 o------o t11
	//     |      |
	//     |      |
	// t00 o------o t10
	// R            G

	vec4 t00 = texelFetch(tex0, ivec2(iX,   iY),   0);
	vec4 t10 = texelFetch(tex0, ivec2(iX+1, iY),   0);
	vec4 t11 = texelFetch(tex0, ivec2(iX+1, iY+1), 0);
	vec4 t01 = texelFetch(tex0, ivec2(iX,   iY+1), 0);

	//vec3 vRGB = vec3(t00.r, (t10.r + t01.r)/2.0f, t11.r);
    vec3 vRGB = vec3(t00.r, t10.r, t11.r);
	//float fLuma = 0.299f*vRGB.r + 0.587*vRGB.g + 0.114*vRGB.b;

	//gl_FragColor = vec4(vRGB, fLuma);
    gl_FragColor = vec4(vRGB, t01.r);
}
