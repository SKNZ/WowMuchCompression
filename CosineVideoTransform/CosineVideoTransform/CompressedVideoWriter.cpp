#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CCompressedVideoWriter::CCompressedVideoWriter(const std::string& filePath, uint16_t width, uint16_t height)
	: m_filePath(filePath), m_fileStream(filePath, ios::out | ios::trunc | ios::binary), m_width(width), m_height(height), m_frameCount(0)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open output compressed file stream.");

	m_fileStream << (uint32_t)0u << m_width << m_height;
	// On met 0 à l'endroit ou le nombre de frame sera mis.
}

void CCompressedVideoWriter::Finalize()
{
	m_fileStream.seekp(ios::beg);
	m_fileStream << m_frameCount;
	m_fileStream.close();
}

void CCompressedVideoWriter::SaveFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame)
{
	m_fileStream.write(reinterpret_cast<const char*>(YVideoFrame.data()), YVideoFrame.count() * sizeof(double));
	m_fileStream.write(reinterpret_cast<const char*>(CbVideoFrame.data()), CbVideoFrame.count() * sizeof(double));
	m_fileStream.write(reinterpret_cast<const char*>(CrVideoFrame.data()), CrVideoFrame.count() * sizeof(double));
	++m_frameCount;
}
