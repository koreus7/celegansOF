#include "experiment_metadata.h"

void ExperimentMetadata::serialize(ofxJSONElement &root) const
{
    root["comments"] = Json::Value(commentsBuffer);
    root["name"] = Json::Value(nameBuffer);
}

void ExperimentMetadata::deserialize(const ofxJSONElement &root)
{
    commentsBuffer = strdup(root["comments"].asCString());
    nameBuffer = strdup(root["name"].asCString());
}

std::string ExperimentMetadata::getUniqueId() const
{
    return "ExperimentMetadata";
}

ExperimentMetadata::ExperimentMetadata()
{
    // Initialise and clear the buffers.
    commentsBuffer = new char[COMMENTS_BUFFER_SIZE];
    memset(&commentsBuffer[0], 0, COMMENTS_BUFFER_SIZE);
    nameBuffer = new char[NAME_BUFFER_SIZE];
    memset(&nameBuffer[0], 0, NAME_BUFFER_SIZE);
}

ExperimentMetadata::~ExperimentMetadata()
{
    delete[] commentsBuffer;
    delete[] nameBuffer;
}
