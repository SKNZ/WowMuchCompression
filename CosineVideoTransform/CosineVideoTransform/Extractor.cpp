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
	
	CComponentFrame yVideoFrame, cbVideoFrame, crVideoFrame;

	CSerializableComponentFrame serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame;

	if (!videoReader.ReadFrame(serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame))
		throw runtime_error("Couldn't read first frame.");

	CRawVideoExporter rawVideoExporter(m_outputFile,
		videoReader.GetWidth(), videoReader.GetHeight(),
		videoReader.GetWidthPadding(), videoReader.GetHeightPadding());

	CDiscreteCosineTransform dct(true, true);

	CChromaSubsampler chromaResampler(true);

	CRunLengthEncoder runLengthDecoder(true);

	for (int i = 0; videoReader.ReadFrame(serializableYVideoFrame, serializableCbVideoFrame, serializableCrVideoFrame); ++i)
	{
		cout << i << ": processing frame." << endl;

		yVideoFrame.resize(videoReader.GetWidth(), videoReader.GetHeight());
		cbVideoFrame.resize(videoReader.GetWidth() / 2, videoReader.GetHeight() / 4);
		crVideoFrame.resize(videoReader.GetWidth() / 2, videoReader.GetHeight() / 4);

		cout << "\tRun length decoding on Y component... ";
		runLengthDecoder(yVideoFrame, serializableYVideoFrame);

		cout << "Done." << endl << "\tRun length decoding on Cb component... ";
		runLengthDecoder(cbVideoFrame, serializableCbVideoFrame);

		cout << "Done." << endl << "\tRun length decoding on Cr component... ";
		runLengthDecoder(crVideoFrame, serializableCrVideoFrame);

		cout << "Done." << endl << "\tApplying inverse discrete cosine transform on Y component... ";
		dct(yVideoFrame);

		cout << "Done." << endl << "\tApplying inverse discrete cosine transform on Cb component... ";
		dct(cbVideoFrame);

		cout << "Done." << endl << "\tApplying inverse discrete cosine transform on Cr component... ";
		dct(crVideoFrame);

		cout << "Done." << endl << "\tChroma resampling on Cb component... ";
		chromaResampler(cbVideoFrame);

		cout << "Done." << endl << "\tChroma resampling on Cr component... ";
		chromaResampler(crVideoFrame);

		cout << "Done." << endl << "\tSaving frame to file... ";
		rawVideoExporter.ExportFrame(yVideoFrame, cbVideoFrame, crVideoFrame);

		cout << "Done." << endl;

		cout << endl;
	}
}
