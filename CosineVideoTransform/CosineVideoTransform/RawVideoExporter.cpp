#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace cv;

CRawVideoExporter::CRawVideoExporter(const std::string& filePath, int width, int height, int widthPadding, int heightPadding)
	: m_filePath(filePath), m_width(width), m_height(height), m_widthPadding(widthPadding), m_heightPadding(heightPadding)
{
	cout << "Video exporter: opening file " << m_filePath << endl;
	m_videoWriter = VideoWriter(m_filePath, CV_FOURCC('D', 'I', 'B', ' ')/*877677894*/, 30.0, Size(width - PIXEL_BLOCK_SIZE + widthPadding, height - PIXEL_BLOCK_SIZE + heightPadding));

	if (!m_videoWriter.isOpened())
		throw runtime_error("RawVideoExporter: output file couldn't be opened.");

	cout << "Video exporter: file opened." << endl;

	cout << endl;
}

void CRawVideoExporter::ExportFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame)
{
	Mat videoFrame(m_height - PIXEL_BLOCK_SIZE + m_heightPadding, m_width - PIXEL_BLOCK_SIZE + m_widthPadding, CV_8UC3);
	
	for (int k = 0; k < videoFrame.cols; ++k)
	{
		for (int n = 0; n < videoFrame.rows; ++n)
		{
			Vec3b rgbValues = videoFrame.at<Vec3b>(n, k);
			YCbCrToRGB(YVideoFrame(k, n), CbVideoFrame(k, n), CrVideoFrame(k, n), rgbValues[2], rgbValues[1], rgbValues[0]);
			videoFrame.at<Vec3b>(n, k) = rgbValues;
		}
	}

	m_videoWriter << videoFrame;
}
