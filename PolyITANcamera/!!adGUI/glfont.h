//*********************************************************
//GLFONT.H -- Header for GLFONT.CPP
//Copyright (c) 1998 Brad Fish 
//See glFont.txt for terms of use
//November 10, 1998
//*********************************************************

#ifndef GL_FONT_H
#define GL_FONT_H

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

//*********************************************************
//Structures
//*********************************************************

//glFont character structure
typedef struct
{
	float dx, dy;
	float tx1, ty1;
	float tx2, ty2;
} GLFONTCHAR;

//glFont structure
typedef struct
{
	int Tex;
	int TexWidth, TexHeight;
	int IntStart, IntEnd;
	GLFONTCHAR *Char;
} GLFONT;

//Creates a glFont
int glFontCreate (GLFONT *Font, char *FileName, int Tex);

//Deletes a glFont
void glFontDestroy (GLFONT *Font);

//Needs to be called before text output
void glFontBegin (GLFONT *Font);

//Needs to be called after text output
void glFontEnd (void);

//Draws text with a glFont
void glFontTextOut (const char *String, float x, float y, float z = 0.f, float scale = 1.f);

void glFontGetLength (const char *String, GLFONT *Font,float& width, float& height, float scale);

#endif