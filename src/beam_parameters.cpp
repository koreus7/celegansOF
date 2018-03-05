#include "beam_parameters.h"

void BeamParameters::serialize(ofxJSONElement& root) const
{
    root["beamWidth"] = this->beamWidth;
    root["beamLength"] = this->beamLength;
    root["totalLength"] = this->totalLength;
    root["fineMeshScale"] = this->fineMeshScale;

}

void BeamParameters::deserialize(const ofxJSONElement& root)
{

    this->beamWidth = root["beamWidth"].asFloat();
    this->beamLength = root["beamLength"].asFloat();
    this->totalLength = root["totalLength"].asFloat();
    this->fineMeshScale = root["fineMeshScale"].asInt();

}

int BeamParameters::getUniqueId() const
{
    return 1;
}
