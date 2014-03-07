/*
	Render
		Holds the functions used for setting up the world and doing the physics and math calculations required to make the helicopter move
			Notable Functions:
			Game_Play() is the intro function required to set everything up
			updateGamePlay() does the physics calulations for the movement of the helicopter

			There are other functions that do the math and other functions
			
		Credits: Brian Schweitzer, Karlee Stein, Tylor Froese, Trevor Douglas (used lab code)

*/



#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>

#include "ModelController.h"
#include "Render.h"
#include "TimerHandler.h"
#include <osg/ShapeDrawable>
#include <iostream>
#include "Trevor/Vector2.h"
#include <osgGA/KeySwitchMatrixManipulator>

void Render::Game_Play(){
	osg::ref_ptr<osg::Node> helicopter = osgDB::readNodeFile("Sikorsky2.osg");
	osg::ref_ptr<osg::Node> ground = osgDB::readNodeFile("lz.osg");
	osg::ref_ptr<osg::Node> dumpTruck = osgDB::readNodeFile("dumptruck.osg");
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Node> spaceShip = osgDB::readNodeFile("spaceShip.osgt");

	osg::ref_ptr<osg::Node> torus1 = osgDB::readNodeFile("cube_mapped_torus.osgt");
	osg::ref_ptr<osg::Node> torus2 = osgDB::readNodeFile("cube_mapped_torus.osgt");
	osg::ref_ptr<osg::Node> torus3 = osgDB::readNodeFile("cube_mapped_torus.osgt");

	/*
	osg::ref_ptr<osg::PositionAttitudeTransform> tor1Tr = new osg::PositionAttitudeTransform;
	osg::ref_ptr<osg::PositionAttitudeTransform> tor2Tr = new osg::PositionAttitudeTransform;
	osg::ref_ptr<osg::PositionAttitudeTransform> tor3Tr = new osg::PositionAttitudeTransform;

	tor1Tr->addChild(torus1.get());
	tor2Tr->addChild(torus2.get());
	tor3Tr->addChild(torus3.get());

	tor1Tr->setPosition(osg::Vec3(0.0f, -200.0f, 80.0f));
	tor2Tr->setPosition(osg::Vec3(10.0f, -300.0f, 70.0f));
	tor3Tr->setPosition(osg::Vec3(20.0f, -400.0f, 60.0f));

	osg::ref_ptr<osg::Group> torusGroup = new osg::Group();
	torusGroup->addChild(tor1Tr);
	torusGroup->addChild(tor2Tr);
	torusGroup->addChild(tor3Tr);

	*/

	

	helicopterTransform = new osg::PositionAttitudeTransform;
	helicopterTransform->addChild(helicopter.get());
	helicopterTransform->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
	helicopterTransform->setAttitude(osg::Quat((3.14/2), osg::Vec3d(1.0, 0.0, 0.0)));
	osg::ref_ptr<ModelController> ctrler = new ModelController(helicopterTransform.get(),this);

	groundTransform = new osg::PositionAttitudeTransform;
	groundTransform->addChild(ground.get());
	groundTransform->setPosition(osg::Vec3(0.0f, 0.0f, -100.0f));
	groundTransform->setScale(osg::Vec3(100.0f, 100.0f, 1.0f));

	osg::ref_ptr<osg::PositionAttitudeTransform> dumpTruckTransform = new osg::PositionAttitudeTransform;
	dumpTruckTransform->addChild(dumpTruck.get());
	dumpTruckTransform->setPosition(osg::Vec3(600.0f,-4000.0f,500));
	dumpTruckTransform->setScale(osg::Vec3(50.0f,50.0f,50.0f));

	osg::ref_ptr<osg::PositionAttitudeTransform> cowTransform = new osg::PositionAttitudeTransform;
	cowTransform->addChild(cow.get());
	cowTransform->setPosition(osg::Vec3(0.0f, -2000.0f, 700.0f));
	cowTransform->setScale(osg::Vec3(60.0f, 60.0f, 60.0f));

	osg::ref_ptr<osg::PositionAttitudeTransform> spaceShipTransform = new osg::PositionAttitudeTransform;
	spaceShipTransform->addChild(spaceShip.get());
	spaceShipTransform->setPosition(osg::Vec3(700.0f,-7000.0f, 1000.0f));
	spaceShipTransform->setScale(osg::Vec3(70.0f,70.0f,70.0f));

	modelPosition.set(helicopterTransform->getPosition());
	modelVelocity.set(osg::Vec3f(0,0,0));

	helicopterThrust = osg::Vec3f(0.0, 0.0, 0.0);


	osg::ref_ptr<osg::Group> rootNode = new osg::Group;  //Create a group node
	rootNode->addChild( groundTransform.get());
	rootNode->addChild( helicopterTransform.get());
	//rootNode->addChild( torusGroup.get());
	rootNode->addChild( spaceShipTransform.get());
	rootNode->addChild( dumpTruckTransform.get());
	rootNode->addChild( cowTransform.get());

	viewer.addEventHandler( ctrler.get());

	viewer.addEventHandler(new TimerHandler(this));
	viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3(0.0f, 75.0f, 10.0f),osg::Vec3(0.0f, 0.0f, 10.0f), osg::Z_AXIS);
	viewer.getCamera()->setAllowEventFocus( false );


	// Found at: http://www2.denizyuret.com/bib/wang/Wang2010osg/OpenSceneGraph.3.0.Beginners.Guide.Dec.2010.pdf

	osg::ref_ptr<osg::GraphicsContext::Traits> traits =
    new osg::GraphicsContext::Traits;
	traits->x = 50;
	traits->y = 50;
	traits->width = 800;
	traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->samples = 4;

	osg::ref_ptr<osg::GraphicsContext> gc =
		osg::GraphicsContext::createGraphicsContext( traits.get() );

	viewer.getCamera()->setGraphicsContext( gc );
	viewer.getCamera()->setViewport(
    new osg::Viewport(0, 0, traits->width, traits->height) );

	viewer.setSceneData( rootNode.get());
	last = 0;
	viewer.run();
}
void Render::increaseRotor()
{
	rotorForce += 0.2;
}

void Render::decreaseRotor()
{
	if(rotorForce > 0)
	{
		rotorForce-= 0.2;
		
	}
	else
	{
		rotorForce = 0;
	}
}

osg::Vec3f Render::calculateForceDirections(float force, osg::Vec2f direction){
	Vector2 vector = Vector2::Vector2(direction.x(), direction.y());
	float viewHeight = viewer.getCamera()->getViewport()->height();
	float viewWidth = viewer.getCamera()->getViewport()->width();
	
	float relationship = (viewHeight<viewWidth)?15/(viewHeight/4):15/(viewWidth/4);

	std::cout << "Relationship: " << relationship << std::endl;
	
	float theta = osg::DegreesToRadians(vector.Length()*relationship);
	float phi = atan2(direction.y(), direction.x());

	std::cout << "Theta: " << theta << std::endl;
	std::cout << "Phi: " << phi << std::endl;

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

 std::cout << "0Centered-X:  " << x << std::endl;
	std::cout << "0Centered-Y:  " << y << std::endl;

 helicopterThrust = calculateForceDirections(rotorForce, xy);
}

void Render::updateGamePlay()
{

	float axForce = helicopterThrust.x();
	float ayForce = helicopterThrust.y();
	float azForce = aGrav + helicopterThrust.z();

	float delta = viewer.getFrameStamp()->getReferenceTime() - last;
	last = viewer.getFrameStamp()->getReferenceTime();

	float xPos = modelPosition.x() + (modelVelocity.x()*delta) + (0.5)*axForce*(pow(delta,2));
	float xVel = (modelVelocity.x() + axForce*delta)*0.99999999999;

	float yPos = modelPosition.y() + (modelVelocity.y()*delta) + (0.5)*ayForce*(pow(delta,2));
	float yVel = (modelVelocity.y() + ayForce*delta)*0.99999999999;

	float zPos = modelPosition.z() + (modelVelocity.z()*delta) + (0.5)*azForce*(pow(delta,2));
	float zVel = (modelVelocity.z() + azForce*delta)*0.99999999999;

	if(zPos < 1){  //these ones should be radius of ball
		zPos = 1;
		zVel *= 0.8;
		zVel = -zVel;
	}

	std::cout << "ZPos: " << zPos << std::endl;
	std::cout << "Z Force: " << helicopterThrust.z() << std::endl;
	std::cout << "X Force: " << helicopterThrust.x() << std::endl;
	std::cout << "Y Force: " << helicopterThrust.y() << std::endl;
	
	modelPosition.set(osg::Vec3d(xPos, yPos, zPos));
	modelVelocity.set(osg::Vec3f(xVel, yVel, zVel));
	viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3(xPos, yPos+75.0f, zPos),osg::Vec3(xPos, yPos, zPos), osg::Z_AXIS);
	helicopterTransform->setPosition(modelPosition); 
}

void Render::startMoving()
{
    viewer.addEventHandler(new TimerHandler(this));
}