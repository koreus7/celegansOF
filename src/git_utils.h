#pragma once
#include <string>
#include <ctime>
#include <thread>

namespace GitUtils
{

    void stashCurrentChanges()
    {
        std::system("git stash");
    }

    void checkoutMaster()
    {
        std::system("git checkout master");
    }

    void checkoutNewBranch(const std::string &branchName)
    {
        std::string checkoutCommand ="git checkout -b " + branchName;
        std::system(checkoutCommand.c_str());
    }

    void applyStashedChanges()
    {
        std::system("git stash apply");
    }

    void popStashedChanges()
    {
        std::system("git stash pop");
    }

    void stageAllAndCommit(const std::string& message)
    {
        std::system("git add -A :/");
        std::string commitCommand = "git commit -m \"" + message + "\"";
        std::system(commitCommand.c_str());
    }


    void pushBranchToOrigin(const std::string &branchName)
    {
        std::string pushCommand = "git push origin " + branchName;
        std::system(pushCommand.c_str());
    }

    void pushBranchToOriginInBackgroundThread(std::string branchName)
    {
        std::thread pushThread(pushBranchToOrigin, branchName);
        pushThread.detach();
    }


}