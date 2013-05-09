#include "testApp.h"

//--------------------------------------------------------------
void testApp::setupGUI()
{
    gui = new ofxUICanvas(0, NI_VIEW_HEIGHT * 2, ofGetWidth(), 100);
    
    loadBtn = new ofxUIImageButton(42, 42, true, "gui/load.png", "LOAD");
    playBtn = new ofxUIImageButton(42, 42, true, "gui/play.png", "PLAY");
    recordBtn = new ofxUIImageButton(42, 42, true, "gui/record.png", "RECORD");
    stopBtn = new ofxUIImageButton(42, 42, true, "gui/stop.png", "STOP");
    rewindBtn = new ofxUIImageButton(42, 42, true, "gui/rewind.png", "REWIND");
    forwardBtn = new ofxUIImageButton(42, 42, true, "gui/forward.png", "FORWARD");
    
    gui->addWidgetRight(loadBtn);
    gui->addWidgetRight(new ofxUISpacer(20, 42));
    gui->addWidgetRight(rewindBtn);
    gui->addWidgetRight(playBtn);
    gui->addWidgetRight(forwardBtn);
    gui->addWidgetRight(recordBtn);
    
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    gui->setColorBack(ofColor(0));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(125));
    
}

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    ofSetLogLevel(OF_LOG_VERBOSE);

#ifdef NI_ENABLE
    openNIDevice.setup();//FromXML("openni/config/ofxopenni_config.xml");
    playContext.setup();
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
    
    if (playContext.isPlaying()) {
        playContext.update();
        currentFrame = playContext.getCurrentFrame();
    }
#endif
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);

#ifdef NI_ENABLE
    openNIDevice.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
    openNIDevice.drawImage(NI_VIEW_WIDTH, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
    
    if (playContext.isPlaying()) {
        playContext.drawDepth(0, NI_VIEW_HEIGHT, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        playContext.drawImage(NI_VIEW_WIDTH, NI_VIEW_HEIGHT, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
    } else {
        ofPushStyle();
        ofSetColor(0);
        ofFill();
        ofRect(0, NI_VIEW_HEIGHT, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        ofPopStyle();
    }
#endif
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(playContext.getCurrentFrame() / 30), 100, NI_VIEW_HEIGHT * 2);

    ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
	verdana.drawString(msg, 20, ofGetHeight() - 26);

}

//--------------------------------------------------------------
void testApp::exit(){
#ifdef NI_ENABLE
    openNIDevice.stop();
    playContext.stop();
#endif
    
    delete gui;
}


/*========================================================================
 *  Events
 *========================================================================
 */

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case ' ': {
            if (openNIDevice.isRecording()) {
                timer.stopTimer();
                openNIDevice.stopRecording();
            } else {
                startTime = ofGetElapsedTimeMillis();
                elapsedTime = 0.;
                timer.startTimer();
                openNIDevice.startRecording(ofToDataPath(ofToString(ofGetUnixTime()) + ".oni"));
            }
            
            isRecording = !isRecording;
            break;
        }
            
#ifdef NI_ENABLE
        case 'i': {
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
            break;
        }
#endif
        case 'o': {
            
            break;
        }
            
        default:
            break;
    }
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
    ofLogVerbose("gui event, value: " + ofToString(playBtn->getValue()));
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if (OFX_UI_WIDGET_IMAGEBUTTON) {
        if ("LOAD" == name && 1 == loadBtn->getValue()) {
            ofFileDialogResult result = ofSystemLoadDialog("Select a .oni file");
            
            if (result.bSuccess) {
                ofFile file(result.getPath());
                
                if (file.exists() && "ONI" == ofToUpper(file.getExtension())){
                    oniFileName = result.getName();
                }
                
                ofLogVerbose(".oni file is " + oniFileName);
                
                playContext.startPlayer(oniFileName);
                playContext.setPaused(true);
                
                numTotalFrames = playContext.getTotalNumFrames();
                ofLogVerbose("total frame: " + ofToString(numTotalFrames));
                ofLogVerbose("total time : " + ofToString(numTotalFrames / 30) + " sec");
            } else {
                ofLogVerbose("User hit cancel");
            }
        } else if ("PLAY" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/pause.png");
            playBtn->setName("PAUSE");
            
            if (playContext.isPlaying()) {
                playContext.setPaused(false);
            }
        } else if ("PAUSE" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/play.png");
            playBtn->setName("PLAY");
            
            playContext.setPaused(true);
        } else if ("RECORD" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/stop.png");
            recordBtn->setName("STOP");
            
            if (!openNIDevice.isRecording()) {
                newOniFileName = ofToDataPath(ofToString(ofGetUnixTime()) + ".oni");
                openNIDevice.startRecording(newOniFileName);
            }
        } else if ("STOP" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/record.png");
            recordBtn->setName("RECORD");
            
            if (openNIDevice.isRecording()) {
                openNIDevice.stopRecording();
                
                playContext.startPlayer(newOniFileName);
                playContext.setPaused(true);
            }
        }
    }
}

