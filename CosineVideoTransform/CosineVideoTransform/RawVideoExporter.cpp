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
	static Mat videoFrame(m_width, m_height, CV_8UC3);

	for (int k = 0; k < m_width; ++k)
	{
		for (int n = 0; k < m_height; ++n)
		{
			Point3_<uchar>* p = videoFrame.ptr<Point3_<uchar>>(n, k);
			YCbCrToRGB(YVideoFrame(k, n), CbVideoFrame(k, n), CrVideoFrame(k, n), p->z, p->y, p->x);
					//	Y					Cb					Cr					B	G		R
		}
	}
	m_videoWriter.write(videoFrame);
}
