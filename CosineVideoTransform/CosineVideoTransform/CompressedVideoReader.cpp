#include "stdafx.h"

using namespace nsWMC;
using namespace std;
using namespace Eigen;

CCompressedVideoReader::CCompressedVideoReader(const std::string& filePath)
	: m_filePath(filePath), m_fileStream(filePath, ios::in | ios::binary),
	m_framesRead(0), m_frameCount(0), m_width(0), m_height(0),
	m_widthPadding(0), m_heightPadding(0)
{
	if (!m_fileStream)
		throw runtime_error("Failed to open input compressed file stream.");

	m_fileStream.read(reinterpret_cast<char*>(&m_frameCount), sizeof(uint32_t));
	m_fileStream.read(reinterpret_cast<char*>(&m_width), sizeof(uint16_t));
	m_fileStream.read(reinterpret_cast<char*>(&m_height), sizeof(uint16_t));
	m_fileStream.read(reinterpret_cast<char*>(&m_widthPadding), sizeof(uint8_t));
	m_fileStream.read(reinterpret_cast<char*>(&m_heightPadding), sizeof(uint8_t));
}

bool CCompressedVideoReader::ReadFrame(CSerializableComponentFrame& YVideoFrame,
	CSerializableComponentFrame& CbVideoFrame,
	CSerializableComponentFrame& CrVideoFrame,
	CComponentFrame& matchesYVideoFrame,
	CComponentFrame& matchesCbVideoFrame,
	CComponentFrame& matchesCrVideoFrame)
{
	if (m_framesRead >= m_frameCount)
		return false;

	CSerializableComponentFrame::size_type size;
	m_fileStream.read(reinterpret_cast<char*>(&size), sizeof(CSerializableComponentFrame::size_type));
	YVideoFrame.resize(size);

	m_fileStream.read(reinterpret_cast<char*>(&size), sizeof(CSerializableComponentFrame::size_type));
	CbVideoFrame.resize(size);

	m_fileStream.read(reinterpret_cast<char*>(&size), sizeof(CSerializableComponentFrame::size_type));
	CrVideoFrame.resize(size);

	uint16_t bufferSize;
	vector<uint8_t> buffer;
	m_fileStream.read(reinterpret_cast<char*>(&bufferSize), sizeof(uint16_t));
	int position = 0;
	int count = 0;
	if (bufferSize)
	{
		buffer.resize(bufferSize);
		m_fileStream.read(reinterpret_cast<char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));
		for (int k = 0; k < matchesYVideoFrame.rows(); ++k)
		{
			for (int n = 0; n < matchesYVideoFrame.cols(); ++n)
			{
				matchesYVideoFrame(k, n) = buffer[count] & (1 << position++);

				if (position > 7)
				{
					count++;
					position = 0;
				}
			}
		}
	}
	m_fileStream.read(reinterpret_cast<char*>(&bufferSize), sizeof(uint16_t));
	buffer.resize(bufferSize);
	if (bufferSize)
	{
		m_fileStream.read(reinterpret_cast<char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));
		position = 0;
		count = 0;
		for (int k = 0; k < matchesCbVideoFrame.rows(); ++k)
		{
			for (int n = 0; n < matchesCbVideoFrame.cols(); ++n)
			{
				matchesCbVideoFrame(k, n) = buffer[count] & (1 << position++);

				if (position > 7)
				{
					count++;
					position = 0;
				}
			}
		}
	}

	m_fileStream.read(reinterpret_cast<char*>(&bufferSize), sizeof(uint16_t));
	buffer.resize(bufferSize);
	if (bufferSize)
	{
		m_fileStream.read(reinterpret_cast<char*>(&buffer[0]), buffer.size() * sizeof(uint8_t));
		position = 0;
		count = 0;
		for (int k = 0; k < matchesCrVideoFrame.rows(); ++k)
		{
			for (int n = 0; n < matchesCrVideoFrame.cols(); ++n)
			{
				matchesCrVideoFrame(k, n) = buffer[count] & (1 << position++);

				if (position > 7)
				{
					count++;
					position = 0;
				}
			}
		}
	}

	if (YVideoFrame.size())
		m_fileStream.read(reinterpret_cast<char*>(&YVideoFrame[0]), YVideoFrame.size() * sizeof(CSerializedComponentType));

	if (CbVideoFrame.size())
		m_fileStream.read(reinterpret_cast<char*>(&CbVideoFrame[0]), CbVideoFrame.size() * sizeof(CSerializedComponentType));

	if (CrVideoFrame.size())
		m_fileStream.read(reinterpret_cast<char*>(&CrVideoFrame[0]), CrVideoFrame.size() * sizeof(CSerializedComponentType));
	++m_framesRead;
	return true;
}
