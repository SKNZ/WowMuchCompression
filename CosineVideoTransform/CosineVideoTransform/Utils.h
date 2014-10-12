#pragma once

#include "stdafx.h"

#define PIXEL_BLOCK_SIZE 8

namespace nsWMC
{
	extern void RGBToYCbCr(uint8_t R, uint8_t G, uint8_t B, double& Y, double& Cb, double& Cr);

	extern void YCbCrToRGB(double Y, double Cb, double Cr, uint8_t& R, uint8_t& G, uint8_t& B);

	const double KInverseOfSQRTOf2 = 1. / sqrt(2.);
	const double KSQRTOf2OnPixelBlockSize = sqrt(2. / PIXEL_BLOCK_SIZE);
	const double KSQRTOf2 = sqrt(2.);

	typedef Eigen::Matrix<double, PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE> CComponentBlock;
	typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> CComponentFrame;
}
