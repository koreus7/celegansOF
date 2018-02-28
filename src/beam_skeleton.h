#pragma once
#include "ofMain.h"
#include "ofxImGui.h"
#include "app_state.h"
#include "constants.h"
#include <string>

class BeamSkeleton
{
public:
	~BeamSkeleton();
	void fitImage(const ofImage& image, ofVec2f tailPoint);
	bool isFitDone();
	void step();
    void setup(AppState* appConfig);
    void update();
    void draw(float x, float y);
    void drawBeamPreview();
	void injectGUI();
	ofFbo debugFBO;
	float angleStep = 0.2;

private:
	const float getSquaredError(ofVec2f beamStart, float angle);
	const float getBeamPixel(float beamY);

	ofImage* scaledImage;
    
    bool showProgress = true;

	// Beam width in fine mesh space.
	float beamWidth = 30.0f;
	// Beam length in fine mesh space.
	float beamLength = 60.0f;
	
    float totalLengthMicrons = 600.0f;
    // Total Length in fine mesh space.
	float totalLength = 600.0f;

	float lastAngle = 0.0f;

	int stepCount = 0;
	int totalSteps = 0;

	// The skeleton in image space.
	ofPolyline polyLine;

	ofVec2f workingPoint;

	static const int FINE_MESH_SCALE = 2;
    static const int MAX_BEAM_SIZE = 256;
    
    ofShader beamShader;
    ofTexture beamPreviewTexture;
    ofFbo beamPreviewFBO;
    
    
    AppState* appState;
    
    //TODO remove this
    ofImage testimg;
    int texID;
};

