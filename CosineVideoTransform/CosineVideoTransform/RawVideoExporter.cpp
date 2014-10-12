#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace cv;

CRawVideoExporter::CRawVideoExporter(const std::string& filePath, int width, int height)
	: m_filePath(filePath), m_width(width), m_height(height)
{
	cout << "Video exporter: opening file " << m_filePath << endl;
	m_videoWriter = VideoWriter(m_filePath, 877677894, 30.0, Size(width, height));

	if (!m_videoWriter.isOpened())
		throw runtime_error("RawVideoExporter: output file couldn't be opened.");

	cout << "Video exporter: file opened." << endl;

	cout << endl;
}

void CRawVideoExporter::ExportFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame)
{
	Mat videoFrame(m_height, m_width, CV_8UC3);
	
	for (int k = 0; k < m_width; ++k)
	{
		for (int n = 0; n < m_height; ++n)
		{
			Vec3b rgbValues = videoFrame.at<Vec3b>(n, k);
			YCbCrToRGB(YVideoFrame(k, n), CbVideoFrame(k, n), CrVideoFrame(k, n), rgbValues[2], rgbValues[1], rgbValues[0]);
			videoFrame.at<Vec3b>(n, k) = rgbValues;
		}
	}

	m_videoWriter << videoFrame;
}
