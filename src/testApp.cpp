#include "testApp.h"

//--------------------------------------------------------------
void testApp::setupGUI()
{
    gui = new ofxUICanvas(0, NI_VIEW_HEIGHT, ofGetWidth(), 150);
    
    loadBtn = new ofxUIImageButton(50, 50, true, "gui/file@2x.png", "LOAD");
    playBtn = new ofxUIImageButton(50, 50, true, "gui/play@2x.png", "PLAY");
    recordBtn = new ofxUIImageButton(50, 50, true, "gui/record@2x.png", "RECORD");
    stopBtn = new ofxUIImageButton(50, 50, true, "gui/stop@2x.png", "STOP");
    rewindBtn = new ofxUIImageButton(50, 50, true, "gui/rewind@2x.png", "REWIND");
    forwardBtn = new ofxUIImageButton(50, 50, true, "gui/forward@2x.png", "FORWARD");
    removeBtn = new ofxUIImageButton(290, 43, 15, 15, true, "gui/remove.png", "REMOVE");
    removeBtn->setVisible(false);
    
    static ofxUISpacer* spacer1 = new ofxUISpacer(1, 7);
    static ofxUISpacer* spacer2 = new ofxUISpacer(ofGetWidth() / 2 - 190, 42);
    static ofxUISpacer* spacer3 = new ofxUISpacer(10, 42);
    spacer1->setDrawFill(false);
    spacer1->setDrawBack(false);
    spacer2->setDrawFill(false);
    spacer2->setDrawBack(false);
    spacer3->setDrawFill(false);
    spacer3->setDrawBack(false);
    
    gui->addWidgetDown(spacer1);
    gui->addWidgetDown(loadBtn);
    gui->addWidget(removeBtn);
    gui->addWidgetRight(spacer2);
    gui->addWidgetRight(spacer3);
    gui->addWidgetRight(rewindBtn);
    gui->addWidgetRight(playBtn);
    gui->addWidgetRight(recordBtn);
    gui->addWidgetRight(forwardBtn);
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    gui->setColorBack(ofColor(0));
    gui->setWidgetColor(OFX_UI_WIDGET_COLOR_BACK, ofColor(255));
    gui->setAutoDraw(false);
}

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofBackground(255);
    ofEnableAlphaBlending();
    ofSetWindowTitle("Recorder");
    ofSetLogLevel(OF_LOG_VERBOSE);

#ifdef NI_ENABLE
    openNIDevice.setupFromXML("openni/config/ofxopenni_config.xml");
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();
    openNIDevice.setDepthColoring(COLORING_RAINBOW);
    openNIDevice.start();
    openNIDevice.setRegister(true);
    
    openNIPlayer.setup();
    openNIPlayer.setLogLevel(OF_LOG_ERROR);
    openNIPlayer.setRegister(true);
    
    isONIPlaying = false;
    isONILoaded = false;
    isDrawDepthOverlay = true;
    
    rgbImg.allocate(NI_VIEW_WIDTH, NI_VIEW_HEIGHT, OF_IMAGE_COLOR_ALPHA);
    depthImg.allocate(NI_VIEW_WIDTH, NI_VIEW_HEIGHT, OF_IMAGE_COLOR_ALPHA);
#endif
    
    videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 10);
    
    setupGUI();
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef NI_ENABLE
    if (openNIDevice.isContextReady() && !isONIPlaying) {
        openNIDevice.update();
    }
    
    if (openNIPlayer.isContextReady() && isONILoaded && isONIPlaying) {
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
    if (isONILoaded) {
        openNIPlayer.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        openNIPlayer.drawImage(NI_VIEW_WIDTH, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        
        if (isDrawDepthOverlay) {
            //openNIPlayer.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
            depthImg.draw(NI_VIEW_WIDTH, 0);
        }
    } else {
        if (openNIDevice.isContextReady()) {
            if (openNIDevice.isImageOn()) {
                openNIDevice.drawImage(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
            }
            
            openNIDevice.drawDepth(0, 0, NI_VIEW_WIDTH, NI_VIEW_HEIGHT);
        }
    }
#endif

    gui->draw();
    
    ofSetColor(255);
    verdana.drawString("ONI file name:\n" + oniFileName, 65, ofGetHeight() - 39);
	verdana.drawString("App    FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 200, ofGetHeight() - 45);
    if (openNIPlayer.isPlaying()) {
        verdana.drawString("OpenNI FPS: " + ofToString(openNIPlayer.getFrameRate()), ofGetWidth() - 200, ofGetHeight() - 25);
    } else {
        verdana.drawString("OpenNI FPS: " + ofToString(openNIDevice.getFrameRate()), ofGetWidth() - 200, ofGetHeight() - 25);
    }

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
                
                ofLogVerbose(".oni file is " + oniFileName);
                
                isONILoaded = true;
                isONIPlaying = true;
                
                openNIPlayer.start();
                openNIPlayer.startPlayer(oniFileName);
                openNIPlayer.setPaused(true);
                openNIPlayer.firstFrame();
                
                removeBtn->setVisible(true);
                
                numTotalFrames = openNIPlayer.getTotalNumFrames();
                ofLogVerbose("total frame: " + ofToString(numTotalFrames));
                ofLogVerbose("total time : " + ofToString(numTotalFrames / 30) + " sec");
            } else {
                removeBtn->setVisible(false);
                ofLogVerbose("User hit cancel");
            }
        } else if ("REMOVE" == name && 1 == removeBtn->getValue()) {
            removeBtn->setVisible(false);
            
            openNIPlayer.setPaused(true);
            oniFileName = "";
            
            isONILoaded = false;
            isONIPlaying = false;
        } else if ("PLAY" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/pause@2x.png");
            playBtn->setName("PAUSE");
            
            if (openNIPlayer.isPlaying()) {
                openNIPlayer.setPaused(false);
                
                isONIPlaying = true;
            }
        } else if ("PAUSE" == name && 1 == playBtn->getValue()) {
            playBtn->setImage("gui/play@2x.png");
            playBtn->setName("PLAY");
            
            if (openNIPlayer.isPlaying()) {
                openNIPlayer.setPaused(true);
                isONIPlaying = false;
            }
            
            if (videoPlayer.isPlaying()) {
                videoPlayer.setPaused(true);
            }
        } else if ("RECORD" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/stop@2x.png");
            recordBtn->setName("STOP");
            
            if (!openNIDevice.isRecording()) {
                newOniFileName = ofToDataPath(ofToString(ofGetTimestampString()) + ".oni");
                openNIDevice.startRecording(newOniFileName);
            }
        } else if ("STOP" == name && 1 == recordBtn->getValue()) {
            recordBtn->setImage("gui/record@2x.png");
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

