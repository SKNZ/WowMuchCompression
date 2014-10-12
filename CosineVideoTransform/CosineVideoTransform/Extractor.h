#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CExtractor
	{
	public:
		CExtractor(const std::string& inputFile, const std::string& outputFile);
		void run() const;

	private:
		std::string m_inputFile;
		std::string m_outputFile;
	};
}
