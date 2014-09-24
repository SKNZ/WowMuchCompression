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
#include <map>

#define PIXEL_BLOCK_SIZE 		5
#define PIXEL_MAX_COLOR_DELTA	5

using namespace std;

namespace
{
	class CPixel;
	class CTranslation;
	
	typedef vector <CPixel> CPixelColumn;
	typedef vector <CPixelColumn> CFrame;
	typedef vector <CFrame> CVideo;
	
	map <int, vector <CTranslation>> Map;
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
		int R, G, B;
	};
	
	class CTranslation
	{
	public:
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
				if (!pixelBlockRef [x][y].compare (pixelBlockTar [x][y]))
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

	void compress()
	{
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
			// fill pixelBlockRef with pixels from reference frame
			// for each pixel of the frame
			for (unsigned x = 0; x < Video [0].size () /*width of a frame*/ - PIXEL_BLOCK_SIZE + 1; ++x)
				for (unsigned y = 0; y < Video [0][0].size () /*height of a frame*/ - PIXEL_BLOCK_SIZE + 1; ++y)
					// for each pixel of the block
					for (unsigned xBlock = 0; x < pixelBlockRef.size (); ++x)
						for (unsigned yBlock = 0; y < pixelBlockRef.size (); ++y)
							pixelBlockRef [xBlock][yBlock] = frameRef[x + xBlock][y + yBlock];// fill the block
			// fill pixelBlockTar with pixels from target frame
			for (unsigned x = 0; x < Video [0].size () /*width of a frame*/ - PIXEL_BLOCK_SIZE + 1; ++x)
				for (unsigned y = 0; y < Video [0][0].size () /*height of a frame*/ - PIXEL_BLOCK_SIZE + 1; ++y)
					for (unsigned xBlock = 0; x < pixelBlockRef.size (); ++x)
						for (unsigned yBlock = 0; y < pixelBlockRef.size (); ++y)
							pixelBlockTar [xBlock][yBlock] = frameTar [x + xBlock][y + yBlock];// fill the block
			
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

