#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CVideoLoader
	{
	public:
		CVideoLoader(const std::string& filePath);

		~CVideoLoader();

		bool GetNextYCbCrFrame(CComponentFrame& YVideoFrame, CComponentFrame& CbVideoFrame, CComponentFrame& CrVideoFrame);

		int GetWidth() const;

		int GetHeight() const;
		
	private:
		const std::string m_filePath;
		cv::VideoCapture m_videoFile;
		int m_width, m_height;
	};
}
