//
//  VideoGrabberManager.cpp
//  wlbankinteractive
//
//  Created by Kong king sin on 2/1/15.
//
//

#include "VideoGrabberManager.h"


void VideoGrabberManager::setup()
{
    //    ofEnableNormalizedTexCoords();
    //    ofDisableArbTex();
    //    blendShader.load("shaders/SmoothEdgeBlend");
    nextCamera = 0;
    blendShader.load("shaders/simpleBlend");
    loadCameras();
    // initialize connection
    for(int i = 0; i < ipcams.size(); i++)
    {
        IPCameraDef& cam = getNextCamera();
        
        ofPtr<IPVideoGrabber> c = ofPtr<IPVideoGrabber>(new IPVideoGrabber);
        
        // if your camera uses standard web-based authentication, use this
        // c->setUsername(cam.username);
        // c->setPassword(cam.password);
        
        // if your camera uses cookies for authentication, use something like this:
        // c->setCookie("user", cam.username);
        // c->setCookie("password", cam.password);
        
        c->setCameraName(cam.getName());
        c->setURI(cam.getURL());
        c->connect(); // connect immediately
        c->setUseTexture(true);
        // if desired, set up a video resize listener
        //        ofAddListener(c->videoResized, this, &ofApp::videoResized);
        
        grabbers.push_back(c);
        ofLogVerbose(ofToString(i))<<"CAM_RECT_"+ofToString(i);
        ofPtr<ofParameterGroup> control = ofPtr<ofParameterGroup>(new ofParameterGroup);
        
        ofPtr<ofParameter<float> > x = ofPtr<ofParameter<float> >(new ofParameter<float>);
        ofPtr<ofParameter<float> > y = ofPtr<ofParameter<float> >(new ofParameter<float>);
        ofPtr<ofParameter<float> > w = ofPtr<ofParameter<float> >(new ofParameter<float>);
        ofPtr<ofParameter<float> > h = ofPtr<ofParameter<float> >(new ofParameter<float>);
        
        control->setName("CAM_RECT_"+ofToString(i));
        control->add(x->set("CAM_RECT_X_"+ofToString(i), 0, -VIDEO_WIDTH, VIDEO_WIDTH*ipcams.size()));
        control->add(y->set("CAM_RECT_Y_"+ofToString(i), 0, -VIDEO_HEIGHT, VIDEO_HEIGHT));
        control->add(w->set("CAM_RECT_W_"+ofToString(i), 0, -VIDEO_WIDTH, VIDEO_WIDTH*ipcams.size()));
        control->add(h->set("CAM_RECT_H_"+ofToString(i), 0, -VIDEO_HEIGHT, VIDEO_HEIGHT*2));
        ipCamX.push_back(x);
        ipCamY.push_back(y);
        ipCamW.push_back(w);
        ipCamH.push_back(h);
        ipcamRectControls.push_back(control);
        ofPtr<ofxCvColorImage> cvimage = ofPtr<ofxCvColorImage>(new ofxCvColorImage);
        cvimage->allocate(VIDEO_WIDTH , VIDEO_HEIGHT);
        cvimage->setUseTexture(true);
        cvImages.push_back(cvimage);
        
        ofPtr<ofxCvColorImage> outputImage = ofPtr<ofxCvColorImage>(new ofxCvColorImage);
        outputImage->allocate(VIDEO_WIDTH , VIDEO_HEIGHT);
        outputImage->setUseTexture(true);
        outputImages.push_back(outputImage);
        
        //        image->allocate(VIDEO_WIDTH , VIDEO_HEIGHT, GL_RGB);
        //        textures->setUseTexture(true);
        //        textures.push_back(image);
        
        
        ofPtr<ofParameterGroup> shaderControl = ofPtr<ofParameterGroup>(new ofParameterGroup);
        ofPtr<ofParameter<float> > al = ofPtr<ofParameter<float> >(new ofParameter<float>);
        ofPtr<ofParameter<float> > ar = ofPtr<ofParameter<float> >(new ofParameter<float>);
        ofPtr<ofParameter<float> > sl = ofPtr<ofParameter<float> >(new ofParameter<float>);
        ofPtr<ofParameter<float> > sr = ofPtr<ofParameter<float> >(new ofParameter<float>);
        
        shaderControl->setName("BLEND_CONTORL_"+ofToString(i));
        shaderControl->add(al->set("ALPHA_LEFT_"+ofToString(i),0,0,1));
        shaderControl->add(ar->set("ALPHA_RIGHT_"+ofToString(i),0,0,1));
        shaderControl->add(sl->set("SIZE_LEFT_"+ofToString(i),0,0,VIDEO_WIDTH));
        shaderControl->add(sr->set("SIZE_RIGHT_"+ofToString(i),0,0,VIDEO_WIDTH));
        ofPtr<ofParameter<float> >  barrelpower = ofPtr<ofParameter<float> > (new ofParameter<float>);
        shaderControl->add(barrelpower->set("BARRELPOWER_"+ofToString(i),0,-2,2));
        
        alphaLs.push_back(al);
        alphaRs.push_back(ar);
        sizeLs.push_back(sl);
        sizeRs.push_back(sr);
        shaderControls.push_back(shaderControl);
        
        
        ofPtr<ofParameterGroup> undistortcontrol = ofPtr<ofParameterGroup>(new ofParameterGroup);
        undistortcontrol->setName("UNDISTORT_CONTROL_"+ofToString(i));
        
        ofPtr<ofParameter<float> >  radialdistX = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  radialdistY = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  tangentdistX = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  tangentdistY = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  focalx = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  focaly = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  centerx = ofPtr<ofParameter<float> > (new ofParameter<float>);
        ofPtr<ofParameter<float> >  centery = ofPtr<ofParameter<float> > (new ofParameter<float>);
        
        
        
        undistortcontrol->add(radialdistX->set("RADIALDIST_X_"+ofToString(i),0,-1,1));
        undistortcontrol->add(radialdistY->set("RADIALDIST_Y_"+ofToString(i),0,-1,1));
        undistortcontrol->add(tangentdistX->set("TANGENTDIST_X_"+ofToString(i),0,-1,1));
        undistortcontrol->add(tangentdistY->set("TANGENTDIST_Y_"+ofToString(i),0,-1,1));
        undistortcontrol->add(focalx->set("FOCAL_X_"+ofToString(i),     VIDEO_WIDTH*0.5,    -VIDEO_WIDTH*2,     VIDEO_WIDTH*2));
        undistortcontrol->add(focaly->set("FOCAL_Y_"+ofToString(i),     VIDEO_HEIGHT*0.5,   -VIDEO_HEIGHT*2,    VIDEO_HEIGHT*2));
        undistortcontrol->add(centerx->set("CENTER_X_"+ofToString(i),   VIDEO_WIDTH*0.5,    -VIDEO_WIDTH*2,     VIDEO_WIDTH*2));
        undistortcontrol->add(centery->set("CENTER_Y_"+ofToString(i),   VIDEO_HEIGHT*0.5,   -VIDEO_HEIGHT*2,   VIDEO_HEIGHT*2));
        
        radialDistX.push_back(radialdistX);
        radialDistY.push_back(radialdistY);
        tangentDistX.push_back(tangentdistX);
        tangentDistY.push_back(tangentdistY);
        focalX.push_back(focalx);
        focalY.push_back(focaly);
        centerX.push_back(centerx);
        centerY.push_back(centery);
        BarrelPowers.push_back(barrelpower);
        
        undistortControls.push_back(undistortcontrol);
        setupQuadGui(i,VIDEO_WIDTH, VIDEO_HEIGHT);
    }
    fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT,GL_RGB);
    
}

VideoGrabberManager::~VideoGrabberManager(){
    
}
//------------------------------------------------------------------------------

IPCameraDef& VideoGrabberManager::getNextCamera()
{
    int index = nextCamera;
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[index];
}

//------------------------------------------------------------------------------

void VideoGrabberManager::loadCameras()
{
    
    // all of these cameras were found using this google query
    // http://www.google.com/search?q=inurl%3A%22axis-cgi%2Fmjpg%22
    // some of the cameras below may no longer be valid.
    
    // to define a camera with a username / password
    //ipcams.push_back(IPCameraDef("http://148.61.142.228/axis-cgi/mjpg/video.cgi", "username", "password"));
    
    ofLog(OF_LOG_NOTICE, "---------------Loading Streams---------------");
    
    ofxXmlSettings XML;
    
    if(XML.loadFile("streams.xml"))
    {
        
        XML.pushTag("streams");
        std::string tag = "stream";
        
        int nCams = XML.getNumTags(tag);
        
        for(std::size_t n = 0; n < nCams; n++)
        {
            
            IPCameraDef def(XML.getAttribute(tag, "name", "", n),
                            XML.getAttribute(tag, "url", "", n),
                            XML.getAttribute(tag, "username", "", n),
                            XML.getAttribute(tag, "password", "", n));
            
            
            std::string logMessage = "STREAM LOADED: " + def.getName() +
            " url: " +  def.getURL() +
            " username: " + def.getUsername() +
            " password: " + def.getPassword();
            
            ofLogNotice() << logMessage;
            
            ipcams.push_back(def);
            
        }
        
        XML.popTag();
        
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Unable to load streams.xml.");
    }

    ofLog(OF_LOG_NOTICE, "-----------Loading Streams Complete----------");
    
    nextCamera = 0;//ipcams.size();
}
void VideoGrabberManager::update()
{
    // update the cameras
    for(std::size_t i = 0; i < grabbers.size(); i++)
    {
        grabbers[i]->update();
        if(grabbers[i]->isFrameNew() && grabbers[i]->isConnected())
        {
            //            textures[i]->loadData(grabbers[i]->getPixels(), grabbers[i]->getWidth(), grabbers[i]->getHeight(),GL_RGB);
            cvImages[i]->setFromPixels(grabbers[i]->getPixels());
            cvImages[i]->undistort(radialDistX[i]->get(),  radialDistY[i]->get(),  tangentDistX[i]->get(),  tangentDistY[i]->get(),  focalX[i]->get(),  focalY[i]->get(),  centerX[i]->get(),  centerY[i]->get()) ;
            vector <ofPoint> quadWarpScaled;
            ofPoint * scaledPoints = quadGuis[i]->getScaledQuadPoints(cameraWidth,cameraHeight);
            for (int i=0; i<4; i++){
                quadWarpScaled.push_back( scaledPoints[i]);
            }
            
            getQuadSubImage(cvImages[i].get(), outputImages[i].get(), quadWarpScaled, OF_IMAGE_COLOR);
        }
    }
    //    ofEnableNormalizedTexCoords();
    
    fbo.begin();
    ofClear(0, 0, 0);
    ofSetColor(ofColor::white);
    for(std::size_t i = 0; i < grabbers.size(); i++)
    {
        
        if(enableShader.get())
        {
            blendShader.begin();
            blendShader.setUniform1f("width", grabbers[i]->getWidth());
            blendShader.setUniform1f("height", grabbers[i]->getHeight());
            blendShader.setUniform1f("alphaL", alphaLs[i]->get());
            blendShader.setUniform1f("alphaR", alphaRs[i]->get());
            blendShader.setUniform1f("sizeL", sizeLs[i]->get());
            blendShader.setUniform1f("sizeR", sizeRs[i]->get());
            blendShader.setUniform1f("BarrelPower", BarrelPowers[i]->get());
            
            blendShader.setUniformTexture("tex0", grabbers[i]->getTexture(), 0);
        }
        outputImages[i]->draw(ipCamX[i]->get(), ipCamY[i]->get(), ipCamW[i]->get(), ipCamH[i]->get());
        if(enableShader.get())
        {
            blendShader.end();
        }
        
    }
    fbo.end();
    
}
void VideoGrabberManager::draw(float x , float y, float w , float h) const
{
    ofSetColor(ofColor::white);
    
    fbo.draw(x,y,w,h);
    
}

//BR: Added some messiness here to setup, draw, and update the gui quad...

void VideoGrabberManager::setupQuadGui (int i, int _cameraWidth, int _cameraHeight )
{
    
    quadGuis.push_back(ofPtr<ofxCvCoordWarpingGui>(new ofxCvCoordWarpingGui));
    cameraWidth = _cameraWidth;
    cameraHeight = _cameraHeight;
    
    // give the gui quad a starting setting
    
    quadGuis.back()->setup("QUAD_"+ofToString(i));
    
    ofPoint quadSrc[4];
    quadSrc[0].set(0, 0);
    quadSrc[1].set(cameraWidth, 0);
    quadSrc[2].set(cameraWidth, cameraHeight);
    quadSrc[3].set(0, cameraHeight);
    quadGuis.back()->setQuadPoints(quadSrc);
    
    // give the gui quad a default setting
    //    settings.quadWarpOriginal[0].set(0, 0);
    //    settings.quadWarpOriginal[1].set(cameraWidth, 0);
    //    settings.quadWarpOriginal[2].set(cameraWidth, cameraHeight);
    //    settings.quadWarpOriginal[3].set(0, cameraHeight);
    
    //BR TO DO: add this into the normal settings file
    quadGuis.back()->width = cameraWidth;
    quadGuis.back()->height = cameraHeight;
    quadGuis.back()->disableAppEvents();
    //    quadGuiSetup = true;
    
    //    quadGui.enableAllEvents();
    quadGuis.back()->bCameraView = true;
    quadGuis.back()->enableAllEvents();
    quadGuis.back()->readFromFile("QUAD_Settings.xml");
};

void VideoGrabberManager::drawQuadGui()
{
    for(int i = 0 ; i< ipcams.size() ;i++)
    {
        drawQuadGui(i,i*VIDEO_WIDTH,0,VIDEO_WIDTH,VIDEO_HEIGHT);
    }
}
void VideoGrabberManager::drawQuadGui(int i){
    quadGuis[i]->draw();
};

void VideoGrabberManager::drawQuadGui(int i, int x, int y, int w, int h ){
    cvImages[i]->draw(x,y,w,h);
    quadGuis[i]->x = x;
    quadGuis[i]->y = y;
    quadGuis[i]->setScale((float) w/quadGuis[i]->width, (float) h/quadGuis[i]->height );
    drawQuadGui(i);
};
void VideoGrabberManager::saveQuad()
{
    for(int  i = 0 ; i < quadGuis.size() ; i++)
    {
        quadGuis[i]->saveToFile("QUAD_Settings.xml");
    }
}
void VideoGrabberManager::getQuadSubImage(ofxCvColorImage* inputImage, ofxCvColorImage* outputImage, vector <ofPoint>& quad, ofImageType imageType) {
    if ( quad.size() < 4 ){
        ofLog( OF_LOG_ERROR, "You must pass a vector of four points to this function");
        return;
    } // weird thing that could happen...
    
    static unsigned char * inpix;
    static unsigned char * outpix;
    inpix   = inputImage->getPixels().getData();
    outpix  = outputImage->getPixels().getData();
    
    int inW, inH, outW, outH;
    inW = inputImage->width;
    inH = inputImage->height;
    outW = outputImage->width;
    outH = outputImage->height;
    
    int bpp = 1;
    if (imageType == OF_IMAGE_COLOR){
        bpp = 3;
    } else if (imageType == OF_IMAGE_COLOR_ALPHA){
        bpp = 4;
    }
    
    int xinput =0;
    int yinput = 0;
    int inIndex = 0;
    int outIndex = 0;
    
    float xlrp = 0.0;
    float ylrp = 0.0;
    
    ofPoint p1, p2, p3, p4;
    p1 = quad[0];
    p2 = quad[1];
    p3 = quad[2];
    p4 = quad[3];
    
    for(int x=0;x<outW;x++) {
        for(int y=0;y<outH;y++) {
            xlrp = x/(float)outW;
            ylrp = y/(float)outH;
            xinput = (p1.x*(1-xlrp)+p2.x*xlrp)*(1-ylrp) + (p4.x*(1-xlrp)+p3.x*xlrp)*ylrp;
            yinput = ((p1.y*(1-ylrp))+(p4.y*ylrp))*(1-xlrp) + (p2.y*(1-ylrp)+p3.y*ylrp)*xlrp;
            inIndex = (xinput + (yinput*inW))*bpp;
            outIndex = (x+y*outW)*bpp;
            memcpy((outpix+outIndex),(inpix+inIndex),sizeof(unsigned char)*bpp);
        }
    }
    outputImage->setFromPixels(outpix, outW, outH);
}
void VideoGrabberManager::enableDrawQuad(bool &b)
{
    for(int i = 0 ; i< ipcams.size() ;i++)
    {
        if(b)
        {
            quadGuis[i]->enableAllEvents();
        }
        else{
            quadGuis[i]->disableAllEvents();
        }
    }
    
}
