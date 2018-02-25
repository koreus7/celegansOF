#pragma once
#include "ofMain.h"
#include "ofxImGui.h"

class BeamSkeleton
{
public:
	BeamSkeleton();
	~BeamSkeleton();
	void fitImage(const ofImage& image, ofVec2f tailPoint);
	bool isFitDone();
	void step();
	// Configure the beam size relative to the image.
	void configureBeamSize(float beamWidth, float beamLength, float totalLength);
	void draw(float x, float y);
	void injectGUI();
	ofFbo debugFBO;
	float angleStep = 0.2;

private:
	const float getSquaredError(ofVec2f beamStart, float angle);
	const float getBeamPixel(float beamY);

	ofImage* scaledImage;

	// Beam width in fine mesh space.
	float beamWidth = 30.0f;
	// Beam length in fine mesh space.
	float beamLength = 60.0f;
	// Total Length in fine mesh space.
	float totalLength = 600.0f;

	float lastAngle = 0.0f;

	int stepCount = 0;
	int totalSteps = 0;

	// The skeleton in image space.
	ofPolyline polyLine;

	const int FINE_MESH_SCALE = 3;
};

