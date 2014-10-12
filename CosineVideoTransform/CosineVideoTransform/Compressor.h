#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CCompressor
	{
	public:
		CCompressor(const std::string& inputFilePath, const std::string& outputFilePath);
		void run();

	private:
		const std::string m_inputFilePath;
		const std::string m_outputFilePath;
	};
}
