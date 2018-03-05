#include "serializable_library.h"


void SerializableLibrary::registerObject(ISerializable& object)
{
    objects[++objectCount] = &object;
}

void SerializableLibrary::serializeAll(const std::string &fileName) const
{

}

std::string SerializableLibrary::serializeAllToString() const
{
    return std::string();
}

void SerializableLibrary::serializeAll(ofxJSONElement& root) const
{

    ofxJSONElement data = ofxJSONElement(Json::objectValue);

    root[name] = data;

    for(int i = 0; i < objectCount; i++)
    {
        ofxJSONElement object;
        objects[i]->serialize(object);
        std::string stringId = std::to_string(objects[i]->getUniqueId());
        data[stringId] = object;
    }


}
