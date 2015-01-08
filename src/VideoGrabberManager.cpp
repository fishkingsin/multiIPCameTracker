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
        control->add(h->set("CAM_RECT_H_"+ofToString(i), 0, -VIDEO_HEIGHT, VIDEO_HEIGHT));
        ipCamX.push_back(x);
        ipCamY.push_back(y);
        ipCamW.push_back(w);
        ipCamH.push_back(h);
        ipcamRectControls.push_back(control);
//        ofPtr<ofxCvColorImage> cvimage = ofPtr<ofxCvColorImage>(new ofxCvColorImage);
//        cvimage->allocate(VIDEO_WIDTH , VIDEO_HEIGHT);
//        cvimage->setUseTexture(true);
//        cvImages.push_back(cvimage);
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
        ofPtr<ofParameter<float> >  barrelpower = ofPtr<ofParameter<float> > (new ofParameter<float>);
        
        
        undistortcontrol->add(radialdistX->set("RADIALDIST_X_"+ofToString(i),0,-1,1));
        undistortcontrol->add(radialdistY->set("RADIALDIST_Y_"+ofToString(i),0,-1,1));
        undistortcontrol->add(tangentdistX->set("TANGENTDIST_X_"+ofToString(i),0,-1,1));
        undistortcontrol->add(tangentdistY->set("TANGENTDIST_Y_"+ofToString(i),0,-1,1));
        undistortcontrol->add(focalx->set("FOCAL_X_"+ofToString(i),0,-VIDEO_WIDTH*2,VIDEO_WIDTH*2));
        undistortcontrol->add(focaly->set("FOCAL_Y_"+ofToString(i),0,-VIDEO_HEIGHT*2,VIDEO_HEIGHT*2));
        undistortcontrol->add(centerx->set("CENTER_X_"+ofToString(i),-VIDEO_WIDTH*2,0,VIDEO_WIDTH*2));
        undistortcontrol->add(centery->set("CENTER_Y_"+ofToString(i),0,-VIDEO_HEIGHT*2,VIDEO_HEIGHT*2));
        undistortcontrol->add(barrelpower->set("BARRELPOWER_"+ofToString(i),0,-100,100));
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
        
    }
    fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT);
}

//------------------------------------------------------------------------------

IPCameraDef& VideoGrabberManager::getNextCamera()
{
    nextCamera = (nextCamera + 1) % ipcams.size();
    return ipcams[nextCamera];
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
    
    nextCamera = ipcams.size();
}
void VideoGrabberManager::update()
{
    // update the cameras
    for(std::size_t i = 0; i < grabbers.size(); i++)
    {
        grabbers[i]->update();
//        if(grabbers[i]->isFrameNew() && grabbers[i]->isConnected())
//        {
////            textures[i]->loadData(grabbers[i]->getPixels(), grabbers[i]->getWidth(), grabbers[i]->getHeight(),GL_RGB);
//            cvImages[i]->setFromPixels(grabbers[i]->getPixels(), grabbers[i]->getWidth(), grabbers[i]->getHeight());
//            cvImages[i]->undistort(radialDistX[i]->get(),  radialDistY[i]->get(),  tangentDistX[i]->get(),  tangentDistY[i]->get(),  focalX[i]->get(),  focalY[i]->get(),  centerX[i]->get(),  centerY[i]->get()) ;
//        }
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

            blendShader.setUniformTexture("tex0", grabbers[i]->getTextureReference(), 0);
        }
        grabbers[i]->draw(ipCamX[i]->get(), ipCamY[i]->get(), ipCamW[i]->get(), ipCamH[i]->get());
        if(enableShader.get())
        {
            blendShader.end();
        }
        
    }
    fbo.end();
    
}
void VideoGrabberManager::draw()
{
    ofSetColor(ofColor::white);

    fbo.draw(0,0,ofGetWidth(),ofGetHeight());
    
}