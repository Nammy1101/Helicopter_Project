/*
	ModelController
		Sets up key events for the helicopter to increase and decrease rotation.
		Uses modified lab code to call our functions

		Credits: Tylor Froese, Brian Schwietzer, Trevor Douglas (using lab code)

*/

#include "ModelController.h"
#include "ScriptRunner.h"
#include "Logger.h"

//Override
bool ModelController::handle( const osgGA::GUIEventAdapter& ea,
		                       osgGA::GUIActionAdapter& aa )
{
	ScriptRunner * sr = ScriptRunner::getInstance();	if ( !_model )
	{
			return false;  //Make sure model is initialized
	}
	switch ( ea.getEventType())
	{

		case osgGA::GUIEventAdapter::KEYDOWN:
			switch( ea.getKey() )
			{
			case '2':
				gameRender->increaseRotor();
				break;
    		case '1':
				gameRender->decreaseRotor();
				break;
				//centers the rotors angle theta and phi(heading and angle) to nut
			case 'c': case 'C':
				gameRender->centerJoyStick();
				break;
			case '3': 
				gameRender->setRotorNeutral();
				break;
			case '0':
				gameRender->setRotorZero();
				break;
			case 'p': case 'P':
				if(!sr->getStatus())
				{
					sr->runScripts();
					Constants::getInstance()->disableMouse = true;
				}
				break;
			case 'l': case 'L':
				Logger::getInstance()->toggleLogging();
			    break;
			case 'd': case 'D':
				gameRender->setRoll(-1);
				break;
			case 'a': case 'A':
				gameRender->setRoll(1);
				break;
			case 'w': case 'W':
				gameRender->setPitch(1);
				break;
			case 's': case 'S':
				gameRender->setPitch(-1);
				break;
			case osgGA::GUIEventAdapter::KEY_Right:
				gameRender->setYaw(-2.0f);
				break;
			case osgGA::GUIEventAdapter::KEY_Left:
				gameRender->setYaw(2.0f);
	    break;
			case 'f' : case 'F':
				gameRender->fireMissile();
				break;
			}
			break;
	}
	return false;

}