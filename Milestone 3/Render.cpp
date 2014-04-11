

/*
	Render
		Holds the functions used for setting up the world and doing the physics and math calculations required to make the helicopter move
			Notable Functions:
			Game_Play() is the intro function required to set everything up
			updateGamePlay() does the physics calulations for the movement of the helicopter including calculating the forces of gravity, throttle, and friction
			setThrottle(int) takes a throttle value between 0-23 and updates the helicopter's throttle accordingly
			setJoystick(float, float) takes a value for theta and phi and updates the helicopter's direcion accordingly
			detectCollision(osg::BoundingSphere&, osg::BoundingSphere&) takes two bounding spheres (one for the helicopter and another for the obstacle) and
				determined if they intersect

			There are other functions that do the math and other functions
			
		Credits: Brian Schweitzer, Karlee Stein, Tylor Froese, Trevor Douglas (used lab code)

*/

#include "ScriptRunner.h"
#include "Logger.h"
#include "ModelController.h"
#include "Render.h"
#include "TimerHandler.h"
#include "Trevor/Vector2.h"
#include "Constants.h"
#include <osg/ShapeDrawable>
#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osg/FrameStamp>

void Render::Game_Play(){
	hud.initializeHudText();
	osg::Camera * hudCamera;
	fired = false;
	ScriptRunner * sr = ScriptRunner::getInstance();
	sr->setRender(this);
	osg::ref_ptr<osg::Node> helicopter = osgDB::readNodeFile("Sikorsky2.osg");
	osg::ref_ptr<osg::Node> ground = osgDB::readNodeFile("lz.osg");
	osg::ref_ptr<osg::Node> missile = osgDB::readNodeFile("missile.3DS") ; 
	ball1  = new osg::ShapeDrawable;
	ball1->setShape( new osg::Sphere(osg::Vec3(0.0f, 0.0f,0.0f), 25.0f));
	ball1->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	osg::ref_ptr<osg::Geode> ball1Node = new osg::Geode;
	ball1Node->addDrawable(ball1.get());

	ball2  = new osg::ShapeDrawable;
	ball2->setShape( new osg::Sphere(osg::Vec3(0.0f, 0.0f,0.0f), 25.0f));
	ball2->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	osg::ref_ptr<osg::Geode> ball2Node = new osg::Geode;
	ball2Node->addDrawable(ball2.get());

	ball3  = new osg::ShapeDrawable;
	ball3->setShape( new osg::Sphere(osg::Vec3(0.0f, 0.0f,0.0f), 25.0f));
	ball3->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	osg::ref_ptr<osg::Geode> ball3Node = new osg::Geode;
	ball3Node->addDrawable(ball3.get());
	
	tor1Tr = new osg::PositionAttitudeTransform;
	tor2Tr = new osg::PositionAttitudeTransform;
	tor3Tr = new osg::PositionAttitudeTransform;

	tor1Tr->addChild(ball1Node.get());
	tor2Tr->addChild(ball2Node.get());
	tor3Tr->addChild(ball3Node.get());

	tor1Tr->setPosition(osg::Vec3(0.0f, -200.0f, 80.0f));
	tor2Tr->setPosition(osg::Vec3(10.0f, -300.0f, 70.0f));
	tor3Tr->setPosition(osg::Vec3(20.0f, -400.0f, 60.0f));

	osg::ref_ptr<osg::Group> torusGroup = new osg::Group();
	torusGroup->addChild(tor1Tr);
	torusGroup->addChild(tor2Tr);
	torusGroup->addChild(tor3Tr);
	

	helicopterTransform = new osg::PositionAttitudeTransform;
	helicopterTransform->addChild(helicopter.get());
	helicopterTransform->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	//helicopterTransform->setAttitude(osg::Quat((3.14/2), osg::Vec3d(1.0, 0.0, 0.0)));
    helicopterTransform->setAttitude(
        osg::Quat(
            osg::DegreesToRadians(90.0f),osg::Vec3f(1,0,0),
            osg::DegreesToRadians(0.0f),osg::Vec3f(0,0,1),
            osg::DegreesToRadians(0.0f),osg::Vec3f(0,1,0)
            )
        );

	osg::ref_ptr<ModelController> ctrler = new ModelController(helicopterTransform.get(),this);

	groundTransform = new osg::PositionAttitudeTransform;
	groundTransform->addChild(ground.get());
	groundTransform->setPosition(osg::Vec3(0.0f, 0.0f, -100.0f));
	groundTransform->setScale(osg::Vec3(30.0f, 30.0f, 1.0f));
	
	modelPosition.set(helicopterTransform->getPosition());
	modelVelocity.set(osg::Vec3f(0,0,0));

	helicopterThrust = osg::Vec3f(0.0, 0.0, 0.0);

	hudCamera = hud.getHudCamera();
	missileTransform = new osg::PositionAttitudeTransform;
	missileTransform->addChild(missile.get());
	missileTransform->setPosition(modelPosition);
	missileTransform->setScale(osg::Vec3(0.002f, 0.002f, 0.002f));
	missilePositon.set(helicopterTransform->getPosition());
	missileVelocity.set(osg::Vec3f(0.0,0.0,0.0));

	osg::ref_ptr<osg::Group> rootNode = new osg::Group;  //Create a group node
	rootNode->addChild( groundTransform.get());
	rootNode->addChild( helicopterTransform.get());
	rootNode->addChild( torusGroup.get());
	rootNode->addChild(hudCamera);
	rootNode->addChild(missileTransform.get());

	viewer.addEventHandler( ctrler.get());

	viewer.addEventHandler(new TimerHandler(this));	

	viewer.setUpViewInWindow(300,300,800,600);

	viewer.setSceneData(rootNode.get());

	//This bit of code will have the camera follow the model
    osg::ref_ptr<osgGA::NodeTrackerManipulator> nodeTracker = new osgGA::NodeTrackerManipulator;
    nodeTracker->setHomePosition( osg::Vec3(0, 100.0, 0),
                                    osg::Vec3(), osg::Z_AXIS );
    //This will track the center of the helicopter and rotate as well.
    nodeTracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
    nodeTracker->setRotationMode( osgGA::NodeTrackerManipulator::TRACKBALL );
    nodeTracker->setTrackNode( helicopter );
    viewer.setCameraManipulator( nodeTracker );


	last = 0;

	viewer.run();
}

void Render::setThrottle(int position)
{
	float baseThrottle = Constants::getInstance()->baseThrottle;
	float maxThrottle = Constants::getInstance()->maxThrottle;

	if (position > 0)
	{
		if (position < maxThrottle)
			rotorForce = baseThrottle*position;
		else
			rotorForce = baseThrottle*maxThrottle;
	}
	else
		rotorForce = 0;
}

void Render::increaseRotor()
{
	float baseThrottle = Constants::getInstance()->baseThrottle;
	float maxThrottle = Constants::getInstance()->maxThrottle;

	if (rotorForce < baseThrottle*maxThrottle){
		rotorForce += baseThrottle; // 0.2;
		std::cout << rotorForce << " ";
	}
	else{
		rotorForce = baseThrottle*maxThrottle;
	}
}

void Render::decreaseRotor()
{
	if(rotorForce > 0)
	{
		rotorForce -= Constants::getInstance()->baseThrottle; //0.2;
		std::cout << rotorForce;
	}
	else
	{
		rotorForce = 0;
	}
}
//sets the rotor to a nutal state
void Render::setRotorNeutral(){
	helicopterThrust = osg::Vec3f(helicopterThrust.x(),helicopterThrust.y(),Constants::getInstance()->helicopter->mass*Constants::getInstance()->gravity);
}
void Render::setRotorZero(){
	rotorForce = 0;
}
void Render::setJoystick(float theta, float phi)
{
	if (theta < 0)
		theta = 0;
	else if (theta > 15)
		theta = 15;
	
	if (phi > 360 || phi < -360)
		phi = fmod(phi, 360);
	if (phi < 0)
		phi += 360;
	
	helicopterThrust = osg::Vec3f(-(rotorForce*sin(theta)*cos(phi)), -(rotorForce*sin(theta)*sin(phi)),(rotorForce*cos(theta)));
}
//Adding centering function for the rotor
void Render::centerJoyStick(){
	float theta = 0, phi = 0 ;
	helicopterThrust = osg::Vec3f(-(rotorForce*sin(theta)*cos(phi)), -(rotorForce*sin(theta)*sin(phi)),(rotorForce*cos(theta)));
}

osg::Vec3f Render::calculateForceDirections(float force, osg::Vec2f direction){
	Vector2 vector = Vector2::Vector2(direction.x(), direction.y());
	float viewHeight = viewer.getCamera()->getViewport()->height();
	float viewWidth = viewer.getCamera()->getViewport()->width();
	
	float relationship = (viewHeight<viewWidth)?15/(viewHeight/4):15/(viewWidth/4);
	
	float theta = osg::DegreesToRadians(vector.Length()*relationship);
	float phi = atan2(direction.y(), direction.x());

	//Constants::getInstance()->setFrictionConstant(theta);
	//std::cout << Constants::getInstance()->frictionConstant << std::endl;
	Logger::getInstance()->log("Theta: " + f2s(theta) + " Phi: " + f2s(phi));
	return osg::Vec3f(-(force*sin(theta)*cos(phi)), -(force*sin(theta)*sin(phi)),(force*cos(theta)));
}

void Render::updateDirection(float x, float y){
	float viewHeight = viewer.getCamera()->getViewport()->height();
	float viewWidth = viewer.getCamera()->getViewport()->width();
 
 y = y - (viewHeight/2);
 x = x - (viewWidth/2);
	if(viewHeight < viewWidth)
	{
		y = (y>viewHeight/4)?(viewHeight/4):y;
		y = (y<(-viewHeight/4))?-(viewHeight/4):y;
		x = (x>viewHeight/4)?(viewHeight/4):x;
		x = (x<(-viewHeight/4))?-(viewHeight/4):x;
	} else {
		y = (y>viewWidth/4)?(viewWidth/4):y;
		y = (y<(-viewWidth/4))?-(viewWidth/4):y;
		x = (x>viewWidth/4)?(viewWidth/4):x;
		x = (x<(-viewWidth/4))?-(viewWidth/4):x;
	}
	
  osg::Vec2f xy(x, y);

 //std::cout << "0Centered-X:  " << x << std::endl;
	//std::cout << "0Centered-Y:  " << y << std::endl;

 helicopterThrust = calculateForceDirections(rotorForce, xy);
}

void Render::setGravity(float gravity)
{
	aGrav = gravity;
}

bool Render::detectCollision(osg::BoundingSphere& bs1, osg::BoundingSphere& bs2)
{
	if (bs1.intersects(bs2))
		return true;
	else
		return false;
}

void Render::updateGamePlay()
{
	if (Render::detectCollision(osg::BoundingSphere(helicopterTransform->getBound()), osg::BoundingSphere(tor1Tr->getBound())) || Render::detectCollision(osg::BoundingSphere(missileTransform->getBound()), osg::BoundingSphere(tor1Tr->getBound())))
	{
		ball1->setColor(osg::Vec4(1.0f,0.0f,0.0f,0.0f));
		Logger::getInstance()->log("Collision with ball #1");
	}
	if (Render::detectCollision(osg::BoundingSphere(helicopterTransform->getBound()), osg::BoundingSphere(tor2Tr->getBound())) || Render::detectCollision(osg::BoundingSphere(missileTransform->getBound()), osg::BoundingSphere(tor2Tr->getBound())))
	{
		ball2->setColor(osg::Vec4(1.0f,0.0f,0.0f,0.0f));
		Logger::getInstance()->log("Collision with ball #2");
	}
	if (Render::detectCollision(osg::BoundingSphere(helicopterTransform->getBound()), osg::BoundingSphere(tor3Tr->getBound())) || Render::detectCollision(osg::BoundingSphere(missileTransform->getBound()), osg::BoundingSphere(tor3Tr->getBound())))
	{
		ball3->setColor(osg::Vec4(1.0f,0.0f,0.0f,0.0f));
		Logger::getInstance()->log("Collision with ball #3");
	}
	//Missile values
	
	float misfrictionScalar = Constants::getInstance()->frictionConstant*sqrt(pow(missileVelocity.x(),2)+pow(missileVelocity.y(),2)+pow(missileVelocity.x(),2));
	float mismass = Constants::getInstance()->missile->mass;
	
	float misaxForce = missileThrust.x() - misfrictionScalar*missileVelocity.x();
	float misayForce = missileThrust.y() - misfrictionScalar*missileVelocity.y();
	float misazForce = aGrav*mismass  + missileThrust.z() - misfrictionScalar*missileVelocity.z();


	float misxAcc = misaxForce/mismass;
	float misyAcc = misayForce/mismass;
	float miszAcc = misazForce/mismass;


	//float misdelta = viewer.getFrameStamp()->getReferenceTime() - last;
	


	float frictionScalar = Constants::getInstance()->frictionConstant*sqrt(pow(modelVelocity.x(),2)+pow(modelVelocity.y(),2)+pow(modelVelocity.z(),2));
	float mass = Constants::getInstance()->helicopter->mass;

	

	float axForce = helicopterThrust.x() - frictionScalar*modelVelocity.x();
	float ayForce = helicopterThrust.y() - frictionScalar*modelVelocity.y();
	float azForce = aGrav*mass + helicopterThrust.z() - frictionScalar*modelVelocity.z();

	float xAcc = axForce/mass;
	float yAcc = ayForce/mass;
	float zAcc = azForce/mass;

	float delta = viewer.getFrameStamp()->getReferenceTime() - last;
	last = viewer.getFrameStamp()->getReferenceTime();

	
	//helicopter
	float xPos = modelPosition.x() + (modelVelocity.x()*delta) + (0.5)*xAcc*(pow(delta,2));
	float xVel = (modelVelocity.x() + xAcc*delta)*0.99;
	//missile
	float misxPos;
	float misxVel;
	
	
	//helicopter
	float yPos = modelPosition.y() + (modelVelocity.y()*delta) + (0.5)*yAcc*(pow(delta,2));
	float yVel = (modelVelocity.y() + yAcc*delta)*0.99;
	//missile
	float misyPos;
	float misyVel;
	
	
	//helicopter
	float zPos = modelPosition.z() + (modelVelocity.z()*delta) + (0.5)*zAcc*(pow(delta,2));
	float zVel = (modelVelocity.z() + zAcc*delta)*0.99;
	//missile
	float miszPos;
	float miszVel;
	
	//cout << fired << " ";
	if(!fired){
		misxPos = modelPosition.x();
		misyPos = modelPosition.y();
		miszPos = modelPosition.z();
		misxVel =-1* modelVelocity.x();
		misyVel =-1* modelVelocity.y();
		miszVel = modelVelocity.z();
		
		missilePositon.set(osg::Vec3d(misxPos, misyPos, miszPos));
		missileVelocity.set(osg::Vec3f(misxVel, misyVel, miszVel));
		missileTransform->setAttitude(
		osg::Quat(
			osg::DegreesToRadians(helicopterOrientation.x_theta ),osg::Vec3f(1,0,0),
			osg::DegreesToRadians(helicopterOrientation.y_theta),osg::Vec3f(0,1,0),
			osg::DegreesToRadians(helicopterOrientation.z_theta ),osg::Vec3f(0,0,1)
			)
		);
		cout << "here " <<endl;
	}else{

		
		misxPos = missilePositon.x() + (missileVelocity.x()*delta);
		misxVel = (missileVelocity.x())*0.99;
		misyPos = missilePositon.y() + (missileVelocity.y()*delta) ;
		misyVel = (missileVelocity.y())*0.99;
		miszPos =  (missilePositon.z() + (missileVelocity.z()*delta)) ;
		miszVel = missileVelocity.z();
		/*if(miszPos < 1){
			miszPos = 1;
			miszVel = 0;
			misxVel = 0;
			misyVel = 0;
		}*/

		missilePositon.set(osg::Vec3d(misxPos, misyPos, miszPos));
		missileVelocity.set(osg::Vec3f(misxVel, misyVel, miszVel));
		/*missileTransform->setAttitude(
		osg::Quat(
			osg::DegreesToRadians(missileOrientation.x_theta ),osg::Vec3f(1,0,0),
			osg::DegreesToRadians(missileOrientation.y_theta),osg::Vec3f(0,1,0),
			osg::DegreesToRadians(missileOrientation.z_theta+180 ),osg::Vec3f(0,0,1)
			)
		);*/
	}
	
	float liftZ = zAcc/Constants::getInstance()->gravity;

	if(zVel < -10 && zPos < 4){
		//cout << "You have crashed" << endl;
		hud.setCrashedText("YOU HAVE CRASHED!");
		zPos = 0;
	}

	if(zPos < 1){  //these ones should be radius of ball
		zPos = 1;
		zVel *= 0.8;
		zVel = -zVel;
	}
	
	if(zPos > 100000){
		zPos = 1;
		zVel *= 0.8;
		zVel = -zVel;
	}
	Logger* logger = Logger::getInstance();
	string something = f2s(xPos);
	logger->log("X Pos: " + f2s(xPos) + " Y Pos: " + f2s(yPos) +" Z Pos: " + f2s(zPos)); 
	logger->log("X Vel: " + f2s(xVel) + " Y Vel: " + f2s(yVel) +" Z Vel: " + f2s(zVel));
	logger->log("X Acc: " + f2s(xAcc) + " Y Acc: " + f2s(yAcc) +" Z Acc: " + f2s(zAcc));
	logger->log("Throttle Position: " + f2s(rotorForce/Constants::getInstance()->baseThrottle));
	
	//missile
	missileTransform->setPosition(missilePositon);
	
	//missileTransform->setPosition(missilePositon);

	//helicopter
	//cout << "x "<<modelVelocity.x() << " y " << modelVelocity.y() << " z " << modelVelocity.z()<< " "<<endl;
	//cout << "m x "<<missilePositon.x() << "m y " << missilePositon.y() << "m z " << missilePositon.z()<< " "<<endl;
	modelPosition.set(osg::Vec3d(xPos, yPos, zPos));
	modelVelocity.set(osg::Vec3f(xVel, yVel, zVel));
	
	helicopterTransform->setPosition(modelPosition);
	helicopterTransform->setAttitude(
		osg::Quat(
			osg::DegreesToRadians(90 + helicopterOrientation.x_theta),osg::Vec3f(1,0,0),
			osg::DegreesToRadians(helicopterOrientation.y_theta),osg::Vec3f(0,1,0),
			osg::DegreesToRadians(helicopterOrientation.z_theta),osg::Vec3f(0,0,1)
			)
		);
	
	if(ScriptRunner::getInstance()->getStatus()){ ScriptRunner::getInstance()->doCommand();}

	hud.updateText(xPos,yPos,zPos,xVel,yVel,zVel,helicopterThrust.x(),helicopterThrust.y(), liftZ, helicopterOrientation.x_theta, helicopterOrientation.y_theta, helicopterOrientation.z_theta);
}

std::string Render::f2s(float num){
	stringstream ss (stringstream::in | stringstream::out);

	ss << num;

  string str = ss.str();
  return str;
}

void Render::setYaw(float angle){
	helicopterOrientation.z_theta += angle;
}
void Render::setPitch(float angle){
	helicopterOrientation.x_theta += angle;
}
void Render::setRoll(float angle){
	helicopterOrientation.y_theta += angle;
}
void Render::fireMissile(){
	//osg::Vec3f velocity;
	if(!fired){
	float velx = (modelVelocity.x() + Constants::getInstance()->missile->Airspeed*sin(helicopterOrientation.z_theta)*cos(helicopterOrientation.y_theta));
	float vely =  (modelVelocity.y() + Constants::getInstance()->missile->Airspeed*cos(helicopterOrientation.z_theta)*sin(helicopterOrientation.y_theta));
	float velz = modelVelocity.z();
	cout << "x "<<velx << " y " << vely << " z " << velz << " "<<endl;
	cout << "x "<<modelVelocity.x() << " y " << modelVelocity.y() << " z " << modelVelocity.z()<< " "<<endl;
	//velocity.set();
	fired = true;
	
	//float velz = velocity*cos(helicopterOrientation.z_theta);
	missileVelocity.set(osg::Vec3f(velx,vely,velz));
	missileTransform->setAttitude(
		osg::Quat(
			osg::DegreesToRadians(helicopterOrientation.x_theta ),osg::Vec3f(1,0,0),
			osg::DegreesToRadians(helicopterOrientation.y_theta),osg::Vec3f(0,1,0),
			osg::DegreesToRadians(helicopterOrientation.z_theta ),osg::Vec3f(0,0,1)
			)
		);
	}
	//if(missileCount > 0){
	//}
}
