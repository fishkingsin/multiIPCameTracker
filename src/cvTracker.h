//
//  cvTracker.h
//  multiIPCameTracker
//
//  Created by James Kong on 9/1/15.
//
//

#ifndef __multiIPCameTracker__cvTracker__
#define __multiIPCameTracker__cvTracker__

#include "ofMain.h"
#include "Constants.h"
#include "ofxOpenCV.h"
#include "ofxOpticalFlowLK.h"
#include "ofxOsc.h"
#define HOST "localhost"
#define PORT 12345
class cvTracker
{
public:
    void setup(int width, int height);
    void update(ofFbo &infbo, int width, int height);
    ofxOpticalFlowLK opticalFlow;
    ofFbo fbo;
    ofPixels pixels;
    ofPixels incoming_pixels;
    ofxCvColorImage cvImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    ofxCvContourFinder 	contourFinder;
    ofParameterGroup trackerControl;
    ofParameter<bool>bUseFlow;
    ofParameter<int>threshold;
    ofParameter<int>minArea;
    ofParameter<int>maxArea;
    ofParameter<int>nConsidered;
    ofParameter<bool>bFindHoles;
    ofParameter<bool>bUseApproximation ;
    ofParameter<bool>bBlur;
    ofParameter<bool>bSetBackground;
        ofParameter<bool>bTrackDiff;
    
    ofxOscSender sender;

};
#endif /* defined(__multiIPCameTracker__cvTracker__) */
