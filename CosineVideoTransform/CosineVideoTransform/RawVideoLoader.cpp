#include "stdafx.h"

using namespace nsWMC;
using namespace cv;
using namespace std;

CRawVideoLoader::CRawVideoLoader(const std::string& filePath) : m_filePath(filePath) // Ouverture du fichier vid�o original
{
	cout << "Video loader: opening file " << filePath << "." << endl;

	m_videoFile = VideoCapture(m_filePath);
	if (!m_videoFile.isOpened()) // V�rification d'ouverture du fichier vid�o original
		throw std::runtime_error("RawVideoLoader: input file could not be opened."); // Lev�e d'erreur si non ouvert

	// R�cuperation de la taille et de l'image du fichier d'origine
	m_width = int(m_videoFile.get(CV_CAP_PROP_FRAME_WIDTH)); 
	m_height = int(m_videoFile.get(CV_CAP_PROP_FRAME_HEIGHT));

	if (m_width > VIDEO_MAX_WIDTH || m_height > VIDEO_MAX_HEIGHT)
		throw runtime_error("Max supported resolution is 1366x768.");

	cout << "Video loader: file opened." << endl
		<< "\tInput video width: " << m_width << endl
		<< "\tInput video height: " << m_height << endl;

	/**
	  * La DCT d�coupe l'image en blocs de taille PIXEL_BLOCK_SIZE (constante qui vaut pr�sent�ment 8).
	  * Il faut donc que les dimensions de l'image soient des multiples de 8.
	  * On calcule le padding: dimension modulo PIXEL_BLOCK_SIZE.
	  * On ajoute aux dimensions la taille du bloc moins le padding.
	  *
	  **/
	if (m_widthPadding = (m_width % PIXEL_BLOCK_SIZE))
		m_width +=  PIXEL_BLOCK_SIZE - m_widthPadding;

	if (m_heightPadding = (m_height % PIXEL_BLOCK_SIZE))
		m_height += PIXEL_BLOCK_SIZE - m_heightPadding;

	cout << endl;
}

bool CRawVideoLoader::GetNextYCbCrFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame)
{
	/**
	  * Cette fonction utilise la biblioth�que OpenCV.
	  * OpenCV est une librairie permettant de lire la grande majorit� des formats vid�os existants.
	  * En l'occurence, elle nous permet d'ouvrir un fichier vid�o et de charger les images
	  * une � une afin d'y appliquer les traitements que nous souhaitons.
	  * M�me si OpenCV permet de lire de nombreux format de vid�o, on pr�f�rera une vid�o
	  * n'ayant jamais �t� compress�e afin de ne pas cumuler les pertes d'information.
	  **/
	static Mat videoFrame;

	// Vrai si le chargement � r�ussi, faux si il y a n'a plus d'images � charger
	if (m_videoFile.read(videoFrame))
	{
		// Pr�paration des frames en les dimensionnant correctement.
		YVideoFrame.resize(m_width, m_height);
		CbVideoFrame.resize(m_width, m_height);
		CrVideoFrame.resize(m_width, m_height);

		for (int k = 0; k < videoFrame.cols; ++k)
		{
			for (int n = 0; n < videoFrame.rows; ++n)
			{
				Vec3b rgbValues = videoFrame.at<Vec3b>(n, k); // Lecture du pixel � la position k, n
				// Conversion et stockage dans la frame du pixel converti en YCbCr.
				RGBToYCbCr(rgbValues[2], rgbValues[1], rgbValues[0], YVideoFrame(k, n), CbVideoFrame(k, n),	CrVideoFrame(k, n));
			}
		}

		return true;
	}
	return false;
}

int CRawVideoLoader::GetWidth() const
{
	return m_width;
}

int CRawVideoLoader::GetHeight() const
{
	return m_height;
}

int CRawVideoLoader::GetWidthPadding() const
{
	return m_widthPadding;
}

int CRawVideoLoader::GetHeightPadding() const
{
	return m_heightPadding;
}
