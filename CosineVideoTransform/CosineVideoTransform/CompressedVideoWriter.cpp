#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CCompressedVideoWriter::CCompressedVideoWriter(const std::string& filePath, uint16_t width, uint16_t height, uint8_t widthPadding, uint8_t heightPadding)
	: m_filePath(filePath), m_fileStream(filePath, ios::out | ios::trunc | ios::binary),
		m_width(width), m_height(height), m_frameCount(0),
		m_widthPadding(widthPadding), m_heightPadding(heightPadding)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open output compressed file stream.");

	m_fileStream.write(reinterpret_cast<const char*>(&m_frameCount), sizeof(uint32_t));
	m_fileStream.write(reinterpret_cast<const char*>(&m_width), sizeof(uint16_t));
	m_fileStream.write(reinterpret_cast<const char*>(&m_height), sizeof(uint16_t));
	m_fileStream.write(reinterpret_cast<const char*>(&m_widthPadding), sizeof(uint8_t));
	m_fileStream.write(reinterpret_cast<const char*>(&m_heightPadding), sizeof(uint8_t));
}

void CCompressedVideoWriter::Finalize()
{
	m_fileStream.seekp(ios::beg);
	cout << "Frames written: " << m_frameCount << '.' << endl;
	m_fileStream.write(reinterpret_cast<const char*>(&m_frameCount), sizeof(uint32_t));
	m_fileStream.close();
}

void CCompressedVideoWriter::SaveFrame(const CSerializableComponentFrame& YVideoFrame,
	const CSerializableComponentFrame& CbVideoFrame,
	const CSerializableComponentFrame& CrVideoFrame)
{
	// On écrit la taille de chaque frame après la RLE
	CSerializableComponentFrame::size_type size = YVideoFrame.size();
	m_fileStream.write(reinterpret_cast<const char*>(&size), sizeof(CSerializableComponentFrame::size_type));

	size = CbVideoFrame.size();
	m_fileStream.write(reinterpret_cast<const char*>(&size), sizeof(CSerializableComponentFrame::size_type));

	size = CrVideoFrame.size();
	m_fileStream.write(reinterpret_cast<const char*>(&size), sizeof(CSerializableComponentFrame::size_type));

	// On écrit la frame
	m_fileStream.write(reinterpret_cast<const char*>(&YVideoFrame[0]), YVideoFrame.size() * sizeof(CSerializedComponentType));
	m_fileStream.write(reinterpret_cast<const char*>(&CbVideoFrame[0]), CbVideoFrame.size() * sizeof(CSerializedComponentType));
	m_fileStream.write(reinterpret_cast<const char*>(&CrVideoFrame[0]), CrVideoFrame.size() * sizeof(CSerializedComponentType));

	++m_frameCount;
}
