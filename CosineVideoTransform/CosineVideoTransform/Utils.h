#pragma once

#include "stdafx.h"

#define PIXEL_BLOCK_SIZE 8
#define VIDEO_MAX_WIDTH 1366
#define VIDEO_MAX_HEIGHT 768 // block matching
#define VIDEO_MAX_WIDTH_BLOCKCOUNT 171
#define VIDEO_MAX_HEIGHT_BLOCKCOUNT 96


namespace nsWMC
{
	typedef int16_t CSerializedComponentType;
	typedef float CComponentType;
	typedef Eigen::Matrix<CComponentType, PIXEL_BLOCK_SIZE, PIXEL_BLOCK_SIZE> CComponentBlock;
	typedef Eigen::Matrix<CComponentType, Eigen::Dynamic, Eigen::Dynamic> CComponentFrame;
	typedef std::vector<int16_t> CSerializableComponentFrame;

	extern void RGBToYCbCr(uint8_t R, uint8_t G, uint8_t B, CComponentType& Y, CComponentType& Cb, CComponentType& Cr);

	extern void YCbCrToRGB(CComponentType Y, CComponentType Cb, CComponentType Cr, uint8_t& R, uint8_t& G, uint8_t& B);

	const double KInverseOfSQRTOf2 = 1. / sqrt(2.);
	const double KSQRTOf2OnPixelBlockSize = sqrt(2. / PIXEL_BLOCK_SIZE);
	const double KSQRTOf2 = sqrt(2.);

}
