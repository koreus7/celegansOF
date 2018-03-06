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


    // TODO Free this memory
    // The image looks like the background so it doesn't show but its
    // not going to cause null pointer errors.
    ofSetColor(backgroundColor);
    state.focusedImage = new ofImage();
    state.focusedImage->allocate(128,128, OF_IMAGE_COLOR);
    state.focusedImage->clear();


    experimentData.registerObject(beamSkeleton.parameters);
    experimentData.registerObject(experimentMetaData);
    experimentData.registerObject(tailSelector);
    experimentData.registerImage(&state.focusedImage, "focused_image");

//    if(ofFile::doesFileExist("config.json"))
//	{
//		config.deserializeFromFile("config.json");
//	}
//


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
		if (ImGui::Button("Load", ImVec2(120, 20))) {

			ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a jpg or png worm image.");
			
			if (openFileResult.bSuccess)
            {
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

        if(ImGui::Button("Log Experiment", ImVec2(120,20)))
        {
            showLogWindow = true;
        }

        if(ImGui::Button("Load Experiment", ImVec2(120,20)))
        {
            ofFileDialogResult openFileResult = ofSystemLoadDialog("Select Experiment JSON file");

            if (openFileResult.bSuccess)
            {
                string path = openFileResult.getPath();
                experimentData.deserializeFromFile(path);
                beamSkeleton.fitImage(*state.focusedImage, *(state.selectedTailPos) - state.focusedImagePos);
            }
        }

        if(showLogWindow) {

            ImGui::Begin("Log Experiment", &showLogWindow);
            ImGui::InputTextMultiline("Comments", experimentMetaData.commentsBuffer, experimentMetaData.COMMENTS_BUFFER_SIZE);
            if(ImGui::Button("Commit", ImVec2(100,20)))
            {
                auto now = std::chrono::system_clock::now();
                const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
                struct tm * t = localtime( &nowTime );
                char* buffer = new char[64];
                strftime(buffer, 64, "%Y-%m-%d-%H-%M-%S", t);
                std::string formatedTime = std::string(buffer);
                experimentData.serializeAll("experiment-data/experiment" + formatedTime + ".json");
                showLogWindow = false;
            }
            ImGui::End();

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

void ofApp::exit()
{
    ofBaseApp::exit();
}
