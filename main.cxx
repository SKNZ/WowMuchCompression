/**
<<<<<<< HEAD
 * @file main.cxx
 * @author NARENJI-SHESHKALANI Floran
 * @author MERINO Mathieu
 * @author MARGIER Thomas
 * @date 18/09/2014
 * @brief WMC Video Compression Codec
 *
 **/

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>

#define PIXEL_BLOCK_SIZE 		5
#define PIXEL_MAX_COLOR_DELTA	5

using namespace std;

namespace
{
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
	class CPixel;
	class CTranslation;
	
	typedef vector <CPixel> CPixelColumn;
	typedef vector <CPixelColumn> CFrame;
	typedef vector <CFrame> CVideo;

	map <int, vector <CTranslation> > TranslationsPerFrame;
	map <int, BMPFileHeader fileHeader> HeaderBMP;
	map <int, BMPV5Header bmpHeader> HeaderBMPV5;
	CVideo Video;
	
	class CPixel
	{
	public:
		CPixel(int r = 0, int g = 0, int b = 0) : R(r), G(g), B(b) {}
		
		/*check red, blue and green of *this and a target pixel, if the difference is bigger than MAX_COLOR_DELTA,
		return false and consider they are not enough resemblant*/
		bool compare (const CPixel & tar) const
		{
			return (tar.R <= R + PIXEL_MAX_COLOR_DELTA || tar.R >= R - PIXEL_MAX_COLOR_DELTA) &&
					(tar.G <= G + PIXEL_MAX_COLOR_DELTA || tar.G >= G - PIXEL_MAX_COLOR_DELTA) &&
					(tar.B <= B + PIXEL_MAX_COLOR_DELTA || tar.B >= B - PIXEL_MAX_COLOR_DELTA);
		}
		void empty ()
		{
			R = G = B = -1;
		}
		bool isEmpty () const
		{
			return R == G && G == B && -1 == B;
		}
		int R, G, B;
	};
	
	class CTranslation
	{
	public:
		CTranslation (int x1, int y1, int x2, int y2):X1(x1), Y1(y1), X2(x2), Y2(y2) {}
		int X1, Y1, X2, Y2; //X1,Y1 top left corner of block on current frame. X2, Y2 top left corner of block on current frame +1
	};

	void printErrorMessage ()
	{
		cerr << "Usage: wmc -[c|x] fileName" << endl;
	}
	
	// compare two matrix of pixels, checking if they are alike depending on % difference pixel per pixel
	bool comparePixelBlocks (const CFrame & pixelBlockRef, const CFrame & pixelBlockTar)
	{
		// double for the check every pixel of every line
		for (int x = 0; x < PIXEL_BLOCK_SIZE; ++x)
		{
			for (int y = 0; y < PIXEL_BLOCK_SIZE; ++y)
			{
				// if even only two pixels of same positions in the 2 matrix are more than X% different, return false
				if (!pixelBlockRef [x][y].compare (pixelBlockTar [x][y]) || pixelBlockTar [x][y].isEmpty ())
					return false;
			}
		}
		return true; // if every equivalent pixel are alike, return true
	}

	bool loadFile (char* fileName)
	{
		if (FILE *file = fopen (fileName, "r"))
		{

			fclose(file);
			return true;
		}

		return false;
	}
	CFrame fillPixelBlock (const CFrame & frame, unsigned xOrigin, unsigned yOrigin)
	{
		CFrame pixelBlock;
		pixelBlock.resize (PIXEL_BLOCK_SIZE);
		for (CPixelColumn Column : pixelBlock)
			pixelBlock.resize (PIXEL_BLOCK_SIZE);
		for (unsigned x = 0; x < PIXEL_BLOCK_SIZE; ++x)
			for (unsigned y = 0; y < PIXEL_BLOCK_SIZE; ++y)
				pixelBlock [x][y] = frame [xOrigin + x][yOrigin + y];
		return pixelBlock;
	}
	void emptyPixelBlock (CFrame & frame, unsigned xOrigin, unsigned yOrigin)
	{
		for (unsigned x = xOrigin; x < xOrigin + PIXEL_BLOCK_SIZE; ++x)
			for (unsigned y = yOrigin; y < yOrigin + PIXEL_BLOCK_SIZE; ++y)
				frame [x][y].empty ();
	}
	void compress()
	{
		//
		// redundancy
		for (unsigned frameNumber = 0; frameNumber < Video.size () - 1; ++frameNumber)// every frame of the video
		{
			CFrame & frameRef = Video [frameNumber];// reference frame which will be used to compress target frame
			CFrame & frameTar = Video [frameNumber + 1];// target frame which will be compressed
			CFrame pixelBlockRef;//	block of pixel from reference frame that will be compared to every block of pixel on target frame
			pixelBlockRef.resize (PIXEL_BLOCK_SIZE);
					
			for (CPixelColumn Column : pixelBlockRef)
				Column.resize (PIXEL_BLOCK_SIZE);
					
			CFrame pixelBlockTar (pixelBlockRef); //PixelBlockTarget

			for (unsigned xRef = 0; xRef < Video [0].size () - PIXEL_BLOCK_SIZE + 1; ++xRef)
				for (unsigned yRef = 0; yRef < Video [0][0].size () - PIXEL_BLOCK_SIZE + 1; ++yRef)
				{
					pixelBlockRef = fillPixelBlock (Video [frameNumber], xRef, yRef);
					for (unsigned xTar = 0; xTar < Video [0].size () - PIXEL_BLOCK_SIZE + 1; ++xTar)
						for (unsigned yTar = 0; yTar < Video [0].size () - PIXEL_BLOCK_SIZE + 1; ++yTar)
						{
							pixelBlockTar = fillPixelBlock (Video [frameNumber + 1], xTar, yTar);
							if (comparePixelBlocks (pixelBlockRef, pixelBlockTar))
							{
								TranslationsPerFrame [frameNumber].push_back (CTranslation (xRef, yRef, xTar, yTar));
								emptyPixelBlock (Video [frameNumber + 1], xTar, yTar - 4);
							}
						}
				}

		}

	}

	void extract()
	{

	}

	CFrame importFrame (string framePath, unsigned frameNumber)
	{
		ifstream originalFile(framePath, ios::binary | ios::in);

		if (!originalFile)
			exit(0);

		BMPFileHeader fileHeader;
		originalFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));

		BMPV5Header bmpHeader;
		originalFile.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));

		int rowSize = 4 * (((bmpHeader.BitsPerPixels * bmpHeader.ImageWidth) + 31) / 32);

		CFrame pixelArray;
		const int paddingBitsCount = rowSize - (bmpHeader.ImageWidth * 3 * sizeof(uint8_t));

		for (int y = 0; y < bmpHeader.ImageHeight; ++y)
		{
			for (int x = 0; x < bmpHeader.ImageWidth; ++x)

			originalFile.seekg(static_cast<int>(originalFile.tellg()) + paddingBitsCount);
		}

		originalFile.close();
		return pixelArray;
	}
	bool exportFrame (CFrame frame, string pathFrame)
	{
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
		return 0;
	}
}

	void test () //
	{

	}
int main(int argc, char** argv)
{
	test ();
	/*if (argc < 3)
	{
		printErrorMessage();
		return EXIT_FAILURE;
	}

	if (loadFile(argv[2]))
	{
		if (!strcmp(argv[1], "-c"))
		{
			compress();
		}
		else if (!strcmp(argv[1], "-x"))
		{
			extract();
		}
		else
		{
			printErrorMessage();
			return EXIT_FAILURE;
		}
	}
	else
	{
		cerr << "The file couldn't be read." << endl;
		return EXIT_FAILURE;
	}
	*/
	return EXIT_SUCCESS;
}

