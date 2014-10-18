#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CRawVideoLoader
	{
	public:
		CRawVideoLoader(const std::string& filePath);

		bool GetNextYCbCrFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame);

		int GetWidth() const;

		int GetHeight() const;

		int GetWidthPadding() const;

		int GetHeightPadding() const;
		
	private:
		const std::string m_filePath;
		cv::VideoCapture m_videoFile;
		int m_width, m_height, m_widthPadding, m_heightPadding;
	};
}
