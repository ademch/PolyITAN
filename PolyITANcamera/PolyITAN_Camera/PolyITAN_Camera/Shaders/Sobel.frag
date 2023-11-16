#version 300 es

precision highp float;


uniform sampler2D tex0;

float lookup(ivec2 p, int dx, int dy)
{
    vec4 c = texelFetch(tex0, p + ivec2(dx, dy), 0);
	
    // return luma
    return 0.299f*c.r + 0.587*c.g + 0.114*c.b;
}

void main()
{
    // OpenGL window space is defined such that pixel centers are on half-integer boundaries.
    // So the center of the lower-left pixel is (0.5, 0.5). int results in floor
    ivec2 p = ivec2(gl_FragCoord.xy);
    
    float tt1 = lookup(p, -1, -1);
    float tt2 = lookup(p, -1,  0);
    float tt3 = lookup(p, -1,  1);
    float tt4 = lookup(p,  1, -1);
    float tt5 = lookup(p,  1,  0);
    float tt6 = lookup(p,  1,  1);

    // simple sobel edge detection
    float gx = 0.0;
    gx += -1.0 * tt1;
    gx += -2.0 * tt2;
    gx += -1.0 * tt3;
    gx +=  1.0 * tt4;
    gx +=  2.0 * tt5;
    gx +=  1.0 * tt6;
    
    float tt7 = lookup(p,  0, -1);
    float tt8 = lookup(p,  0,  1);

    float gy = 0.0;
    gy += -1.0 * tt1;
    gy += -2.0 * tt7;
    gy += -1.0 * tt4;
    gy +=  1.0 * tt3;
    gy +=  2.0 * tt8;
    gy +=  1.0 * tt6;
    
    float g = sqrt(gx*gx + gy*gy);
    
	gl_FragColor = vec4(g, g, g, 1.0);
}
