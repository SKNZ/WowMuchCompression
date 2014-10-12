#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace Eigen;

CCompressedVideoReader::CCompressedVideoReader(const std::string& filePath)
	: m_filePath(filePath), m_fileStream(filePath, ios::in | ios::binary), m_framesRead(0)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open input compressed file stream.");

	m_fileStream >> m_frameCount >> m_width >> m_height;
}

bool CCompressedVideoReader::ReadFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame)
{
	if (m_framesRead >= m_frameCount)
		return false;

	m_fileStream.read(reinterpret_cast<char*>(YVideoFrame.data()), m_width * m_height * sizeof(double));
	m_fileStream.read(reinterpret_cast<char*>(CbVideoFrame.data()), m_width * m_height * sizeof(double));
	m_fileStream.read(reinterpret_cast<char*>(CrVideoFrame.data()), m_width * m_height * sizeof(double));
	++m_framesRead;
	return true;
}
