#include "ofApp.h"
#include "Simulation.h"
#include "Brain.h"
#include "Tournament.h"

Tournament* tournament;
Sim sim;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	tournament = new Tournament(20,20);
	std::vector<int> hidden_layers = { 2,2 };
	tournament->Initialize_Population(11, hidden_layers);
	tournament->Initialize(20);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255, 204, 153);
	tournament->Run_Tournament();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
