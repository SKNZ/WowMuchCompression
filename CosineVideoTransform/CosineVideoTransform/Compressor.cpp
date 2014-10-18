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
	  **/
	CComponentFrame currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame;

	/**
	  * La classe CSerializableComponentFrame représente les mêmes données que CComponentFrame
	  * mais sous la forme d'une suite de données continue, plus simple à stocker dans des fichiers.
	  *
	  **/
	CSerializableComponentFrame serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame;

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

	/**
	  * CCompressedVideoWriter est une classe permettant d'exporter vers un fichier les données une fois compréssée.
	  * Les données sont stockées dans un format propre au codec WMC.
	  * Un fichier créer par CCompressedVideoWriter doit donc être lu avec CCompressedVideoReader.
	  * 
	  **/
	CCompressedVideoWriter compressedVideoWriter(m_outputFilePath,
						rawVideoLoader.GetWidth(), rawVideoLoader.GetHeight(),
						rawVideoLoader.GetWidthPadding(), rawVideoLoader.GetHeightPadding());

	/**
	  * CDiscreteCosineTransform est une classe qui applique la transformée en cosinus discrète sur un CComponentFrame.
	  * Elle accepte un premier paramètre booléen, indiquant si la dct doit être inverse (iDCT).
	  * Elle accepte comme deuxième paramètre paramètre un booléen, indiquant si une quantification doit avoir lieu.
	  * Elle accepte un troisème paramètre entier, indiquant la qualité en pourcentage après la quantification
	  * par rapport à l'image originale.
	  *
	  **/
	CDiscreteCosineTransform dct(false, true);

	/**
	  * CChromaSubsampler est une classe qui sous échantillone une composante.
	  * A terme, elle permet de ne stocker, pour 4 composantes Y, une seule composante Cb et une seule composante Cr.
	  * C'est une opération "avec perte".
	  *
	  **/
	CChromaSubsampler chromaSubsampler;

	/**
	  * CRunLengthEncoder une classe permettant de "factoriser" les suites de zéros consécutifs.
	  * Sur une matrice DCT quantifiée, plus on est loin de (0,0) (hautes fréquences), plus les valeurs sont petites.
	  * Les valeurs proches de 0 sont arrondies à 0 lors de la quantification.
	  * Le RunLengthEncoder permet alors de factoriser une suite de zéros dans la matrice en ne gardant qu'un seul zéro
	  * suivi du nombre de zéro qu'il y avait originalement.
	  *
	  **/
	CRunLengthEncoder runLengthEncoder;
	
	// Pour chaque frame
	for (int i = 0; rawVideoLoader.GetNextYCbCrFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame); ++i)
	{
		cout << i << ": processing frame." << endl;

		cout << "\tChroma subsampling on Cb component... ";
		chromaSubsampler(currentCbVideoFrame);

		cout << "Done." << endl << "\tChroma subsampling on Cr component... ";
		chromaSubsampler(currentCrVideoFrame);

		cout << "Done." << endl << "\tApplying discrete cosine transform on Y component... ";
		dct(currentYVideoFrame);

		cout << "Done." << endl << "\tApplying discrete cosine transform on Cb component... ";
		dct(currentCbVideoFrame);

		cout << "Done." << endl << "\tApplying discrete cosine transform on Cr component... ";
		dct(currentCrVideoFrame);

		cout << "Done." << endl << "\tRun length encoding on Y component... ";
		runLengthEncoder(currentYVideoFrame, serializableYVideoFrame);

		cout << "Done." << endl << "\tRun length encoding on Cb component... ";
		runLengthEncoder(currentCbVideoFrame, serializableCbVideoFrame);

		cout << "Done." << endl << "\tRun length encoding on Cr component... ";
		runLengthEncoder(currentCrVideoFrame, serializableCrVideoFrame);

		cout << "Done." << endl << "\tSaving frame to file... ";
		compressedVideoWriter.SaveFrame(serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame);
		
		cout << "Done." << endl;

		cout << endl;
	}

	cout << "There are no more frames to be read." << endl;

	/**
	  * On cloture le fichier en inscrivant au début le nombre de frames écrites
	  * et en libérant les ressources systèmes allouées.
	  *
	  **/
	compressedVideoWriter.Finalize();
}
