#include "experiment_metadata.h"

void ExperimentMetadata::serialize(ofxJSONElement &root) const
{

    std::time_t unixTime = std::time(nullptr);
    std::string version = "experiment" + std::to_string(unixTime);
    std::system("git stash");
    std::string checkoutCommand ="git checkout -b " + version;
    std::system(checkoutCommand.c_str());
    std::system("git stash apply");
    std::system("git add -A :/");
    std::string commitCommand = "git commit -m \"" + version + "\"";
    std::system(commitCommand.c_str());
    std::string pushCommand = "git push origin " + version;
    std::system(pushCommand.c_str());
    std::system("git checkout master");
    std::system("git stash pop");

    root["comments"] = Json::Value(commentsBuffer);
    root["VERSION"] = version;
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
}

ExperimentMetadata::~ExperimentMetadata()
{
    delete[] commentsBuffer;
}
