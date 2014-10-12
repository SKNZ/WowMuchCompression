#include "stdafx.h"

void nsWMC::YCbCrToRGB(double Y, double Cb, double Cr, uint8_t& R, uint8_t& G, uint8_t& B)
{
	R = (uint8_t)round(Y + 1.402 * (Cr - 128));
	G = (uint8_t)round(Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128));
	B = (uint8_t)round(Y + 1.772 * (Cb - 128));
}

void nsWMC::RGBToYCbCr(uint8_t R, uint8_t G, uint8_t B, double& Y, double& Cb, double& Cr)
{
	Y = 0.299 * R + 0.587 * G + 0.114 * B;
	Cb = -0.168736 * R - 0.331264 * G + 0.5 * B + 128;
	Cr = 0.5 * R - 0.418688 * G - 0.081312 * B + 128;
}
