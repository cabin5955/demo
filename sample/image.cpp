#include <assert.h>
#include <fstream>

#include "image.h"
#include "include/GL/glew.h"

using namespace std;

Image::Image(char* ps, int w, int h, int f) : pixels(ps), width(w), height(h), format(f) {
	
}

Image::~Image() {
	delete[] pixels;
}

namespace {
	//Converts a four-character array to an integer, using little-endian form
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
					 ((unsigned char)bytes[2] << 16) |
					 ((unsigned char)bytes[1] << 8) |
					 (unsigned char)bytes[0]);
	}
	
	//Converts a two-character array to a short, using little-endian form
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
					   (unsigned char)bytes[0]);
	}
	
	//Reads the next four bytes as an integer, using little-endian form
	int readInt(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}
	
	//Reads the next two bytes as a short, using little-endian form
	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}
}

int c_match_len(int len)
{
	int r_len = 0;
	r_len = ((len /4)+1)*4;
	return r_len;
}

unsigned char * loadBMP24(const char * imagepath, 
						unsigned int& outWidth, 
						unsigned int& outHeight, 
						bool flipY){

	printf("Reading image %s\n", imagepath);
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("Image could not be opened\n"); return NULL;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 byes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return NULL;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return NULL;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	outWidth   = *(int*)&(header[0x12]);
	outHeight  = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=outWidth*outHeight*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
		
	// Everything is in memory now, the file wan be closed
	fclose (file);

	if (flipY){
		// swap y-axis
		unsigned char * tmpBuffer = new unsigned char[outWidth*3];
		int size = outWidth*3;
		for (int i=0;i<outHeight/2;i++){
			// copy row i to tmp
			memcpy_s(tmpBuffer,size,data+outWidth*3*i,size);
			// copy row h-i-1 to i
			memcpy_s(data+outWidth*3*i, size, data+outWidth*3*(outHeight-i-1), size);
			// copy tmp to row h-i-1
			memcpy_s(data+outWidth*3*(outHeight-i-1), size,tmpBuffer, size);
		}
		delete [] tmpBuffer;
	}

	C_CLR_RGB *rgb_data = new C_CLR_RGB[outWidth*outHeight];

	memcpy((unsigned char*)rgb_data,data,imageSize);
	
	return (unsigned char*)rgb_data;
}

Image* loadBMP32(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);
	
	//Read the header
	int headerSize = readInt(input);
	int width;
	int height;
	int bits;
	int bit_width;
	switch(headerSize) {
		case 40:
			//V3
			width = readInt(input);
			height = readInt(input);
			input.ignore(2);
			bit_width = readShort(input);
			if(bit_width == 24)
				bits = 3;
			else if(bit_width == 32)
				bits = 4;

			//assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			assert(readShort(input) == 0 || !"Image is compressed");
			break;
		case 12:
			//OS/2 V1
			width = readShort(input);
			height = readShort(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			break;
		case 64:
			//OS/2 V2
			assert(!"Can't load OS/2 V2 bitmaps");
			break;
		case 108:
			//Windows V4
			assert(!"Can't load Windows V4 bitmaps");
			break;
		case 124:
			//Windows V5
			assert(!"Can't load Windows V5 bitmaps");
			break;
		default:
			assert(!"Unknown bitmap format");
	}
	
	//Read the data
	//int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int bytesPerRow = (width*bits+3)&(~3);
	
	int size = bytesPerRow * height;
	char *pixels = (char*)malloc(size);
	int match_width = 0;
	int match_height = 0;
	
	if(width%4 == 0){
		match_width = width;
	}
	else{
		match_width = c_match_len(width);
	}

	if(height%4 == 0){
		match_height = height;
	}
	else{
		match_height = c_match_len(height);
	}

	int match_size = match_width * match_height * bits;
	C_CLR_RGBA *imgData = (C_CLR_RGBA*)malloc(match_size);
	memset(imgData,0,match_size);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels, size);
	
	//Get the data into the right format
	char *pixels2 = (char*)malloc(width * height * bits);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			for(int c = 0; c < bits; c++) {
				pixels2[bits * (width * y + x) + c] = pixels[bytesPerRow * y + bits * x + (bits - 1 - c)];
				int ii = match_width * y + x;
				unsigned char data = pixels2[bits * (width * y + x) + c];				
				if(c == 0){
					imgData[ii].a = data;
				}
				else if(c == 1){
					imgData[ii].r = data;
				}
				else if(c == 2){
					imgData[ii].g = data;
				}
				else if(c == 3){
					imgData[ii].b = data;
				}
			}			
		}
	}
	
	input.close();

	return new Image((char*)imgData, match_width, match_height, GL_RGBA);
}

Image* loadBMP(const char * imagepath){
	printf("Reading image %s\n", imagepath);
	unsigned int outWidth;
	unsigned int outHeight;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("Image could not be opened\n"); return NULL;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 byes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return NULL;
	}
	fclose (file);
	
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return NULL;}

	if ( *(int*)&(header[0x1C])!=24 ){
		return loadBMP32(imagepath);
	}
	else{
		data = loadBMP24(imagepath, outWidth, outHeight, false);
		return new Image((char*)data, outWidth, outHeight, GL_RGB);
	}
}
