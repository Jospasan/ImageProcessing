#pragma once
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class TGA {
public:
	struct Header {
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;
	};

	struct Pixel {
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;
	};

	Header* Head = new Header;
	vector<Pixel> Pictures;
	TGA();
	void addHeader(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin, short colorMapLength, char colorMapDepth, short xOrigin, short yOrigin, short width, short height, char bitsPerPixel, char imageDescriptor);
	void addPixel(unsigned char Red, unsigned char Green, unsigned char Blue);
};