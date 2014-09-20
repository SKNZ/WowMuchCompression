// WaveletTesting1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

namespace
{
	const char* KInputFile = "original.bmp";
	const char* KCompressedFile = "compressed.bmp";
	const char* KExtractedFile = "uncompressed.bmp";

#pragma pack(2)
	struct BMPFileHeader
	{
		uint16_t Signature;
		uint32_t FileSize;
		uint16_t Reserved1;
		uint16_t Reserved2;
		uint32_t PixelArrayOffset;
	};

	struct BMPV5Header
	{
		uint32_t DIBHeaderSize;
		int32_t ImageWidth;
		int32_t ImageHeight;
		uint16_t Planes;
		uint16_t BitsPerPixels;
		uint32_t Compression;
		uint32_t ImageSize;
		int32_t XPixelsPerMeter;
		int32_t YPixelsPerMeter;
		int32_t ColorsInColorsTable;
		int32_t ImportantColorCount;
		int32_t RedChannelBitmask;
		int32_t GreenChannelBitmask;
		int32_t BlueChannelBitmask;
		int32_t AlphaChannelBitmask;
		int32_t ColorSpaceType;
		char ColorSpaceEndpoints[36];
		int32_t RedChannelGamma;
		int32_t GreenChannelGamma;
		int32_t BlueChannelGamma;
		int32_t Intent;
		int32_t ICCProfileData;
		int32_t ICCProfileSpace;
		int32_t Reserved;
	};

	struct Pixel
	{
		uint8_t B, G, R;
	};
#pragma pack()

	void Compress(Pixel** pixelArray, int height, int width)
	{
	}

	void Extract(Pixel** pixelArray, int height, int width)
	{
	}
}

int _tmain(int argc, _TCHAR** argv)
{
	ifstream originalFile(KInputFile, ios::binary | ios::in);
	
	if (!originalFile)
		return EXIT_FAILURE;

	BMPFileHeader fileHeader;
	originalFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));

	BMPV5Header bmpHeader;
	originalFile.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));
	
	int rowSize = 4 * (((bmpHeader.BitsPerPixels * bmpHeader.ImageWidth) + 31) / 32);

	Pixel** pixelArray = new Pixel*[bmpHeader.ImageWidth];
	for (int i = 0; i < bmpHeader.ImageWidth; ++i)
		pixelArray[i] = new Pixel[bmpHeader.ImageHeight];

	const int paddingBitsCount = rowSize - (bmpHeader.ImageWidth * sizeof(Pixel));

	for (int y = 0; y < bmpHeader.ImageHeight; ++y)
	{
		for (int x = 0; x < bmpHeader.ImageWidth; ++x)
			originalFile.read(reinterpret_cast<char*>(&pixelArray[x][y]), sizeof(Pixel));

		originalFile.seekg(static_cast<int>(originalFile.tellg()) + paddingBitsCount);
	}

	originalFile.close();

	cout << "BMP File Information" << endl
		<< "=========================" << endl
		<< "Signature: " << fileHeader.Signature << endl
		<< "FileSize: " << fileHeader.FileSize << endl
		<< "Reserved: " << fileHeader.Reserved1 << endl
		<< "Reserved: " << fileHeader.Reserved2 << endl
		<< "PixelArrayOffset: " << fileHeader.PixelArrayOffset << endl
		<< "Width: " << bmpHeader.ImageWidth << endl
		<< "Height: " << bmpHeader.ImageHeight << endl
		<< "Bits per pixel: " << bmpHeader.BitsPerPixels << endl
		<< "PixelArrayRowSize: " << rowSize << endl
		<< "PixelArraySize: " << rowSize * bmpHeader.ImageWidth << endl
		<< "=========================" << endl;
	
	Compress(pixelArray, bmpHeader.ImageHeight, bmpHeader.ImageWidth);

	ofstream compressedFile(KCompressedFile, ios::binary | ios::out | ios::trunc);

	if (!compressedFile)
		return EXIT_FAILURE;

	compressedFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
	compressedFile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));

	for (int y = 0; y < bmpHeader.ImageHeight; ++y)
	{
		for (int x = 0; x < bmpHeader.ImageWidth; ++x)
			compressedFile.write(reinterpret_cast<char*>(&pixelArray[x][y]), sizeof(Pixel));

		for (int i = 0; i < paddingBitsCount; ++i)
			compressedFile << 0;
	}

	compressedFile.close();

	Extract(pixelArray, bmpHeader.ImageHeight, bmpHeader.ImageWidth);

	ofstream extractedFile(KExtractedFile, ios::binary | ios::out | ios::trunc);

	if (!extractedFile)
		return EXIT_FAILURE;

	extractedFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
	extractedFile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));


	for (int y = 0; y < bmpHeader.ImageHeight; ++y)
	{
		for (int x = 0; x < bmpHeader.ImageWidth; ++x)
			extractedFile.write(reinterpret_cast<char*>(&pixelArray[x][y]), sizeof(Pixel));

		for (int i = 0; i < paddingBitsCount; ++i)
			extractedFile << 0;
	}

	extractedFile.close();

	for (int i = 0; i < bmpHeader.ImageWidth; ++i)
		delete[] pixelArray[i];
	delete[] pixelArray;

	system("pause");
	return EXIT_SUCCESS;
}
