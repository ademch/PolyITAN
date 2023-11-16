#version 300 es

precision highp float;

uniform sampler2D tex0;

void main()
{
	// OpenGL window space is defined such that pixel centers are on half-integer boundaries.
	// So the center of the lower-left pixel is (0.5, 0.5). int results in floor
	int iX = int(gl_FragCoord.x);
	int iY = int(gl_FragCoord.y);
	
	// Minification
	//  _________________________
	// | | | | | | | | | | | | | |  <- texels
	// 
	//  _________________________
	// |                         |  <- pixel
	//              ^ 
	//            texture coordinate is sent from the center of the pixel

	
	vec4 t00 = texelFetch(tex0, ivec2(iX, iY), 0);

	vec3 vY = vec3( 0.299,  0.587,  0.114); // [0;1]
	vec3 vU = vec3(-0.169, -0.331,  0.5);   // [-0.5;0.5]
	vec3 vV = vec3( 0.5,   -0.419, -0.081); // [-0.5;0.5]

	gl_FragColor = vec4(dot(t00.rgb, vY), dot(t00.rgb, vU)+0.5f, dot(t00.rgb, vV)+0.5f, t00.g-t00.a+0.5f);

	//vec4 rgba;
    //vec3 yuv = vec3(gl_FragData[0].r, gl_FragData[0].g - 0.5, gl_FragData[0].b - 0.5);
	//rgba.r = yuv.x * 1.0 + yuv.y * 0.0 +    yuv.z * 1.4;
	//rgba.g = yuv.x * 1.0 + yuv.y * -0.343 + yuv.z * -0.711;
	//rgba.b = yuv.x * 1.0 + yuv.y * 1.765 +  yuv.z * 0.0;
	//rgba.a = 1.0;
	
	//gl_FragData[0] = rgba;

}
