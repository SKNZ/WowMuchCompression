// CosineVideoTransform.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace nsWMC;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		CCompressor compressor("original.avi", "compressed.wmc");
		compressor.run();

		/*CExtractor extractor("compressed.wmc", "extracted.avi");
		extractor.run();*/
	}
	catch (const exception& e) // Gestion d'erreur
	{
		cout << "Unhandled exception: " << e.what() << endl;
	}

	system("pause");
	return 0;
}

