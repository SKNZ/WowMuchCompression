#include "stdafx.h"

using namespace cv;
using namespace std;

nsWMC::CVideoLoader::CVideoLoader(const std::string& filePath) : m_filePath(filePath) // Ouverture du fichier vidéo original
{
	cout << "Video loader: opening file " << filePath << "." << endl;
	m_videoFile = VideoCapture(m_filePath);
	if (!m_videoFile.isOpened()) // Vérification d'ouverture du fichier vidéo original
		throw std::runtime_error("Original file could not be opened."); // Levée d'erreur si non ouvert

	m_width = int(m_videoFile.get(CV_CAP_PROP_FRAME_WIDTH));
	m_height = int(m_videoFile.get(CV_CAP_PROP_FRAME_HEIGHT));
	cout << "Video loader: file opened." << endl
		<< "\tInput video width: " << m_width << endl
		<< "\tInput video height: " << m_height << endl;

	cout << endl;
}

nsWMC::CVideoLoader::~CVideoLoader()
{
	m_videoFile.release(); // Désallocation des ressources systèmes utilisées pour l'ouverture du fichier.
}

bool nsWMC::CVideoLoader::GetNextYCbCrFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame)
{
	static Mat videoFrame;
	if (m_videoFile.read(videoFrame)) // Si il reste encore une image à charger
	{
		YVideoFrame.resize(m_width, m_height);
		CbVideoFrame.resize(m_width, m_height);
		CrVideoFrame.resize(m_width, m_height);

		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				Point3_<uchar>* p = videoFrame.ptr<Point3_<uchar>>(y, x);
				RGBToYCbCr(p->z, p->y, p->x, YVideoFrame(x, y), CbVideoFrame(x, y),	CrVideoFrame(x, y));
						  // B,    G,   R
			}
		}

		return true;
	}
	return false;
}

int nsWMC::CVideoLoader::GetWidth() const
{
	return m_width;
}

int nsWMC::CVideoLoader::GetHeight() const
{
	return m_height;
}
