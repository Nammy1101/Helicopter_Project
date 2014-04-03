
#include "Hud.h"

#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

Hud::Hud()
{
    camera = new osg::Camera;
    hudGeode = new osg::Geode;
    camera->setClearMask( GL_DEPTH_BUFFER_BIT);
    camera->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setRenderOrder( osg::Camera::POST_RENDER );
    camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
}

void Hud::initializeHudText()
{
    //initialize our text pointer;
    text = new osgText::Text;
    text->setFont(osgText::readFontFile("fonts/vera.ttf"));
    text->setColor(osg::Vec4(0,0,0,2.0f));
    text->setCharacterSize(18.0f);
    text->setLayout( osgText::Text::LEFT_TO_RIGHT );
    text->setText("");
    text->setPosition(osg::Vec3(0,950,0));

    hudGeode->addDrawable(text);

    camera->addChild(hudGeode);

}

osg::Camera * Hud::getHudCamera()
{
    return camera;
}

osg::Geode * Hud::getHudGeode()
{
    return hudGeode;
}


void Hud::setText(const std::string& hudText)
{
    text->setText(hudText);
}

void Hud::setPosition(osg::Vec3d position)
{
    text->setPosition(position);
}

void Hud::setSize(float size){
	text->setCharacterSize(size);
}

void Hud::setLogTextPos(const std::string& logText){
    text->setText(logText);
	text->setPosition(osg::Vec3(0,200,0));
}
void Hud::setLogTextVel(const std::string& logText){
    text->setText(logText);
	text->setPosition(osg::Vec3(0,180,0));
}
void Hud::setLogTextAcc(const std::string& logText){
    text->setText(logText);
	text->setPosition(osg::Vec3(0,160,0));
}

void Hud::setThrust(const std::string& thrust){
	text->setText(thrust);
	text->setPosition(osg::Vec3(0,140,0));
}

void Hud::setOrientation(const std::string& orientation){
	text->setText(orientation);
	text->setPosition(osg::Vec3(0,120,0));
}

