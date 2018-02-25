#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "point_selector.h"
#include "beam_skeleton.h"
#include <string.h>


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseScrolled(float x, float y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxImGui::Gui gui;
		
		ImVec4 backgroundColor;
		

		static const int MAX_BEAM_SIZE = 256;
		int beamWidth;
		int beamLength;
		
		float micronToPixelRatio = 0.65f;

		const int CELEGANS_LENGTH_MICRONS = 600;

		ofShader beamShader;
		ofTexture beamPreviewTexture;
		ofFbo beamPreviewFBO;

		int previewCount = 0;
		static const int NUMBER_PREVIEW_IMAGES = 12;
		ofImage previewImages[NUMBER_PREVIEW_IMAGES];
		GLuint previewImageIds[NUMBER_PREVIEW_IMAGES];
		string previewPaths[NUMBER_PREVIEW_IMAGES];

		PointSelector tailSelector;

		BeamSkeleton beamSkelton;

		ofImage* focusedImage;
		ofVec2f focusedImagePos;

		const static int PREVIEW_SIZE = 128;
};
