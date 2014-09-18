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
#include <iostream>

using namespace std;

namespace
{
	class Pixel
	{
	public:
		Pixel(int r, int g, int b) : m_r(r), m_g(g), m_b(b) { }

		int getR() { return m_r; }
		int getG() { return m_g; }
		int getB() { return m_b; }

		void setR(int r) { m_r = r; }
		void setG(int g) { m_g = g; }
		void setB(int b) { m_b = b; }

	private:
		int m_r, m_g, m_b;
	};

	void printErrorMessage()
	{
		cerr << "Not enough arguments: wmc -[c|x] fileName" << endl;
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

	if (argv[1] == "-c") //strcmp fdp
	{
		compress(argv[2]);
	}
	else if (argv[1] == "-x")
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

