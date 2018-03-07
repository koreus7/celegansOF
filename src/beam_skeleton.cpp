#include "beam_skeleton.h"


BeamSkeleton::~BeamSkeleton()
{
	delete scaledImage;
}

void BeamSkeleton::setup(AppState* appState)
{
    this->appState = appState;
    
    beamShader.load("shadersGL3/beam");

	ofFbo::Settings fboSettings;
	fboSettings.width = MAX_BEAM_SIZE;
	fboSettings.height = MAX_BEAM_SIZE;
	fboSettings.internalformat = GL_RGBA;

	// This is vital for the texture ID to be recognised by ImGui.
	fboSettings.textureTarget = GL_TEXTURE_2D;

    beamPreviewFBO.allocate(fboSettings);
}

const float BeamSkeleton::getSquaredError(ofVec2f beamStart, float angle)
{

	const int BEAM_RES_X = (int)floor(parameters.beamLength);
	const int BEAM_RES_Y = (int)floor(parameters.beamWidth);

	const int imageWidth = (int)scaledImage->getWidth();
	const int imageHeight = (int)scaledImage->getHeight();

	const ofPixels& imagePixels = scaledImage->getPixels();

	float errorSum = 0;


	// Here x and y refer to the local frame of the beam.
	// where the x axis lies along the length of the beam
	for (int x = 0; x < BEAM_RES_X; x++)
	{
		for (int y = -BEAM_RES_Y/2; y < BEAM_RES_Y/2; y++)
		{
			float length = sqrtf(x*x + y*y);

			int imageX = (int)floor(cosf(atan2f(y, x) + angle)*length + beamStart.x);
			int imageY = (int)floor(-sinf(atan2f(y, x) + angle)*length + beamStart.y);

			// If the pixel is out of bounds.
			if (imageX < 0 || imageY < 0 || imageX >= imageWidth || imageY >= imageHeight) 
			{
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
	return 1.0f - 0.5f * (1 + cosf(((float)PI*beamY) / (parameters.beamWidth*0.5f)));
}

void BeamSkeleton::fitImage(const ofImage& image, ofVec2f tailPoint)
{
	delete scaledImage;
	scaledImage = new ofImage(image);
	scaledImage->resize((int)image.getWidth()*parameters.fineMeshScale, (int)image.getHeight()*parameters.fineMeshScale);

	totalSteps = (int)floor(parameters.totalLength / (0.5*parameters.beamLength));
	stepCount = 0;

	if (totalSteps == 0) 
	{
		ofLogError(__FUNCTION__) << "Skeleton fitting generated 0 steps for fit, decrease beam size.";
	}


	polyLine.clear();
	polyLine.addVertex(ofVec2f(tailPoint));

	workingPoint = tailPoint * parameters.fineMeshScale;
}

bool BeamSkeleton::isFitDone()
{
	return stepCount >= totalSteps;
}

void BeamSkeleton::step()
{
	float testAngle, nextAngle = 0;
	int angleSteps = (int)floor(PI / angleStep);
	float minError, testError;

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

		testError = getSquaredError(workingPoint, testAngle);

		if (j == 0 || testError < minError)
		{
			minError = testError;
			nextAngle = testAngle;
		}

	}

	workingPoint.x = workingPoint.x + 0.5f*parameters.beamLength * cosf(nextAngle);
	workingPoint.y = workingPoint.y - 0.5f*parameters.beamLength * sinf(nextAngle);
	polyLine.addVertex(workingPoint / parameters.fineMeshScale);

	lastAngle = nextAngle;

	ofImage img;
	stepCount++;
}

void BeamSkeleton::draw(float x, float y)
{

	ofTranslate(x, y);
	ofSetColor(255, 255, 255, 255);
	polyLine.draw();
	ofTranslate(-x, -y);

}

void BeamSkeleton::update()
{
    
    if(showProgress) {
        
        // One step a frame.
        if(!isFitDone())
        {
            step();
        }
        
    } else {
        
        // All steps in one frame.
        while (!isFitDone())
        {
            step();
        }
        
    }
}

void BeamSkeleton::drawBeamPreview()
{
    beamPreviewFBO.begin();
    beamShader.begin();
    beamShader.setUniform1f("beamWidth", parameters.beamWidth);
    beamShader.setUniform1f("beamLength", parameters.beamLength);
    beamShader.setUniform1f("size", MAX_BEAM_SIZE);
    ofDrawRectangle(0, 0, MAX_BEAM_SIZE, MAX_BEAM_SIZE);
    beamShader.end();
    beamPreviewFBO.end();
}

void BeamSkeleton::injectGUI()
{
    ImGui::Begin("Beam Fit");
    ImGui::Image((void*)(unsigned int)beamPreviewFBO.getTexture().texData.textureID, ImVec2(MAX_BEAM_SIZE,MAX_BEAM_SIZE));
    ImGui::PushItemWidth(120.0f);
    ImGui::SliderFloat("Beam Width", &parameters.beamWidth, 1, MAX_BEAM_SIZE);
    ImGui::SliderFloat("Beam Length", &parameters.beamLength, 1, MAX_BEAM_SIZE);
    ImGui::DragFloat("Total Length", &parameters.totalLength, 1.0f, 1.0f,1000.0f,"%.3f");
    ImGui::PopItemWidth();
    ImGui::Checkbox("Show Progress", &showProgress);
    if (ImGui::Button("Run Fit", ImVec2(100, 20)))
    {
        fitImage(*appState->focusedImage, *(appState->selectedTailPos) - appState->focusedImagePos);
    }
    ImGui::End();
    
}

void BeamSkeleton::serialize(ofxJSONElement &root) const
{
	Json::Value skeletonObj = Json::Value(Json::arrayValue);

	for(int i = 0; i < polyLine.size(); i++)
	{
		ofPoint p = polyLine[i];

		Json::Value pointX = Json::Value(p.x);
		Json::Value pointY = Json::Value(p.y);

		skeletonObj.append(pointX);
		skeletonObj.append(pointY);

	}

	root["skeletonPoints"] = skeletonObj;
}

void BeamSkeleton::deserialize(const ofxJSONElement &root)
{
	if(root.isMember("skeletonPoints"))
	{

		polyLine.clear();

		Json::Value skeletonPoints = root.get("skeletonPoints", Json::Value(Json::arrayValue));

		for(int i = 0; i < skeletonPoints.size() - 1; i+=2)
		{
			float x = skeletonPoints[i].asFloat();
			float y = skeletonPoints[i+1].asFloat();

			polyLine.addVertex(x, y);

		}

	}

}

std::string BeamSkeleton::getUniqueId() const
{
	return "BeamSkeleton";
}
