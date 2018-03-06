#include "experiment_metadata.h"
#include "git_utils.h"
#include "time_utils.h"

void pushBranch(std::string branchName)
{
    std::string pushCommand = "git push origin " + branchName;
    std::system(pushCommand.c_str());
}

void ExperimentMetadata::serialize(ofxJSONElement &root) const
{
    std::string branchName = "experiment" + TimeUtils::getUnixTimeAsString();

    GitUtils::commitCurrentChangesToBranch(branchName, true);

    root["comments"] = Json::Value(commentsBuffer);
    root["VERSION"] = branchName;
}

void ExperimentMetadata::deserialize(const ofxJSONElement &root)
{
    commentsBuffer = strdup(root["comments"].asCString());
}

std::string ExperimentMetadata::getUniqueId() const
{
    return "ExperimentMetadata";
}

ExperimentMetadata::ExperimentMetadata()
{
    commentsBuffer = new char[COMMENTS_BUFFER_SIZE];
    memset(&commentsBuffer[0], 0, COMMENTS_BUFFER_SIZE);
}

ExperimentMetadata::~ExperimentMetadata()
{
    delete[] commentsBuffer;
}
