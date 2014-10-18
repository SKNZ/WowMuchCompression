#include "stdafx.h"

using namespace nsWMC;

CChromaSubsampler::CChromaSubsampler(bool resample)
	:m_resample(resample)
{
}

void CChromaSubsampler::operator()(CComponentFrame& frame)
{
	if (!m_resample)
	{
		// On réduit de moitié la quantité de composantes chromatiques
		CComponentFrame subsampledFrame(frame.rows() / 2, frame.cols() / 4);

		// On prend la moyenne du bloc.
		for (int k = 0; k < subsampledFrame.rows(); ++k)
			for (int n = 0; n < subsampledFrame.cols(); ++n)
				subsampledFrame(k, n) = frame.block(2 * k, 4 * n, 2, 4).mean();

		frame = subsampledFrame;
	}
	else
	{
		// On redimensionne la frame à sa taille originale
		CComponentFrame resampledFrame(frame.rows() * 2, frame.cols() * 4);

		// On rempli deux blocs avec la valeur moyenne calculée lors de la compression (il y a donc perte d'information)
		for (int k = 0; k < resampledFrame.rows(); k += 2)
			for (int n = 0; n < resampledFrame.cols(); n += 4)
				resampledFrame.block(k, n, 2, 4).fill(frame(k / 2, n / 4));

		frame = resampledFrame;
	}
}
