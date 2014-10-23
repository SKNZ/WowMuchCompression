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

	CComponentFrame currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame,
		prevYVideoFrame, prevCbVideoFrame, prevCrVideoFrame;

	CComponentFrame matchesYVideoFrame, matchesCbVideoFrame, matchesCrVideoFrame;

	CSerializableComponentFrame serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame;
	
	if (!videoReader.ReadFrame(serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame,
					matchesYVideoFrame, matchesCbVideoFrame, matchesCrVideoFrame))
		throw runtime_error("Couldn't read first frame.");

	CRawVideoExporter rawVideoExporter(m_outputFile,
		videoReader.GetWidth(), videoReader.GetHeight(),
		videoReader.GetWidthPadding(), videoReader.GetHeightPadding());

	CDiscreteCosineTransform dct(true, true);

	CChromaSubsampler chromaResampler(true);

	CRunLengthEncoder runLengthDecoder(true);

	CBlockMatcher blockMatcher(true);

	int i = 0;

	do
	{
		cout << i << ": processing frame." << endl;

		currentYVideoFrame.resize(videoReader.GetWidth(), videoReader.GetHeight());
		currentCbVideoFrame.resize(videoReader.GetWidth() / 2, videoReader.GetHeight() / 4);
		currentCrVideoFrame.resize(videoReader.GetWidth() / 2, videoReader.GetHeight() / 4);
		
		matchesYVideoFrame.resize(currentYVideoFrame.rows() / 8, currentYVideoFrame.cols() / 8);
		matchesCbVideoFrame.resize(currentCbVideoFrame.rows() / 8, currentCbVideoFrame.cols() / 8);
		matchesCrVideoFrame.resize(currentCrVideoFrame.rows() / 8, currentCrVideoFrame.cols() / 8);

		cout << "\tRun length decoding on Y component... ";
		runLengthDecoder(serializableYVideoFrame);

		cout << "Done." << endl << "\tRun length decoding on Cb component... ";
		runLengthDecoder(serializableCbVideoFrame);

		cout << "Done." << endl << "\tRun length decoding on Cr component... ";
		runLengthDecoder(serializableCrVideoFrame);

		if (i != 0) // dont block match for the first frame
		{
			blockMatcher(prevYVideoFrame, currentYVideoFrame, serializableYVideoFrame, matchesYVideoFrame);
			blockMatcher(prevCbVideoFrame, currentCbVideoFrame, serializableCbVideoFrame, matchesCbVideoFrame);
			blockMatcher(prevCrVideoFrame, currentCrVideoFrame, serializableCrVideoFrame, matchesCrVideoFrame);
		}
		else
		{
			for (int k = 0; k < currentYVideoFrame.rows(); ++k)
				for (int n = 0; n < currentYVideoFrame.cols(); ++n)
				{
				currentYVideoFrame(k, n) = serializableYVideoFrame[k * currentYVideoFrame.cols() + n];
				}
			for (int k = 0; k < currentCbVideoFrame.rows(); ++k)
				for (int n = 0; n < currentCbVideoFrame.cols(); ++n)
					currentCbVideoFrame(k, n) = serializableCbVideoFrame[k * currentCbVideoFrame.cols() + n];

			for (int k = 0; k < currentCrVideoFrame.rows(); ++k)
				for (int n = 0; n < currentCrVideoFrame.cols(); ++n)
					currentCrVideoFrame(k, n) = serializableCrVideoFrame[k * currentCrVideoFrame.cols() + n];
		}

		cout << "Done." << endl << "\tApplying inverse discrete cosine transform on Y component... ";
		dct(currentYVideoFrame);

		cout << "Done." << endl << "\tApplying inverse discrete cosine transform on Cb component... ";
		dct(currentCbVideoFrame);

		cout << "Done." << endl << "\tApplying inverse discrete cosine transform on Cr component... ";
		dct(currentCrVideoFrame);

		cout << "Done." << endl << "\tChroma resampling on Cb component... ";
		chromaResampler(currentCbVideoFrame);

		cout << "Done." << endl << "\tChroma resampling on Cr component... ";
		chromaResampler(currentCrVideoFrame);

		cout << "Done." << endl << "\tSaving frame to file... ";
		rawVideoExporter.ExportFrame(currentYVideoFrame, currentCbVideoFrame, currentCrVideoFrame);

		cout << "Done." << endl;

		prevYVideoFrame = currentYVideoFrame;
		prevCbVideoFrame = currentCbVideoFrame;
		prevCrVideoFrame = currentCrVideoFrame;

		cout << endl;
		++i;
	} while (videoReader.ReadFrame(serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame, matchesYVideoFrame, matchesCbVideoFrame, matchesCrVideoFrame));
}
