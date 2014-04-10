#ifndef HUD_H
#define HUD_H

#include <osg/Camera>
#include <osgText/Text>
#include <osg/Geode>
#include <iostream>
#include <iomanip>

using namespace std;

class Hud{

private:
    osg::ref_ptr<osg::Camera> camera;
    osg::ref_ptr<osg::Geode> hudGeode;

    osgText::Text * controlsText;
	osgText::Text * position;
	osgText::Text * velocity;
	osgText::Text * acceleration;
	osgText::Text * forces;
	osgText::Text * orientation;
	osgText::Text * missleInfo;
	osgText::Text * crashed;
public:

    Hud();
    void run1();
    void run2();
    void setCrashedText(const std::string& hudTest);
	void updateText(float xPos, float yPos, float zPos, float xVel, float yVel, float zVel, float xThrust, float yThrust, float zLift);
	//void setSize(float size);

	std::string setPositionString(float xPos, float yPos, float zPos);
	std::string setVelocityString(float xVel, float yVel, float zVel);
	std::string setForcesString(float xThrust, float yThrust, float zLift);
	  
    osg::Camera * getHudCamera();
    osg::Geode * getHudGeode();
    void initializeHudText();

};

#endif
