#pragma once
#include "serializable.h"

class ExperimentMetadata : public ISerializable
{
public:
    ExperimentMetadata();
    ~ExperimentMetadata();
    const static int COMMENTS_BUFFER_SIZE = 2048;
    const static int NAME_BUFFER_SIZE = 256;
    char * commentsBuffer;
    char * nameBuffer;
private:
    void serialize(ofxJSONElement &root) const override;
    void deserialize(const ofxJSONElement &root) override;
    std::string getUniqueId() const override;
};