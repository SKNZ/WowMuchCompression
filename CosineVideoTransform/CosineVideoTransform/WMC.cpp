// CosineVideoTransform.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace std::chrono;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		/**
		  * D�claration des outils de mesure du temps d'ex�cution.
		  **/
		time_point<system_clock> start = system_clock::now(), compression, extraction;

		/**
		  * CCompressor est une classe encapsulant l'algorithme de compression.
		  * Param�tres: fichier vid�o d'origine, fichier compress�
		  **/
		CCompressor compressor("original.avi", "compressed.wmc");
		compressor.run(); // Execution de la compression
		compression = system_clock::now(); // Arr�t du premier chronom�tre

		/**
		  * Similaire � CCompressor, mais pour l'extraction.
		  * Param�tres: fichier compress�, fichier vid�o de sortie
		  **/
		CExtractor extractor("compressed.wmc", "extracted.avi");
		extractor.run(); // Execution de l'extraction
		extraction = system_clock::now(); // Arr�t du deuxi�me chronom�tre

		/**
		  * Affichage des chronom�tres
		  **/
		cout << endl << "Compression: " << duration_cast<milliseconds>(compression - start).count() << "ms" << endl
			<< "Extraction: " << duration_cast<milliseconds>(extraction - compression).count() << "ms" << endl;
	}
	catch (const exception& e) // Affichage des �ventuelles erreurs remont�es
	{
		cout << "Unhandled exception: " << e.what() << endl;
	}

	cout << endl;
	system("pause");
	return 0;
}

