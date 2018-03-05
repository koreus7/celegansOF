#pragma once
#include "ofMain.h"
#include "ofxJSON.h"
#include <string>

class ISerializable
{
public:

    virtual ~ISerializable() = default;

    // Serialize.
    virtual void serialize(ofxJSONElement& root) const = 0;
    string serializeToJSONString() const;
    void serializeToFile(const string& fileName) const;

    // Deserialize.
    virtual void deserialize(const ofxJSONElement& root) = 0;
    void deserializeFile(const string& fileName);
    void deserializeJSONString(const string& data);

    virtual int getUniqueId() const = 0;

};
