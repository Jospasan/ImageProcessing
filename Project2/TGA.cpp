#include "TGA.h"

TGA::TGA() {
}

void TGA::addHeader(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin, short colorMapLength, char colorMapDepth, short xOrigin, short yOrigin, short width, short height, char bitsPerPixel, char imageDescriptor) {
	Head->idLength = idLength;
	Head->colorMapType = colorMapType;
	Head->dataTypeCode = dataTypeCode;
	Head->colorMapOrigin = colorMapOrigin;
	Head->colorMapLength = colorMapLength;
	Head->colorMapDepth = colorMapDepth;
	Head->xOrigin = xOrigin;
	Head->yOrigin = yOrigin;
	Head->width = width;
	Head->height = height;
	Head->bitsPerPixel = bitsPerPixel;
	Head->imageDescriptor = imageDescriptor;
}

void TGA::addPixel(unsigned char Red, unsigned char Green, unsigned char Blue) {
	Pixel* Temp_ = new Pixel;
	Temp_->Red = Red;
	Temp_->Green = Green;
	Temp_->Blue = Blue;
	Pictures.push_back(*Temp_);
}