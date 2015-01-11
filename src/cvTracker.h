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
    ofPtr<ofParameterGroup> trackerControl;
    ofPtr<ofParameter<int> >  threshold;
    ofPtr<ofParameter<int> > minArea;
    ofPtr<ofParameter<int> > maxArea;
    ofPtr<ofParameter<int> > nConsidered;
    ofPtr<ofParameter<bool> > bFindHoles;
    ofPtr<ofParameter<bool> > bUseApproximation ;
    ofPtr<ofParameter<bool> > bBlur;
    
    ofxOscSender sender;

};
#endif /* defined(__multiIPCameTracker__cvTracker__) */
