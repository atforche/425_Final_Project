#include "ofApp.h"
#include "Simulation.h"
#include "Brain.h"
#include "Tournament.h"

Tournament* tournament;
Sim sim;
int num_generations;

enum MODE {Position, Sensors, Image_Sensor};

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(1000);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	MODE mode = Sensors;
	int population_size = 50;
	num_generations = 100;
	tournament = new Tournament(population_size,num_generations);
	std::vector<int> hidden_layers;
	switch (mode) {
	case Position:
		hidden_layers = { 3,3 };
		tournament->Initialize_Population(3, hidden_layers);
		break;
	case Sensors:
		hidden_layers = { 11,11 };
		tournament->Initialize_Population(11, hidden_layers);
		break;
	case Image_Sensor:

		break;
	}
	tournament->Set_Mode(mode);
	tournament->Initialize(population_size);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetFrameRate(1000);
	if (tournament->Get_Generation() % num_generations == 0) {
		ofSetFrameRate(60);
	}
	ofBackground(255, 204, 153);
	//ofDrawCircle(700, 100, 5);
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
