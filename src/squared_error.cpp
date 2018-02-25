#include "squared_error.h"

SquaredError::~SquaredError()
{
}

void SquaredError::setup() 
{
	shader.load("shadersGL3/squared_error");

	frameBuffer.allocate(1024, 1024, GL_RGB, 0);
}

void SquaredError::calcSquaredError(ofTexture texA, ofTexture texB)
{
	frameBuffer.begin();
	
	ofClear(255, 255, 255, 255);

	shader.begin();

	shader.setUniformTexture("tex0", texA, 1);
	shader.setUniformTexture("tex1", texB, 1);

	ofDrawRectangle(0, 0, texA.getWidth(), texA.getHeight());

	shader.end();
	frameBuffer.end();
}

ofTexture & SquaredError::getResult()
{
	return frameBuffer.getTextureReference();
}

