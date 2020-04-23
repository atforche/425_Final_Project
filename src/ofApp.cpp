#include "ofApp.h"
#include "Simulation.h"
#include "Brain.h"
#include "Tournament.h"

Tournament* tournament;
Sim sim;
int num_generations;

enum MODE {Position, Sensors, Image, Image_and_Sensors};

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	MODE mode = Image_and_Sensors;
	int population_size = 15;
	num_generations = 20;
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
	case Image:
		hidden_layers = { 200,50 };
		tournament->Initialize_Population(90, hidden_layers);
		break;
	case Image_and_Sensors:
		hidden_layers = { 200,100 };
		tournament->Initialize_Population(101, hidden_layers);
	}
	tournament->Set_Mode(mode);
	tournament->Initialize(population_size);
	//tournament->Simulate_Brains_Initialize();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetFrameRate(1000);
	//ofSetFrameRate(30);
	//ofSetFrameRate(1);
	if (tournament->Get_Generation() % num_generations == 0) {
		ofSetFrameRate(30);
	}
	ofBackground(255, 204, 153);
	//ofDrawCircle(700, 100, 5);
	tournament->Run_Tournament(true);
	//tournament->Render(true);
	//tournament->Test();
	//tournament->Simulate_Brains();
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
