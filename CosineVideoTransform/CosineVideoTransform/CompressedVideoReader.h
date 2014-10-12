#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CCompressedVideoReader
	{
	public:
		CCompressedVideoReader(const std::string& filePath);
		bool ReadFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame);

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

	private:
		uint32_t m_frameCount, m_framesRead;
		uint16_t m_width, m_height;
		std::string m_filePath;
		std::ifstream m_fileStream;
	};
}
