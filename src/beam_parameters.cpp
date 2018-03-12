#include "beam_parameters.h"

void BeamParameters::serialize(ofxJSONElement& root) const
{
    root["initialAngle"] = this->initialAngle;
    root["beamWidth"] = this->beamWidth;
    root["beamLength"] = this->beamLength;
    root["totalLength"] = this->totalLength;
    root["fineMeshScale"] = this->fineMeshScale;
    root["shallowAngleBias"] = this->shallowAngleBias;
    root["mixedBeamChoiceBias"] = this->mixedBeamChoiceBias;
    root["beamType"] = this->beamType;
}

void BeamParameters::deserialize(const ofxJSONElement& root)
{

    this->initialAngle = root["initialAngle"].asFloat();
    this->beamWidth = root["beamWidth"].asFloat();
    this->beamLength = root["beamLength"].asFloat();
    this->totalLength = root["totalLength"].asFloat();
    this->fineMeshScale = root["fineMeshScale"].asInt();
    this->shallowAngleBias = root["shallowAngleBias"].asFloat();
    this->mixedBeamChoiceBias = root["mixedBeamChoiceBias"].asFloat();
    this->beamType = root["beamType"].asInt();

}

std::string BeamParameters::getUniqueId() const
{
    return "BeamParameters";
}
