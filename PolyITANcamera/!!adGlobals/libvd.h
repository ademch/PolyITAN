#ifndef LIBVD_H
#define LIBVD_H

template <typename T>
struct color
{	T b,g,r; ///< colors as they appear in the file
	color(T r=0, T g=0, T b=0): b(b),g(g),r(r) {} ///< constructor
};

/* Read OpenGL buffer and create a raw 24bit image */
void glScreenshot(color<unsigned char>* &image, unsigned int &width, unsigned int &height);

// 2D Color read
void BMPread(const char* filename, color<unsigned char>* &image, unsigned int &width, unsigned int &height);

// 2D color write
void BMPwrite(const char* filename, const color<unsigned char>* image, unsigned int width, unsigned int height);
void BMPcreateBlankImage(const char* filename, unsigned int width, unsigned int height);

#endif /* LIBVD_H */