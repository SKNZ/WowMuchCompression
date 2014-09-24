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
	};
#pragma pack()

#define N 8
	const double inv16 = 1.0 / 16.0;
	const double SQRT2o2 = 1.414213562373095048801688724209 * 0.5;
	double alpha(int i)
	{
		if (i == 0)
			return SQRT2o2 * 0.5;
		return 0.5;
	}

	void Compress(Pixel** pixelArray, int height, int width)
	{
		double cosinay[N][N];
		for (int y = 0; y < 8; y++)
			for (int x = 0; x < 8; x++)
				cosinay[x][y] = cos(M_PI * x * (2.0 * y + 1) * inv16);

		Pixel** pixelBlock = new Pixel*[N];
		for (int x = 0; x < N; ++x)
			pixelBlock[x] = new Pixel[N];

		for (int x = 0; x < N; ++x)
			for (int y = 0; y < N; ++y)
				pixelBlock[x][y] = pixelArray[x][y];

		for (int y = 0; y < N; ++y)
		{
			for (int x = 0; x < N; ++x)
				cout << (int)pixelBlock[x][y].R << " ";
			cout << endl;
		}
		cout << endl;


		double** dctR = new double*[N];
		for (int x = 0; x < N; ++x)
			dctR[x] = new double[N];

		for (int y = 0; y < N; ++y)
		{
			for (int x = 0; x < N; ++x)
			{
				dctR[x][y] = 0;
				for (int u = 0; u < N; ++u)
				{
					for (int v = 0; v < N; ++v)
					{
						dctR[x][y] += alpha(u) * alpha(v) * (pixelBlock[u][v].R - 128) * cosinay[u][x] * cosinay[v][y];
					}
				}
			}
		}

		for (int y = 0; y < N; ++y)
		{
			for (int x = 0; x < N; ++x)
				cout << dctR[x][y] << " ";
			cout << endl;
		}
		//

		////ofstream file("debug.txt", ios::out | ios::trunc);
		////file.precision(3);
		////cout.precision(3);
		//cout << std::fixed;
		//double T[N][N];

		//for (int i = 0; i < N; ++i)
		//{
		//	for (int j = 0; j < N; ++j)
		//	{
		//		if (i == 0)
		//			T[i][j] = 1 / sqrt(N);
		//		else
		//			T[i][j] = sqrt(2.0 / N) * cos((((2 * j) + 1) * i * M_PI) / (2 * N));
		//	}
		//}

		//double** cofactorT = new double*[N];
		//for (int i = 0; i < N; ++i)
		//	cofactorT[i] = new double[N];

		//double** Tptr = new double*[N];
		//for (int i = 0; i < N; ++i)
		//	Tptr[i] = new double[N];

		//for (int i = 0; i < N; ++i)
		//	for (int j = 0; j < N; ++j)
		//		Tptr[i][j] = T[i][j];

		//double det = Determinant(Tptr, N);
		//CoFactor(Tptr, N, cofactorT);
		//Transpose(cofactorT, N);

		//for (int i = 0; i < N; ++i)
		//	for (int j = 0; j < N; ++j)
		//		cofactorT[i][j] /= det;

		//for (int currH = 0; currH < width - N - 1; currH += N)
		//{
		//	for (int currW = 0; currW < height - N - 1; currW += N)
		//	{

		//		int16_t M[N][N][3];
		//		for (int i = 0; i < N; ++i)
		//		{
		//			for (int j = 0; j < N; ++j)
		//			{
		//				M[i][j][0] = pixelArray[currH + i][currW + j].R - 128;
		//				M[i][j][1] = pixelArray[currH + i][currW + j].G - 128;
		//				M[i][j][2] = pixelArray[currH + i][currW + j].B - 128;
		//			}
		//		}
		//		 
		//		double TM[N][N][3];
		//		for (int i = 0; i < N; ++i)
		//		{
		//			for (int j = 0; j < N; ++j)
		//			{
		//				for (int k = 0; k < N; ++k)
		//				{
		//					TM[i][j][0] += T[i][k] * M[k][j][0];
		//					TM[i][j][1] += T[i][k] * M[k][j][1];
		//					TM[i][j][2] += T[i][k] * M[k][j][2];
		//				}
		//			}
		//		}

		//		double dct[N][N][3];
		//		for (int i = 0; i < N; ++i)
		//		{
		//			for (int j = 0; j < N; ++j)
		//			{
		//				for (int k = 0; k < N; ++k)
		//				{
		//					dct[i][j][0] += TM[i][k][0] * cofactorT[k][j];
		//					dct[i][j][1] += TM[i][k][1] * cofactorT[k][j];
		//					dct[i][j][2] += TM[i][k][2] * cofactorT[k][j];
		//				}
		//			}
		//		}
		//	}
		//}

		//cout << endl << endl;
		
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

	Pixel** pixelArray = new Pixel*[bmpHeader.ImageWidth];
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
