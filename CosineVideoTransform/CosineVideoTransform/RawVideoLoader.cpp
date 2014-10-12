#include "stdafx.h"

using namespace cv;
using namespace std;

nsWMC::CRawVideoLoader::CRawVideoLoader(const std::string& filePath) : m_filePath(filePath) // Ouverture du fichier vidéo original
{
	cout << "Video loader: opening file " << filePath << "." << endl;

	m_videoFile = VideoCapture(m_filePath);
	if (!m_videoFile.isOpened()) // Vérification d'ouverture du fichier vidéo original
		throw std::runtime_error("RawVideoLoader: input file could not be opened."); // Levée d'erreur si non ouvert

	m_width = int(m_videoFile.get(CV_CAP_PROP_FRAME_WIDTH));
	m_height = int(m_videoFile.get(CV_CAP_PROP_FRAME_HEIGHT));

	cout << "Video loader: file opened." << endl
		<< "\tInput video width: " << m_width << endl
		<< "\tInput video height: " << m_height << endl;

	cout << endl;
}

bool nsWMC::CRawVideoLoader::GetNextYCbCrFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame)
{
	static Mat videoFrame;
	if (m_videoFile.read(videoFrame)) // Si il reste encore une image à charger
	{
		YVideoFrame.resize(m_width, m_height);
		CbVideoFrame.resize(m_width, m_height);
		CrVideoFrame.resize(m_width, m_height);

		for (int k = 0; k < m_width; ++k)
		{
			for (int n = 0; n < m_height; ++n)
			{
				Vec3b rgbValues = videoFrame.at<Vec3b>(n, k);
				RGBToYCbCr(rgbValues[2], rgbValues[1], rgbValues[0], YVideoFrame(k, n), CbVideoFrame(k, n),	CrVideoFrame(k, n));
			}
		}

		return true;
	}
	return false;
}

int nsWMC::CRawVideoLoader::GetWidth() const
{
	return m_width;
}

int nsWMC::CRawVideoLoader::GetHeight() const
{
	return m_height;
}
