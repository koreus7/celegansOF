#pragma once
#include "ofMain.h"
#include "ofxImGui.h"
#include "app_state.h"
#include "constants.h"
#include <string>
#include "beam_parameters.h"
#include "serializable.h"
#include "point_selector.h"
#include "beam_tweaker.h"

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
	const float getSquaredError(ofVec2f beamStart, float angle, bool invertBeam);
	const float getBeamPixel(float beamY, bool invertBeam);
	const float getFanAngle(int pointIndex, int angleStep);

    ofImage* originalImage;
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
	bool  invertedBeamWasChosenAtPoint[MAX_POINTS];
	ofImage* beamSlices[MAX_ANGLES];

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

    BeamTweaker tweaker;

    void populateBeamSlices();

    /// Iterate over the beam, sampling the image for each pixel in the beam rectangle.
    /// \param beamStart Start position of the beam in image space.
    /// \param angle Angle of the beam from the horizonal, anticlockwise.
    /// \param beamScale Scale of the beam relative to the size in parameters
    /// \param image Image to sample from
    /// \param iteratorState State pointer that is passed to the doIteratorFunction
    /// \param doIteration function that does the actual work each iteration.
    /// \param outOfBoundsAction function that is called if a beam pixel is out of the image bounds.
    const void iterateOverBeam(ofVec2f beamStart, float angle, float beamScale, ofImage* image, void * iteratorState,
           void (BeamSkeleton::*doIteration) (ofColor, ofVec2f, ofVec2f, void *), void (*outOfBoundsAction)(void *) );


    void squaredErrorIteration(ofColor imageSample, ofVec2f beamFrame, ofVec2f beamPosition, void *iteratorData);

    void populateBeamSlicesIteration(ofColor imageSample, ofVec2f beamFrame, ofVec2f beamPosition, void *iteratorData);
};

