#include "stdafx.h"

void nsWMC::YCbCrToRGB(CComponentType Y, CComponentType Cb, CComponentType Cr, uint8_t& R, uint8_t& G, uint8_t& B)
{
	R = (uint8_t)round(Y + 1.402 * (Cr - 128));
	G = (uint8_t)round(Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128));
	B = (uint8_t)round(Y + 1.772 * (Cb - 128));
}

void nsWMC::RGBToYCbCr(uint8_t R, uint8_t G, uint8_t B, CComponentType& Y, CComponentType& Cb, CComponentType& Cr)
{
	Y = 0.299f * R + 0.587f * G + 0.114f * B;
	Cb = -0.168736f * R - 0.331264f * G + 0.5f * B + 128;
	Cr = 0.5f * R - 0.418688f * G - 0.081312f * B + 128;
}
