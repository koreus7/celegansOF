#include "ofApp.h"
#include "imgui_utils.h"
#include "math.h"
#include "file_load.h"


bool themeSet = false;

//--------------------------------------------------------------
void ofApp::setup()
{

	beamShader.load("shadersGL3/beam");
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	//required call
	gui.setup();

	ImGui::GetIO().MouseDrawCursor = false;

	backgroundColor = ofColor(114, 144, 154);
	beamWidth = 30;
	beamLength = 20;

	beamPreviewFBO = ofFbo();
	beamPreviewFBO.allocate(MAX_BEAM_SIZE, MAX_BEAM_SIZE, GL_RGBA);
	beamPreviewTexture = beamPreviewFBO.getTexture();

	tailSelector.setup();


}

//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	ofSetBackgroundColor(backgroundColor);

	ofSetColor(255, 255, 255);
	beamPreviewTexture.draw(10, 10);

	if (focusedImage) {
		
		ofSetColor(255, 255, 255);
		
		float x = (ofGetWidth() - focusedImage->getWidth())*0.5f;
		float y = (ofGetHeight() - focusedImage->getHeight())*0.5f;
		focusedImage->draw(x, y);
		beamSkelton.draw(x, y);
		tailSelector.draw();

	}

	gui.begin();

	tailSelector.injectGUI();

	if (!themeSet) {
		setupTheme(ImGui::GetStyle());
		themeSet = true;
	}

	{
		ImGui::Text("Worm Beam");
		//this will change the app background color
		//ImGui::ColorEdit3("Background Color", (float*)&backgroundColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::Button("Load", ImVec2(100, 20))) {

			ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a jpg or png worm image.");
			
			if (openFileResult.bSuccess) {

				string path = openFileResult.getPath();
				ofImage image = loadFromPath(path);
				ofImage previewImage = image;
				previewImage.resize(PREVIEW_SIZE, PREVIEW_SIZE);

				previewImages[previewCount] = image;
				previewImageIds[previewCount] = gui.loadImage(previewImage);
				previewPaths[previewCount] = path;

				previewCount = (previewCount + 1) % NUMBER_PREVIEW_IMAGES;

			}

		}

		for (int i = 0; i < previewCount; i++) {

			if (ImGui::ImageButton((ImTextureID)(uintptr_t)previewImageIds[i], ImVec2(PREVIEW_SIZE, PREVIEW_SIZE))) {

				focusedImage = &previewImages[i];
			}

		}

	}

	{

		ImGui::SetNextWindowSize(ofVec2f(512, 512), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Beam Fit");
		ImGui::SliderInt("Beam Width", &beamWidth, 1, MAX_BEAM_SIZE);
		ImGui::SliderInt("Beam Length", &beamLength, 1, MAX_BEAM_SIZE);
		ImGui::DragFloat("Micron to Pixel Ratio", &micronToPixelRatio, 1.0f, 0.0001f, 100.0f, "%.5f");
		if (ImGui::Button("Run Fit", ImVec2(100, 20)))
		{
			beamSkelton.fitImage(*focusedImage, tailSelector.pos);
		}

		ImGui::End();
	
	}


	if (!beamSkelton.isFitDone())
	{
		beamSkelton.step();
	}



	gui.end();


	beamPreviewFBO.begin();
	beamShader.begin();
	beamShader.setUniform1f("beamWidth", beamWidth);
	beamShader.setUniform1f("beamLength", beamLength);
	ofDrawRectangle(0, 0, 256, 256);
	beamShader.end();
	beamPreviewFBO.end();

	beamSkelton.configureBeamSize(beamWidth, beamLength, micronToPixelRatio*CELEGANS_LENGTH_MICRONS);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	ofLogVerbose(__FUNCTION__) << key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

	ofLogVerbose(__FUNCTION__) << key;

}


void ofApp::mouseScrolled(float x, float y)
{
	ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

	

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}