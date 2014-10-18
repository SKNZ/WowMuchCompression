// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include "intrin.h"

#include "Utils.h"
#include "RawVideoLoader.h"
#include "DiscreteCosineTransform.h"
#include "Compressor.h"
#include "CompressedVideoWriter.h"
#include "Extractor.h"
#include "CompressedVideoReader.h"
#include "RawVideoExporter.h"
#include "ChromaSubsampler.h"
#include "RunLengthEncoder.h"
#include "MotionCompensator.h"

// TODO: reference additional headers your program requires here
