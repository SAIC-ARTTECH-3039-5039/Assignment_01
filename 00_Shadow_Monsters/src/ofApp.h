//
// This is an educational cover of:
//
// Shadow Monsters (2004)
// by Philip Worthington
//
// [Project](https://www.moma.org/collection/works/110196)
// [Video](https://youtu.be/XNHv6VryB8o)
//


#pragma once


#include "ofMain.h"
#include "ofxARTTECH3039.h"


class ofApp: public ofBaseApp
{
public:

    // Q. What does `override` mean in this context?
    void setup() override;
    void update() override;
    void draw() override;

    void setupGui();
    void drawGui();

    ofPolyline getPreparedContour(std::size_t i) const;

    void drawContour(const ofPolyline& contour) const;

    std::vector<float> getCurvatures(const ofPolyline& contour) const;

    void drawCurvatures(const ofPolyline& contour,
                        const std::vector<float>& curvatures) const;

    void drawHull(const ConvexHull& convexHull) const;

    void drawDebug(const ofPolyline& contour,
                   const std::vector<float>& curvatures,
                   const ConvexHull& convexHull) const;

    /// \brief The video player.
    ofVideoPlayer video;

    /// \brief The ContourFinder object.
    ofxCv::ContourFinder contourFinder;

    /// \brief A gui.
    ofxPanel guiFinder;
    ofxPanel guiContour;
    ofxPanel guiCurvature;
    ofxPanel guiHull;

    /// \brief The contour threshold.
    ofParameter<float> finderThreshold;

    /// \brief True if hue / saturation should be tracked.
    ofParameter<bool> finderTrackHueSaturation;

    /// \brief The target color to threshold.
    ofParameter<ofColor> finderTargetColor;

    ofParameter<bool> contourDraw;
    ofParameter<bool> contourDrawFilled;
    ofParameter<bool> contourSimplifyEnable;
    ofParameter<float> contourSimplify;
    ofParameter<bool> contourResampleEnable;
    ofParameter<float> contourResampleDistance;

    ofParameter<bool> curvatureDraw;
    ofParameter<float> curvatureSmoothingAlpha;
    ofParameter<float> curvatureScaler;
    ofParameter<float> curvatureThreshold;
    ofParameter<bool> curvatureShowDirection;

    ofParameter<bool> hullDraw;
    ofParameter<bool> hullDefectsDraw;
    ofParameter<float> hullMinumumDefectDepth;

};
