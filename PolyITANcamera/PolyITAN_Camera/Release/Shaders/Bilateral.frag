#version 130

uniform sampler2D tex0;
uniform vec3 vGain;

#define SIGMA   3
#define SIGMAB  0.4

vec3 lookup(ivec2 p, int dx, int dy)
{
    return texelFetch(tex0, p + ivec2(dx, dy), 0).rgb;
}

vec3 normpdf(vec3 x)
{
    x *= 255.0f;

	vec3 distrib;
	distrib.r = 0.39894/SIGMAB*exp(-0.5*x.r*x.r/(SIGMA*SIGMA));
	distrib.g = 0.39894/SIGMAB*exp(-0.5*x.g*x.g/(SIGMA*SIGMA));
	distrib.b = 0.39894/SIGMAB*exp(-0.5*x.b*x.b/(SIGMA*SIGMA));

	 return distrib;
}

vec3 vecCompMult(vec3 v0, vec3 v1)
{
    return vec3(v0.r*v1.r, v0.g*v1.g, v0.b*v1.b);
}

void main()
{
    // OpenGL window space is defined such that pixel centers are on half-integer boundaries.
    // So the center of the lower-left pixel is (0.5, 0.5). int results in floor
    ivec2 p = ivec2(gl_FragCoord.xy);
    
    vec3 tt0 = lookup(p,  0,  0);
    vec3 tt1 = lookup(p, -1, -1);
    vec3 tt2 = lookup(p, -1,  0);
    vec3 tt3 = lookup(p, -1,  1);
    vec3 tt4 = lookup(p,  1, -1);
    vec3 tt5 = lookup(p,  1,  0);
    vec3 tt6 = lookup(p,  1,  1);
    vec3 tt7 = lookup(p,  0, -1);
    vec3 tt8 = lookup(p,  0,  1);

	vec3 vFiltered  = vec3(0,0,0);
	vec3 vWeightT = vec3(0,0,0);
	vec3 vWeight;

    vWeight = 0.204 * normpdf(tt0-tt0);
	vFiltered += vecCompMult(vWeight, tt0);
    vWeightT += vWeight;

	vWeight = 0.075 * normpdf(tt1-tt0);
	vFiltered += vecCompMult(vWeight, tt1);
    vWeightT += vWeight;

    vWeight = 0.124 * normpdf(tt2-tt0);
	vFiltered += vecCompMult(vWeight, tt2);
    vWeightT += vWeight;

    vWeight = 0.075 * normpdf(tt3-tt0);
	vFiltered += vecCompMult(vWeight, tt3);
    vWeightT += vWeight;

    vWeight = 0.075 * normpdf(tt4-tt0);
	vFiltered += vecCompMult(vWeight, tt4);
    vWeightT += vWeight;

    vWeight = 0.124 * normpdf(tt5-tt0);
	vFiltered += vecCompMult(vWeight, tt5);
    vWeightT += vWeight;

    vWeight = 0.075 * normpdf(tt6-tt0);
	vFiltered += vecCompMult(vWeight, tt6);
    vWeightT += vWeight;

    vWeight = 0.124 * normpdf(tt7-tt0);
	vFiltered += vecCompMult(vWeight, tt7);
    vWeightT += vWeight;

    vWeight = 0.124 * normpdf(tt8-tt0);
	vFiltered += vecCompMult(vWeight, tt8);
    vWeightT += vWeight;

    vFiltered = vec3(vFiltered.r/vWeightT.r, vFiltered.g/vWeightT.g, vFiltered.b/vWeightT.b);
	vec3 vRGB = vecCompMult(vGain, vFiltered);
    
	gl_FragColor = vec4(vRGB, 1.0);

}
