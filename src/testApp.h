#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxTimer.h"
#include "ofxUI.h"

#define NI_ENABLE "niEnable"
#define NI_VIEW_WIDTH 448
#define NI_VIEW_HEIGHT 336

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
        
    void timerReached(ofEventArgs &e);

    ofTrueTypeFont verdana;
    
	ofxOpenNI openNIDevice, playContext;
    string oniFileName, newOniFileName;
    int numTotalFrames, currentFrame;
    
    ofxUICanvas *gui;
    ofxUIImageButton *loadBtn, *playBtn, *pauseBtn, *recordBtn, *stopBtn, *rewindBtn, *forwardBtn;
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    
    
    ofxTimer timer;
    float startTime, elapsedTime;
    bool isRecording;
};

#endif
