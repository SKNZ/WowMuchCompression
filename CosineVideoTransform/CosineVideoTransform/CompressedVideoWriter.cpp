#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CCompressedVideoWriter::CCompressedVideoWriter(const std::string& filePath, uint16_t width, uint16_t height)
	: m_filePath(filePath), m_fileStream(filePath, ios::out | ios::trunc | ios::binary), m_width(width), m_height(height), m_frameCount(0)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open output compressed file stream.");

	m_fileStream.write(reinterpret_cast<const char*>(&m_frameCount), sizeof(uint32_t));
	m_fileStream.write(reinterpret_cast<const char*>(&m_width), sizeof(uint16_t));
	m_fileStream.write(reinterpret_cast<const char*>(&m_height), sizeof(uint16_t));
}

void CCompressedVideoWriter::Finalize()
{
	m_fileStream.seekp(ios::beg);
	cout << "Frames written: " << m_frameCount << '.' << endl;
	m_fileStream.write(reinterpret_cast<const char*>(&m_frameCount), sizeof(uint32_t));
	m_fileStream.close();
}

void CCompressedVideoWriter::SaveFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame)
{
	m_fileStream.write(reinterpret_cast<const char*>(YVideoFrame.data()), YVideoFrame.rows() * YVideoFrame.cols() * sizeof(double));
	m_fileStream.write(reinterpret_cast<const char*>(CbVideoFrame.data()), CbVideoFrame.rows() * CbVideoFrame.cols() * sizeof(double));
	m_fileStream.write(reinterpret_cast<const char*>(CrVideoFrame.data()), CrVideoFrame.rows() * CrVideoFrame.cols() * sizeof(double));
	++m_frameCount;
}
