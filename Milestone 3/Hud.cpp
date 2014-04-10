
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
	controlsText = new osgText::Text;
	controlsText->setFont(osgText::readFontFile("fonts/vera.ttf"));
	controlsText->setColor(osg::Vec4(0,0,0,2.0f));
	controlsText->setCharacterSize(20.0f);
	controlsText->setLayout( osgText::Text::LEFT_TO_RIGHT );
	controlsText->setText("Controls: \nPitch: forward: W Backwards: S \nRoll: Left: A Right: D\nYaw: Left: left arrow right: right arrow\nRotorThrust: increase: 2 decrease: 1\nHover: 3\nNo power: 0\nCenter Joystick: C\nMove: Point Mouse");
	controlsText->setPosition(osg::Vec3(900,980,0));

	hudGeode->addDrawable(controlsText);

	position = new osgText::Text;
	position->setFont(osgText::readFontFile("fonts/vera.ttf"));
	position->setColor(osg::Vec4(0,0,0,2.0f));
	position->setCharacterSize(22.0f);
	position->setLayout( osgText::Text::LEFT_TO_RIGHT );
	position->setText(setPositionString(0,0,0));
	position->setPosition(osg::Vec3(0,200,0));
	hudGeode->addDrawable(position);

	velocity = new osgText::Text;
	velocity->setFont(osgText::readFontFile("fonts/vera.ttf"));
	velocity->setColor(osg::Vec4(0,0,0,2.0f));
	velocity->setCharacterSize(22.0f);
	velocity->setLayout( osgText::Text::LEFT_TO_RIGHT );
	velocity->setText(setVelocityString(0,0,0));
	velocity->setPosition(osg::Vec3(0,180,0));
	hudGeode->addDrawable(velocity);

	crashed = new osgText::Text;
	crashed->setFont(osgText::readFontFile("fonts/vera.ttf"));
	crashed->setColor(osg::Vec4(0,0,0,2.0f));
	crashed->setCharacterSize(22.0f);
	crashed->setLayout( osgText::Text::LEFT_TO_RIGHT );
	crashed->setText("");
	crashed->setPosition(osg::Vec3(0,220,0));
	hudGeode->addDrawable(crashed);

	forces = new osgText::Text;
	forces->setFont(osgText::readFontFile("fonts/vera.ttf"));
	forces->setColor(osg::Vec4(0,0,0,2.0f));
	forces->setCharacterSize(22.0f);
	forces->setLayout( osgText::Text::LEFT_TO_RIGHT );
	forces->setText(setForcesString(0,0,0));
	forces->setPosition(osg::Vec3(0,160,0));
	hudGeode->addDrawable(forces);

	orientation = new osgText::Text;
	orientation->setFont(osgText::readFontFile("fonts/vera.ttf"));
	orientation->setColor(osg::Vec4(0,0,0,2.0f));
	orientation->setCharacterSize(22.0f);
	orientation->setLayout( osgText::Text::LEFT_TO_RIGHT );
	orientation->setText(setOrientationString(0,0,0));
	orientation->setPosition(osg::Vec3(0,140,0));
	hudGeode->addDrawable(orientation);
	/*
	controlsText = new osgText::Text;
	controlsText->setFont(osgText::readFontFile("fonts/vera.ttf"));
	controlsText->setColor(osg::Vec4(0,0,0,2.0f));
	controlsText->setCharacterSize(18.0f);
	controlsText->setLayout( osgText::Text::LEFT_TO_RIGHT );
	controlsText->setText("Controls: \nPitch: forward: W Backwards: S \nRoll: Left: A Right: D\nYaw: Left: left arrow right: right arrow\nRotorThrust: increase: 2 decrease: 1\nHover: 3\nNo power: 0\nCenter Joystick: C\nMove: Point Mouse");
	controlsText->setPosition(osg::Vec3(0,950,0));
	hudGeode->addDrawable(controlsText);
	*/


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

std::string Hud::setPositionString(float xPos, float yPos, float zPos){
	stringstream ss (stringstream::in | stringstream::out);

	ss << "Position x: " << setw(9) << setprecision(0) << fixed << xPos
		<< setw(9) << "y: " << setw(9) << yPos << setw(9) << "z: " << setw(9)
		<< zPos;

	string str = ss.str();
	return str;
}

std::string Hud::setVelocityString(float xVel, float yVel, float zVel){
	stringstream ss (stringstream::in | stringstream::out);

	ss << "Velocity x: " << setw(9) << setprecision(0) << fixed << xVel
		<< setw(9) << "y: " << setw(9) << yVel << setw(9) << "z: " << setw(9)
		<< zVel;

	string str = ss.str();
	return str;
}

std::string Hud::setForcesString(float xThrust, float yThrust, float zLift){
	stringstream ss (stringstream::in | stringstream::out);

	ss << "Thrust x: " << setw(10) << setprecision(1) << fixed << xThrust
		<< setw(10) << "y: " << setw(9) << yThrust << setw(15) << "Lift z: " << setw(9)
		<< zLift;

	string str = ss.str();
	return str;
}

std::string Hud::setOrientationString(float xO, float yO, float zO){
		stringstream ss (stringstream::in | stringstream::out);

	ss << "Orientation x: " << setw(10) << setprecision(1) << fixed << xO + 90
		<< setw(10) << "y: " << setw(9) << yO << setw(15) << "z: " << setw(9)
		<< zO;

	string str = ss.str();
	return str;
}

void Hud::updateText(float xPos, float yPos, float zPos,float xVel, float yVel, float zVel, float xThrust, float yThrust, float zLift, float xO, float yO, float zO){
	position->setText(setPositionString(xPos,yPos,zPos));
	velocity->setText(setVelocityString(xVel,yVel,zVel));
	forces->setText(setForcesString(xThrust,yThrust,zLift));
	orientation->setText(setOrientationString(xO,yO,zO));
}

void Hud::setCrashedText(const std::string& hudText)
{
crashed->setText(hudText);
}
/*
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
*/
