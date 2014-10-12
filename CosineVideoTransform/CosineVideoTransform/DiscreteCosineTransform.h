#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CDiscreteCosineTransform
	{
	public:
		CDiscreteCosineTransform(bool quantize = false, int quantizationLevel = 50);

		void setQuantization(bool quantize);
		bool isQuantizing() const;
		void setQuantizationLevel(int quality);
		int getQuantizationLevel() const;

		void operator()(CComponentFrame& frame) const;

	private:
		bool m_quantize;
		int m_quantizationLevel;
		CComponentBlock m_dctMatrix;
		CComponentBlock m_dctMatrixInverse;
		CComponentBlock m_quantizationMatrix;
	};
}
