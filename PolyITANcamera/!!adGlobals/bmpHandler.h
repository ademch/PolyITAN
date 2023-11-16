#ifndef BMP_HANDLER_H
#define BMP_HANDLER_H

#include <vector>
#include <list>
#include <utility>

typedef struct tagRGBQUAD {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned short WORD;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

#  define BI_RGB       0             /* No compression - straight BGR data */
#  define BI_RLE8      1             /* 8-bit run-length compression */
#  define BI_RLE4      2             /* 4-bit run-length compression */
#  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

int saveGreyscaleBMP(char *fname, unsigned char *buf, int _width, int _height, int _bits);
int loadGreyscaleBMP(char *fname, unsigned char **dataOut, BITMAPINFOHEADER *bmih);

unsigned char* BitmapResizeGreyscale05x(unsigned char* dataIn, unsigned int _width, unsigned int _height);
unsigned char* BitmapResizeGreyscale2x(unsigned char* dataIn, unsigned int _width, unsigned int _height);
unsigned char* BitmapResizeRGB2x(unsigned char* dataIn, unsigned int _width, unsigned int _height);
unsigned char* BitmapTrimOddRGBA(unsigned char* dataIn, unsigned int& _width, unsigned int& _height);
unsigned char* BitmapRGBA2GreyscaleRGBA(unsigned char* dataIn, unsigned int _width, unsigned int _height);
unsigned char* BitmapRGBA2Greyscale(unsigned char* dataIn, unsigned int _width, unsigned int _height);

struct Bucket
{	//                            shade     pix count
	std::vector<std::pair<unsigned char, unsigned int>> shadesMappedToBucket;
	unsigned int price;
	unsigned char color;

	Bucket()
	{
		price = 0;
		color = 0;
	}
};

struct GreyscaleImageDescriptor
{
	RGBQUAD* palette;
	unsigned int iPaletteSize;
	
	unsigned char* dataOut;
	int byteCount;

	GreyscaleImageDescriptor()
	{
		palette      = NULL;
		iPaletteSize = 0;
		dataOut      = NULL;
		byteCount    = 0;
	}
};

typedef std::list<Bucket*>::iterator BucketIterType;

class BitmapColorDepthDecreaser
{
public:
	std::list<Bucket*> PaletteBuckets;
	unsigned int histoLuma[256];

	~BitmapColorDepthDecreaser();

	GreyscaleImageDescriptor Decrease(unsigned char* data, unsigned int _width, unsigned int _height);

protected:

	int PixelsRemappingPrice(BucketIterType bucket, unsigned char colorMiddle);
	BucketIterType FindBucketWithHighestPrice();

	unsigned char CalculateBucketColor(BucketIterType bucket);

	void MoveBucketRegion(BucketIterType bucketBefore, BucketIterType bucket, int iMiddle );
	int PixelsOnTheLeftCount(BucketIterType bucket, int iMiddle);
	int PixelsOnTheRightCount(BucketIterType bucket, int iMiddle);
	void SplitLayer();
	void SplitBucket(BucketIterType bucket);
};

#endif
