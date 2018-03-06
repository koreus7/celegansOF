#pragma once
#include <string>
#include "serializable.h"
#include "ofMain.h"
#include "ofxJSON.h"

class SerializableLibrary
{
public:

    // Serialize.
    std::string serializeAllToString() const;
    void serializeAll(ofxJSONElement& root) const;
    void serializeAll(std::string folder, const std::string& name);

    // Deserialize.
    void deserializeFromJSONString(const std::string& serialisedString);
    void deserializeFromFile(const std::string& fileName);

    void registerObject(ISerializable& object);
    void registerImage(ofImage** image, std::string name);
private:
    void writeToAllObjects();
    void writeToObject(int index);
    void writeToAllImages();
    void writeToImage(int index);
    const static int MAX_OBJECTS = 1024;
    const static int MAX_IMAGES = 128;
    ISerializable* objects[MAX_OBJECTS];
    ofImage** images[MAX_IMAGES];
    std::string imageNames[MAX_IMAGES];
    std::string imageDirectory;
    ofxJSONElement deserializedData;
    int objectCount = 0;
    int imageCount = 0;
};

