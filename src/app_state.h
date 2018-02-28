#pragma once
#include "ofMain.h"

class AppState
{
public:
    float micronToPixelRatio = 0.65;
    ofImage* focusedImage;
    ofVec2f focusedImagePos;
    ofVec2f* selectedTailPos;
};
