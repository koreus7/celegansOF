#pragma once
#include "serializable.h"
#include "ofxJSON.h"

class BeamParameters : public ISerializable
{
public:
    ~BeamParameters() = default;
    float initialAngle = 0.0f;
    float beamWidth = 30.0f;
    float beamLength = 60.0f;
    float totalLength = 1000.0f;
    int fineMeshScale = 2;
    void serialize(ofxJSONElement& root) const override;
    void deserialize(const ofxJSONElement& root) override;
    std::string getUniqueId() const override;
};
