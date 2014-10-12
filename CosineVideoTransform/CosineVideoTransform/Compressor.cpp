#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CCompressor::CCompressor(const std::string& inputFilePath, const std::string& outputFilePath)
	: m_inputFilePath(inputFilePath), m_outputFilePath(outputFilePath)
{

}

void CCompressor::run()
{
	/**
	* CVideoLoader est une classe nous permettant d'importer des fichiers vid�os.
	* Elle se base sur OpenCV, une biblioth�que qui permet entre autre la manipulation de fichiers vid�os (image uniquement).
	* Ici, la classe va tenter d'ouvrir le fichier original.avi. Si une erreur se produit (droits d'acc�s insuffisants, fichier inexistant...)
	* une exception (erreur) sera lev�e.
	*
	**/
	CVideoLoader videoLoader(m_inputFilePath);

	/**
	* La classe CComponentFrame est une classe qui permet de stocker toutes les valeurs d'une composante pour une image.
	* Par exemple, toutes les valeurs de Y pour une image I. Elle fait donc office de matrice pour une composante image enti�re.
	*
	* Il y a 3 composantes - Y, Cb & Cr. On d�clare donc 3 CComponentFrame par image.
	*
	* On stocke l'image courrante ainsi que l'image suivante - on d�clare donc 6 CComponentFrame.
	*
	**/
	CComponentFrame currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame,
		nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame;

	/**
	* Les frames (images) du fichier vid�o sont import�es avec comme format de couleur RGB (Red Green Blue).
	* Pour servir nos objectifs, nous convertissons le RGB en YCbCr (Y �tant la luminance, Cb et Cr �tant des valeurs de chrominance).
	* La fonction GetYCbCrFrames va remplir nos vecteurs de CComponentFrame.
	* Pour cela, elle va charger une frame depuis le fichier, convertir chaque pixel de RGB vers YCbCr, et les stocker en m�moire.
	* On charge les deux premi�res frames du fichier.
	*
	**/
	if (!videoLoader.GetNextYCbCrFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame))
		throw runtime_error("Couldn't load first frame.");

	if (!videoLoader.GetNextYCbCrFrame(nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame))
		throw runtime_error("Couldn't load second frame.");

	/**
	  * CCompressedVideoWriter est une classe permettant d'exporter vers un fichier les donn�es une fois compr�ss�e.
	  * Les donn�es sont stock�es dans un format propre au codec WMC.
	  * Un fichier cr�er par CCompressedVideoWriter doit donc �tre lu avec CCompressedVideoLoader.
	  * 
	  **/
	CCompressedVideoWriter compressedVideoWriter(m_outputFilePath, videoLoader.GetWidth(), videoLoader.GetHeight());

	/**
	  * CDiscreteCosineTransform est une classe qui applique la transform�e en cosinus discr�te sur un CComponentFrame.
	  * Elle accepte comme param�tres un bool�en, indiquant si une quantification doit avoir lieu.
	  * Elle accepte un deuxi�me param�tre entier, indiquant la qualit� en pourcentage apr�s la quantification
	  * par rapport � l'image originale.
	  *
	  **/
	CDiscreteCosineTransform dct(true);

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

		if (!videoLoader.GetNextYCbCrFrame(nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame))
		{
			cout << "There are no more frames to be read." << endl;
			break;
		}
	}
}
