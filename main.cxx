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
#include <iostream>

using namespace std;

namespace
{
	class Pixel
	{
	public:
		Pixel(int r, int g, int b) : R(r), G(g), B(b) { }

		int R, G, B;
	};

	void printErrorMessage()
	{
		cerr << "Usage: wmc -[c|x] fileName" << endl;
	}

	void compress(char* fileName)
	{

	}

	void extract(char* fileName)
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

	// File validity checks
	if (true /* to be replaced with file validity checks*/)
	{

	}
	else
	{
		cerr << "The file couldn't be read." << endl;
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[1], "-c"))
	{
		compress(argv[2]);
	}
	else if (!strcmp(argv[1], "-x"))
	{
		extract(argv[2]);
	}
	else
	{
		printErrorMessage();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

