#include "testApp.h"

//--------------------------------------------------------------
void testApp::setupGUI()
{
    gui = new ofxUICanvas();
    gui->addImageButton("RECORD", "gui/play.png", false, 42, 42);
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255, 150));
}

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(30);
    ofEnableSmoothing();
    
    ofSetLogLevel(OF_LOG_VERBOSE);

#ifdef NI_ENALBE
    openNIDevice.setup();//FromXML("openni/config/ofxopenni_config.xml");
    openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();   // comment this out
    openNIDevice.start();
    // openNIDevice.addInfraGenerator(); // and uncomment this to see infrared generator
                                        // or press the 'i' key when running
    
    isRecording = false;
#endif
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
    setupGUI();
    
    startTime = elapsedTime = 0.;
    timer.setup(1, true);
    timer.stopTimer();
    ofAddListener(timer.TIMER_REACHED, this, &testApp::timerReached);
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef NI_ENABLE
    openNIDevice.update();
#endif
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);

#ifdef NI_ENABLE
    openNIDevice.drawDebug(); // draws all generators
    openNIDevice.drawDepth(0, 0);
    //openNIDevice.drawImage(640, 0);
#endif

    ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
	verdana.drawString(msg, 20, ofGetHeight() - 26);

}

//--------------------------------------------------------------
void testApp::exit(){
#ifdef NI_ENABLE
    openNIDevice.stop();
#endif
    
    delete gui;
}


/*========================================================================
 *  Events
 *========================================================================
 */

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case ' ':
            if (isRecording) {
                timer.stopTimer();
            } else {
                startTime = ofGetElapsedTimeMillis();
                elapsedTime = 0.;
                timer.startTimer();
            }
            
            isRecording = !isRecording;
            break;
        case 'i':
#ifdef NI_ENABLE
            if (openNIDevice.isImageOn()){
                openNIDevice.removeImageGenerator();
                openNIDevice.addInfraGenerator();
                break;
            }
            if (openNIDevice.isInfraOn()){
                openNIDevice.removeInfraGenerator();
                openNIDevice.addImageGenerator();
                break;
            }
#endif
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::timerReached(ofEventArgs &e)
{
    elapsedTime = ofGetElapsedTimeMillis() - startTime;
    cout << ofToString(elapsedTime / 1000) << ofToString(elapsedTime) << endl;
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    
}

