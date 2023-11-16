
#include "bmpHandler.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int saveGreyscaleBMP(char *fname, unsigned char *buf, int _width, int _height, int _bits)
{
	assert((_bits == 8) || (_bits == 4));
	
	FILE *fp = fopen(fname, "wbx");
	if (!fp) {
		perror(fname);
		return false;
	}

	GreyscaleImageDescriptor palImage;
	
	if (_bits == 8)
	{
		assert(_width % 4 == 0);

		palImage.iPaletteSize = 256;
		palImage.palette      = new RGBQUAD[palImage.iPaletteSize];
		palImage.byteCount    = _height * _width;
		palImage.dataOut      = buf;

		for (int i = 0; i<256; i++) {
			palImage.palette[i].rgbBlue  = i;
			palImage.palette[i].rgbGreen = i;
			palImage.palette[i].rgbRed   = i;
			palImage.palette[i].rgbReserved = 0;
		}
	}
	else if (_bits == 4)
	{
		BitmapColorDepthDecreaser sss;
		palImage = sss.Decrease(buf, _width, _height);
	}

	BITMAPFILEHEADER bmfh;
	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	bmfh.bfType = 0x4D42;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	if (_bits <= 8) {
		bmfh.bfOffBits += palImage.iPaletteSize*sizeof(RGBQUAD);
	}
	bmfh.bfSize = bmfh.bfOffBits + palImage.byteCount;

	BITMAPINFOHEADER bmih;
	memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
	bmih.biSize        = sizeof(BITMAPINFOHEADER);
	bmih.biWidth       = _width;
	bmih.biHeight      = _height;
	bmih.biPlanes      = 1;
	bmih.biBitCount    = _bits;	// per pixel
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage   = palImage.byteCount;	 // size of raw pixel data wo padding, can be zero for no compression
	bmih.biClrUsed     = palImage.iPaletteSize;

	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, fp);
	
	if (_bits <= 8) {
		fwrite(palImage.palette, palImage.iPaletteSize*sizeof(RGBQUAD), 1, fp);

		// bmp is defined from the bottom to the top from the left to the right
		fwrite(palImage.dataOut, palImage.byteCount, 1, fp);
	}

	fclose(fp);

	delete[] palImage.palette;
	if (palImage.dataOut != buf) delete[] palImage.dataOut;

	return true;
}

int loadGreyscaleBMP(char *filename, unsigned char **dataOutPtr, BITMAPINFOHEADER *bmih)
{
    //open file in read binary mode
    FILE *fp = fopen(filename,"rb");
	if (!fp) {
		perror(filename);
		return false;
	}

    // read the bitmap file header
    BITMAPFILEHEADER bmfh;
    fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);

    // verify that this is a .BMP file by checking bitmap id
    if (bmfh.bfType != 0x4D42) {
        fclose(fp);
        return false;
    }

    //read the bitmap info header
    fread(bmih, sizeof(BITMAPINFOHEADER), 1, fp); 

    //move file pointer to the beginning of bitmap data
//    fseek(fp, bmfh.bfOffBits, SEEK_SET);

	RGBQUAD* palette = NULL;
	if (bmih->biBitCount <= 8) {

		// allocate enough memory for the image palette data
		palette = new RGBQUAD[bmih->biClrUsed];

		// verify memory allocation
		if (!palette) {
			fclose(fp);
			return false;
		}

		fread(palette, bmih->biClrUsed*sizeof(RGBQUAD), 1, fp);
	}

    // allocate memory for the bitmap image data
    unsigned char* data = (unsigned char*)malloc(bmih->biSizeImage);

    // verify memory allocation
    if (!data) {
        fclose(fp);
        return false;
    }

    // read in the bitmap image data
    fread(data, bmih->biSizeImage, 1, fp);

	unsigned char* dataOut = (unsigned char*)malloc(bmih->biWidth*bmih->biHeight);
	if (bmih->biBitCount == 8) {
		for (unsigned int idx=0; idx < bmih->biSizeImage; idx++) {
			dataOut[idx] = palette[data[idx]].rgbBlue;
		}
	}
	else
	if (bmih->biBitCount == 4) {
		for (unsigned int idx=0, idxOut=0; idx < bmih->biSizeImage; idx++,idxOut+=2) {
			// left pixels (earlier stream pixels) were placed higher in byte 
			dataOut[idxOut]   = palette[data[idx] >> 4 ].rgbBlue;
			dataOut[idxOut+1] = palette[data[idx] & 0xF].rgbBlue;
		}
	}
	*(dataOutPtr) = dataOut;

	//  if (bmih->biBitCount == 24) {
	//	// BGR -> RGB
	//	unsigned char tempRGB;
	//	for (int idx = 0; idx < bmih->biSizeImage; idx+=3) {
	//		tempRGB = bitmapImage[idx];
	//		bitmapImage[idx] = bitmapImage[idx + 2];
	//		bitmapImage[idx + 2] = tempRGB;
	//	}
	//}

    fclose(fp);

	delete [] palette;
	free(data);
	
    return true;
}


BitmapColorDepthDecreaser::~BitmapColorDepthDecreaser()
{	
	BucketIterType bucket;
	for (bucket = PaletteBuckets.begin(); bucket != PaletteBuckets.end(); bucket++)
	{
		delete (*bucket);
	}
}

GreyscaleImageDescriptor BitmapColorDepthDecreaser::Decrease(unsigned char* dataIn, unsigned int _width, unsigned int _height)
{
	unsigned int iBitCount   = 4;
	unsigned int _pixelCount = _width*_height;

	// 8bytes*4bits = 4bytes (needed for padding)
	assert(_width % 8 == 0);

	// 1. Map the image into histogram
	memset(histoLuma, 0, 256 * sizeof(unsigned int));
	for (unsigned int i = 0; i < _pixelCount; i++) {
		histoLuma[dataIn[i]]++;
	}

	// 2. Associate all found shades to a new bucket
	Bucket *bucket = new Bucket();
	for (unsigned int i = 0; i < 256; i++)
	{
		if ( histoLuma[i] )
			bucket->shadesMappedToBucket.push_back( std::make_pair((unsigned char)i, histoLuma[i]) );
	}
	PaletteBuckets.push_back(bucket);

	// 3. Split the bucket
	for (unsigned int i=0; i<iBitCount; i++)
		SplitLayer();

	// 4. It may happen less than 2^bit buckets are found (e.g. some bucket contained only single color)
	if (PaletteBuckets.size() < pow(2, iBitCount))
		SplitBucket( FindBucketWithHighestPrice() );

	// 5. Create palette and remapping table
	RGBQUAD* palette = new RGBQUAD[PaletteBuckets.size()];

	unsigned char aRemap[256];
	memset(aRemap, 0, 256 * sizeof(unsigned char));

	int index = 0;
	for (BucketIterType bucket = PaletteBuckets.begin(); bucket != PaletteBuckets.end(); bucket++)
	{
		unsigned char color = CalculateBucketColor( bucket );

		palette[index].rgbRed = palette[index].rgbBlue = palette[index].rgbGreen = color;
		palette[index].rgbReserved = 0;

		for (unsigned int i=0; i < (*bucket)->shadesMappedToBucket.size(); i++)
			aRemap[(*bucket)->shadesMappedToBucket[i].first] = index;

		index++;
	}

	// 6. Remap all colors to optimized values of each bucket
	for (unsigned int i = 0; i < _pixelCount; i++) {
		dataIn[i] = aRemap[dataIn[i]];
	}

	// 7. Prepare output descriptor
	GreyscaleImageDescriptor image;
	image.palette       = palette;
	image.iPaletteSize  = PaletteBuckets.size();
	image.byteCount     = _height * _width / 2;	// raw byte count (wo padding)
	image.dataOut       = new unsigned char[image.byteCount];
	memset(image.dataOut, 0, image.byteCount);

// 8. Decrease physical size
// left pixels (earlier stream pixels) are placed higher in memory 
unsigned int iOut = 0;
for (unsigned int i = 0; i < _pixelCount; i += 2, iOut++)
{
	image.dataOut[iOut] |= dataIn[i + 0] << 4;
	image.dataOut[iOut] |= dataIn[i + 1] << 0;
}

return image;
}

// Resize image 50%
unsigned char* BitmapResizeGreyscale05x(unsigned char* dataIn, unsigned int _width, unsigned int _height)
{
	assert(_width  % 8 == 0);
	assert(_height % 2 == 0);

	unsigned char* dataOut = new unsigned char[_width*_height / 4];
	memset(dataOut, 0,_width*_height / 4);

	// left pixels (earlier stream pixels) are placed higher in memory 
	unsigned int iOut = 0;
	unsigned int iAccum = 0;
	for (unsigned int i = 0; i < _height - 1; i += 2)
	{
		for (unsigned int j = 0; j < _width - 1; j += 2, iOut++)
		{
			iAccum =  dataIn[      i*_width + j];
			iAccum += dataIn[(i + 1)*_width + j];
			iAccum += dataIn[      i*_width + j + 1];
			iAccum += dataIn[(i + 1)*_width + j + 1];
			iAccum /= 4;

			dataOut[iOut] = iAccum;
		}
	}

	return dataOut;
}

// Resize image 200%
unsigned char* BitmapResizeGreyscale2x(unsigned char* dataIn, unsigned int _width, unsigned int _height)
{
	_width *= 2;
	_height *= 2;

	unsigned char* dataOut = new unsigned char[_width*_height];
	memset(dataOut, 0,_width*_height);

	// left pixels (earlier stream pixels) are placed higher in memory
	unsigned int iIn = 0;
	for (unsigned int i = 0; i < _height - 1; i += 2)
	{
		for (unsigned int j = 0; j < _width - 1; j += 2, iIn++)
		{
			dataOut[      i*_width + j]     = dataIn[iIn];
			dataOut[(i + 1)*_width + j]     = dataIn[iIn];
			dataOut[      i*_width + j + 1] = dataIn[iIn];
			dataOut[(i + 1)*_width + j + 1] = dataIn[iIn];
		}
	}

	return dataOut;
}

// Resize image 200%
unsigned char* BitmapResizeRGB2x(unsigned char* dataIn, unsigned int _width, unsigned int _height)
{
	unsigned char* dataOut = new unsigned char[_width*_height * 3 * 4];
	memset(dataOut, 0,_width*_height * 3 * 4);

	// left pixels (earlier stream pixels) are placed higher in memory
	unsigned int iInStart;
	for (unsigned int i = 0, iOut = 0, iIn = 0; i < _height; i++)
	{
		iInStart = iIn;	// remember start position
		for (unsigned int k = 0; k <= 1; k++) {
			iIn = iInStart;
			for (unsigned int j = 0; j < _width; j++, iOut += 3 * 2, iIn += 3)
			{
				dataOut[iOut + 0] = dataIn[iIn + 0];
				dataOut[iOut + 1] = dataIn[iIn + 1];
				dataOut[iOut + 2] = dataIn[iIn + 2];

				dataOut[iOut + 3] = dataIn[iIn + 0];
				dataOut[iOut + 4] = dataIn[iIn + 1];
				dataOut[iOut + 5] = dataIn[iIn + 2];
			}
		}
	}

	return dataOut;
}


// Trim odd finishing column/row of the image
unsigned char* BitmapTrimOddRGBA(unsigned char* dataIn, unsigned int& _width, unsigned int& _height)
{
	if ((_width % 2 == 0) && (_height % 2 == 0))
	{
		unsigned char* dataOut = new unsigned char[_width*_height * 4];
		memcpy(dataOut, dataIn, _width*_height * 4);

		return dataOut;
	}
	
	assert(_height > 1);
	assert(_width  > 1);

	unsigned int _height_cor = _height;
	unsigned int _width_cor  = _width; 

	if (_height_cor % 2 != 0) _height_cor--;
	if (_width_cor  % 2 != 0) _width_cor--;

	unsigned char* dataOut = new unsigned char[_width_cor*_height_cor * 4];
	memset(dataOut, 0,_width_cor*_height_cor * 4);

	unsigned int iOut = 0;
	for (unsigned int i = 0; i<_height_cor; i++)
	{
		for (unsigned int j = 0; j<_width_cor*4; j+=4, iOut += 4)
		{
			dataOut[iOut + 0] = dataIn[i*_width*4 + j + 0];
			dataOut[iOut + 1] = dataIn[i*_width*4 + j + 1];
			dataOut[iOut + 2] = dataIn[i*_width*4 + j + 2];
			dataOut[iOut + 3] = dataIn[i*_width*4 + j + 3];
		}
	}

	_width  = _width_cor;
	_height = _height_cor;

	return dataOut;
}


// Greyscale RGBA image
unsigned char* BitmapRGBA2GreyscaleRGBA(unsigned char* dataIn, unsigned int _width, unsigned int _height)
{
	unsigned char* dataOut = new unsigned char[_width*_height * 4];
	memset(dataOut, 0,_width*_height * 4);

	unsigned int iOut = 0;
	for (unsigned int i = 0; i<_height; i++)
	{
		for (unsigned int j = 0; j<_width * 4; j += 4, iOut += 4)
		{
			float fLuma = 0.299f*dataIn[i*_width * 4 + j + 0] +
				          0.587f*dataIn[i*_width * 4 + j + 1] +
				          0.114f*dataIn[i*_width * 4 + j + 2];

			fLuma = round(fLuma);
			if (fLuma > 255.0f) fLuma = 255.0f;

			dataOut[iOut + 0] = (unsigned char)fLuma;
			dataOut[iOut + 1] = (unsigned char)fLuma;
			dataOut[iOut + 2] = (unsigned char)fLuma;
			dataOut[iOut + 3] = dataIn[i*_width * 4 + j + 3];
		}
	}

	return dataOut;
}


// Greyscale RGBA image and store as greyscale loosing alpha (potentailly alpha could be used to mix background color)
unsigned char* BitmapRGBA2Greyscale(unsigned char* dataIn, unsigned int _width, unsigned int _height)
{
	unsigned char* dataOut = new unsigned char[_width*_height];
	memset(dataOut, 0,_width*_height);

	unsigned int iOut = 0;
	for (unsigned int i = 0; i<_height; i++)
	{
		for (unsigned int j = 0; j<_width * 4; j += 4, iOut++)
		{
			float fLuma = 0.299f*dataIn[i*_width * 4 + j + 0] +
				          0.587f*dataIn[i*_width * 4 + j + 1] +
				          0.114f*dataIn[i*_width * 4 + j + 2];

			fLuma = round(fLuma);
			if (fLuma > 255.0f) fLuma = 255.0f;

			dataOut[iOut] = (unsigned char)fLuma;
		}
	}

	return dataOut;
}

int BitmapColorDepthDecreaser::PixelsRemappingPrice(BucketIterType bucket, unsigned char colorMiddle)
{
	int res = 0;
	for (unsigned int i=0; i < (*bucket)->shadesMappedToBucket.size(); i++)
	{
		res += (*bucket)->shadesMappedToBucket[i].second * abs((*bucket)->shadesMappedToBucket[i].first - colorMiddle);
	}
	return res;
}

BucketIterType BitmapColorDepthDecreaser::FindBucketWithHighestPrice()
{
	BucketIterType found;
	unsigned int price = 0;
	for (BucketIterType bucket = PaletteBuckets.begin(); bucket != PaletteBuckets.end(); bucket++)
	{
		if ((*bucket)->price >= price)
			found = bucket;
	}

	return found;
}

unsigned char BitmapColorDepthDecreaser::CalculateBucketColor(BucketIterType bucket)
{
	if ((*bucket)->shadesMappedToBucket.size() == 1)
		return (*bucket)->shadesMappedToBucket[0].first;	// retun color; price remains zero!

	int cMin = (*bucket)->shadesMappedToBucket.front().first;
	int cMax = (*bucket)->shadesMappedToBucket.back().first;

	int iMiddleColor = (cMax + cMin)/2;
	int iPriceCurrent = PixelsRemappingPrice(bucket, iMiddleColor);
	int iPriceRight   = (iMiddleColor+1 <= cMax) ? PixelsRemappingPrice(bucket, iMiddleColor+1) : iPriceCurrent;
	int iPriceLeft    = (iMiddleColor-1 >= cMin) ? PixelsRemappingPrice(bucket, iMiddleColor-1) : iPriceCurrent;

	(*bucket)->price = iPriceCurrent;

	if ((iPriceRight < iPriceLeft) && (iPriceRight < iPriceCurrent))
	{
		// Try moving middle pointer to the right
		while (iMiddleColor+1 <= cMax)
		{
			iPriceCurrent = PixelsRemappingPrice(bucket, iMiddleColor);
			iPriceRight   = (iMiddleColor+1 <= cMax) ? PixelsRemappingPrice(bucket, iMiddleColor+1) : iPriceCurrent;
			iPriceLeft    = (iMiddleColor-1 >= cMin) ? PixelsRemappingPrice(bucket, iMiddleColor-1) : iPriceCurrent;
			(*bucket)->price = iPriceCurrent; 

			if ((iPriceRight < iPriceLeft) && (iPriceRight < iPriceCurrent))
				iMiddleColor++;
			else
				break;
		}
	}
	else if ((iPriceLeft < iPriceRight) && (iPriceLeft < iPriceCurrent))
	{
		// Try moving middle pointer to the left
		while (iMiddleColor-1 >= cMin)
		{
			iPriceCurrent = PixelsRemappingPrice(bucket, iMiddleColor);
			iPriceRight   = (iMiddleColor+1 <= cMax) ? PixelsRemappingPrice(bucket, iMiddleColor+1) : iPriceCurrent;
			iPriceLeft    = (iMiddleColor-1 >= cMin) ? PixelsRemappingPrice(bucket, iMiddleColor-1) : iPriceCurrent;
			(*bucket)->price = iPriceCurrent;

			if ((iPriceLeft < iPriceRight) && (iPriceLeft < iPriceCurrent))
				iMiddleColor--;
			else
				break;
		}
	}

	return (unsigned char)iMiddleColor;

}

void BitmapColorDepthDecreaser::MoveBucketRegion(BucketIterType bucketBefore, BucketIterType bucket, int iMiddle )
{
	for (int i=0; i < iMiddle; i++) {
		(*bucketBefore)->shadesMappedToBucket.push_back((*bucket)->shadesMappedToBucket[i]);
	}

	(*bucket)->shadesMappedToBucket.erase((*bucket)->shadesMappedToBucket.begin(), (*bucket)->shadesMappedToBucket.begin() + iMiddle);
}

int BitmapColorDepthDecreaser::PixelsOnTheLeftCount(BucketIterType bucket, int iMiddle)
{
	int res = 0;
	for (int i=0; i < iMiddle; i++) {
			res += (*bucket)->shadesMappedToBucket[i].second;
	}
	return res;
}

int BitmapColorDepthDecreaser::PixelsOnTheRightCount(BucketIterType bucket, int iMiddle)
{
	int res = 0;
	for (unsigned int i=iMiddle; i < (*bucket)->shadesMappedToBucket.size(); i++) {
			res += (*bucket)->shadesMappedToBucket[i].second;	// sum the number of pixels mapped
	}
	return res;
}

void BitmapColorDepthDecreaser::SplitLayer()
{
	for (BucketIterType bucket = PaletteBuckets.begin(); bucket != PaletteBuckets.end(); bucket++)
	{
		SplitBucket(bucket);
	}
}


void BitmapColorDepthDecreaser::SplitBucket(BucketIterType bucket)
{
	if ((*bucket)->shadesMappedToBucket.size() > 1)
	{
		int iMiddle = (*bucket)->shadesMappedToBucket.size() / 2;
	
		if ((*bucket)->shadesMappedToBucket.size() == 2)
			iMiddle = 1;
		else
		{
			int iLeft  = PixelsOnTheLeftCount(bucket, iMiddle);
			int iRight = PixelsOnTheRightCount(bucket, iMiddle);

			if (iLeft < iRight)
			{
				// Try moving middle pointer to the right
				while (iMiddle+1 < int((*bucket)->shadesMappedToBucket.size()))	// touch the last element we iterate until size <
				{
					if (PixelsOnTheLeftCount(bucket, iMiddle+1) < PixelsOnTheRightCount(bucket, iMiddle+1))
						iMiddle++;
					else
						break;
				}
			}
			else if (iLeft > iRight)
			{
				// Try moving middle pointer to the left
				while (iMiddle-1 > 0)	// prevent from touching zero as we iterate until iMiddle <
				{
					if (PixelsOnTheLeftCount(bucket, iMiddle-1) > PixelsOnTheRightCount(bucket, iMiddle-1))
						iMiddle--;
					else
						break;
				}
			}
		}

		// prepend a new bucket and move half of the contents from the current one
		BucketIterType bucketBefore;
		bucketBefore = PaletteBuckets.insert(bucket, new Bucket());

		MoveBucketRegion(bucketBefore, bucket, iMiddle);
	}
}
