#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CChromaSubsampler
	{
	public:
		CChromaSubsampler(bool resample = false);
		void operator()(CComponentFrame& videoFrame);

	private:
		bool m_resample;
	};
}
