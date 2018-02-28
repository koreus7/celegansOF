#pragma once
#include "ofMain.h"

class AppState
{
public:
    ofImage* focusedImage;
    ofVec2f focusedImagePos;
    ofVec2f* selectedTailPos;
};
