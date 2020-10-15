// Paste from Template (Top)-------------------------------------
#include "ofApp.h"

ofTrueTypeFont font;

int Width = 800;
int Height = 480;

int pgM21 = 0;
int  pReset = 0;

float deltaTime1 = 0.0;
float deltaTime2 = 0.0;

float pos1 = 0.0;
float pos2 = 0.0;
float posSpeed = 1.0;
int PosTotal = 18;
int mid = (PosTotal-2)/2;

int MorS;
int C_pos;

int Check_Angle = 0;
int CheckSpeed = 0;
bool text = 0;

float t1 = 0.0;

int StartT1;
int StartT2;

// Paste from Template (bottom) ----------------------------------

//i2c(Top)-----------------------------------------------------
/*
int i_i2c;
int handle;//master
int result;//master
bsc_xfer_t xfer; //slave
int status;//slave
char a ;
*/
//i2c(Bottom)--------------------------------------------------

ofEasyCam cam;

float t2 = 0.0;
float tDeg = 0.0;

int StartPoint = 300;
int BoxSize = 50;

float brightness2 = 0.5;
float brightness3 = -0.5;

float camDeg = 0;
float camRad = 0.0;
int camDis = 1500;

ofLight PointLight2;
ofLight PointLight3;

ofPoint LightPoint2 = ofPoint(1000,0,0); 
ofPoint LightPoint3 = ofPoint(-3000,10000,0); 
ofPoint LightPoint4 = ofPoint(-1000,250,-500); 
ofPoint LightPoint5 = ofPoint(-1000,250,500); 

bool flatMoon = 0;
bool solidMoon = 0;
bool nomBoxes = 0;

float camRotateSpeed = 0.3;

float elapsed4;
float elapsed7;

//--------------------------------------------------------------
void ofApp::setup(){

 //大きすぎるデータの読み込みは動作に影響を与えるので注意！
 // Paste from Template (Top)-------------------------------------
    MyClass m;

    font.load("RictyDiminished-Regular.ttf",12);
    ofSetVerticalSync(false);
    // ofSetFrameRate(60);
    ofSetWindowShape(Width,Height);
    ofHideCursor();
    
    gpioInitialise();
    gpioSetMode(21,PI_INPUT);
    gpioSetMode(20,PI_OUTPUT);//master
    gpioSetMode(16,PI_INPUT);//slave

        MorS =  ofToInt(ofBufferFromFile("master_or_slave.txt").getText());
    if(MorS == 0){
        C_pos = 0;
    }
    if(MorS == 1){
        C_pos = PosTotal/2;
    }
    
 // Paste from Template (bottom) ----------------------------------

 //i2c(Top)-----------------------------------------------------
    /*
    handle = i2cOpen(1,0x04,0);//master
    if(handle<0) {std::cout << "can not open i2c" << endl; }
    xfer.control = (0x04<<16) | 0x305 ;
    */
 //i2c(Bottom)--------------------------------------------------

 //ofRPitouch(Top)----------------------------------------------
    touch.init("/dev/input/event0");    // change according to your setup (evdev)
    ofLog()<<touch.getName();
 //ofRPitouch(Bottom)-------------------------------------------

    ofEnableDepthTest();
    cam.setDistance(camDis); //EasyCam
    cam.setFov(10.0f);

}

//--------------------------------------------------------------
void ofApp::update(){

  //i2c(Top)-----------------------------------------------------
    /*
    i_i2c++;
    if(i_i2c >= 3){
        i_i2c = 0;
        //送信量を増やすと動かなくなることがあるので注意！
        
        if(MorS == 0){
            char buf[2] = {(char)CheckSpeed,(char)Check_Angle }; 
            result = i2cWriteI2CBlockData(handle,0x00,buf,2);
            if( result < 0 ) { std::cout << "can not send Data" << endl; }
        }
        if(MorS == 1){
            status = bscXfer(&xfer);
            if(xfer.rxCnt > 0){
                a = xfer.rxBuf[1];
                CheckSpeed = (unsigned char)a;
                a = xfer.rxBuf[2];
                Check_Angle = (unsigned char)a;
            }
        }
    }
    */
    //i2c(Bottom)--------------------------------------------------

    //ofRPitouch(Top)----------------------------------------------
    if(touch.getButton() == 1){ ofExit(); }
    //ofRPitouch(Bottom)-------------------------------------------

    // Paste from Template (Top)-------------------------------------
    if(ofGetFrameRate() != 0){t1 += 20 / ofGetFrameRate(); }
    if(t1  >= elapsed7 ){ t1 = 0; }

    int StartT1 = elapsed4 - 100;

     if(gpioRead(16) == 1 ||  (int)t1 ==  StartT1){
        if( pReset == 0 ){
            t1 = StartT1;
            t2 = 0;
            flatMoon = 0;
            solidMoon = 0;
            nomBoxes = 0;
            gpioWrite(20,1);//master
        }
        pReset = 1; //slave
    }else{
        gpioWrite(20,0); //master
        pReset =  0;   
    }//if(gpioRead(16) == 1 ||  (int)t ==  0)

    if(ofGetFrameRate() != 0){ deltaTime1= deltaTime1+ 1/ofGetFrameRate(); }

    if(gpioRead(21) == 1 ){
        
        if(pgM21 == 0 ){
            pos1 = C_pos ;
            pos2 = pos1;

            if(deltaTime1 != 0){
              posSpeed  =  PosTotal / (2 * deltaTime1) ;
            //   posSpeed  = 1;
            }else{
               posSpeed = 1;
            }
            deltaTime1 = 0;
        }
        pgM21 = 1;

    }else{
        
        if(pgM21 == 1 ){

            pos1 = pos1 + PosTotal/2  ;
            if(pos1 <  0 || PosTotal <=  pos1){  pos1 = float(fmod(pos1,PosTotal)); }
            pos2 = pos1;

             if(deltaTime1 != 0){
                posSpeed  =  PosTotal / (2 * deltaTime1) ;
            }else{
               posSpeed = 1;
            }
            deltaTime1 = 0;

        }// if(pgM21 == 0 )
       pgM21 = 0;

    } //if(gpioRead(21) == 1 )

    if(ofGetFrameRate() != 0){ deltaTime2 = deltaTime2 + 1 / ofGetFrameRate(); }

    pos2 = pos2 +posSpeed *deltaTime2  ;
    if(pos2 <  0 || PosTotal <=  pos2){  pos2 = float(fmod(pos2,PosTotal)); }
    deltaTime2 = 0;

    // Paste from Template (bottom) ----------------------------------
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    //Sequence(Top)------------------------------------------------

    // e++;
    if(ofGetFrameRate() != 0){  t1+= 60 / ofGetFrameRate(); }
   
    float elapsed0 = 0;
    float dElapsed0 = 0; 
    if(elapsed0 <= t1 && t1 < elapsed0 + dElapsed0){  } else {  }

    //フラット円
    float elapsed1 = elapsed0 + dElapsed0;
    float dElapsed1 = 300; 
    if(elapsed1 <= t1 && t1 < elapsed1 + dElapsed1){ flatMoon = 1; } else { flatMoon = 0; }

    //月
    float elapsed2 = elapsed1 + dElapsed1;
    float dElapsed2 = 1200; 
    if(elapsed2 <= t1 && t1 < elapsed2 + dElapsed2){ solidMoon = 1; } else { solidMoon = 0; }

    //暗転
    elapsed4 = elapsed2 + dElapsed2;
    float dElapsed4 = 150; 
    if(elapsed4 <= t1 && t1 < elapsed4 + dElapsed4){  } else {  }
    
    //立方体
    float elapsed5 = elapsed4 + dElapsed4;
    float dElapsed5 = 1800; 
    if(elapsed5 <= t1 && t1 < elapsed5 + dElapsed5){ nomBoxes = 1; }  else { nomBoxes = 0; }
    // if(int(e)  == elapsed5 + dElapsed5){ nomBoxes = 0; }

    //暗転
    float elapsed6 = elapsed5 + dElapsed5;
    float dElapsed6 = 100;
    if(elapsed6 <= t1 && t1 < elapsed6 + dElapsed6){  } else {  }
  
     elapsed7 = elapsed6 + dElapsed6;

    //Sequence(bottom)----------------------------------------------

    camDeg = 360/PosTotal*int(pos2);
    cam.orbitDeg(0,camDeg,camDis,{-260,0,0});
    cam.rotate(-10.0,0.0,cos(camDeg*PI/180),sin(camDeg*PI/180));

    cam.begin();
    ofBackground(0);

    if(ofGetFrameRate() != 0){ t2 += 20 / ofGetFrameRate(); }
    int StartT2 = 360*2;
    if(t2 >= StartT2 ){ t2 = 0; }
    tDeg = t2 * PI/180;

    //Moon---------------------------------------------------------

    if(flatMoon == 1){
        // PointLight2.setAmbientColor(ofFloatColor(1.5));
        PointLight2.setAmbientColor(ofFloatColor(1.5));
        PointLight2.setPosition(ofPoint( 0 , 0 , 0 ));

        PointLight2.enable();
            
            ofSetColor(255);
            ofDrawSphere(ofPoint(0,0,0),120);  

        PointLight2.disable();
    }

    if(solidMoon == 1){
        PointLight2.setAmbientColor(ofFloatColor(-0.1));
        // PointLight2.setAmbientColor(ofFloatColor(brightness2));
        PointLight2.setPosition(ofPoint( 5000 , 10000 , 0 ));
        PointLight2.enable();
    
            ofSetColor(255,255,180);
                moon.set(120,10);
                moon.setPosition(0,0,0);
                moon.drawFaces();
            ofPopMatrix();

        PointLight2.disable();
    }

    //Moon---------------------------------------------------------

    //Boxes--------------------------------------------------------
    
    if(nomBoxes == 1){
        PointLight2.setAmbientColor(ofFloatColor(brightness2));
        PointLight2.setAmbientColor(ofFloatColor(-0.1));
        PointLight2.setPosition(LightPoint4);
        PointLight2.enable(); 
        
        PointLight3.setAmbientColor(ofFloatColor(brightness2));
        PointLight3.setAmbientColor(ofFloatColor(0.0));
        PointLight3.setPosition(LightPoint5);
        PointLight3.enable(); 
        int BoxRotate = t2/2;

        ofSetColor(255);

        ofPushMatrix();
            ofTranslate(140*cos(-tDeg-0.3*PI),-30,-20);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),50);    
        ofPopMatrix();
        ofPushMatrix();
            ofTranslate(140*cos(-tDeg),30,0);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),60);    
        ofPopMatrix();
        ofPushMatrix();
            ofTranslate(130*cos(-tDeg+0.1*PI),70,10);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),40);    
        ofPopMatrix();

        ofPushMatrix();
            ofTranslate(60*cos(-tDeg-0.8*PI),-50,0);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),60);    
        ofPopMatrix();
        ofPushMatrix();
            ofTranslate(60*cos(-tDeg-0.6*PI),-10,-40);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),70);    
        ofPopMatrix();
        ofPushMatrix();
            ofTranslate(70*cos(-tDeg-0.2*PI),20,20);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),80);    
        ofPopMatrix();

        ofPushMatrix();
            ofTranslate(90*cos(-tDeg+0.9*PI),-10,-20);
            // ofRotateDeg(t,45,45,45);
            ofRotateDeg(20,0,1,0);
            ofRotateDeg(BoxRotate,1,0,0);
            ofDrawBox(ofPoint(0,0,0),45);    
        ofPopMatrix();

        //中心点
        // ofPushMatrix();
        //     ofSetColor(255,0,0);
        //     ofDrawSphere(ofPoint(0,0,0),1);   
        // ofPopMatrix();

        //中心線
        // ofDrawLine(-ofGetWidth()/2, 0, 0, ofGetWidth()/2, 0, 0);
            
        PointLight3.disable();
        PointLight2.disable();
    }

    //Boxes--------------------------------------------------------

    cam.end();

    // Paste from Template (Top)-------------------------------------

    // ofBackground(0);

    if(Check_Angle== 100){
        float x;
        float X = Width/(PosTotal-1) ;

        ofBackground(0);

        if(int(pos2) == 0){
            x = int(pos2)+ mid ;
            ofSetColor(255);
        }
        if(1 <= int(pos2) && int(pos2) <= mid){
            x = int(pos2)+ mid ;
            ofSetColor(0,0,255);
        }
        if(mid+1 <= int(pos2) && int(pos2) <= PosTotal-1){
            x = int(pos2)+mid-PosTotal;
            ofSetColor(255,0,0);
        }

        ofRect(X*x,0,X,Height);
        ofSetColor(255);

    }// if(Check_Angle== true)

    if(CheckSpeed== 100){

        ofBackground(0);
        ofSetColor(255);

        if(posSpeed > 65.0 ){
            ofSetColor(255,0,0);
        }
        if(posSpeed < 55.0 ){
            ofSetColor(0,0,255);
        }
        ofRect(Width-(200*posSpeed/PosTotal),0,Width,Height);
    }
    
    if(text == 1){
        ofSetColor(255);
        font.drawString("Framerate: " + ofToString(ofGetFrameRate()) , 10, 20);
        font.drawString("t2 : " + ofToString(t2) , 10, 40);
        font.drawString("pos2: " + ofToString(pos2) , 10, 80);
        font.drawString("C_pos: " + ofToString(C_pos) , 10, 100);
        // font.drawString(ofToString(touch.getButton()), 10, 280);

         font.drawString("t1: " + ofToString(t1) , 10, 140);
    }

    // Paste from Template (bottom) ----------------------------------

}

void ofApp::keyPressed(int key){

    // Paste from Template (Top)-------------------------------------
    if (key == 't'){ 
        if(text == 0){
            text = 1;
        }else{
            text = 0;
        }
    }

     if (key == 'c'){ 
        if(Check_Angle == 0){
            Check_Angle = 100;
        }else{
            Check_Angle = 0;
        }   
    }

     if (key == 'v'){ 
        if(CheckSpeed == 0){
            CheckSpeed = 100; 
        }else{
            CheckSpeed = 0; 
        }
    }

    if (key == 'z'){ 
        if( C_pos == 0){
            C_pos = PosTotal/2; 
        }else{
            C_pos = 0; 
        }
    }
    // Paste from Template (bottom) ----------------------------------
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
