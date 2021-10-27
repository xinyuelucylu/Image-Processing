#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	ofSetFrameRate(Constants::DESIRED_FRAMERATE);
	show_eyedropper = false;
	set_br = false;
	m_eye.load("eye.png");
	m_gui.setup();

	pickCol=ofColor(0, 0, 0, 100);

}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::white);
	if (m_edittedImage.getWidth() > 1000 || m_edittedImage.getHeight() > 1000) {
		m_edittedImage.draw(0.0f, 0.0f, m_edittedImage.getWidth() / 2.0f, m_edittedImage.getHeight() / 2.0f);
		m_origImg.draw(m_edittedImage.getWidth()/2, 0.0f, m_edittedImage.getWidth() / 8.0f, m_edittedImage.getHeight() / 8.0f);
	}
	else {
		m_edittedImage.draw(0.0f, 0.0f);
		m_origImg.draw(m_edittedImage.getWidth(), 0.0f, m_edittedImage.getWidth() / 4.0f, m_edittedImage.getHeight() / 4.0f);
	}

	m_gui.begin();
	
		ImGui::Begin("Control Panel", &my_tool_active, ImGuiWindowFlags_MenuBar);
		
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open")) {
					ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a jpg or png");

					if (openFileResult.bSuccess) {
						std::cout << "User selected a file" << std::endl;
						openFile(openFileResult);
						applyReset();
					}
					else {
						std::cout << "User hit cancel" << std::endl;
					}
				}if (ImGui::MenuItem("Save")) {
					ofFileDialogResult saveFileResult = ofSystemSaveDialog("default.jpg", "Save your file");
					if (saveFileResult.bSuccess) {
						string path = saveFileResult.getPath() + "." + ofToLower(originalFileExtension);
						m_edittedImage.save(path);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		if (ImGui::CollapsingHeader("Instruction")) {
			
			ImGui::Text("This program helps user do image processing.");
			ImGui::Text("MENU BAR:");
			ImGui::BulletText("File-Open: Open image file");
			ImGui::BulletText("File-Save: Save the processing image");
			ImGui::Text("IMAGE ADJUSTMENTS:");
			ImGui::BulletText("Drag the bar to adjust each value.");
			ImGui::BulletText("Effects cannot be sueprimposed");
			ImGui::Text("FILTERS:");
			ImGui::BulletText("Click buttons to add filters to image.");
			ImGui::BulletText("Effects cannot be sueprimposed");
			ImGui::Text("COLOR:");
			ImGui::BulletText("Selected color: select a color");
			ImGui::BulletText("Set Background Color: Set background color");
			ImGui::Text("	(auto disable brush)");
			ImGui::BulletText("Eyedropper Tool: Select a color from the editted image");
			ImGui::Text("	(click again to disable)");
			ImGui::BulletText("Brush Tool: Use brush on background");
			ImGui::Text("	(click again to disable)");
			ImGui::Text("RESET:");
			ImGui::BulletText("Reset editted image and clear background.");
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Image Adjustments")) {
			if (ImGui::SliderFloat("Brightness", &m_brightness, -50.0f, 50.0f)) {
				applyBrightness(m_brightness);
			}
			if (ImGui::SliderFloat("Contrast", &m_contrast, -50.0f, 50.0f)) {
				applyContrast(m_contrast);
			}
			if (ImGui::SliderFloat("Saturation", &m_saturation, -50.0f, 50.0f)) {
				applySaturation(m_saturation);
			}
			if (ImGui::SliderFloat("red", &m_red, -50.0f, 50.0f)) {
				red(m_red);
			}ImGui::SameLine(); HelpMarker("Cyan <--> Red");

			if (ImGui::SliderFloat("green", &m_green, -50.0f, 50.0f)) {
				green(m_green);
			}ImGui::SameLine(); HelpMarker("Magenta <--> Green");

			if (ImGui::SliderFloat("blue", &m_blue, -50.0f, 50.0f)) {
				blue(m_blue);
			}ImGui::SameLine(); HelpMarker("Yellow <--> Blue");
		}

		if (ImGui::CollapsingHeader("Filters")) {
			if (ImGui::Button("Enable Gaussian Blur")) {
				enableGaussianBlurFilter();
			}
			if (ImGui::Button("Enable Invert")) {
				enableInvertFilter();
			}
			if (ImGui::Button("Enable Sepia")) {
				enableSepiaFilter();
			}
			if (ImGui::Button("Enable Hue Rotation")) {
				enableHueRotateFilter();
			}
		}

		if (ImGui::CollapsingHeader("Color")) {
			ImGui::ColorEdit4("Selected color", (float*)&pickCol, ImGuiColorEditFlags_NoInputs); 

			if (ImGui::Button("Set Background Color")) {
				set_br = false;
				bgCol = pickCol;
				ofSetBackgroundColor(pickCol);
			}ImGui::SameLine(); HelpMarker("Set background color to selected color.");

			ImGui::Checkbox("Eyedropper Tool", &show_eyedropper);
			ImGui::SameLine(); HelpMarker("Pick a color from editted image.");
			ImGui::Checkbox("Brush Tool", &set_br);
			ImGui::SameLine(); HelpMarker("Left mouse button: brush with selected color. Right mouse button: eraser.");

			if (show_eyedropper) {
				set_br = false;
				ofSetColor(ofColor::white);
				m_eye.draw(ofGetMouseX(), ofGetMouseY() - m_eye.getHeight());
			}
			if (set_br) {
				ofSetBackgroundAuto(false);
				show_eyedropper = false;
			}
			if(!set_br)
				ofSetBackgroundAuto(true);
		}

		ImGui::Separator();
		if (ImGui::Button("Reset")) {
			applyReset();
		}ImGui::SameLine(); HelpMarker("Reset editted image to original image, clear background");
		ImGui::End();
	m_gui.end();

}

void ofApp::applyReset() {
	m_edittedImage = m_origImg;
	m_brightness = m_contrast = m_saturation = m_red = m_green = m_blue = 0.0f;
	setup();
	ofSetBackgroundAuto(true);
}
void ofApp::applyBrightness(float intensity){

	ofColor tempCol;
	ofColor newCol;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			
			tempCol = m_origImg.getColor(x, y);

			newCol.set( ofClamp(tempCol.r + intensity, 0.0f, 255.0f), 
						ofClamp(tempCol.g + intensity, 0.0f, 255.0f),
						ofClamp(tempCol.b + intensity, 0.0f, 255.0f));

			m_edittedImage.setColor(x, y, newCol);
		}
	}
	m_edittedImage.update(); 
}
void ofApp::applyContrast(float intensity) {

	ofColor tempCol;
	ofColor newCol;

	const float contrastfactor = (259.0f * (255.0f + intensity)) / (255.0f * (259.0f - intensity));

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			tempCol = m_origImg.getColor(x, y);

			newCol.set(	ofClamp(contrastfactor * (tempCol.r - 128.0f) + 128.0f, 0.0f, 255.0f),
						ofClamp(contrastfactor * (tempCol.g - 128.0f) + 128.0f, 0.0f, 255.0f),
						ofClamp(contrastfactor * (tempCol.b - 128.0f) + 128.0f, 0.0f, 255.0f));

			m_edittedImage.setColor(x, y, newCol);
		}
	}
	m_edittedImage.update(); 
}

void ofApp::applySaturation(float intensity) {

	ofColor Col;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			Col = m_origImg.getColor(x, y);

			float sat = Col.getSaturation();
			Col.setSaturation(sat + intensity);
			
			m_edittedImage.setColor(x, y, Col);
		}
	}
	m_edittedImage.update(); 
}

void ofApp::red(float intensity) {

	ofColor tempCol;
	ofColor newCol;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			tempCol = m_origImg.getColor(x, y);

			newCol.set(ofClamp(tempCol.r + intensity, 0.0f, 255.0f), tempCol.g, tempCol.b);

			m_edittedImage.setColor(x, y,newCol);
		}
	}
	m_edittedImage.update(); 
}
void ofApp::green(float intensity) {

	ofColor tempCol;
	ofColor newCol;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			tempCol = m_origImg.getColor(x, y);

			newCol.set(tempCol.r, ofClamp(tempCol.g + intensity, 0.0f, 255.0f), tempCol.b);

			m_edittedImage.setColor(x, y, newCol);
		}
	}
	m_edittedImage.update();
}

void ofApp::blue(float intensity) {

	ofColor tempCol;
	ofColor newCol;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			tempCol = m_origImg.getColor(x, y);

			newCol.set(tempCol.r, tempCol.g, ofClamp(tempCol.b + intensity, 0.0f, 255.0f));

			m_edittedImage.setColor(x, y, newCol);
		}
	}
	m_edittedImage.update();
}

void ofApp::enableGaussianBlurFilter() {
	std::cout << "gaussianblur" << std::endl;
	applyConvolution3x3(Constants::CONVOLUTION_MAT_TYPE::GAUSSIAN_BLUR);
}

void ofApp::enableInvertFilter() {
	std::cout << "Inversion" << std::endl;
	ofColor tempCol;
	ofColor newCol;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			tempCol = m_origImg.getColor(x, y);

			newCol.set(	255.0f - tempCol.r,
						255.0f - tempCol.g,
						255.0f - tempCol.b);

			m_edittedImage.setColor(x, y, newCol);
		}
	}
	m_edittedImage.update();
}

void ofApp::enableSepiaFilter() {
	std::cout << "Sepia" << std::endl;
	ofColor tempCol;
	ofColor newCol;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			tempCol = m_origImg.getColor(x, y);
			
			newCol.set(	ofClamp(0.393f*tempCol.r + 0.769f*tempCol.g + 0.189f*tempCol.b, 0.0f, 255.0f),
						ofClamp(0.349f*tempCol.r + 0.686f*tempCol.g + 0.168f*tempCol.b, 0.0f, 255.0f),
						ofClamp(0.272f*tempCol.r + 0.536f*tempCol.g + 0.131f*tempCol.b, 0.0f, 255.0f));

			m_edittedImage.setColor(x, y, newCol);
		}
	}
	m_edittedImage.update();
}

void ofApp::enableHueRotateFilter() {
	std::cout << "Hue Rotation" << std::endl;
	ofColor Col;

	for (int y = 0; y < m_edittedImage.getHeight(); y++) {
		for (int x = 0; x < m_edittedImage.getWidth(); x++) {
			Col = m_origImg.getColor(x, y);

			float h = Col.getHueAngle() + 90;
			if (h > 360)
				h = h - 360;
			Col.setHueAngle(h);

			m_edittedImage.setColor(x, y, Col);
		}
	}
	m_edittedImage.update();
}


void ofApp::applyConvolution3x3(Constants::CONVOLUTION_MAT_TYPE matType) {
	std::cout << "convolution: " << std::endl;

	ofColor tempCol;
	ofColor newCol;
	ofVec3f sum;
	int neighbourX = 0;
	int neighbourY = 0;
	int startIndex = (int)matType * 9;
	int imgWidth = m_edittedImage.getWidth();
	int imgHeight = m_edittedImage.getHeight();

	for (int y = 0; y < imgHeight; y++) {
		for (int x = 0; x < imgWidth; x++) {
			sum.set(0.0f, 0.0f, 0.0f);

			//loop through picel and neighbours
			for (int k = startIndex; k < startIndex + 9; k++) {
				neighbourX = x + Constants::CONVOLUTION_KERNAL_CONVERT_X[k % 9];
				if (neighbourX < 0) {
					neighbourX = 0;
				}
				else if(neighbourX > imgWidth - 1){
					neighbourX = imgWidth - 1;
				}

				neighbourY = y + Constants::CONVOLUTION_KERNAL_CONVERT_Y[k % 9];
				if (neighbourY < 0) {
					neighbourY = 0;
				}
				else if (neighbourY > imgHeight - 1) {
					neighbourY = imgHeight - 1;
				}

				//get color of neighbour and multiply by factor
				tempCol = m_origImg.getColor(neighbourX, neighbourY);

				//sum kermal factors
				sum.x += tempCol.r * Constants::CONVOLUTION_MATS_3X3[k];
				sum.y += tempCol.g * Constants::CONVOLUTION_MATS_3X3[k];
				sum.z += tempCol.b * Constants::CONVOLUTION_MATS_3X3[k];
			}

			newCol.set(	ofClamp(sum.x, 0.0f, 255.0f),
						ofClamp(sum.y, 0.0f, 255.0f),
						ofClamp(sum.z, 0.0f, 255.0f));

			m_edittedImage.setColor(x, y, newCol);

		}
	}

	m_edittedImage.update(); 
}


void ofApp::openFile(ofFileDialogResult openFileResult) {

	std::cout << "getName(): " + openFileResult.getName() << std::endl;
	std::cout << "getPath(): " + openFileResult.getPath() << std::endl;

	ofFile file(openFileResult.getPath());

	if (file.exists()) {

		string fileExtension = ofToUpper(file.getExtension());

		if (fileExtension == "JPG" || fileExtension == "PNG") {
			originalFileExtension = fileExtension;
			std::cout << originalFileExtension << std::endl;
			m_origImg.load(openFileResult.getPath());
			m_edittedImage = m_origImg;
		}
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	if (show_eyedropper) {
		if (m_edittedImage.getWidth() > 1000 || m_edittedImage.getHeight() > 1000) {
			if ((x >= 0 && x <= m_edittedImage.getWidth() / 2) &&
				(y >= 0 && y <= m_edittedImage.getHeight() / 2)) {
				if (button == OF_MOUSE_BUTTON_LEFT) {
					pickCol = m_edittedImage.getColor(x * 2, y * 2);
				}
			}
		}else{
			if ((x >= 0 && x <= m_edittedImage.getWidth()) &&
				(y >= 0 && y <= m_edittedImage.getHeight())) {
				if (button == OF_MOUSE_BUTTON_LEFT) {
					pickCol = m_edittedImage.getColor(x, y);
				}
			}
		}
	}
}

void ofApp::mouseDragged(int x, int y, int button) {
	//only draw circles when dragging mouse
	if (set_br) {
		if (button == OF_MOUSE_BUTTON_LEFT) {
			ofSetColor(pickCol);
			ofDrawCircle(x, y, 50.0f);
		}
		if (button == OF_MOUSE_BUTTON_RIGHT) {
			ofSetColor(bgCol);
			ofDrawCircle(x, y, 50.0f);
		}
	}
}

void ofApp::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}



