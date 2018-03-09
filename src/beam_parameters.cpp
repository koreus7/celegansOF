#include "beam_parameters.h"

void BeamParameters::serialize(ofxJSONElement& root) const
{
    root["initialAngle"] = this->initialAngle;
    root["beamWidth"] = this->beamWidth;
    root["beamLength"] = this->beamLength;
    root["totalLength"] = this->totalLength;
    root["fineMeshScale"] = this->fineMeshScale;
}

void BeamParameters::deserialize(const ofxJSONElement& root)
{

    this->initialAngle = root["initialAngle"].asFloat();
    this->beamWidth = root["beamWidth"].asFloat();
    this->beamLength = root["beamLength"].asFloat();
    this->totalLength = root["totalLength"].asFloat();
    this->fineMeshScale = root["fineMeshScale"].asInt();

}

std::string BeamParameters::getUniqueId() const
{
    return "BeamParameters";
}
