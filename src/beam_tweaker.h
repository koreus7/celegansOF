#pragma once

#include "ofMain.h"
#include "beam_parameters.h"

class BeamTweaker {
public:
    ~BeamTweaker();
    BeamTweaker();

    /// Adjust the line based on orthogonal contrast readings and beam width.
    /// \param parameters
    /// \param originalImage
    /// \param fittedLine - The line that has already been fitted, this will be tweaked.
    const void tweak(const BeamParameters& parameters, const ofImage& originalImage, ofPolyline& fittedLine);
private:

};
