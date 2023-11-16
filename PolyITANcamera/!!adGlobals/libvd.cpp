#include "stdafx.h"
#include <gl/GL.h>

template <typename T>
struct color
{	T b,g,r; ///< colors as they appear in the file
	color(T r=0, T g=0, T b=0): b(b),g(g),r(r) {} ///< constructor
};

/* Read OpenGL buffer and create a raw 24bit image */
void glScreenshot(color<unsigned char>* &image, unsigned int &width, unsigned int &height)
{
	/* NB: This takes whatever is in the default OpenGL pixel buffer,
		* we flush it to make sure it ready */
	glFlush();

	/* get viewport size */
	GLint    viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	width  = viewport[2];
	height = viewport[3];

	/* create an image buffer */
	unsigned char* buffer = new unsigned char [width * height * 4];

	/* read openGL buffer in rgba (this should be supported with most drivers) */
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	/* transfer buffer to image */
	image = new color<unsigned char> [width * height];
	for(unsigned int i=0; i<width*height; ++i)
		image[i].r = buffer[i*4+0],
		image[i].g = buffer[i*4+1],
		image[i].b = buffer[i*4+2];

	delete [] buffer;
}

// 2D Color read
void BMPread(const char* filename, color<unsigned char>* &image, unsigned int &width, unsigned int &height)
{
FILE *fd;
unsigned int w, h;

	/* open BMP file */
	fd = fopen(filename, "rb");

	/* read header */
	unsigned short signature = 0;
	fread(&signature, 2, 1, fd);

//	if(signature != 0x4d42) throw LOCATE(Error("format"));

	fseek(fd, 18, SEEK_SET);
	fread(&w, 4, 1, fd);		
	fread(&h, 4, 1, fd);
	fseek(fd, 54, SEEK_SET);

	image  = new color<unsigned char> [w*h];
	width  = w;
	height = h;

	// padding
	unsigned bytesPerRow = width*3;
	bytesPerRow = (bytesPerRow & 0x3) ? ((bytesPerRow >> 2) + 1) << 2 : bytesPerRow;
	unsigned padding = bytesPerRow - width*3;

	/* read bitmap */
	for(unsigned int j=0; j<h; ++j)
	{
		unsigned int n = (unsigned int)fread((char*)&image[j*width], 3, width, fd);
//		if(n != width) throw LOCATE(Error("EOF reached while reading"));

		if(padding != 0)
		{
			int zero;
			fread((char*)&zero, 1, padding, fd);
		}
	}

	/* close BMP file */
	fclose(fd);
}


// 2D color write
void BMPwrite(const char* filename, const color<unsigned char>* image, unsigned int width, unsigned int height)
{
	FILE *fd;
	int header = 0;
	int zero = 0;

	/* open/create BMP file */
	fd = fopen(filename, "wb");
//	if(!fd) throw LOCATE(Error("open"));

	/* Write BMP header */
	header = 0x84364d42; 
	fwrite(&header, 4, 1, fd);

	header = 0x3; 
	fwrite(&header, 4, 1, fd);
	fwrite(&zero, 2, 1, fd);
	
	header = 0x36; 
	fwrite(&header, 4, 1, fd);

	header = 0x28;  
	fwrite(&header, 4, 1, fd);

	header = width;   
	fwrite(&header, 4, 1, fd);	/* width */

	header = height;
	fwrite(&header, 4, 1, fd);	/* height */

	header = 0x180001;
	fwrite(&header, 4, 1, fd);
	fwrite(&zero, 4, 1, fd);

	header = width*height*3;
	fwrite(&header, 4, 1, fd);	/* file size */

	header = 0x0b000012; 
	fwrite(&header, 4, 1, fd);

	header = 0x0b;       
	fwrite(&header, 4, 1, fd);
	fwrite(&zero, 4, 1, fd);
	fwrite(&zero, 4, 1, fd);
	
	// padding
	unsigned bytesPerRow = width*3;
	bytesPerRow = (bytesPerRow & 0x3) ? ((bytesPerRow >> 2) + 1) << 2 : bytesPerRow;
	unsigned padding = bytesPerRow - width*3;

	/* Write DATA */
	for(unsigned int j=0; j<height; ++j)
	{
		fwrite((char*)&image[j*width], 3, width, fd);
		
		if(padding != 0)
		{
			int zero = 0;
			fwrite((char*)&zero, 1, padding, fd);
		}
	}

	/* close BMP file */
	fclose(fd);
}

void BMPcreateBlankImage(const char* filename, unsigned int width, unsigned int height)
{
unsigned int i,j;

	color<unsigned char> *image = new color<unsigned char> [width * height];
	for	(i=0;i<height;i++)
	{	for	(j=0;j<width;j++)
		{
			image[i*width + j].r = 255;
			image[i*width + j].g = 255;
			image[i*width + j].b = 255;
		}
	}

	BMPwrite(filename, image, width, height);
	delete [] image;
}


