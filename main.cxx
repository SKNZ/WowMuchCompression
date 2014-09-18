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
#include <vector>

#define PIXEL_BLOCK_SIZE 		5
#define PIXEL_MAX_COLOR_DELTA	5

using namespace std;

namespace
{
	class CPixel;
	
	typedef vector <CPixel> CPixelColumn;
	typedef vector <CPixelColumn> CFrame;
	typedef vector <CFrame> CVideo;
	
	CVideo Video;
	
	class CPixel
	{
	public:
		CPixel(int r = 0, int g = 0, int b = 0) : R(r), G(g), B(b) {}
		
		bool compare (const CPixel & tar) const
		{
			return (tar.R <= R + PIXEL_MAX_COLOR_DELTA || tar.R >= R - PIXEL_MAX_COLOR_DELTA) &&
					(tar.G <= G + PIXEL_MAX_COLOR_DELTA || tar.G >= G - PIXEL_MAX_COLOR_DELTA) &&
					(tar.B <= B + PIXEL_MAX_COLOR_DELTA || tar.B >= B - PIXEL_MAX_COLOR_DELTA);
		}
		int R, G, B;
	};

	void printErrorMessage ()
	{
		cerr << "Usage: wmc -[c|x] fileName" << endl;
	}
	
	bool comparePixelBlocks (const CFrame & pixelBlockRef, const CFrame & pixelBlockTar)
	{
		for (int x = 0; x < PIXEL_BLOCK_SIZE; ++x)
		{
			for (int y = 0; y < PIXEL_BLOCK_SIZE; ++y)
			{
				if (!pixelBlockRef [x][y].compare (pixelBlockTar [x][y]))
					return false;
			}
		}
		return true;
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

	void compress()
	{
		//Redondance avec vecteurs
		for (int frameNumber = 0; frameNumber < Video.size () - 1; ++frameNumber)
		{
			CFrame & frameRef = Video [frameNumber];
			CFrame pixelBlockRef;
			pixelBlockRef.resize (PIXEL_BLOCK_SIZE);
					
			for (CPixelColumn Column : pixelBlockRef)
			Column.resize (PIXEL_BLOCK_SIZE);
					
			CFrame pixelBlockTar (pixelBlockRef); //PixelBlockTarget
			for (int x = 0; x < Video [0].size () - PIXEL_BLOCK_SIZE + 1; ++x)
			{
				for (int y = 0; y < Video [0][0].size () - PIXEL_BLOCK_SIZE + 1; ++y)
				{
					for (int xBlock = 0; x < pixelBlockRef.size (); ++x)
						for (int yBlock = 0; y < pixelBlockRef.size (); ++y)
							pixelBlockRef [xBlock][yBlock] = frameNumber [x][y];

				}
			}
		}
		
		
	}

	void extract()
	{

	}
}

int main(int argc, char** argv)
{
	if (argc < 3)
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

	return EXIT_SUCCESS;
}

