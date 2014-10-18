#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CMotionCompensator
	{
	public:
		CMotionCompensator(bool inverse = false);
		void operator()(CComponentFrame& referenceFrame, CComponentFrame& targetFrame, CTranslationVectors& translations);

	private:
		bool m_inverse;
	};
}
