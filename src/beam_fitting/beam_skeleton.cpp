#include "beam_skeleton.h"


BeamSkeleton::BeamSkeleton()
{
	//debugImage.allocate();
	debugFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
}

BeamSkeleton::~BeamSkeleton()
{
	delete scaledImage;
	scaledImage = nullptr;
}


const float BeamSkeleton::getSquaredError(ofVec2f beamStart, float angle)
{

	const int BEAM_RES_X = floor(beamLength);
	const int BEAM_RES_Y = floor(beamWidth);

	const int imageWidth = scaledImage->getWidth();
	const int imageHeight = scaledImage->getHeight();

	const ofPixels& imagePixels = scaledImage->getPixels();

	float errorSum = 0;


	// Here x and y refer to the local frame of the beam.
	// where the x axis lies along the length of the beam
	for (int x = 0; x < BEAM_RES_X; x++)
	{
		for (int y = -BEAM_RES_Y/2; y < BEAM_RES_Y/2; y++)
		{
			float length = sqrt(x*x + y*y);

			int imageX = floor(cos(atan2(y, x) + angle)*length + beamStart.x);
			int imageY = floor(-sin(atan2(y, x) + angle)*length + beamStart.y);

			// If the pixel is out of bounds.
			if (imageX < 0 || imageY < 0 || imageX > imageWidth || imageY > imageHeight) 
			{
				if (imageX > 0 && imageY > 0 && imageX < debugFBO.getWidth() && imageY < debugFBO.getHeight())
				{
					debugFBO.begin();
					ofSetColor(255, 0, 0);
					ofDrawRectangle(imageX, imageY, 1, 1);
					debugFBO.end();
				}
				//ofLogVerbose(__FUNCTION__) << "Pixel out of range for error calc " << imageX << " " << imageY;
				errorSum += 2;
			}
			else
			{
				ofColor imageSample = imagePixels.getColor(imageX, imageY);
				float beamSample = getBeamPixel(y);

				// Assuming grayscale so G, B and A channels are redundant.
				float error = beamSample - imageSample.r / 255.0f;

				errorSum += error * error;
			}

		}
	}

	return errorSum;

}

const float BeamSkeleton::getBeamPixel(float beamY)
{
	
	//beamAngle = PI - beamAngle;

	////Convert into local frame.
	//float r = fabs(tan(beamAngle)*(testPoint.x - beamstart.x) + beamstart.y - testPoint.y) / sqrt(1 + tan(beamAngle)*tan(beamAngle));
	
	return 0.5 * (1 + cos((PI*beamY) / (beamWidth*0.5)));
}

void BeamSkeleton::fitImage(const ofImage& image, ofVec2f tailPoint)
{

	if (scaledImage) {

		delete scaledImage;

	}

	scaledImage = new ofImage(image);
	scaledImage->resize(image.getWidth()*FINE_MESH_SCALE, image.getHeight()*FINE_MESH_SCALE);


	totalSteps = floor(totalLength / (0.5*beamLength));
	stepCount = 0;


	polyLine.clear();
	polyLine.addVertex(ofVec2f(tailPoint));

	debugFBO.begin();
	ofSetColor(0, 255, 0);
	ofDrawRectangle(1, 1, 1, 1);
	debugFBO.end();
	
}

bool BeamSkeleton::isFitDone()
{
	return stepCount >= totalSteps;
}

void BeamSkeleton::step()
{
	float testAngle, nextAngle = 0;
	int angleSteps = floor(PI / angleStep);
	float minError, testError;

	ofVec2f currentPoint = polyLine[polyLine.size() -1];

	for (int j = 0; j < angleSteps; j++)
	{
		if (j % 2 == 0)
		{
			testAngle = lastAngle + angleStep*(j / 2);
		}
		else
		{
			testAngle = lastAngle - angleStep*(j / 2);
		}

		testError = getSquaredError(currentPoint, testAngle);

		ofLogVerbose(__FUNCTION__) << "Err" << testError;

		if (j == 0 || testError < minError)
		{
			minError = testError;
			nextAngle = testAngle;
		}

	}

	currentPoint.x = currentPoint.x + 0.5*beamLength * cos(nextAngle);
	currentPoint.y = currentPoint.y - 0.5*beamLength * sin(nextAngle);
	polyLine.addVertex(currentPoint/FINE_MESH_SCALE);

	lastAngle = nextAngle;

	ofImage img;
	img.allocate(debugFBO.getWidth(), debugFBO.getHeight(), OF_IMAGE_COLOR);
	debugFBO.readToPixels(img.getPixels());
	img.save("debug.png", OF_IMAGE_QUALITY_BEST);
}

void BeamSkeleton::configureBeamSize(float beamWidth, float beamLength, float totalLength)
{
	this->beamWidth = beamWidth * FINE_MESH_SCALE;
	this->beamLength = beamLength * FINE_MESH_SCALE;
	this->totalLength = totalLength * FINE_MESH_SCALE;
}

void BeamSkeleton::draw(float x, float y)
{

	ofTranslate(-x, -y);
	polyLine.draw();
	ofTranslate(x, y);

}

void BeamSkeleton::injectGUI()
{
}
