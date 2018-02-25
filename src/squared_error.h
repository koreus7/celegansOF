#pragma once

#include "ofMain.h"

class SquaredError
{
public:
	~SquaredError();
	void calcSquaredError(ofTexture texA, ofTexture texB);
	void setup();
	ofTexture& getResult();

private:
	ofFbo frameBuffer;
	ofShader shader;
};