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
		  * Déclaration des outils de mesure du temps d'exécution.
		  **/
		time_point<system_clock> start = system_clock::now(), compression, extraction;

		/**
		  * CCompressor est une classe encapsulant l'algorithme de compression.
		  * Paramètres: fichier vidéo d'origine, fichier compressé
		  **/
		CCompressor compressor("original.avi", "compressed.wmc");
		compressor.run(); // Execution de la compression
		compression = system_clock::now(); // Arrêt du premier chronomètre

		/**
		  * Similaire à CCompressor, mais pour l'extraction.
		  * Paramètres: fichier compressé, fichier vidéo de sortie
		  **/
		CExtractor extractor("compressed.wmc", "extracted.avi");
		extractor.run(); // Execution de l'extraction
		extraction = system_clock::now(); // Arrêt du deuxième chronomètre

		/**
		  * Affichage des chronomètres
		  **/
		cout << endl << "Compression: " << duration_cast<milliseconds>(compression - start).count() << "ms" << endl
			<< "Extraction: " << duration_cast<milliseconds>(extraction - compression).count() << "ms" << endl;
	}
	catch (const exception& e) // Affichage des éventuelles erreurs remontées
	{
		cout << "Unhandled exception: " << e.what() << endl;
	}

	cout << endl;
	system("pause");
	return 0;
}

