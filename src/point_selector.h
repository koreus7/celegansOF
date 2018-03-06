#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "serializable.h"

class PointSelector : public ISerializable
{
public:
	//PointSelector();
	~PointSelector();
	void setup();
	void draw();
	ofVec2f pos;

	void serialize(ofxJSONElement &root) const override;

	int getUniqueId() const override;

	void deserialize(const ofxJSONElement &root) override;

	ofColor activeColor = ofColor(255,0,0);
	ofColor notActiveColor = ofColor(255,255,0);
	bool active = true;
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void injectGUI();

private:
	const int CROSS_SIZE = 8;
	const int MAX_SQUARE_DELTA = 3;
	bool guiVisible = true;
	ofVec2f pressPosition;
};

