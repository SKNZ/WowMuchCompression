#include "stdafx.h"

using namespace nsWMC;
using namespace std;
CDiscreteCosineTransform::CDiscreteCosineTransform(bool inverse, bool quantize, int quantizationLevel)
	: m_inverse(inverse), m_quantize(quantize), m_quantizationLevel(quantizationLevel)
{
	cout << "DCT provider: initializing." << endl;

	for (int k = 0; k < PIXEL_BLOCK_SIZE; ++k)
		for (int n = 0; n < PIXEL_BLOCK_SIZE; ++n)
			m_dctMatrix(k, n) = cos((M_PI * k * (n + .5)) / PIXEL_BLOCK_SIZE);

	m_dctMatrix.row(0) *= KInverseOfSQRTOf2; // Multiplication de la première ligne par 1/sqrt(2)
	m_dctMatrix *= KSQRTOf2OnPixelBlockSize;  // Multiplication de la matrice par sqrt(2)/PIXEL_BLOCK_SIZE

	m_dctMatrixInverse = m_dctMatrix.inverse(); // DCT inverse matrix

	setQuantizationLevel(quantizationLevel);

	cout << "DCT provider: initialized." << endl << endl;
}

void CDiscreteCosineTransform::setQuantization(bool quantize)
{
	m_quantize = quantize;
}

bool CDiscreteCosineTransform::isQuantizing() const
{
	return m_quantize;
}

void CDiscreteCosineTransform::setInverse(bool inverse)
{
	m_inverse = inverse;
}

bool CDiscreteCosineTransform::getInverse() const
{
	return m_inverse;
}

void CDiscreteCosineTransform::setQuantizationLevel(int quality)
{
	m_quantizationLevel = quality;
	if (m_quantizationLevel < 0 || m_quantizationLevel > 100)
		throw runtime_error("Discrete cosine transform quantization quality must be between 0 and 100%.");

	// Matrice de quantification (source: JPEG)
	m_quantizationMatrix <<	16,  12,  14,  14,  18,  24,  49,  72,
							11,  12,  13,  17,  22,  35,  64,  92,
							10,  14,  16,  22,  37,  55,  78,  95,
							16,  19,  24,  29,  56,  64,  87,  98,
							24,  26,  40,  51,  68,  81, 103, 112,
							40,  58,  57,  87, 109, 104, 121, 100,
							51,  60,  69,  80, 103, 113, 120, 103,
							61,  55,  56,  62,  77,  92, 101,  99;
	
	/**
	  * On multiplie la matrice de quantification par:
	  * qualité > 50, (100 - qualité / 50)
	  * qualité < 50, 50 / qualité;
	  *
	  **/
	if (m_quantizationLevel != 50)
		m_quantizationMatrix *= m_quantizationLevel > 50 ? (100 - m_quantizationLevel) / 50 : 50 / m_quantizationLevel;
}

int CDiscreteCosineTransform::getQuantizationLevel() const
{
	return m_quantizationLevel;
}

void CDiscreteCosineTransform::operator()(CComponentFrame& frame) const
{
	// Pour chaque bloc de 8
	for (int k = 0; k < frame.rows(); k += PIXEL_BLOCK_SIZE)
	{
		for (int n = 0; n < frame.cols(); n += PIXEL_BLOCK_SIZE)
		{
			// On "extrait" un sous-bloc de la matrice de la tailel de PIXEL_BLOCK_SIZE sur PIXEL_BLOCK_SIZE
			CComponentBlock componentBlock = frame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(k, n);
			
			if (m_inverse)
			{
				componentBlock = m_dctMatrixInverse * componentBlock * m_dctMatrix;
				
				if (m_quantize)
				{
					componentBlock.cwiseQuotient(m_quantizationMatrix); // Multiplication membre à membre avec la matrice de quantification

					// Arrondi
					for (int kprime = 0; kprime < PIXEL_BLOCK_SIZE; ++kprime)
						for (int nprime = 0; nprime < PIXEL_BLOCK_SIZE; ++nprime)
							componentBlock(kprime, nprime) = int(componentBlock(kprime, nprime)) + (componentBlock(kprime, nprime) - (int)componentBlock(kprime, nprime) > 0.5 ? 1 : 0);
				}

				// Addition membre à membre
				for (int kprime = 0; kprime < PIXEL_BLOCK_SIZE; ++kprime)
					for (int nprime = 0; nprime < PIXEL_BLOCK_SIZE; ++nprime)
						componentBlock(kprime, nprime) += 128;
			}
			else
			{
				// Soustraction membre à membre
				for (int kprime = 0; kprime < PIXEL_BLOCK_SIZE; ++kprime)
					for (int nprime = 0; nprime < PIXEL_BLOCK_SIZE; ++nprime)
						componentBlock(kprime, nprime) -= 128;

				componentBlock = m_dctMatrix * componentBlock * m_dctMatrixInverse;
				
				if (m_quantize)
				{
					componentBlock.cwiseProduct(m_quantizationMatrix); // Division membre à membre avec la matrice de quantification
				
					// Arrondi
					for (int kprime = 0; kprime < PIXEL_BLOCK_SIZE; ++kprime)
						for (int nprime = 0; nprime < PIXEL_BLOCK_SIZE; ++nprime)
							componentBlock(kprime, nprime) = int(componentBlock(kprime, nprime)) + (componentBlock(kprime, nprime) - (int)componentBlock(kprime, nprime) > 0.5 ? 1 : 0);
				}
			}

			// On replace le sous bloc
			frame.block<PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE>(k, n) = componentBlock;
		}
	}
}
