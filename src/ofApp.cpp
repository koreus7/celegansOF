#include <regex>
#include "ofApp.h"
#include "imgui_utils.h"
#include "file_load.h"
#include "time_utils.h"
#include "git_utils.h"




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

    mainDrawArea.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    // TODO Free this memory
    // The image looks like the background so it doesn't show but its
    // not going to cause null pointer errors.
    ofSetColor(backgroundColor);
    state.focusedImage = new ofImage();
    state.focusedImage->allocate(128,128, OF_IMAGE_COLOR);

    experimentData.registerObject(beamSkeleton);
    experimentData.registerObject(beamSkeleton.parameters);
    experimentData.registerObject(experimentMetaData);
    experimentData.registerObject(tailSelector);
    experimentData.registerImage(&state.focusedImage, "focused_image");
    experimentData.registerImage(&screenCapture, "result");

}

//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

    mainDrawArea.begin();

    ofSetColor(backgroundColor);
    ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight());

	if (state.focusedImage)
    {
		
		ofSetColor(255, 255, 255);
		
		state.focusedImagePos.x = (ofGetWidth() - state.focusedImage->getWidth())*0.5f;
		state.focusedImagePos.y = (ofGetHeight() - state.focusedImage->getHeight())*0.5f;
		state.focusedImage->draw(state.focusedImagePos);
		beamSkeleton.draw(state.focusedImagePos.x, state.focusedImagePos.y);
		tailSelector.draw();

	}
    
    beamSkeleton.drawBeamPreview();

    mainDrawArea.end();

    ofSetColor(255,255,255);
    mainDrawArea.draw(0,0);

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

        if (ImGui::Button("Image Load", ImVec2(120, 20))) {

            loadImage();
		}

        if(ImGui::Button("Experiment Load", ImVec2(120,20)))
        {
            loadExperiment();
        }

        if(ImGui::Button("Log", ImVec2(120,20)))
        {
            showLogWindow = true;
        }


        if(showLogWindow) {

            ImGui::Begin("Log Experiment", &showLogWindow);
            ImGui::InputTextMultiline("Comments", experimentMetaData.commentsBuffer, experimentMetaData.COMMENTS_BUFFER_SIZE);
            ImGui::InputText("Name", experimentMetaData.nameBuffer, experimentMetaData.NAME_BUFFER_SIZE);
            if(ImGui::Button("Commit", ImVec2(100,20)))
            {
                commitExperiment();
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

void ofApp::loadImage()
{
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

void ofApp::loadExperiment()
{
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select Experiment JSON file");

    if (openFileResult.bSuccess)
    {
        string path = openFileResult.getPath();
        experimentData.deserializeFromFile(path);
    }
}

void ofApp::commitExperiment()
{
    delete screenCapture;
    screenCapture = new ofImage();

    ofPixels pixels;
    mainDrawArea.readToPixels(pixels);
    ofImage fullScreen = ofImage(pixels);

    screenCapture->cropFrom(fullScreen, (int)state.focusedImagePos.x, (int)state.focusedImagePos.y,
                            (int)state.focusedImage->getWidth(), (int)state.focusedImage->getHeight() );

    std::string experimentName = std::string(experimentMetaData.nameBuffer);

    // Alphanumeric separated by dashes spaces or dots.
    std::regex directoryNameRegex("[_a-zA-Z0-9\\-\\.]+");

    // If the name is empty or not a valid directory name then
    // the experiment is named by timestamp.
    if(!std::regex_match(experimentName, directoryNameRegex))
    {
        std::string formatedTime = TimeUtils::getCurrentTimeForFileName();
        experimentName = "experiment" + formatedTime;
        strcpy(experimentMetaData.nameBuffer, experimentName.c_str());
    }

    // If we have no current changes then
    // we will end up popping whatever was on the stash
    // so make sure we clear it first.
    GitUtils::clearStash();
    GitUtils::stashCurrentChanges();
    GitUtils::checkoutNewBranch(experimentName);
    GitUtils::applyStashedChanges();

    std::string directoryName = "experiment-data/" + experimentName + "/";
    ofDirectory::createDirectory(directoryName);
    experimentData.serializeAll(directoryName, "experiment.json");

    GitUtils::stageAllAndCommit(experimentName);
    GitUtils::checkoutMaster();
    GitUtils::popStashedChanges();

    GitUtils::pushBranchToOriginInBackgroundThread(experimentName);

    showLogWindow = false;
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
