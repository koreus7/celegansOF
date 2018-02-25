#pragma once

#include "ofMain.h";
#include "ofxImGui.h";

class PointSelector
{
public:
	//PointSelector();
	~PointSelector();
	void setup();
	void draw();
	ofVec2f pos;
	ofColor activeColor = ofColor(255,0,0);
	ofColor notActiveColor = ofColor(255,255,0);
	bool active = true;
	void mousePressed(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void injectGUI();

	//TODO REMOVE
	bool clicked;

private:
	const int CROSS_SIZE = 8;
	const int MAX_SQUARE_DELTA = 3;
	bool guiVisible = true;
	ofVec2f pressPosition;
};

