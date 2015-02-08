#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofApp.h"

//========================================================================
int main( ){
#if TARGET_OS_WIN32
    HWND handleWindow;
    AllocConsole();
    handleWindow = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(handleWindow, 0);
#endif
    ofAppGlutWindow window; // create a window
    // set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
    ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
    
    ofRunApp(new ofApp());
    
}
