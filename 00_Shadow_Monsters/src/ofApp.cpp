#include "ofApp.h"


void ofApp::setup()
{
    // Load a video.
    video.load("Shadows.mov");

    // Start the video playing.
    video.play();

    // Set the blob minimum size.
    contourFinder.setMinAreaRadius(80);

    // Set the blob maximum size.
    contourFinder.setMaxAreaRadius(1000);

    // Q. What does this do?
    contourFinder.setSimplify(true);

    // Setup the gui.
    setupGui();

}

void ofApp::update()
{
    video.update();

    if (video.isFrameNew())
    {
        // Q. How is `finderTrackHueSaturation ? ofxCv::TRACK_COLOR_HS : ofxCv::TRACK_COLOR_RGB` evaluated by the program?
        // Q. What are the advantages and disadvantages of using Ternary Operator syntax?
        // Q. Re-write this line using an if/then statement rather than a Ternary Operator.
        // Q. Why are we setting the target color every frame? Is this necessary?
        // Q. Did you see the question about `override` in the `ofApp.h`? ;)
        contourFinder.setTargetColor(finderTargetColor, finderTrackHueSaturation ? ofxCv::TRACK_COLOR_HS : ofxCv::TRACK_COLOR_RGB);

        contourFinder.findContours(video);
    }
}

void ofApp::draw()
{
    ofBackground(0);
    ofSetColor(255);
    video.draw(video.getWidth(), 0);

    // Iterate through all found contours.
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        // This contour has been resampled and smoothed. The smoothing and
        // resampling parameters can be controlled with the gui.
        const ofPolyline contour = getPreparedContour(contourIndex);

        // This represents the curvatures for each point. The size of this
        // vector is the same as the size of the contour above. The index of
        // each curvature corresponds to a point in the contour.
        //
        // Curvatures can be positive or negative (indicated as red / green
        // in the debug draw) depending on if they are curving to the left or
        // to the right. Tighter curvatures are represented by longer
        // lines. Areas of high curvature are found where the contour changes
        // directions rapidly (like a U-turn or a sharp corner). Areas of low
        // curvature represent flat places in the curve.
        std::vector<float> curvatures = getCurvatures(contour);

        // The convex hull is calculated here and contains a large amount of
        // information about the locations and qualities of the convexities.
        ConvexHull convexHull(contour, hullMinumumDefectDepth);

/////////////////////////// YOUR WORK AREA /////////////////////////////////////
        // Implement the following pseudocode:

        // For every other vertex in the contour ...
        //
        // ... if the curvature at that vertex is > 0 and < 0.02 (you may need
        //     to modify these values to get something that looks good to you)
        //
        // ... draw a triangle on that is rotated to match the orientation
        //     of the contour.
        //
        //     These might ultimately look like the sharp spines on the back of
        //     a reptile.
        //
        //     For one possible look, see the README.md file / and spines.png.
        //
        //     There several ways to approach this.
        //
        //     Method 1:
        //     One is to construct the spines using lines / shapes (e.g. ofPath,
        //     ofTriangle, ofPolyline).
        //
        //     This approach will require that you calculate each of the exact
        //     points with which to draw. (i.e. for a triangle you need to know
        //     know three points, the two bases on the contour and the apex / tip).
        //
        //     Thus, the triangle might connect to the vertex that was skipped
        //     (one base), current vertex (another base) and a normal point
        //     above the current vertex (the apex).
        //
        //     Method 2:
        //     The second approach is to attach an existing shape or image to
        //     contour.  For instance if you wanted to create images of spines
        //     and attach them.  Or if you drew a collection of shapes from
        //     ofPath.  This is the approach taken in the
        //     06_polyline_orient_images example.
        //
        //     Bonus: The height of the triangle should be scaled based on the
        //     curvature at that vertex. (see 03_polyline_curves for more).
        //
        //     Bonus: Vary the look, shape, spacing, etc of the spines to vary
        //     based on other factors of your choice.

/////////////////////////// YOUR WORK AREA END /////////////////////////////////

        // Call the debug draw functions.
        drawDebug(contour, curvatures, convexHull);
    }

    ofFill();
    ofSetColor(255);
    ofDrawRectangle(0, video.getHeight(), ofGetWidth(), ofGetHeight());

    drawGui();

}

















/// Setup stuff below.

void ofApp::setupGui()
{
    // Set up the gui.
    guiFinder.setup();
    guiFinder.setPosition(0, video.getHeight());
    guiFinder.add(finderThreshold.set("Threshold", 60, 0, 255));
    guiFinder.add(finderTrackHueSaturation.set("Track Hue/Saturation", false));
    guiFinder.add(finderTargetColor.set("Track Color", ofColor(38, 23, 19)));

    guiContour.setup();
    guiContour.setPosition(guiFinder.getWidth(), guiFinder.getPosition().y);
    guiContour.add(contourDraw.set("Draw", true));
    guiContour.add(contourDrawFilled.set("Draw Filled", true));
    guiContour.add(contourSimplifyEnable.set("Simplify Enable", true));
    guiContour.add(contourSimplify.set("Simplify", 2, 0, 8));
    guiContour.add(contourResampleEnable.set("Resample Enable", true));
    guiContour.add(contourResampleDistance.set("Resample Distance", 5, 2, 100));

    guiCurvature.setup();
    guiCurvature.setPosition(guiContour.getPosition().x + guiContour.getWidth(), guiContour.getPosition().y);
    guiCurvature.add(curvatureDraw.set("Draw", true));
    guiCurvature.add(curvatureSmoothingAlpha.set("Curvature Smoothing", .65, 0, 0.999999));
    guiCurvature.add(curvatureScaler.set("Curvature Scaler", 500, 1, 1000));
    guiCurvature.add(curvatureShowDirection.set("Show Direction", true));

    guiHull.setup();
    guiHull.setPosition(guiCurvature.getPosition().x + guiCurvature.getWidth(), guiCurvature.getPosition().y);
    guiHull.add(hullDraw.set("Draw Convex Hull", true));
    guiHull.add(hullDefectsDraw.set("Draw Hull Defects", true));
    guiHull.add(hullMinumumDefectDepth.set("Min Defect Depth", 10, 0, 50));
}


void ofApp::drawGui()
{
    guiFinder.draw();
    guiContour.draw();
    guiCurvature.draw();
    guiHull.draw();
}


ofPolyline ofApp::getPreparedContour(std::size_t i) const
{
    // Get a copy of the contour.
    ofPolyline _contour = contourFinder.getPolylines()[i];

    // Simplify the contour.
    if (contourSimplifyEnable)
    {
        _contour.simplify(contourSimplify);
    }

    // Resample the contour by a reasonable amount.
    if (contourResampleEnable)
    {
        _contour = _contour.getResampledBySpacing(contourResampleDistance);
    }

    return _contour;
}


void ofApp::drawContour(const ofPolyline& contour) const
{
    if (contourDraw)
    {
        // Draw the contour.
        ofSetColor(255, 127);

        if (contourDrawFilled)
        {
            ofPushMatrix();
            ofFill();
            ofSetColor(255);
            ofBeginShape();
            for (auto point: contour) ofVertex(point);
            ofEndShape();
        }
        else
        {
            contour.draw();
            // Draw the points.
            ofSetColor(255, 255, 0, 127);
            ofNoFill();
            for (const auto& point: contour) ofDrawCircle(point, 2);
        }
    }
}


std::vector<float> ofApp::getCurvatures(const ofPolyline& contour) const
{
    // Smooth the curvatures for various effects.
    return CvUtils::smooth(CvUtils::getCurvatures(contour), curvatureSmoothingAlpha);
}


void ofApp::drawCurvatures(const ofPolyline& contour,
                           const std::vector<float>& curvatures) const
{
    if (curvatureDraw)
    {
        for (std::size_t j = 0; j < curvatures.size(); ++j)
        {
            // Must call const-reference or else normals are re-calculated each time.
            auto currentPoint = contour[j];
            auto normalPoint = contour.getNormalAtIndex(j);
            float scale = curvatures[j] * curvatureScaler;

            if (!curvatureShowDirection) scale = std::abs(scale);

            auto normalEndPoint = currentPoint - (normalPoint * scale);

            if (scale > 0) ofSetColor(0, 255, 0);
            else ofSetColor(255, 0, 0);

            ofDrawLine(currentPoint, normalEndPoint);
        }
    }
}


void ofApp::drawHull(const ConvexHull& convexHull) const
{

    if (hullDraw)
    {
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(ofColor::pink, 90);
        convexHull.convexHull().draw();
        ofPopStyle();
    }

    if (hullDefectsDraw)
    {
        for (auto defect: convexHull.convexHullDefects())
        {
            ofSetColor(ofColor::yellow, 127);
            ofDrawCircle(defect.point, 10);

            ofSetColor(ofColor::red, 80);
            ofDrawLine(defect.point, defect.defectHullChordNormalPoint);
            ofSetColor(ofColor::darkRed, 80);
            ofDrawLine(defect.point, defect.endPoint);
            ofSetColor(ofColor::yellow, 200);
            ofDrawLine(defect.point, defect.midPoint);
            ofSetColor(ofColor::gold, 80);
            ofDrawLine(defect.point, defect.startPoint);
        }
    }
}


void ofApp::drawDebug(const ofPolyline& contour,
                      const std::vector<float>& curvatures,
                      const ConvexHull& convexHull) const
{
    // Draw the contour.
    drawContour(contour);

    // Draw the curvatures.
    drawCurvatures(contour, curvatures);

    // Draw convex hull.
    drawHull(convexHull);
}
