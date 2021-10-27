#pragma once

#include "ofMain.h"

namespace Constants{
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	static const int DESIRED_FRAMERATE = 60;

	static const std::string IMG_PATH = "JupiterFromJunoSmall.jpg";
	

	enum CONVOLUTION_MAT_TYPE {
		GAUSSIAN_BLUR = 0
	};

	static const float CONVOLUTION_MATS_3X3[] = {
													//GAUSSIAN BLUR
													1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
													2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
													1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
	};

	static const int CONVOLUTION_KERNAL_CONVERT_X[]{ -1, 0, 1,
													 -1, 0, 1,
													 -1, 0, 1											
	};

	static const int CONVOLUTION_KERNAL_CONVERT_Y[]{ -1, -1, -1,
													  0,  0,  0,
													  1,  1,  1									
	};
}
