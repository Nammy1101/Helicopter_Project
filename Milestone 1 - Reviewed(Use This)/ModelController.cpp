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
				gameRender->setRotorNutral();
				break;
			}
			break;
	}
	return false;

}