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
		CPixel (int r, int g, int b) : R(r), G(g), B(b) {}

		int R, G, B;
	};

	void printErrorMessage ()
	{
		cerr << "Usage: wmc -[c|x] fileName" << endl;
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
		CFrame Matrix;
		
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

