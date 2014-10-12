#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CCompressedVideoWriter::CCompressedVideoWriter(const std::string& filePath, uint16_t width, uint16_t height)
	: m_filePath(filePath), m_fileStream(filePath, ios::out | ios::trunc | ios::binary), m_width(width), m_height(height)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open output compressed file stream.");

	m_fileStream << "WMCFile" << m_width << m_height;
}

CCompressedVideoWriter::~CCompressedVideoWriter()
{
	m_fileStream.close();
}

void CCompressedVideoWriter::SaveFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame)
{
	m_fileStream.write(reinterpret_cast<const char*>(YVideoFrame.data()), YVideoFrame.count() * sizeof(double));
	m_fileStream.write(reinterpret_cast<const char*>(CbVideoFrame.data()), CbVideoFrame.count() * sizeof(double));
	m_fileStream.write(reinterpret_cast<const char*>(CrVideoFrame.data()), CrVideoFrame.count() * sizeof(double));
}
