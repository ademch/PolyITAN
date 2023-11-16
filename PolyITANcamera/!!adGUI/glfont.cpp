//*********************************************************
//GLFONT.CPP -- glFont routines
//Copyright (c) 1998 Brad Fish
//See glFont.txt for terms of use
//November 10, 1998
//*********************************************************
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GL/gl.h>
#include "glfont.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

//*********************************************************
//Variables
//*********************************************************

//Current font
GLFONT *glFont;

//*********************************************************
//Functions
//*********************************************************
int glFontCreate (GLFONT *Font, char *FileName, int Tex)
{
FILE *Input;
char *TexBytes;
int Num;

	//Open font file
	if ((Input = fopen(FileName, "rb")) == NULL)
		return FALSE;

	//Read glFont structure
	fread(Font, sizeof(GLFONT), 1, Input);

	//Save texture number
	Font->Tex = Tex;

	//Get number of characters
	Num = Font->IntEnd - Font->IntStart + 1;
	
	//Allocate memory for characters
	if ((Font->Char = (GLFONTCHAR *)malloc(sizeof(GLFONTCHAR)*Num))==NULL)
		return FALSE;
	
	//Read glFont characters
	fread(Font->Char, sizeof(GLFONTCHAR), Num, Input);

	//Get texture size
	Num = Font->TexWidth * Font->TexHeight * 2;
	
	//Allocate memory for texture data
	if ((TexBytes = (char *)malloc(Num)) == NULL)
		return FALSE;
	
	//Read texture data
	fread(TexBytes, sizeof(char), Num, Input);

	//Set texture attributes
	glBindTexture(GL_TEXTURE_2D, Font->Tex);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);  
	
	//Create texture
	glTexImage2D(GL_TEXTURE_2D, 0, 2, Font->TexWidth,
		         Font->TexHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, (void *)TexBytes);

	//Clean up
	free(TexBytes);
	fclose(Input);

	//Return pointer to new font
	return TRUE;
}
//*********************************************************
void glFontDestroy (GLFONT *Font)
{
	//Free character memory
	free(Font->Char);
	Font->Char = NULL;
}
//*********************************************************
void glFontBegin (GLFONT *Font)
{
	//Save pointer to font structure
	if (Font->Char != NULL)
		glFont = Font;
	else
		glFont = NULL;	
	
	//Bind to font texture
	glBindTexture(GL_TEXTURE_2D, Font->Tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
//*********************************************************
void glFontEnd (void)
{
	//Font no longer current
	glFont = NULL;
}
//*********************************************************
void glFontTextOut (const char *String, float x, float y, float z, float scale)
{
	int Length, i;
	GLFONTCHAR *Char;

	//Return if we don't have a valid glFont 
	if (glFont == NULL)
		return;
	
	//Get length of string
	Length = (int)strlen(String);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	//Begin rendering quads
	glBegin(GL_QUADS);
	
	//Loop through characters
	for (i = 0; i < Length; i++)
	{
		//Get pointer to glFont character
		if ((int)String[i]<0)
			Char = &glFont->Char[256+(int)String[i] - glFont->IntStart];//russification
		else
			Char = &glFont->Char[(int)String[i] - glFont->IntStart];
		
		//Specify vertices and texture coordinates
		glTexCoord2f(Char->tx1, Char->ty2);
		glVertex3f(x, y, z);

		glTexCoord2f(Char->tx2, Char->ty2);
		glVertex3f(x + Char->dx*scale, y, z);

		glTexCoord2f(Char->tx2, Char->ty1);
		glVertex3f(x + Char->dx*scale, y + Char->dy*scale, z);

		glTexCoord2f(Char->tx1, Char->ty1);
		glVertex3f(x, y + Char->dy*scale, z);
	
		//Move to next character
		x += Char->dx*scale;
	}

	//Stop rendering quads
	glEnd();
}
//*********************************************************

void glFontGetLength (const char *String, GLFONT *Font, float& width, float& height, float scale)
{
int Length, i;
GLFONTCHAR *Char;

	width=0.;
	height=0.;

	if (Font->Char != NULL)
		glFont = Font;
	else
		return;

	//Get length of string
	Length = (int)strlen(String);

	//Loop through characters
	for (i = 0; i < Length; i++)
	{
		//Get pointer to glFont character
		if ((int)String[i]<0)
			Char = &glFont->Char[256+(int)String[i] - glFont->IntStart];//russification
		else
			Char = &glFont->Char[(int)String[i] - glFont->IntStart];

		//Move to next character
		width += Char->dx*scale;
		height= max(Char->dy*scale, height);
	}
	return;
}

//End of file



