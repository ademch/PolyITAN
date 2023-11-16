#include "../../!!adExtensions/Shaders.h"


#define TEXTURE_CAM_BAYER_OUT     "1. cam bayer"
#define TEXTURE_RGBG_OUT          "2. bayer2rgbg out"
#define TEXTURE_BAYER2RGB_OUT     "3. bayer2rgb out"
#define TEXTURE_BILATERAL_OUTPUT  "4. bilateral out"
#define TEXTURE_RGB2YUV_OUT       "5. rgb2yuv out"
#define TEXTURE_SOBEL_OUT         "6. sobel out"


class GLSL_Pipeline : public GLSL_AbstractPipeline
{
public:
	float fDenoiseSqr;

	bool init_shaders();
	
	virtual ~GLSL_Pipeline() {	}
	
protected:
private:
};
