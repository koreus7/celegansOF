#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "serializable.h"

class PointSelector : public ISerializable
{
public:
	~PointSelector();
	void setup(std::string name);
	void draw();
	ofVec2f pos;

	void serialize(ofxJSONElement &root) const override;

	std::string getUniqueId() const override;

	void deserialize(const ofxJSONElement &root) override;

	ofColor activeColor = ofColor(0,255,0);
	ofColor notActiveColor = ofColor(255,0,0);
	bool active = true;
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void injectGUI();
private:
	std::string name;
	const int CROSS_SIZE = 8;
	const int MAX_SQUARE_DELTA = 3;
	int selectorIndex = 0;
	static int activeSelector;
	static int selectorCount;
	bool guiVisible = true;
	ofVec2f pressPosition;
};

