#include "ofApp.h"
#include "imgui_utils.h"
#include "math.h"
#include "file_load.h"




bool themeSet = false;

//--------------------------------------------------------------
void ofApp::setup()
{

	ofSetLogLevel(OF_LOG_VERBOSE);

	//required call
	gui.setup();

	ImGui::GetIO().MouseDrawCursor = false;

	backgroundColor = ofColor(114, 144, 154);

	tailSelector.setup();
    beamSkeleton.setup(&state);
    
    state.selectedTailPos = &tailSelector.pos;
    
}

//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	ofSetBackgroundColor(backgroundColor);

	if (state.focusedImage) {
		
		ofSetColor(255, 255, 255);
		
		state.focusedImagePos.x = (ofGetWidth() - state.focusedImage->getWidth())*0.5f;
		state.focusedImagePos.y = (ofGetHeight() - state.focusedImage->getHeight())*0.5f;
		state.focusedImage->draw(state.focusedImagePos);
		beamSkeleton.draw(state.focusedImagePos.x, state.focusedImagePos.y);
		tailSelector.draw();

	}
    
    beamSkeleton.drawBeamPreview();

	gui.begin();

	tailSelector.injectGUI();
    beamSkeleton.injectGUI();

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

				state.focusedImage = &previewImages[i];
			}

		}

	}
    
	gui.end();
    
    beamSkeleton.update();
    
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
