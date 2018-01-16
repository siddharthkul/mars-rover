//--------------------------------------------------------------
//
//  CS235 - User Interface Design - Kevin M. Smith
//
//  Assignment 5 -  Mars HiRise Project - startup scene
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  Name:   < Siddharth Kulkarni >
//  Date: <date of last version>

#include "ofApp.h"

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup() {

	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bPointSelected = false;
	bRover = false;
	bTeleportRover = false;
	bNoAxis = true;
	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	// load model (but do not scale)
	//
	mars.loadModel("mars-low-v2.obj");
	mars.setScaleNormalization(false);
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
	if (bMoveCameraForward && !bCtrlKeyDown) moveCamera(CamMoveForward, .05);
	else if (bMoveCameraBackward && !bCtrlKeyDown) moveCamera(CamMoveBackward, .05);

	else if (bRotateCameraLeft) rotateCamera(CamRotateLeft, -1.0);   // rotate left/right
	else if (bRotateCameraRight) rotateCamera(CamRotateRight, 1.0);
	else if (bMoveCameraForward && bCtrlKeyDown) rotateCamera(CamTiltUp, -1.0);  // rotate up/down
	else if (bMoveCameraBackward && bCtrlKeyDown) rotateCamera(CamTiltDown, 1.0);

	else if (bTeleport) teleportCamera();
}

//--------------------------------------------------------------
void ofApp::draw() {

	
	ofBackground(ofColor::black);
	
	//Background used for screen shot
	//ofColor colorOne(255, 165, 0);
	//ofColor colorTwo(0, 0, 255);
	//ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);

	cam.begin();
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
	}

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, 0.1);
	}

	// draw rover
	//
	if (bRover) {
		rover.drawFaces();
	}

	// teleport rover
	if (bTeleportRover) {
		teleportRover();
	}

	// draw axis when no button is pressed
	if (bNoAxis) {
		drawAxis(0);
	}

	// draw axis with x pressed
	if (bXAxis) {
		drawAxis(1);
	} 

	// draw axis with y pressed
	if (bYAxis) {
		drawAxis(2);
	}

	// draw axis with z pressed
	if (bZAxis) {
		drawAxis(3);
	}

	ofPopMatrix();
	cam.end();
}

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(int key) {

	ofPushMatrix();
	ofSetColor(ofColor(255, 0, 0));
	// draw x axis on rover
	if (bAxisOnRover) {
		ofPoint roverPosition = rover.getPosition();
		ofDrawLine(roverPosition, ofPoint(roverPosition.x+1, roverPosition.y, roverPosition.z));
	}
	else {
		ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	}
	if (key == 1) {
		ofSetLineWidth(3);
		if (bAxisOnRover) {
			ofPoint roverPosition = rover.getPosition();
			ofDrawLine(roverPosition, ofPoint(roverPosition.x + 1, roverPosition.y, roverPosition.z));
		}
		else {
			ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
		}
		ofSetLineWidth(1);
	}
	ofSetColor(ofColor(0, 255, 0));
	// draw y axis on rover
	if (bAxisOnRover) {
		ofPoint roverPosition = rover.getPosition();
		ofDrawLine(roverPosition, ofPoint(roverPosition.x, roverPosition.y + 1, roverPosition.z));
	}
	else {
		ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
	}
	if (key == 2) {
		ofSetLineWidth(10);
		if (bAxisOnRover) {
			ofPoint roverPosition = rover.getPosition();
			ofDrawLine(roverPosition, ofPoint(roverPosition.x, roverPosition.y + 1, roverPosition.z));
		}
		else {
			ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
		}
		ofSetLineWidth(1);
	}
	ofSetColor(ofColor(0, 0, 255));
	// draw z axis on rover
	if (bAxisOnRover) {
		ofPoint roverPosition = rover.getPosition();
		ofDrawLine(roverPosition, ofPoint(roverPosition.x, roverPosition.y, roverPosition.z + 1));
	}
	else {
		ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
	}
	if (key == 3) {
		ofSetLineWidth(10);
		if (bAxisOnRover) {
			ofPoint roverPosition = rover.getPosition();
			ofDrawLine(roverPosition, ofPoint(roverPosition.x, roverPosition.y, roverPosition.z + 1));
		}
		else {
			ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
		}
		ofSetLineWidth(1);
	}

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'r':
		cam.reset();
		break;
	case 's':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_LEFT:
		bRotateCameraLeft = true;
		break;
	case OF_KEY_RIGHT:
		bRotateCameraRight = true;
		break;
	case OF_KEY_UP:
		bMoveCameraForward = true;
		break;
	case OF_KEY_DOWN:
		bMoveCameraBackward = true;
		break;
	case OF_KEY_F1:
		teleportStartPoint = cam.getPosition();
		bTeleport = true;
		break;
	case OF_KEY_F2:
		teleportStartPoint = rover.getPosition();
		bTeleportRover = true;
		break;

	case 'x':
	case 'X':
		bNoAxis = false;
		bXAxis = true;
		break;
	case 'y':
	case 'Y':
		bNoAxis = false;
		bYAxis = true;
		break;
	case 'z':
	case 'Z':
		bNoAxis = false;
		bZAxis = true;
		break;
	case 'm':
	case 'M':
		if (&rover != NULL) {
			bAxisOnRover = !bAxisOnRover;
		}
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
	switch (key) {

	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_LEFT:
		bRotateCameraLeft = false;
		break;
	case OF_KEY_RIGHT:
		bRotateCameraRight = false;
		break;
	case OF_KEY_UP:
		bMoveCameraForward = false;
		break;
	case OF_KEY_DOWN:
		bMoveCameraBackward = false;
		break;
	case 'x':
	case 'X':
	case 'y':
	case 'Y':
	case 'z':
	case 'Z':
		bNoAxis = true;
		bXAxis = false;
		bYAxis = false;
		bZAxis = false;
		break;
	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
	if (bMoveRover) {
		
		ofVec3f intersection = rover.getPosition();
		//Get the plane normal
		ofVec3f planeNormal = cam.getZAxis().normalize();
		//Check if the intersection occurs
		bool doesIntersect = mouseIntersectPlane(intersection, planeNormal, intersection);
		//Set the rover position according to the intersections
		if (doesIntersect) {
			if (bNoAxis) {
				rover.setPosition(intersection.x, intersection.y, intersection.z);
			} 
			if (bXAxis) {
				rover.setPosition(intersection.x, rover.getPosition().y, rover.getPosition().z);
			}
			if (bYAxis) {
				rover.setPosition(rover.getPosition().x, intersection.y, rover.getPosition().z);
			}
			if (bZAxis) {
				rover.setPosition(rover.getPosition().x, rover.getPosition().y, intersection.z);
			}
			
		}
		
	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
	if (bAltKeyDown == false) {
		if (bAxisOnRover) {
			bMoveRover = true;
		}
		else {
			doPointSelection();
		}
	}
}

//
//  Select Target Point on Terrain
//
void ofApp::doPointSelection() {
	if (bAxisOnRover) {
		return;
	}
	//get the mesh
	ofMesh mesh = mars.getMesh(0);
	//get the number of vertices
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	float nearestDistanceFromCam = -1;
	ofVec3f nearestVertex;
	int nearestIndex = 0;
	double minZ = 0;
	//get current camera position
	ofVec3f camPos = cam.getPosition();

	//get the mouse pointer
	ofVec2f mouse(mouseX, mouseY);

	//find the nearest point to the current mouse click
	for (int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = cur.distance(mouse);
		if (i == 0 || (distance < nearestDistance)) {
			//cout << " index ->" << i << " distanceToMouse ->" << distance << " point coords ->"<< cam.screenToWorld(cur);
			float camDistance = cur.distance(camPos);
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
	bPointSelected = true;
	//convert the nearestVertex to world coordinate and assign it to selected point
	ofVec3f tempNearestVertex;

	//Pick the nearestVertex that is close to camera
	for (int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		float disfromNearestVertex = nearestVertex.distance(cur);
		float distanceFromCamera = camPos.distance(cur);
		if (disfromNearestVertex < 5 && (nearestDistanceFromCam == 1 || nearestDistanceFromCam < distanceFromCamera)) {
			tempNearestVertex = cur;
		}
	}

	//convert nearest vertex back to world coordinates
	selectedPoint = cam.screenToWorld(tempNearestVertex);

}

// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {
	if (bAxisOnRover) {
		cam.setTarget(rover.getPosition());
	}
	else {
		cam.setTarget(selectedPoint);
	}
}

// Animate camera by moving it to the selected points (using update())
//
void ofApp::moveCamera(CamMoveDirection dir, float step) {

	ofVec3f currentCamPosition = cam.getPosition();
	ofVec3f normalizedPosition = cam.getZAxis().getNormalized();

	if (dir == 1) {
		//If down arrow pressed
		cam.setPosition(currentCamPosition.x + (normalizedPosition.x*step), currentCamPosition.y + (normalizedPosition.y*step), currentCamPosition.z + (normalizedPosition.z*step));
	}
	else {
		//if up arrow pressed
		cam.setPosition(currentCamPosition.x - (normalizedPosition.x*step), currentCamPosition.y - (normalizedPosition.y*step), currentCamPosition.z - (normalizedPosition.z*step));
	}

}

// Rotate Camera Left/Right
//
void ofApp::rotateCamera(CamMoveDirection dir, float step) {
	//if rotate is triggered
	if (dir == CamRotateLeft || dir == CamRotateRight) {
		cam.pan(-step);
	}
	else {
		//if control is pressed with up and down arrow
		cam.tilt(-step);
	}
}


//  "Teleport" Camera to Selected Target
void ofApp::teleportCamera() {
	ofVec3f currentCamPosition = cam.getPosition();

	//distance covered till now
	double percentCompelete = currentCamPosition.distance(teleportStartPoint) / teleportStartPoint.distance(selectedPoint);

	//calculating step using sin
	double step = sin(percentCompelete*PI);
	if (step == 0) {
		step = 0.001;
	}

	ofVec3f dir = (selectedPoint - currentCamPosition).normalize() * step;

	cam.setPosition(currentCamPosition + dir);

	if (percentCompelete > 0.95) {
		bTeleport = false;
	}

}

//  "Teleport" Camera to Selected Target
void ofApp::teleportRover() {
	ofVec3f currentRoverPosition = rover.getPosition();

	//distance covered till now
	double fractionCompelete = currentRoverPosition.distance(teleportStartPoint) / teleportStartPoint.distance(selectedPoint);

	//calculating step using sin
	double step = sin(fractionCompelete*PI);
	if (step == 0) {
		step = 0.001;
	}

	ofVec3f dir = (selectedPoint - currentRoverPosition).normalize() * step;
	ofVec3f movement = currentRoverPosition + dir;

	rover.setPosition(movement.x,movement.y,movement.z);

	//stop when percent compelete is more then 0.9
	if (fractionCompelete > 0.9) {
		bTeleportRover = false;
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bMoveRover = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	bRover = true;
	//load the model
	rover.loadModel(dragInfo.files[0]);
	//scale the model
	rover.setScale(0.0015,0.0015,0.0015);
	ofVec3f intersectionPoint;
	//use camera z axis as normal for the view plane
	ofVec3f planeNormal = cam.getZAxis().normalize();
	//check the cam position and accordingly find the sign to use while getting plane point
	int sign = -1;
	if ((mars.getPosition().z - cam.getPosition().z) > 0) {
		sign = 1;
	} 
	//check intersection using plane point as camPosition() - 1
	if (mouseIntersectPlane(cam.getPosition() + 2*sign, planeNormal, intersectionPoint)) {
		rover.setPosition(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
	}
}

//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{ 5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}


// Given an plane (point and normal), return intersection point with mouse ray.
// If there is intersection, return true, else return false;
//
bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}


// -------------------------------------------------------------- -
// test if a ray intersects a plane.  If there is an intersection, 
// return true and put point of intersection in "point"
//
bool ofApp::rayIntersectPlane(ofVec3f rayPoint, ofVec3f raydir, ofVec3f planePoint,
	ofVec3f planeNorm, ofVec3f &point) {

	// if d1 is 0, then the ray is on the plane or there is no intersection
	//
	const float eps = .000000001;
	float d1 = (planePoint - rayPoint).dot(planeNorm);
	if (abs(d1) < eps) return false;

	//  if d2 is 0, then the ray is parallel to the plane
	//
	float d2 = raydir.dot(planeNorm);
	if (abs(d2) < eps) return false;

	//  compute the intersection point and return it in "point"
	//
	point = (d1 / d2) * raydir + rayPoint;
	return true;
}
