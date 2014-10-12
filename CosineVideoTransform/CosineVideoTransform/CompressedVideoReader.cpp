#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace Eigen;

CCompressedVideoReader::CCompressedVideoReader(const std::string& filePath)
	: m_filePath(filePath), m_fileStream(filePath, ios::in | ios::binary), m_framesRead(0), m_frameCount(0), m_width(0), m_height(0)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open input compressed file stream.");

	m_fileStream.read(reinterpret_cast<char*>(&m_frameCount), sizeof(uint32_t));
	m_fileStream.read(reinterpret_cast<char*>(&m_width), sizeof(uint16_t));
	m_fileStream.read(reinterpret_cast<char*>(&m_height), sizeof(uint16_t));
}

bool CCompressedVideoReader::ReadFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame)
{
	if (m_framesRead >= m_frameCount)
		return false;

	YVideoFrame.resize(m_width, m_height);
	CbVideoFrame.resize(m_width, m_height);
	CrVideoFrame.resize(m_width, m_height);

	m_fileStream.read(reinterpret_cast<char*>(YVideoFrame.data()), YVideoFrame.rows() * YVideoFrame.cols() * sizeof(double));
	m_fileStream.read(reinterpret_cast<char*>(CbVideoFrame.data()), CbVideoFrame.rows() * CbVideoFrame.cols() * sizeof(double));
	m_fileStream.read(reinterpret_cast<char*>(CrVideoFrame.data()), CrVideoFrame.rows() * CrVideoFrame.cols() * sizeof(double));

	++m_framesRead;
	return true;
}
