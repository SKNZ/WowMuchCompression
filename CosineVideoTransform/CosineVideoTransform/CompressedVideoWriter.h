#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CCompressedVideoWriter
	{
	public:
		CCompressedVideoWriter(const std::string& filePath, uint16_t width, uint16_t height, uint8_t widthPadding, uint8_t heightPadding);
		void SaveFrame(const CSerializableComponentFrame& YVideoFrame,
			const CSerializableComponentFrame& CbVideoFrame,
			const CSerializableComponentFrame& CrVideoFrame,
			const CComponentFrame& matchesYVideoFrame,
			const CComponentFrame& matchesCbVideoFrame,
			const CComponentFrame& matchesCrVideoFrame);
		void Finalize();

	private:
		uint32_t m_frameCount;
		std::string m_filePath;
		std::ofstream m_fileStream;
		uint16_t m_width, m_height;
		uint8_t m_widthPadding, m_heightPadding;
	};
}