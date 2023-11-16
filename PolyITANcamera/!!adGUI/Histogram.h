#include "glfont.h"
#include "gui_element.h"
#include "../!!adGlobals/vector_math.h"
#include <string>

class Histogram : public GUI_Element
{
public:
	Histogram(std::string caption, int px, int py, int width, int height, float size);

	float m_width;
	float m_height;

	unsigned int m_iPixelCount;

	void Draw();
	void UpdateHistogram(unsigned char* data, unsigned int _pixelCount, unsigned int _iCompNumber);
	int EstimateExposureCorrection();


protected:
	float _size;
	std::string _text;

	unsigned int histoLuma[256];
	unsigned int histoSigmaLuma[7];
	unsigned int histoR[256];
	unsigned int histoSigmaR[7];
	unsigned int histoG[256];
	unsigned int histoSigmaG[7];
	unsigned int histoB[256];
	unsigned int histoSigmaB[7];

private:
	float fMaxLuma;
	float fMaxR;
	float fMaxG;
	float fMaxB;

	int posx;
	int posy;

	// Sigma mapping
	unsigned int aSigmaMap[256];
	float aSigmaGUI[7];

	float SigmaPercentage(unsigned int sigmaCount);

};
