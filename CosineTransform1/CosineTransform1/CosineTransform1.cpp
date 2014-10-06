// CosineTransform1.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "Matrix.h"

using namespace std;

namespace
{
	const char* KInputFile = "original.bmp";
	const char* KCompressedFile = "compressed.bmp";
	const char* KExtractedFile = "uncompressed.bmp";

#pragma pack(2)
	struct BMPFileHeader
	{
		uint16_t Signature;
		uint32_t FileSize;
		uint16_t Reserved1;
		uint16_t Reserved2;
		uint32_t PixelArrayOffset;
	};

	struct BMPV5Header
	{
		uint32_t DIBHeaderSize;
		int32_t ImageWidth;
		int32_t ImageHeight;
		uint16_t Planes;
		uint16_t BitsPerPixels;
		uint32_t Compression;
		uint32_t ImageSize;
		int32_t XPixelsPerMeter;
		int32_t YPixelsPerMeter;
		int32_t ColorsInColorsTable;
		int32_t ImportantColorCount;
		int32_t RedChannelBitmask;
		int32_t GreenChannelBitmask;
		int32_t BlueChannelBitmask;
		int32_t AlphaChannelBitmask;
		int32_t ColorSpaceType;
		char ColorSpaceEndpoints[36];
		int32_t RedChannelGamma;
		int32_t GreenChannelGamma;
		int32_t BlueChannelGamma;
		int32_t Intent;
		int32_t ICCProfileData;
		int32_t ICCProfileSpace;
		int32_t Reserved;
	};

	struct Pixel
	{
		uint8_t B, G, R;

		double Y()
		{
			return 0.299 * R + 0.587 * G + 0.114 * B;
		}

		double Cb()
		{
			return -0.1687 * R - 0.3313 * G + 0.5 * B + 128;
		}

		double Cr()
		{
			return 0.5 * R - 0.4187 * G - 0.0813 * B + 128;
		}

		void setR(double Y, double Cr)
		{
			R = uint8_t(Y + 1.402 * (Cr - 128));
		}

		void setG(double Y, double Cb, double Cr)
		{
			G = uint8_t(Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128));
		}

		void setB(double Y, double Cb)
		{
			B = uint8_t(Y + 1.772 * (Cb - 128));
		}
	};
#pragma pack()

#define N 8

	using Eigen::Matrix;
	using Eigen::MatrixXd;

	const double KInverseOfSQRTOf2 = 1. / sqrt(2.);
	const double KSQRTOf2N = sqrt(2. / N);
	const double KSQRTOf2 = sqrt(2.);
	typedef Matrix<double, N, N> MatrixN;

	void doDCT(double pixelBlock[N][N], double dct[N][N])
	{
		MatrixN pixelMatrix, dctMatrix;
		for (int k = 0; k < N; ++k)
			for (int n = 0; n < N; ++n)
				pixelMatrix(k, n) = pixelBlock[k][n] - 128;

		for (int k = 0; k < N; ++k)
		{
			for (int n = 0; n < N; ++n)
			{
				dctMatrix(k, n) = cos((M_PI * k * (n + .5)) / N);
			}
		}

		dctMatrix.row(0) *= KInverseOfSQRTOf2; // Following to the JPEG standard, the first line is multiplied by 1/sqrt(2)
		dctMatrix *= KSQRTOf2N;

		//cout << dctMatrix << endl << endl;

		pixelMatrix = dctMatrix * pixelMatrix * dctMatrix.transpose();

		quantize(pixelMatrix);

		for (int k = 0; k < N; ++k)
			for (int n = 0; n < N; ++n)
				dct[k][n] = pixelMatrix(k, n);
	}

	void undoDCT(double pixelBlock[N][N], double dct[N][N])
	{
		MatrixN pixelMatrix, dctMatrix;
		for (int k = 0; k < N; ++k)
			for (int n = 0; n < N; ++n)
				dctMatrix(k, n) = dct[k][n];

		for (int k = 0; k < N; ++k)
		{
			pixelMatrix(k, 0) = .5;
			for (int n = 1; n < N; ++n)
				pixelMatrix(k, n) = cos((M_PI * n * (k + .5)) / N);
		}

		pixelMatrix.col(0) *= KSQRTOf2;
		pixelMatrix *= KSQRTOf2N;

		//cout << pixelMatrix << endl << endl;

		pixelMatrix = pixelMatrix * dctMatrix * pixelMatrix.inverse();

		for (int k = 0; k < N; ++k)
			for (int n = 0; n < N; ++n)
				pixelMatrix(k, n) += 128;

		//cout << pixelMatrix << endl << endl;
	}

	void Compress(Pixel** pixelArray, int height, int width)
	{
		for (int u = 0; u < width - 8; u += 8)
		{
			for (int v = 0; v < height - 8; v += 8)
			{
				double pixelBlock[N][N];
				for (int up = 0; up < N; ++up) // kp = k'
					for (int vp = 0; vp < N; ++vp) // np = n'
						pixelBlock[up][vp] = pixelArray[u + up][v + vp].Y(); // TOUT BI RIPLASSAIDE OUIZ IUVE LUMINENSSE

				/*pixelBlock[0][0] = 52;
				pixelBlock[0][1] = 55;
				pixelBlock[0][2] = 61;
				pixelBlock[0][3] = 66;
				pixelBlock[0][4] = 70;
				pixelBlock[0][5] = 61;
				pixelBlock[0][6] = 64;
				pixelBlock[0][7] = 73;
				pixelBlock[1][0] = 63;
				pixelBlock[1][1] = 59;
				pixelBlock[1][2] = 55;
				pixelBlock[1][3] = 90;
				pixelBlock[1][4] = 109;
				pixelBlock[1][5] = 85;
				pixelBlock[1][6] = 69;
				pixelBlock[1][7] = 72;
				pixelBlock[2][0] = 62;
				pixelBlock[2][1] = 59;
				pixelBlock[2][2] = 68;
				pixelBlock[2][3] = 113;
				pixelBlock[2][4] = 144;
				pixelBlock[2][5] = 104;
				pixelBlock[2][6] = 66;
				pixelBlock[2][7] = 73;
				pixelBlock[3][0] = 63;
				pixelBlock[3][1] = 58;
				pixelBlock[3][2] = 71;
				pixelBlock[3][3] = 122;
				pixelBlock[3][4] = 154;
				pixelBlock[3][5] = 106;
				pixelBlock[3][6] = 70;
				pixelBlock[3][7] = 69;
				pixelBlock[4][0] = 67;
				pixelBlock[4][1] = 61;
				pixelBlock[4][2] = 68;
				pixelBlock[4][3] = 104;
				pixelBlock[4][4] = 126;
				pixelBlock[4][5] = 88;
				pixelBlock[4][6] = 68;
				pixelBlock[4][7] = 70;
				pixelBlock[5][0] = 79;
				pixelBlock[5][1] = 65;
				pixelBlock[5][2] = 60;
				pixelBlock[5][3] = 70;
				pixelBlock[5][4] = 77;
				pixelBlock[5][5] = 68;
				pixelBlock[5][6] = 58;
				pixelBlock[5][7] = 75;
				pixelBlock[6][0] = 85;
				pixelBlock[6][1] = 71;
				pixelBlock[6][2] = 64;
				pixelBlock[6][3] = 59;
				pixelBlock[6][4] = 55;
				pixelBlock[6][5] = 61;
				pixelBlock[6][6] = 65;
				pixelBlock[6][7] = 83;
				pixelBlock[7][0] = 87;
				pixelBlock[7][1] = 79;
				pixelBlock[7][2] = 69;
				pixelBlock[7][3] = 68;
				pixelBlock[7][4] = 65;
				pixelBlock[7][5] = 76;
				pixelBlock[7][6] = 78;
				pixelBlock[7][7] = 94;*/

				double dct[N][N];
				for (int up = 0; up < N; ++up)
					for (int vp = 0; vp < N; ++vp)
						dct[up][vp] = 0.f;

				cout.precision(4);

				doDCT(pixelBlock, dct);
				undoDCT(pixelBlock, dct);

				for (int up = 0; up < N; ++up) // kp = k'
					for (int vp = 0; vp < N; ++vp) // np = n'
					{
						pixelArray[u + up][v + vp].setR(pixelBlock[up][vp], pixelArray[u + up][v + vp].Cr());
						pixelArray[u + up][v + vp].setG(pixelBlock[up][vp], pixelArray[u + up][v + vp].Cb(), pixelArray[u + up][v + vp].Cr());
						pixelArray[u + up][v + vp].setB(pixelBlock[up][vp], pixelArray[u + up][v + vp].Cb());
					}
			}
		}
	}

	void Extract(Pixel** pixelArray, int height, int width)
	{

	}
}

int _tmain(int argc, _TCHAR** argv)
{
	ifstream originalFile(KInputFile, ios::binary | ios::in);

	if (!originalFile)
		return EXIT_FAILURE;

	BMPFileHeader fileHeader;
	originalFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));

	BMPV5Header bmpHeader;
	originalFile.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));

	int rowSize = 4 * (((bmpHeader.BitsPerPixels * bmpHeader.ImageWidth) + 31) / 32);

	MatrixXd pixelArray = new Pixel*[bmpHeader.ImageWidth];
	for (int i = 0; i < bmpHeader.ImageWidth; ++i)
		pixelArray[i] = new Pixel[bmpHeader.ImageHeight];

	const int paddingBitsCount = rowSize - (bmpHeader.ImageWidth * sizeof(Pixel));

	for (int y = 0; y < bmpHeader.ImageHeight; ++y)
	{
		for (int x = 0; x < bmpHeader.ImageWidth; ++x)
			originalFile.read(reinterpret_cast<char*>(&pixelArray[x][y]), sizeof(Pixel));

		originalFile.seekg(static_cast<int>(originalFile.tellg()) + paddingBitsCount);
	}

	originalFile.close();

	cout << "BMP File Information" << endl
		<< "=========================" << endl
		<< "Signature: " << fileHeader.Signature << endl
		<< "FileSize: " << fileHeader.FileSize << endl
		<< "Reserved: " << fileHeader.Reserved1 << endl
		<< "Reserved: " << fileHeader.Reserved2 << endl
		<< "PixelArrayOffset: " << fileHeader.PixelArrayOffset << endl
		<< "Width: " << bmpHeader.ImageWidth << endl
		<< "Height: " << bmpHeader.ImageHeight << endl
		<< "Bits per pixel: " << bmpHeader.BitsPerPixels << endl
		<< "PixelArrayRowSize: " << rowSize << endl
		<< "PixelArraySize: " << rowSize * bmpHeader.ImageWidth << endl
		<< "=========================" << endl;

	Compress(pixelArray, bmpHeader.ImageHeight, bmpHeader.ImageWidth);

	ofstream compressedFile(KCompressedFile, ios::binary | ios::out | ios::trunc);

	if (!compressedFile)
		return EXIT_FAILURE;

	compressedFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
	compressedFile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));

	for (int y = 0; y < bmpHeader.ImageHeight; ++y)
	{
		for (int x = 0; x < bmpHeader.ImageWidth; ++x)
			compressedFile.write(reinterpret_cast<char*>(&pixelArray[x][y]), sizeof(Pixel));

		for (int i = 0; i < paddingBitsCount; ++i)
			compressedFile << 0;
	}

	compressedFile.close();

	Extract(pixelArray, bmpHeader.ImageHeight, bmpHeader.ImageWidth);

	ofstream extractedFile(KExtractedFile, ios::binary | ios::out | ios::trunc);

	if (!extractedFile)
		return EXIT_FAILURE;

	extractedFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
	extractedFile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPV5Header));


	for (int y = 0; y < bmpHeader.ImageHeight; ++y)
	{
		for (int x = 0; x < bmpHeader.ImageWidth; ++x)
			extractedFile.write(reinterpret_cast<char*>(&pixelArray[x][y]), sizeof(Pixel));

		for (int i = 0; i < paddingBitsCount; ++i)
			extractedFile << 0;
	}

	extractedFile.close();

	for (int i = 0; i < bmpHeader.ImageWidth; ++i)
		delete[] pixelArray[i];
	delete[] pixelArray;

	system("pause");
	return EXIT_SUCCESS;
}
