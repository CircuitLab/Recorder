#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxUI.h"

#define NI_ENABLE "niEnable"
#define NI_VIEW_WIDTH 640
#define NI_VIEW_HEIGHT 480


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    ofTrueTypeFont verdana;
    
	ofxOpenNI openNIDevice, openNIPlayer;
    string oniFileName, newOniFileName;
    int numTotalFrames, currentFrame;
    bool isONIPlaying;
    bool isDrawDepthOverlay;
    
    ofQTKitPlayer videoPlayer;
    string movFileName;
    float scale;
    
    ofImage rgbImg, depthImg;
    ofImage combinedImg;
    
    ofxUICanvas *gui;
    ofxUIImageButton *loadBtn, *playBtn, *pauseBtn, *recordBtn, *stopBtn, *rewindBtn, *forwardBtn, *removeBtn;
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
};

#endif
