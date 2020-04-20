#include "Wall.h"

Wall::Wall() {
	beginning = Coordinate(0, 0);
	ending = Coordinate(0, 0);
}

//--------------------------------------------------------------------------------------

Wall::Wall(Coordinate begin, Coordinate end) {
	beginning = begin;
	ending = end;
}

//--------------------------------------------------------------------------------------

void Wall::Render() {
	ofSetColor(0, 255, 0);
	ofSetLineWidth(5);
	ofDrawLine(beginning.GetX(), beginning.GetY(), ending.GetX(), ending.GetY());
}

//--------------------------------------------------------------------------------------

Coordinate Wall::GetBeginning() {
	return beginning;
}

//--------------------------------------------------------------------------------------

Coordinate Wall::GetEnding() {
	return ending;
}

//--------------------------------------------------------------------------------------

ofColor& Wall::Get_Color() {
	return color;
}