#pragma once
#include "ofMain.h"
#include "ofxImGui.h"
#include "app_state.h"
#include "constants.h"
#include <string>
#include "beam_parameters.h"
#include "serializable.h"
#include "point_selector.h"

class BeamSkeleton : public ISerializable
{
public:
	~BeamSkeleton() override;
	void fitImage(const ofImage& image);
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

	float angleGranularity = 0.2f;

    BeamParameters parameters;
    PointSelector skeletonPoint;
    PointSelector tailPoint;

private:
	const float getSquaredError(ofVec2f beamStart, float angle);
	const float getBeamPixel(float beamY);
	const float getFanAngle(int pointIndex, int angleStep);

	ofImage* scaledImage;
    
    bool showProgress = true;

	int totalAngleSteps = 0;
	float lastAngle = 0.0f;
	int stepCount = 0;
	int totalSteps = 0;

	// The skeleton in image space.
	ofPolyline polyLine;


	const static int MAX_POINTS = 1028;
	const static int MAX_ANGLES = 64;

	// The angle around which the fan of beams
	// was tested at each point.
	float centerAngleAtPoint[MAX_POINTS];


	// Store the calculated squared error for choosing
	// a beam at each angle for every point tested.
	float errorAtPointAngles[MAX_POINTS*MAX_ANGLES];
    float maxErrorAtPoint[MAX_POINTS];
	float minErrorAtPoint[MAX_POINTS];

	float getErrorAtPointAngle(int point, int angleStep);
    void setErrorAtPointAngle(int point, int angleStep, float error);

	ofVec2f workingPoint;

    static const int MAX_BEAM_SIZE = 128;
    
    ofShader beamShader;
    ofFbo beamPreviewFBO;

	void onKeyReleased(ofKeyEventArgs & keyArgs);

    bool hideAll = false;
    bool showPoints = true;

    const static int HIDE_FAN = 0;
    const static int SHOW_SINGLE_FAN = 1;
    const static int SHOW_ALL_FAN = 2;
    int fanShowState = HIDE_FAN;

	unsigned closestVertexToSelectorIndex;
    
    AppState* appState;
};

