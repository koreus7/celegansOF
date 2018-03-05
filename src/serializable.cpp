#include "serializable.h"

void ISerializable::deserializeFile(const string& filename)
{
    ofxJSONElement root;
    root.openLocal(filename);
    this->deserialize(root);
}

void ISerializable::deserializeJSONString(const string& data)
{
    this->deserialize(ofxJSONElement(data));
}

string ISerializable::serializeToJSONString() const
{
    ofxJSONElement root;
    this->serialize(root);
    string jsonString = root.toStyledString();
    return jsonString;
}

void ISerializable::serializeToFile(const string& fileName) const
{
    ofxJSONElement root;
    this->serialize(root);
    root.save(fileName, true);
}
