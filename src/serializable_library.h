#pragma once
#include <string>
#include "serializable.h"
#include "ofxJSON.h"

class SerializableLibrary
{
public:

    std::string serializeAllToString() const;
    void serializeAll(ofxJSONElement& root) const;
    void serializeAll(const std::string& fileName) const;
    void registerObject(ISerializable& object);
private:
    std::string name;
    const static int MAX_OBJECTS = 1024;
    ISerializable* objects[MAX_OBJECTS];
    int objectCount = 0;
};

