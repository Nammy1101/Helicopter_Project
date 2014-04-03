#ifndef HUD_H
#define HUD_H

#include <osg/Camera>
#include <osgText/Text>
#include <osg/Geode>
#include <iostream>

using namespace std;

class Hud{

private:
    osg::ref_ptr<osg::Camera> camera;
    osg::ref_ptr<osg::Geode> hudGeode;

    osgText::Text * text;
public:

    Hud();
    void run1();
    void run2();
    void setText(const std::string& hudTest);
	void setSize(float size);
	void setLogTextPos(const std::string& logText);
	void setLogTextVel(const std::string& logText);
	void setLogTextAcc(const std::string& logText);
	void setThrust(const std::string& thrust);
	void setOrientation(const std::string& orientation);

    void setPosition(osg::Vec3d position);
	  
    osg::Camera * getHudCamera();
    osg::Geode * getHudGeode();
    void initializeHudText();

};

#endif
