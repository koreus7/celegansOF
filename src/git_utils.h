#pragma once
#include <string>
#include <ctime>
#include <thread>

namespace GitUtils
{
    namespace
    {

        void pushBranch(std::string branchName)
        {
            std::string pushCommand = "git push origin " + branchName;
            std::system(pushCommand.c_str());
        }

        void checkoutBranch(std::string branchName)
        {
            std::string checkoutCommand ="git checkout -b " + branchName;
            std::system(checkoutCommand.c_str());
        }

        void commit(std::string message)
        {
            std::string commitCommand = "git commit -m \"" + message + "\"";
            std::system(commitCommand.c_str());
        }

    }


    /// Commit the working changes to a branch then return to master as if nothing happened.
    /// Changes are pushed to origin asynchronously.
    /// \param branchName
    /// \param pushChanges
    void commitCurrentChangesToBranch(std::string branchName, bool pushChanges)
    {
        // Stash the current changes.
        std::system("git stash");

        // Checkout the new branch.
        checkoutBranch(branchName);

        // Apply the changes from the stash to the new branch and stage them.
        std::system("git stash apply && git add -A :/");

        // Commit the changes to the branch.
        commit(branchName + "working changes");

        // Change back to master branch and put our changes back in place.
        std::system("git checkout master && git stash pop");

        // Push to git in seperate thread so we don't wait for network io.
        if(pushChanges)
        {
            std::thread pushThread(pushBranch, branchName);
            pushThread.detach();
        }
    }

}