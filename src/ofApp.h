#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "point_selector.h"
#include "beam_skeleton.h"
#include "app_state.h"
#include "serializable_library.h"
#include <string.h>
#include "experiment_metadata.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(float x, float y);
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg);
        void exit() override;

        void loadImage();
        void loadExperiment();
        void commitExperiment();

		ofxImGui::Gui gui;
		
		ImVec4 backgroundColor;
		

		int previewCount = 0;
		static const int NUMBER_PREVIEW_IMAGES = 12;
		ofImage previewImages[NUMBER_PREVIEW_IMAGES];
		GLuint previewImageIds[NUMBER_PREVIEW_IMAGES];
		string previewPaths[NUMBER_PREVIEW_IMAGES];


		ofFbo mainDrawArea;

		PointSelector tailSelector;

		BeamSkeleton beamSkeleton;

        AppState state;

        ofImage* screenCapture = new ofImage();
		SerializableLibrary experimentData;
        ExperimentMetadata experimentMetaData;

        bool showLogWindow = false;

		const static int PREVIEW_SIZE = 128;
};
