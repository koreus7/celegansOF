#include "beam_skeleton.h"


BeamSkeleton::~BeamSkeleton()
{
	delete scaledImage;
    ofRemoveListener(ofEvents().keyReleased, this, &BeamSkeleton::onKeyReleased);
}

void BeamSkeleton::onKeyReleased(ofKeyEventArgs &keyArgs)
{
    if(keyArgs.key == 'o')
    {
        this->hideAll = !this->hideAll;
    }

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

    tailPoint.setup("Tail Point");
    skeletonPoint.setup("Skeleton Vertex");

    ofAddListener(ofEvents().keyReleased, this, &BeamSkeleton::onKeyReleased);
}

const float BeamSkeleton::getSquaredError(ofVec2f beamStart, float angle, bool invertBeam)
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
				float beamSample = getBeamPixel(y, invertBeam);

				// Assuming grayscale so G, B and A channels are redundant.
				float error = beamSample - imageSample.r / 255.0f;

				errorSum += error * error;
			}

		}
	}

	return errorSum;

}

const float BeamSkeleton::getBeamPixel(float beamY, bool invertBeam)
{
    float beamPixel = 0.5f * (1 + cosf(((float)PI*beamY) / (parameters.beamWidth*0.5f)));
    //return invertBeam? 1.0f - beamPixel : beamPixel;
    return beamPixel/2.0f;
}

void BeamSkeleton::fitImage(const ofImage& image)
{
	delete scaledImage;
	scaledImage = new ofImage(image);
	scaledImage->resize((int)image.getWidth()*parameters.fineMeshScale, (int)image.getHeight()*parameters.fineMeshScale);

    totalAngleSteps = (int)floor(PI / angleGranularity);
	totalSteps = (int)floor((parameters.totalLength) / (0.5*(parameters.beamLength)));
	stepCount = 0;
	lastAngle = parameters.initialAngle;

	if (totalSteps == 0) 
	{
		ofLogError(__FUNCTION__) << "Skeleton fitting generated 0 steps for fit, decrease beam size.";
	}

    ofVec2f tailPointImageSpace =  tailPoint.pos - appState->focusedImagePos;

	polyLine.clear();
	polyLine.addVertex(ofVec2f(tailPointImageSpace));

	workingPoint = tailPointImageSpace * parameters.fineMeshScale;
}

bool BeamSkeleton::isFitDone()
{
	return stepCount >= totalSteps;
}


const float BeamSkeleton::getFanAngle(int pointIndex, int angleStep)
{
    return centerAngleAtPoint[pointIndex] - (float)PI/2.0f + angleGranularity*angleStep;
}


float BeamSkeleton::getErrorAtPointAngle(int point, int angleStep)
{
    return errorAtPointAngles[point*totalAngleSteps + angleStep];
}

void BeamSkeleton::setErrorAtPointAngle(int point, int angleStep, float error)
{
    errorAtPointAngles[point*totalAngleSteps + angleStep] = error;
}


void BeamSkeleton::step()
{
	float testAngle = 0;
    float nextAngle = 0;
	float minError  = 0;
    float testError = 0;

    centerAngleAtPoint[stepCount] = lastAngle;
    maxErrorAtPoint[stepCount] = 0;


	for (int j = 0; j < totalAngleSteps; j++)
	{
        testAngle = getFanAngle(stepCount, j);

        float invertError = getSquaredError(workingPoint, testAngle, true);
        float nonInvertError = getSquaredError(workingPoint, testAngle, false);

        bool invertWasChosenAtAngle = false;


        if(parameters.beamType == BeamParameters::BEAM_NON_INVERTED)
        {
            testError = nonInvertError;
        }
        else if (parameters.beamType == BeamParameters::BEAM_INVERTED)
        {
            testError = invertError;
            invertWasChosenAtAngle = true;
        }
        else if(parameters.beamType == BeamParameters::BEAM_MIXED)
        {
            if(invertError*(parameters.mixedBeamChoiceBias + 1) < nonInvertError)
            {
                testError = invertError;
                invertWasChosenAtAngle = true;
            }
            else
            {
                testError = nonInvertError;
            }

        }


        float angleScale = powf( abs(j - totalAngleSteps/2.0f)/totalAngleSteps, 4);
        testError+= angleScale*parameters.shallowAngleBias*100.0f;

		if (j == 0 || testError < minError)
		{
            invertedBeamWasChosenAtPoint[stepCount] = invertWasChosenAtAngle;
			minError = testError;
			nextAngle = testAngle;
		}

        if(testError > maxErrorAtPoint[stepCount])
        {
            maxErrorAtPoint[stepCount] = testError;
        }

        setErrorAtPointAngle(stepCount, j, testError);
	}


    minErrorAtPoint[stepCount] = minError;

	workingPoint.x = workingPoint.x + 0.5f*parameters.beamLength * parameters.fineMeshScale * cosf(nextAngle);
	workingPoint.y = workingPoint.y - 0.5f*parameters.beamLength * parameters.fineMeshScale * sinf(nextAngle);
	polyLine.addVertex(workingPoint / parameters.fineMeshScale);

	lastAngle = nextAngle;
	stepCount++;

}

void BeamSkeleton::draw(float x, float y)
{
    if(!hideAll)
    {
        ofTranslate(x, y);
        ofSetColor(255, 255, 255, 255);
        polyLine.draw();

        ofSetColor(0,255,0,255);

        if(showPoints)
        {
            for(int i = 0; i < polyLine.size(); i++)
            {
                ofPoint p = polyLine[i];
                ofDrawCircle(p, 2);
            }
        }

        if(fanShowState != HIDE_FAN)
        {
            for(int i = 0; i < stepCount; i++)
            {
                for(int j = 0; j < totalAngleSteps; j++)
                {
                    if(fanShowState == SHOW_ALL_FAN ||
                       (fanShowState == SHOW_SINGLE_FAN && closestVertexToSelectorIndex == i))
                    {
                        ofPoint p = polyLine[i];

                        float minErr = minErrorAtPoint[i];
                        float maxErr = maxErrorAtPoint[i];
                        float err = getErrorAtPointAngle(i,j);

                        int scaledError = (int)floor(255*(1.0 - (err - minErr)/(maxErr - minErr)));
                        ofSetColor(255, 0, 0, scaledError);
                        float angle = getFanAngle(i, j);
                        ofDrawLine(p.x,p.y, p.x + cosf(angle)*parameters.beamLength, p.y - sinf(angle)*parameters.beamLength);
                    }
                }
            }

        }

        for(int i = 0; i < stepCount - 1; i++)
        {
            if(invertedBeamWasChosenAtPoint[i])
            {
                ofPoint p1 = polyLine[i];
                ofPoint p2 = polyLine[i + 1];
                ofSetColor(0,0,255);
                ofDrawLine(p1, p2);
            }
        }

        ofTranslate(-x, -y);

        skeletonPoint.draw();
        tailPoint.draw();

    }
}

void BeamSkeleton::update()
{
    polyLine.getClosestPoint(skeletonPoint.pos - appState->focusedImagePos, &closestVertexToSelectorIndex);

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

struct HistogramGetterData
{
    int offset;
    const float * array;
};

float histogramGetter(void *data, int index)
{
    HistogramGetterData* d = (HistogramGetterData *)data;
    return d->array[d->offset + index];
}

void BeamSkeleton::injectGUI()
{
    ImGui::Begin("Beam Fit");
    ImGui::Image((void*)(unsigned int)beamPreviewFBO.getTexture().texData.textureID, ImVec2(MAX_BEAM_SIZE,MAX_BEAM_SIZE));
    ImGui::PushItemWidth(120.0f);
    ImGui::SliderFloat("Beam Width", &parameters.beamWidth, 1, MAX_BEAM_SIZE);
    ImGui::SliderFloat("Beam Length", &parameters.beamLength, 1, MAX_BEAM_SIZE);
    ImGui::DragFloat("Total Length", &parameters.totalLength, 1.0f, 1.0f,1000.0f,"%.3f");
    ImGui::SliderFloat("Shallow Angle Bias", &parameters.shallowAngleBias, 0.0f, 100.0f, "%.3f");
    ImGui::SliderInt("Fine mesh scale", &parameters.fineMeshScale, 1, 5);
    ImGui::SliderFloat("InitialAngle", &parameters.initialAngle, 0.0f, 2.0f*(float)PI, "%.3f");
    ImGui::PopItemWidth();
    ImGui::Checkbox("Show Progress", &showProgress);
    if (ImGui::Button("Run Fit", ImVec2(100, 20)))
    {
        fitImage(*appState->focusedImage);
    }
    ImGui::Text("Beam Type");
    ImGui::RadioButton("Standard", &parameters.beamType, BeamParameters::BEAM_NON_INVERTED);
    ImGui::RadioButton("Inverted", &parameters.beamType, BeamParameters::BEAM_INVERTED);
    ImGui::RadioButton("Mixed", &parameters.beamType, BeamParameters::BEAM_MIXED);
    ImGui::SliderFloat("Bias", &parameters.mixedBeamChoiceBias, -1.0f, 1.0f, "%.3f");
    ImGui::Spacing();
    ImGui::Text("Visualisation");
    ImGui::Checkbox("Show Points", &showPoints);
    ImGui::RadioButton("No Fan", &fanShowState, HIDE_FAN);
    ImGui::RadioButton("Single Fan", &fanShowState, SHOW_SINGLE_FAN);
    ImGui::RadioButton("All Fans", &fanShowState, SHOW_ALL_FAN);
    ImGui::End();

    if(fanShowState == SHOW_SINGLE_FAN && totalAngleSteps != 0)
    {
        ImGui::SetNextWindowContentSize(ImVec2(320,220));
        ImGui::Begin("Error Distribution");
        ImGui::Text(to_string(closestVertexToSelectorIndex).c_str());

        static HistogramGetterData histogramData;
        histogramData.offset = closestVertexToSelectorIndex*totalAngleSteps;
        histogramData.array = errorAtPointAngles;

        ImGui::PlotHistogram("", histogramGetter, (void *)&histogramData,
                             totalAngleSteps,
                             0,
                             "Error Distribution",
                            FLT_MIN,
                            FLT_MAX,
                            ImVec2(300,200));
        ImGui::End();
    }

    tailPoint.injectGUI();
    skeletonPoint.injectGUI();
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
