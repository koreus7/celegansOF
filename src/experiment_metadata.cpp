#include "experiment_metadata.h"

void ExperimentMetadata::serialize(ofxJSONElement &root) const
{
    root["comments"] = Json::Value(commentsBuffer);
    root["VERSION"] = "0.1";
}

void ExperimentMetadata::deserialize(const ofxJSONElement &root)
{
    commentsBuffer = strdup(root["comments"].asCString());
}

int ExperimentMetadata::getUniqueId() const
{
    return 3;
}

ExperimentMetadata::ExperimentMetadata()
{
    commentsBuffer = new char[COMMENTS_BUFFER_SIZE];
}

ExperimentMetadata::~ExperimentMetadata()
{
    delete[] commentsBuffer;
}
