#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CCompressor::CCompressor(const std::string& inputFilePath, const std::string& outputFilePath)
	: m_inputFilePath(inputFilePath), m_outputFilePath(outputFilePath)
{

}

void CCompressor::run() const
{
	/**
	  * CRawVideoLoader est une classe nous permettant d'importer des fichiers vidéos.
	  * Elle se base sur OpenCV, une bibliothèque qui permet entre autre la manipulation de fichiers vidéos (image uniquement).
	  * Ici, la classe va tenter d'ouvrir le fichier original.avi. Si une erreur se produit (droits d'accès insuffisants, fichier inexistant...)
	  * une exception (erreur) sera levée.
	  *
	  **/
	CRawVideoLoader rawVideoLoader(m_inputFilePath);

	/**
	  * La classe CComponentFrame est une classe qui permet de stocker toutes les valeurs d'une composante pour une image.
	  * Par exemple, toutes les valeurs de Y pour une image I. Elle fait donc office de matrice pour une composante image entière.
	  *
	  * Il y a 3 composantes - Y, Cb & Cr. On déclare donc 3 CComponentFrame par image.
	  *
	  * On stocke l'image courrante ainsi que l'image suivante - on déclare donc 6 CComponentFrame.
	  *
	  **/
	CComponentFrame currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame,
		nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame;

	/**
	  * Les frames (images) du fichier vidéo sont importées avec comme format de couleur RGB (Red Green Blue).
	  * Pour servir nos objectifs, nous convertissons le RGB en YCbCr (Y étant la luminance, Cb et Cr étant des valeurs de chrominance).
	  * La fonction GetYCbCrFrames va remplir nos vecteurs de CComponentFrame.
	  * Pour cela, elle va charger une frame depuis le fichier, convertir chaque pixel de RGB vers YCbCr, et les stocker en mémoire.
	  * On charge les deux premières frames du fichier.
	  *
	  **/
	if (!rawVideoLoader.GetNextYCbCrFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame))
		throw runtime_error("Couldn't load first frame.");

	if (!rawVideoLoader.GetNextYCbCrFrame(nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame))
		throw runtime_error("Couldn't load second frame.");

	/**
	  * CCompressedVideoWriter est une classe permettant d'exporter vers un fichier les données une fois compréssée.
	  * Les données sont stockées dans un format propre au codec WMC.
	  * Un fichier créer par CCompressedVideoWriter doit donc être lu avec CCompressedVideoReader.
	  * 
	  **/
	CCompressedVideoWriter compressedVideoWriter(m_outputFilePath, rawVideoLoader.GetWidth(), rawVideoLoader.GetHeight());

	/**
	  * CDiscreteCosineTransform est une classe qui applique la transformée en cosinus discrète sur un CComponentFrame.
	  * Elle accepte un premier paramètre booléen, indiquant si la dct doit être inverse (iDCT).
	  * Elle accepte comme deuxième paramètre paramètre un booléen, indiquant si une quantification doit avoir lieu.
	  * Elle accepte un troisème paramètre entier, indiquant la qualité en pourcentage après la quantification
	  * par rapport à l'image originale.
	  *
	  **/
	CDiscreteCosineTransform dct(false, true);

	for (int i = 0;; ++i)
	{
		cout << i << ": processing frame." << endl;

		cout << "\tApplying discrete cosine transform on Y component... ";
		dct(currentYVideoFrame);

		cout << "Done." << endl << "\tApplying discrete cosine transform on Cb component... ";
		dct(currentCbVideoFrame);

		cout << "Done." << endl << "\tApplying discrete cosine transform on Cr component... ";
		dct(currentCrVideoFrame);

		cout << "Done." << endl << "\tSaving frame to file... ";
		compressedVideoWriter.SaveFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame);
		
		cout << "Done." << endl;

		currentYVideoFrame = nextYVideoFrame;
		currentCbVideoFrame = nextCbVideoFrame;
		currentCrVideoFrame = nextCrVideoFrame;

		cout << endl;
		if (!rawVideoLoader.GetNextYCbCrFrame(nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame))
		{
			cout << i + 1 << ": processing frame." << endl;

			cout << "\tApplying discrete cosine transform on Y component... ";
			dct(currentYVideoFrame);

			cout << "Done." << endl << "\tApplying discrete cosine transform on Cb component... ";
			dct(currentCbVideoFrame);

			cout << "Done." << endl << "\tApplying discrete cosine transform on Cr component... ";
			dct(currentCrVideoFrame);

			cout << "Done." << endl << "\tSaving frame to file... ";
			compressedVideoWriter.SaveFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame);

			cout << "Done." << endl;
			break;
		}
	}

	cout << "There are no more frames to be read." << endl;

	/**
	  * On cloture le fichier en inscrivant au début le nombre de frames écrites
	  * et en libérant les ressources systèmes allouées.
	  *
	  **/
	compressedVideoWriter.Finalize();
}
