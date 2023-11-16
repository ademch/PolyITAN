#include "Histogram.h"
#include <GL/gl.h>
#include <assert.h>
#include <string.h>

extern GLFONT font;

#define SigmaSSS 0
#define SigmaSS  1
#define SigmaS   2
#define SigmaM   3
#define SigmaL   4
#define SigmaLL  5
#define SigmaLLL 6

Histogram::Histogram(std::string caption, int px, int py, int width, int height, float size) :
	                 posx(px), posy(py)
{
	m_width  = float(width);
	m_height = float(height);

	_size = size;
	_text = caption;

	fMaxLuma = 0;
	fMaxR    = 0;
	fMaxG    = 0;
	fMaxB    = 0;

	m_iPixelCount = 0;

	unsigned int sigma = 21;	//~255/12
	for (unsigned int i = 0; i < 256; i++)
	{
		if (i >= 0 && i <= sigma)
			aSigmaMap[i] = 0;
		if (i > sigma && i <= sigma*2)
			aSigmaMap[i] = 1;
		if (i > sigma*2 && i <= sigma*3)
			aSigmaMap[i] = 2;

		if (i > sigma*3 && i <= sigma*9)
			aSigmaMap[i] = 3;

		if (i > sigma*9 && i <= sigma*10)
			aSigmaMap[i] = 4;
		if (i > sigma*10 && i <= sigma*11)
			aSigmaMap[i] = 5;
		if (i > sigma*11 && i <= 255)
			aSigmaMap[i] = 6;
	}

	aSigmaGUI[0] = 0.083f;
	aSigmaGUI[1] = 0.167f;
	aSigmaGUI[2] = 0.25f;
	aSigmaGUI[3] = 0.5f;
	aSigmaGUI[4] = 0.75f;
	aSigmaGUI[5] = 0.833f;
	aSigmaGUI[6] = 0.917f;
}

void Histogram::Draw()
{
	glDisable(GL_LIGHTING);

	glColor3f(0.1f, 0.5f, 0.1f);
	char text_buf[300];
	glFontBegin(&font);
		// draw the value
		glFontTextOut(_text.c_str(), float(posx), float(posy) + m_height, -5, _size);

		// draw Sigma labels  
		glFontTextOut("-6S", float(posx)-10,                   float(posy) - 20, -5, _size - 0.5f);
		glFontTextOut("6S",  float(posx) + m_width - 7,        float(posy) - 20, -5, _size - 0.5f);

		glFontTextOut("-3S", float(posx) + m_width*0.25f - 10, float(posy) - 20, -5, _size - 0.5f);
		glFontTextOut("3S",  float(posx) + m_width*0.75f - 7,  float(posy) - 20, -5, _size - 0.5f);

		// Luma max
		sprintf(text_buf, "%d", int(fMaxLuma));
		glFontTextOut(text_buf, float(posx) + m_width - 30,    float(posy) + m_height, -5, _size - 1);

		for (unsigned int j = 0; j < 7; j++)
		{
			int iOffset = -10; // central value
			if (j < 3) iOffset = -15; else if (j > 3) iOffset = 5;

			// Sigma Luma
			glColor3f(0.5f, 0.5f, 0.5f);
			sprintf(text_buf, "%d", int(histoSigmaLuma[j] / (256.0*150.0)*100.0));
			glFontTextOut(text_buf, float(posx) + m_width*aSigmaGUI[j] + iOffset, float(posy) - 40, 3, _size);

			// Sigma Red
			glColor3f(0.6f, 0.1f, 0.1f);
			sprintf(text_buf, "%d", int(histoSigmaR[j] / (256.0*150.0)*100.0));
			glFontTextOut(text_buf, float(posx) + m_width*aSigmaGUI[j] + iOffset, float(posy) - 55, 3, _size);

			// Sigma Green
			glColor3f(0.1f, 0.6f, 0.1f);
			sprintf(text_buf, "%d", int(histoSigmaG[j] / (256.0*150.0)*100.0));
			glFontTextOut(text_buf, float(posx) + m_width*aSigmaGUI[j] + iOffset, float(posy) - 70, 3, _size);

			// Sigma Blue
			glColor3f(0.2f, 0.2f, 0.8f);
			sprintf(text_buf, "%d", int(histoSigmaB[j] / (256.0*150.0)*100.0));
			glFontTextOut(text_buf, float(posx) + m_width*aSigmaGUI[j] + iOffset, float(posy) - 85, 3, _size);
		}

		glColor3f(0.4f, 0.4f, 0.4f);
		glFontTextOut("%", float(posx) + m_width*aSigmaGUI[3] + 30, float(posy) - 70, -5, _size + 2);

	glFontEnd();

	glDisable(GL_TEXTURE_2D);

	glColor3f(0.1f, 0.5f, 0.1f);

	// secondary lines
	glLineWidth(1);
	glLineStipple(3, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glVertex3f(float(posx) + m_width*0.17f, float(posy), 4);
		glVertex3f(float(posx) + m_width*0.17f, float(posy) + m_height, 4);
		glVertex3f(float(posx) + m_width*0.83f, float(posy), 4);
		glVertex3f(float(posx) + m_width*0.83f, float(posy) + m_height, 4);

		glVertex3f(float(posx) + m_width*0.08f, float(posy), 4);
		glVertex3f(float(posx) + m_width*0.08f, float(posy) + m_height, 4);
		glVertex3f(float(posx) + m_width*0.92f, float(posy), 4);
		glVertex3f(float(posx) + m_width*0.92f, float(posy) + m_height, 4);

		glVertex3f(float(posx) + m_width*0.25f, float(posy),            4);
		glVertex3f(float(posx) + m_width*0.25f, float(posy) + m_height, 4);
		glVertex3f(float(posx) + m_width*0.75f, float(posy),            4);
		glVertex3f(float(posx) + m_width*0.75f, float(posy) + m_height, 4);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	// frame
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
		glVertex3f( float(posx)-1,            float(posy) -1,  4);
		glVertex3f( float(posx) + m_width +1, float(posy) -1,  4);
		glVertex3f( float(posx) + m_width +1, float(posy) + m_height + 1, 4);
		glVertex3f( float(posx)-1,            float(posy) + m_height + 1, 4);
	glEnd();

	// ticks
	glBegin(GL_LINES);
		float fVal_max = 256;
		float fVal_min = 0;
		float fTickGranularity = 256.0f/12.0f;
		float iTickCount = (fVal_max - fVal_min) / fTickGranularity;
		float fCurrentVal = fVal_min;
		float fullness;
		unsigned int iTickStep = int(ceil(iTickCount / 100.0f));	// decimate ticks to have tidy look
		for (unsigned int iTick = 0; iTick <= iTickCount; iTick += iTickStep)
		{
			fullness = (fCurrentVal - fVal_min) / (fVal_max - fVal_min);

			glVertex3f(float(posx) + fullness*m_width, float(posy),     4);
			glVertex3f(float(posx) + fullness*m_width, float(posy) - 5, 4);
			fCurrentVal = fCurrentVal + float(iTickStep*fTickGranularity);
		}
	glEnd();

	// Histogram Luma
	glColor3f(0.5f, 0.5f, 0.5f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		for (int i = 0; i < 256; i++) {
			glVertex3i(posx+i, posy, 0);
			glVertex3i(posx+i, posy + int(float(histoLuma[i])/fMaxLuma * m_height), 0);
		}
	glEnd();

	// Histogram R
	glColor3f(0.6f, 0.1f, 0.1f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 256; i++) {
		glVertex3i(posx + i, posy + int(float(histoR[i]) / fMaxR * m_height), 1);
	}
	glEnd();

	// Histogram G
	glColor3f(0.1f, 0.6f, 0.1f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 256; i++) {
		glVertex3i(posx + i, posy + int(float(histoG[i]) / fMaxG * m_height), 1);
	}
	glEnd();

	// Histogram B
	glColor3f(0.2f, 0.2f, 0.8f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 256; i++) {
		glVertex3i(posx + i, posy + int(float(histoB[i]) / fMaxB * m_height), 1);
	}
	glEnd();
}


void Histogram::UpdateHistogram(unsigned char* data, unsigned int _pixelCount, unsigned int _iCompNumber)
{
	m_iPixelCount = _pixelCount;

	memset(histoLuma, 0, 256 * sizeof(unsigned int));
	memset(histoR,    0, 256 * sizeof(unsigned int));
	memset(histoG,    0, 256 * sizeof(unsigned int));
	memset(histoB,    0, 256 * sizeof(unsigned int));

	unsigned int len = _pixelCount*_iCompNumber;
	for (unsigned int i = 0; i < len; i+=4)
	{
		histoR[data[i]]++;
		histoG[data[i+1]]++;
		histoB[data[i+2]]++;
		histoLuma[data[i+3]]++;
	}

	memset(histoSigmaLuma, 0, 7 * sizeof(unsigned int));
	memset(histoSigmaR,    0, 7 * sizeof(unsigned int));
	memset(histoSigmaG,    0, 7 * sizeof(unsigned int));
	memset(histoSigmaB,    0, 7 * sizeof(unsigned int));

	// find max values
	fMaxLuma = 0;
	fMaxR = 0;
	fMaxG = 0;
	fMaxB = 0;
	for (unsigned int i = 0; i < 256; i++)
	{
		if (histoLuma[i] > fMaxLuma) fMaxLuma = float(histoLuma[i]);
		if (histoR[i] > fMaxR)       fMaxR = float(histoR[i]);
		if (histoG[i] > fMaxG)       fMaxG = float(histoG[i]);
		if (histoB[i] > fMaxB)       fMaxB = float(histoB[i]);
	}

	// sum sigma buckets over sigma ranges
	for (unsigned int i = 0; i < 256; i++)
	{
		histoSigmaLuma[aSigmaMap[i]] += histoLuma[i];
		histoSigmaR[aSigmaMap[i]] += histoR[i];
		histoSigmaG[aSigmaMap[i]] += histoG[i];
		histoSigmaB[aSigmaMap[i]] += histoB[i];
	}
}

float Histogram::SigmaPercentage(unsigned int sigmaCount)
{
	return float(sigmaCount) / float(m_iPixelCount)*100.0f;
}


int Histogram::EstimateExposureCorrection()
{
	if (SigmaPercentage(histoSigmaLuma[SigmaLLL]) > 3.0)
	{
		return -1;
	}
	if (SigmaPercentage(histoSigmaLuma[SigmaLL]) > 20.0)
	{
		printf("-1 %f\n", SigmaPercentage(histoSigmaLuma[SigmaLL]));
		return -1;
	}
	if (SigmaPercentage(histoSigmaLuma[SigmaLL] + histoSigmaLuma[SigmaLLL]) < 3.0)
	{
		printf("1 %f %f\n", SigmaPercentage(histoSigmaLuma[SigmaLL]), SigmaPercentage(histoSigmaLuma[SigmaLLL]));
		return 1;
	}


	return 0;
}
