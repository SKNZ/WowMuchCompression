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
	const CSerializableComponentFrame& CrVideoFrame,
	const CComponentFrame& matchesYVideoFrame,
	const CComponentFrame& matchesCbVideoFrame,
	const CComponentFrame& matchesCrVideoFrame)
{
	// On écrit la taille de chaque frame après la RLE et le blockmatching
	CSerializableComponentFrame::size_type size = YVideoFrame.size();
	m_fileStream.write(reinterpret_cast<const char*>(&size), sizeof(CSerializableComponentFrame::size_type));

	size = CbVideoFrame.size();
	m_fileStream.write(reinterpret_cast<const char*>(&size), sizeof(CSerializableComponentFrame::size_type));

	size = CrVideoFrame.size();
	m_fileStream.write(reinterpret_cast<const char*>(&size), sizeof(CSerializableComponentFrame::size_type));
	
	uint16_t bufferSize = 0;
	// On écrit les matchs
	vector<uint8_t> buffer(1);
	buffer[0] = 0;
	int bitPosition = 0;
	for (int k = 0; k < matchesYVideoFrame.rows(); ++k)
	{
		for (int n = 0; n < matchesYVideoFrame.cols(); ++n)
		{
			if (matchesYVideoFrame(k, n))
				buffer[buffer.size() - 1] |= 1 << bitPosition;
			++bitPosition;

			if (bitPosition > 7)
			{
				buffer.push_back(0);
				bitPosition = 0;
			}
		}
	}
	bufferSize = buffer.size();
	m_fileStream.write(reinterpret_cast<const char*>(&bufferSize), sizeof(uint16_t));
	if (bufferSize)
		m_fileStream.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));

	buffer.resize(1);
	buffer[0] = 0;
	bitPosition = 0;
	for (int k = 0; k < matchesCbVideoFrame.rows(); ++k)
	{
		for (int n = 0; n < matchesCbVideoFrame.cols(); ++n)
		{
			if (matchesCbVideoFrame(k, n))
				buffer[buffer.size() - 1] |= 1 << bitPosition;
			++bitPosition;

			if (bitPosition > 7)
			{
				buffer.push_back(0);
				bitPosition = 0;
			}
		}
	}
	bufferSize = buffer.size();
	m_fileStream.write(reinterpret_cast<const char*>(&bufferSize), sizeof(uint16_t));
	if (bufferSize)
		m_fileStream.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));

	buffer.resize(1);
	buffer[0] = 0;
	bitPosition = 0;
	for (int k = 0; k < matchesCrVideoFrame.rows(); ++k)
	{
		for (int n = 0; n < matchesCrVideoFrame.cols(); ++n)
		{
			if (matchesCrVideoFrame(k, n))
				buffer[buffer.size() - 1] |= 1 << bitPosition;
			++bitPosition;

			if (bitPosition > 7)
			{
				buffer.push_back(0);
				bitPosition = 0;
			}
		}
	}
	bufferSize = buffer.size();
	m_fileStream.write(reinterpret_cast<const char*>(&bufferSize), sizeof(uint16_t));
	if (bufferSize)
		m_fileStream.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));

	// On écrit la frame
	if (YVideoFrame.size())
		m_fileStream.write(reinterpret_cast<const char*>(&YVideoFrame[0]), YVideoFrame.size() * sizeof(CSerializedComponentType));

	if (CbVideoFrame.size())
		m_fileStream.write(reinterpret_cast<const char*>(&CbVideoFrame[0]), CbVideoFrame.size() * sizeof(CSerializedComponentType));

	if (CrVideoFrame.size())
		m_fileStream.write(reinterpret_cast<const char*>(&CrVideoFrame[0]), CrVideoFrame.size() * sizeof(CSerializedComponentType));

	++m_frameCount;
}
