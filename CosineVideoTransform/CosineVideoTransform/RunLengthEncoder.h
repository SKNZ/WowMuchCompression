#pragma once

#include "stdafx.h"

namespace nsWMC
{
	class CRunLengthEncoder
	{
	public:
		CRunLengthEncoder(bool decode = false);
		void operator()(CComponentFrame& frame, CSerializableComponentFrame& serializedFrame);
	
	private:
		bool m_decode;
	};
}
