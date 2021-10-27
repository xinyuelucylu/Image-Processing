#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxImGui.h"




class ofApp : public ofBaseApp{

	public:
		
		bool my_tool_active;

		ofImage m_origImg;
		ofImage m_edittedImage;
		ofImage m_tempImage;
		ofImage m_rawImage;
		ofColor selectCol;
		ImVec4 bgCol;
		ImVec4 pickCol;
		ofImage m_eye;
		bool show_eyedropper;
		bool set_br;
		
		ImVec4 col;

		float m_brightness;
		float m_contrast;
		float m_saturation;
		float m_red;
		float m_green;
		float m_blue;

		

		ofxImGui::Gui m_gui;

		void setup();
		void update();
		void draw();

		void applyBrightness(float intensity);
		void applyContrast(float intensity);
		void applySaturation(float intensity);
		void red(float intensity);
		void green(float intensity);
		void blue (float intensity);

		void enableGaussianBlurFilter();
		void enableInvertFilter();
		void enableSepiaFilter();
		void enableHueRotateFilter();
		void applyConvolution3x3(Constants::CONVOLUTION_MAT_TYPE matType);

		void applyReset();

		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void HelpMarker(const char* desc);

		void openFile(ofFileDialogResult openFileResult);
		string originalFileExtension;
};

