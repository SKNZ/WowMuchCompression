#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CExtractor::CExtractor(const std::string& inputFile, const std::string& outputFile)
	: m_inputFile(inputFile), m_outputFile(outputFile)
{

}

void CExtractor::run() const
{
	/**
	  * CCompressedVideoReader est une classe lisant les fichiers WMC créer par la classe CCompressedVideoWriter.
	  * Elle se charge d'ouvrir le fichier, et nous permet ensuite de lire toutes les frames du fichier une par une.
	  *
	  **/
	CCompressedVideoReader videoReader(m_inputFile);
	
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

	if (!videoReader.ReadFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame))
		throw runtime_error("Couldn't read first frame.");

	if (!videoReader.ReadFrame(nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame))
		throw runtime_error("Couldn't read second frame.");

	CRawVideoExporter rawVideoExporter(m_outputFile, videoReader.GetWidth(), videoReader.GetHeight());

	CDiscreteCosineTransform dct(true, true);

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
		rawVideoExporter.ExportFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame);

		cout << "Done." << endl;

		currentYVideoFrame = nextYVideoFrame;
		currentCbVideoFrame = nextCbVideoFrame;
		currentCrVideoFrame = nextCrVideoFrame;

		cout << endl;

		if (!videoReader.ReadFrame(nextYVideoFrame, nextCbVideoFrame, nextCrVideoFrame))
		{
			cout << i + 1 << ": processing frame." << endl;

			cout << "\tApplying discrete cosine transform on Y component... ";
			dct(currentYVideoFrame);

			cout << "Done." << endl << "\tApplying discrete cosine transform on Cb component... ";
			dct(currentCbVideoFrame);

			cout << "Done." << endl << "\tApplying discrete cosine transform on Cr component... ";
			dct(currentCrVideoFrame);

			cout << "Done." << endl << "\tSaving frame to file... ";
			rawVideoExporter.ExportFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame);

			cout << "Done." << endl;
			break;
		}
	}
}
