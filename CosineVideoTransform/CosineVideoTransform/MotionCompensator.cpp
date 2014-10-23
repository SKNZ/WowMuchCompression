#include "stdafx.h"

using namespace nsWMC;
using namespace std;

CMotionCompensator::CMotionCompensator(bool inverse)
	: m_inverse(inverse)
{

}

void CMotionCompensator::operator()(CComponentFrame& referenceFrame, CComponentFrame& targetFrame, CTranslationVectors& translations)
{
	if (m_inverse)
	{
		for (CTranslationVector translation : translations)
			targetFrame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(translation.kD, translation.nD)
			= referenceFrame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(translation.kO, translation.nO);
	}
	else
	{
		for (int kO = 0; kO < referenceFrame.rows(); kO += PIXEL_BLOCK_SIZE)
		{
			for (int nO = 0; nO < referenceFrame.cols(); nO += PIXEL_BLOCK_SIZE)
			{
				CComponentBlock componentBlockO = referenceFrame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(kO, nO);

				if (componentBlockO.mean() == 0)
					continue;

				for (int kD = 0; kD < targetFrame.rows(); kD += PIXEL_BLOCK_SIZE)
				{
					for (int nD = 0; nD < targetFrame.cols(); nD += PIXEL_BLOCK_SIZE)
					{
						CComponentBlock componentBlockD = targetFrame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(kD, nD);

						if (componentBlockD.mean() == 0)
							continue;

						bool compensate = true;
						for (int kp = 0; kp < PIXEL_BLOCK_SIZE && compensate; ++kp)
							for (int np = 0; np < PIXEL_BLOCK_SIZE && compensate; ++np)
								if (componentBlockO(kp, np) - componentBlockD(kp, np) > 10)
									compensate = false;

						if (compensate)
							translations.push_back(CTranslationVector{ kO = kO, kD = kD, nO = nO, nD = nD });

						componentBlockD.fill(0);
						targetFrame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(kD, nD) = componentBlockD;
					}
				}
			}
		}
	}
}
