#pragma once
#include "ofMain.h"
#include "ofxImGui.h"
#include "app_state.h"
#include "constants.h"
#include <string>
#include "beam_parameters.h"
#include "serializable.h"

class BeamSkeleton : public ISerializable
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

	void serialize(ofxJSONElement &root) const override;

	void deserialize(const ofxJSONElement &root) override;

	std::string getUniqueId() const override;

	ofFbo debugFBO;
	float angleStep = 0.2;
    BeamParameters parameters;

private:
	const float getSquaredError(ofVec2f beamStart, float angle);
	const float getBeamPixel(float beamY);

	ofImage* scaledImage;
    
    bool showProgress = true;

	float lastAngle = 0.0f;
	int stepCount = 0;
	int totalSteps = 0;

	// The skeleton in image space.
	ofPolyline polyLine;

	ofVec2f workingPoint;

	static const int fineMeshScale = 3;
    static const int MAX_BEAM_SIZE = 128;
    
    ofShader beamShader;
    ofFbo beamPreviewFBO;
    
    
    AppState* appState;

};

