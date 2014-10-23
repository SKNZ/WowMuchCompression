#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CCompressedVideoReader
	{
	public:
		CCompressedVideoReader(const std::string& filePath);
		bool ReadFrame(CSerializableComponentFrame& YVideoFrame,
			CSerializableComponentFrame& CbVideoFrame,
			CSerializableComponentFrame& CrVideoFrame,
			CComponentFrame& matchesYVideoFrame,
			CComponentFrame& matchesCbVideoFrame,
			CComponentFrame& matchesCrVideoFrame);

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		int GetWidthPadding() const { return m_widthPadding; }
		int GetHeightPadding() const { return m_heightPadding; }

	private:
		uint32_t m_frameCount, m_framesRead, m_YSize, m_CbSize, m_CrSize;
		uint16_t m_width, m_height;
		std::string m_filePath;
		std::ifstream m_fileStream;
		uint8_t m_widthPadding, m_heightPadding;
	};
}
