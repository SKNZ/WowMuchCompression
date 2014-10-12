#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CCompressedVideoWriter
	{
	public:
		CCompressedVideoWriter(const std::string& filePath, uint16_t width, uint16_t height);
		~CCompressedVideoWriter();
		void SaveFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame);

	private:
		std::string m_filePath;
		std::ofstream m_fileStream;
		uint16_t m_width, m_height;
	};
}