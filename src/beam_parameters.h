#pragma once
#include "serializable.h"
#include "ofxJSON.h"

class BeamParameters : public ISerializable
{
public:
    ~BeamParameters() = default;

    const static int BEAM_NON_INVERTED = 0;
    const static int BEAM_INVERTED = 1;
    const static int BEAM_MIXED = 2;

    float initialAngle = 0.0f;
    float beamWidth = 30.0f;
    float beamLength = 60.0f;
    float totalLength = 1000.0f;
    float shallowAngleBias = 0.0f;
    float mixedBeamChoiceBias = 0.0f;
    // TODO serialize this.
    float lowVarianceBias = 0.0f;
    int fineMeshScale = 2;
    int beamType = BEAM_MIXED;

    void serialize(ofxJSONElement& root) const override;
    void deserialize(const ofxJSONElement& root) override;
    std::string getUniqueId() const override;



};
