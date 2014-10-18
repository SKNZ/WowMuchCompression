#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CRawVideoExporter
	{
	public:
		CRawVideoExporter(const std::string& filePath, int width, int height, int widthPadding, int heightPadding);
		void ExportFrame(const CComponentFrame& YVideoFrame, const CComponentFrame& CbVideoFrame, const CComponentFrame& CrVideoFrame);

	private:
		std::string m_filePath;
		cv::VideoWriter m_videoWriter;
		int m_width, m_height, m_widthPadding, m_heightPadding;

	};
}
