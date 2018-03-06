#include "serializable_library.h"


void SerializableLibrary::registerObject(ISerializable& object)
{
    objects[objectCount] = &object;
    writeToObject(objectCount);
    objectCount++;
}

void SerializableLibrary::serializeAll(std::string folder, const std::string& name)
{
    ofxJSONElement root;
    imageDirectory = folder;
    serializeAll(root);
    root.save(folder + name);
}

std::string SerializableLibrary::serializeAllToString() const
{
    ofxJSONElement root;
    serializeAll(root);
    return root.toStyledString();
}

void SerializableLibrary::serializeAll(ofxJSONElement& root) const
{
    for(int i = 0; i < objectCount; i++)
    {
        ofxJSONElement object;
        objects[i]->serialize(object);
        root[objects[i]->getUniqueId()] = object;
    }


    if(imageCount)
    {
        ofxJSONElement imagesIndex;
        for(int i = 0; i < imageCount; i++)
        {
            std::string fileName = imageDirectory + imageNames[i] + ".png";
            imagesIndex[imageNames[i]] = fileName;
            (*images[i])->save(fileName, OF_IMAGE_QUALITY_BEST);
        }

        root["images"] = imagesIndex;
    }

}

void SerializableLibrary::deserializeFromJSONString(const std::string& serialisedString)
{
    deserializedData = ofxJSONElement(serialisedString);
    writeToAllObjects();
    writeToAllImages();
}

void SerializableLibrary::deserializeFromFile(const std::string &fileName)
{
    deserializedData = ofxJSONElement();
    deserializedData.open(fileName);
    writeToAllObjects();
    writeToAllImages();
}

void SerializableLibrary::writeToAllObjects()
{
    for(int i = 0; i < objectCount; i++)
    {
        writeToObject(i);
    }
}

void SerializableLibrary::writeToObject(int index)
{
    if(objects[index] == nullptr)
    {

        ofLogError(__FUNCTION__) << "Tried to write to non existent object. Object index invalid.";

    }

    std::string stringId = objects[index]->getUniqueId();

    Json::Value defaultValue;

    if(deserializedData.isMember(stringId))
    {
        ofxJSONElement extractedObjectData = deserializedData.get(stringId, defaultValue);
        objects[index]->deserialize(extractedObjectData);
    }
    else
    {
        ofLogVerbose(__FUNCTION__) << "Tried to write to object with no data stored.";
    }
}

void SerializableLibrary::registerImage(ofImage** image, std::string name)
{
    images[imageCount] = image;
    imageNames[imageCount] = name;
    writeToImage(imageCount);
    imageCount++;
}


void SerializableLibrary::writeToAllImages()
{
    for(int i = 0; i < imageCount; i++)
    {
        writeToImage(i);
    }
}

void SerializableLibrary::writeToImage(int index)
{
    if(images[index] == nullptr)
    {
        ofLogError(__FUNCTION__) << "Tried to write to non existent image. Image index invalid.";
    }

    if(deserializedData.isMember("images"))
    {
        Json::Value imageIndex;

        imageIndex = deserializedData.get("images", imageIndex);

        std::string name = imageNames[index];
        if(imageIndex.isMember(name))
        {
            std::string path = imageIndex.get(name, Json::Value("")).asString();
            (*images[index])->load(path);
        }
        else
        {
            ofLogVerbose(__FUNCTION__) << "Tried to write to image with no data stored.";
        }
    }
}

