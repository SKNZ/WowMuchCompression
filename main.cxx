/**
 * 
 * @Author: Mathieu MERINO, Thomas MARGIER, Floran NARENJI-SHESHKALANI
 * 
 */
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		cerr << "Not enough arguments: wmc -[c|d] fileName" << endl;
		return 1;
	}
	//test si le fichier existe
	if (argv[1] == "-c")
	{
		Compression ();
		return 0;
	}
	
	else
	{
		Decompression ();
		return 0;
	}
}

