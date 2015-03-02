/*
 -----------------------------------------------------------------------------
 Filename:    SimulationManager.h
 -----------------------------------------------------------------------------

 based on the source file which is part of the
 ___                 __    __ _ _    _
 /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
 / \_// (_| | | |  __/  \  /\  /| |   <| |
 \___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
 |___/
 Tutorial Framework
 http://www.ogre3d.org/tikiwiki/
 -----------------------------------------------------------------------------
 */
#ifndef __SimulationManager_h_
#define __SimulationManager_h_

//# corresponding header
#include "SimulationManager.h"

//# forward declarations
class Environment;
class Logger;

//# system headers
//## controller headers

//## model headers
#include <EALib/PopulationT.h>

//## view headers
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <SDL_syswm.h>

//# custom headers
//## base headers
#include "BaseApplication.h"

//## configuration headers
#include "configuration/EvolutionConfiguration.h"

//## controller headers
#include "controller/StateHandler.h"
#include "controller/camera/CameraHandler.h"
#include "controller/input/SDL2InputHandler.h"

//## model headers
#include "model/evolution/juries/Ones.h"

//## view headers
#include "view/MathGL/MathGLWindow.h"
#include "view/CEGUI/GUISheetHandler.h"
#include "view/CEGUI/ParamsPanel.h"

//## utils headers
#include <utils/logging/Logger.h>

class SimulationManager: public BaseApplication {
private:

	// StateHandler
	StateHandler mStateHandler;

	Environment* mTerrain;

	// Game component handlers
	SDL2InputHandler mInputHandler;
	SDL_Window *mSdlWindow;
	CameraHandler mCameraHandler;

	CEGUI::System* mSystem;
	CEGUI::Window* mLayout;



	//SheetHandler
	GUISheetHandler mGUISheetHandler;

	//CEGUI
	ParamsPanel* mFpsPanel;
	ParamsPanel* mDetailsPanel;

	// Logger
	static BoostLogger mBoostLogger;

	boost::posix_time::ptime mStart;
	boost::posix_time::ptime mNow;
	boost::posix_time::time_duration mRuntime;

	static class _Init {
	public:
		_Init() {
			mBoostLogger.add_attribute("ClassName",
					boost::log::attributes::constant<std::string>("SimulationManager"));
		}
	} _initializer;

public:
	SimulationManager(void);
	virtual ~SimulationManager(void);

	// Accessor methods
	// Accessor methods
	CameraHandler & getCameraHandler() {
		return mCameraHandler;
	}

	SDL2InputHandler getInputHandler() {
		return mInputHandler;
	}

	StateHandler getStateHandler() {
		return mStateHandler;
	}

	Ogre::Camera * getCamera() {
		return mCamera;
	}

	ParamsPanel*& getDetailsPanel();
	ParamsPanel*& getFpsPanel();
	void setDetailsPanel(ParamsPanel* detailsPanel);
	void setFpsPanel(ParamsPanel* fpsPanel);
	CEGUI::Window*& getLayout();
    Ogre::SceneManager*& getSceneManager();
    CEGUI::OgreRenderer*& getRenderer();
    Ogre::Root*& getRoot();
    bool configure(void);

	bool quit();
	CEGUI::System*& getCEGUISystem();
	virtual void windowResized(Ogre::RenderWindow* rw);

	int t;
	PopulationT<bool> parents;
	PopulationT<bool> offsprings;
	Ones jury;
	//
	// scaling window
	//
	std::vector<double> window;

protected:
	CEGUI::OgreRenderer* mRenderer;

	MathGLWindow* mTestObject;

	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void destroyScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void updatePanels();
	void updateEvolution();
	virtual void windowFocusChange(Ogre::RenderWindow* rw);
};

#endif // #ifndef __SimulationManager_h_
