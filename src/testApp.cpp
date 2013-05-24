#include "testApp.h"

//--------------------------------------------------------------
void testApp::setupGUI()
{
    gui = new ofxUICanvas(0, NI_VIEW_HEIGHT, ofGetWidth(), 100);
    
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
    ofBackground(255);
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetLogLevel(OF_LOG_VERBOSE);

#ifdef NI_ENABLE
    openNIDevice.setupFromXML("openni/config/ofxopenni_config.xml");
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();
    openNIDevice.setDepthColoring(COLORING_CYCLIC_RAINBOW);
    //openNIDevice.addDepthThreshold(500, 1000);
    openNIDevice.start();
    openNIDevice.setRegister(true);
    // openNIDevice.addInfraGenerator(); // and uncomment this to see infrared generator
                                        // or press the 'i' key when running
    
    ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 10000, true, true, true, true, true);
    openNIDevice.addDepthThreshold(depthThreshold);
    // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
    // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true,
    // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false,
    // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
    
    openNIPlayer.setup();
    openNIPlayer.setLogLevel(OF_LOG_ERROR);
    openNIPlayer.setRegister(true);
    
    isONIPlaying = false;
    isDrawDepthOverlay = true;
    
    rgbImg.allocate(NI_VIEW_WIDTH, NI_VIEW_HEIGHT, OF_IMAGE_COLOR_ALPHA);
    depthImg.allocate(NI_VIEW_WIDTH, NI_VIEW_HEIGHT, OF_IMAGE_COLOR_ALPHA);
#endif
    
    videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
    setupGUI();
            
    scale = 1920 * (NI_VIEW_WIDTH / 1920);
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef NI_ENABLE
    if (openNIDevice.isContextReady()) {
        openNIDevice.update();
    }
    
    if (openNIPlayer.isContextReady() && openNIPlayer.isPlaying()) {
        openNIPlayer.update();
        rgbImg.setFromPixels(openNIPlayer.getImagePixels());
        
        
        
        if (isDrawDepthOverlay) {
            depthImg.setFromPixels(openNIPlayer.getDepthPixels());
        }
        currentFrame = openNIPlayer.getCurrentFrame();
    }
    
    if (videoPlayer.isLoaded() && videoPlayer.isPlaying()) {
        videoPlayer.update();
    }
#endif
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255, 255, 255);

#ifdef NI_ENABLE
    if (videoPlayer.isLoaded()) {
        videoPlayer.draw(NI_VIEW_WIDTH - (853 - 640) / 2, 0, 853, NI_VIEW_HEIGHT);
        // videoPlayer.draw(0, 0, videoPlayer.getWidth() * scale, videoPlayer.getHeight() * scale);
    }
    
    if (openNIDevice.isNewFrame()) {
        if (openNIDevice.isImageOn()) {
            openNIDevice.drawImage(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        }
        
        openNIDevice.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
    }
    
    if (openNIPlayer.isContextReady() && openNIPlayer.isPlaying()) {
        openNIPlayer.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        openNIPlayer.drawImage(NI_VIEW_WIDTH, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        
        if (isDrawDepthOverlay) {
            //openNIPlayer.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
            depthImg.draw(NI_VIEW_WIDTH, 0);
        }
    } else {
        ofPushStyle();
        ofSetColor(0);
        ofFill();
        ofRect(0, NI_VIEW_HEIGHT, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        ofPopStyle();
    }
#endif
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(openNIPlayer.getCurrentFrame() / 30.0f), 100, NI_VIEW_HEIGHT * 2);

    ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
	verdana.drawString(msg, 20, ofGetHeight() - 26);

}

//--------------------------------------------------------------
void testApp::exit(){
#ifdef NI_ENABLE
    openNIDevice.stop();
    openNIPlayer.stop();
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
        case 'd': {
            isDrawDepthOverlay = !isDrawDepthOverlay;
            break;
        }
            
#ifdef NI_ENABLE
        case 'i': {
            if (openNIDevice.isImageOn()) {
                openNIDevice.removeImageGenerator();
            } else {
                openNIDevice.addImageGenerator();
            }
            break;
        }
#endif
        case 'o': {
            
            break;
        }
            
        case '1':
            openNIDevice.setDepthColoring(COLORING_CYCLIC_RAINBOW);
            break;
        case '2':
            openNIDevice.setDepthColoring(COLORING_BLUES);
            break;
        case '3':
            openNIDevice.setDepthColoring(COLORING_COUNT);
            break;
        case '4':
            openNIDevice.setDepthColoring(COLORING_GREY);
            break;
        case '5':
            openNIDevice.setDepthColoring(COLORING_PSYCHEDELIC);
            break;
        case '6':
            openNIDevice.setDepthColoring(COLORING_PSYCHEDELIC_SHADES);
            break;
        case '7':
            openNIDevice.setDepthColoring(COLORING_RAINBOW);
            break;
        case '8':
            openNIDevice.setDepthColoring(COLORING_STATUS);
            break;
            
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
void testApp::guiEvent(ofxUIEventArgs &e)
{
    ofLogVerbose("gui event, value: " + ofToString(playBtn->getValue()));
    
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if (OFX_UI_WIDGET_IMAGEBUTTON) {
        if ("LOAD" == name && 1 == loadBtn->getValue()) {
            ofFileDialogResult result = ofSystemLoadDialog("Select a .oni file");
            
            if (result.bSuccess) {
                string path = result.getPath();
                ofFile oniFile(path);
                
                ofStringReplace(path, ".oni", ".mov");
                ofLogVerbose(".mov file is " + path);
                ofFile movFile(path);
                
                if (oniFile.exists() && "ONI" == ofToUpper(oniFile.getExtension())){
                    oniFileName = result.getName();
                }
                
//                if (movFile.exists() && "MOV" == ofToUpper(movFile.getExtension())) {
//                    videoPlayer.closeMovie();
//                    videoPlayer.loadMovie(movFile.path(), OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
//                    videoPlayer.firstFrame();
//                }
                
                ofLogVerbose(".oni file is " + oniFileName);
                
                openNIDevice.stop();
                
                openNIPlayer.startPlayer(oniFileName);
                openNIPlayer.setPaused(true);
                openNIPlayer.firstFrame();
                
                numTotalFrames = openNIPlayer.getTotalNumFrames();
                ofLogVerbose("total frame: " + ofToString(numTotalFrames));
                ofLogVerbose("total time : " + ofToString(numTotalFrames / 30) + " sec");
            } else {
                ofLogVerbose("User hit cancel");
            }
        } else if ("PLAY" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/pause.png");
            playBtn->setName("PAUSE");
            
            if (openNIPlayer.isPlaying()) {
                openNIPlayer.setPaused(false);
                
                isONIPlaying = true;
            }
            
            if (videoPlayer.isPlaying()) {
                videoPlayer.setPaused(false);
            } else {
                videoPlayer.play();
            }
        } else if ("PAUSE" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/play.png");
            playBtn->setName("PLAY");
            
            if (openNIPlayer.isPlaying()) {
                openNIPlayer.setPaused(true);
                isONIPlaying = false;
            }
            
            if (videoPlayer.isPlaying()) {
                videoPlayer.setPaused(true);
            }
        } else if ("RECORD" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/stop.png");
            recordBtn->setName("STOP");
            
            if (!openNIDevice.isRecording()) {
                newOniFileName = ofToDataPath(ofToString(ofGetTimestampString()) + ".oni");
                openNIDevice.startRecording(newOniFileName);
            }
        } else if ("STOP" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/record.png");
            recordBtn->setName("RECORD");
            
            if (openNIDevice.isRecording()) {
                openNIDevice.stopRecording();
                
                openNIPlayer.startPlayer(newOniFileName);
                openNIPlayer.setPaused(true);
            }
        } else if ("REWIND" == name && 1 == rewindBtn->getValue()) {
            openNIPlayer.previousFrame();
        } else if ("FORWARD" == name && 1 == forwardBtn->getValue()) {
            openNIPlayer.nextFrame();
        }
    }
}

