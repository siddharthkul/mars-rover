#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"


typedef enum { CamMoveForward, CamMoveBackward, CamRotateLeft, CamRotateRight, CamTiltUp, CamTiltDown } CamMoveDirection;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(int key);
		void initLightingAndMaterials();
		void savePicture();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f & point);
		bool rayIntersectPlane(ofVec3f rayPoint, ofVec3f raydir, ofVec3f planePoint, ofVec3f planeNorm, ofVec3f & point);
		void toggleWireframeMode();
		void togglePointsDisplay();
		void setCameraTarget();
		void doPointSelection();
		void moveCamera(CamMoveDirection dir, float step);
		void rotateCamera(CamMoveDirection dir, float step);
		void teleportCamera();

		void teleportRover();

		ofEasyCam cam;
		ofxAssimpModelLoader mars;
		ofxAssimpModelLoader rover;
		ofLight light;


		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bMoveCameraForward;
		bool bMoveCameraBackward;
		bool bRotateCameraLeft;
		bool bRotateCameraRight;
		bool bTeleport;
		bool bTeleportRover;
		bool bRover;
		bool bXAxis;
		bool bYAxis;
		bool bZAxis;
		bool bNoAxis;
		bool bAxisOnRover;
		bool bMoveRover;
		ofVec3f selectedPoint;
		ofVec3f clickedPoint;
		ofVec3f teleportStartPoint;
		ofVec3f startPosition;

		const float selectionRange = 4.0;
};
