#include "point_selector.h"



void PointSelector::setup() {

	ofAddListener(ofEvents().mousePressed, this, &PointSelector::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &PointSelector::mouseReleased);
}


//PointSelector::PointSelector()
//{
//
//}


PointSelector::~PointSelector()
{
	ofRemoveListener(ofEvents().mousePressed, this, &PointSelector::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &PointSelector::mouseReleased);
}

void PointSelector::draw()
{
	ofSetLineWidth(1.0f);

	if (active) 
	{
		ofSetColor(activeColor);
	} 
	else 
	{
		ofSetColor(notActiveColor);
	}

	ofDrawLine(pos.x - CROSS_SIZE/2, pos.y, pos.x + CROSS_SIZE/2, pos.y);
	ofDrawLine(pos.x, pos.y - CROSS_SIZE/2, pos.x, pos.y + CROSS_SIZE/2);
}

void PointSelector::mousePressed(ofMouseEventArgs & mouse)
{

	ofLogVerbose(__FUNCTION__) << "MOUSE PRESSED";

	if (active) 
	{
		pressPosition.x = mouse.x;
		pressPosition.y = mouse.y;
	}

}

void PointSelector::mouseReleased(ofMouseEventArgs & mouse)
{
	ofLogVerbose(__FUNCTION__) << "MOUSE RELEASED";

	if (active && !ImGui::IsMouseHoveringAnyWindow())
	{
		float deltaX = fabs(mouse.x - pressPosition.x);
		float deltaY = fabs(mouse.y - pressPosition.y);

		float deltaMagnitudeSquared = deltaX*deltaX + deltaY*deltaY;

		if (deltaMagnitudeSquared < MAX_SQUARE_DELTA)
		{
			pos.x = mouse.x;
			pos.y = mouse.y;
		}

	}
}

void PointSelector::injectGUI()
{

	ImGui::Begin("Point Selector", &guiVisible);

	if (active) {
		ImGui::TextColored(ofColor(0, 255, 0), "ACTIVE");
	} else {
		ImGui::TextColored(ofColor(255, 0, 0), "DISABLED");
	}

	ImGui::Text("Selected Point X: %.3f  Y: %.3f", pos.x, pos.y);
	ImGui::End();

}

void PointSelector::serialize(ofxJSONElement &root) const
{
	root["x"] = pos.x;
	root["y"] = pos.y;
}

void PointSelector::deserialize(const ofxJSONElement &root)
{
	pos.x = root.get("x", Json::Value(0.0f)).asFloat();
	pos.y = root.get("y", Json::Value(0.0f)).asFloat();
}

int PointSelector::getUniqueId() const
{
	return 90;
}
