//# corresponding headers
#include <SimulationManager.hpp>

//# forward declarations
//# system headers
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>

//## controller headers
#include <SDL.h>
#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include <SDL_version.h>
#include <SDL_video.h>

//## model headers
//### Boost headers
#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/time.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/log/core/record.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/parameter/keyword.hpp>

//## Bullet headers
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

//## view headers
//### CeGUI headers
#include <CEGUI/FontManager.h>
#include <CEGUI/GUIContext.h>
#include <CEGUI/Logger.h>
#include <CEGUI/MouseCursor.h>
#include <CEGUI/NamedXMLResourceManager.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/Singleton.h>
#include <CEGUI/Size.h>
#include <CEGUI/String.h>
#include <CEGUI/System.h>
#include <CEGUI/UDim.h>
#include <CEGUI/Vector.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/WindowManager.h>

//### Ogre headers
#include <OgreCamera.h>
#include <OgreColourValue.h>
#include <OgreCommon.h>
#include <OgreConfigOptionMap.h>
#include <OgreException.h>
#include <OgreFrameListener.h>
#include <OgreLight.h>
#include <OgreLogManager.h>
#include <OgrePlatform.h>
#include <OgreQuaternion.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreRenderTarget.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreUTFString.h>
#include <OgreVector3.h>
#include <OgreWindowEventUtilities.h>

//# custom headers
//## base headers
//## configuration headers
#include <configuration/ApplicationConfiguration.hpp>
#include <configuration/EnvironmentConfiguration.hpp>
#include <configuration/OgreSystemConfigStrings.hpp>
#include <configuration/PopulationConfiguration.hpp>
#include <configuration/LoggerConfiguration.hpp>

//## controller headers
#include <controller/universe/environments/Hills.hpp>
#include <controller/universe/environments/Plane.hpp>
#include <controller/universe/evolution/population/Population.hpp>
#include <controller/universe/evolution/Evolution.hpp>
#include <controller/universe/Planet.hpp>

//## model headers
//## view headers
#include <view/visualization/CEGUI/CEGUIBuilder.hpp>

//## utils headers
#include <utils/Randomness.hpp>

SimulationManager* SimulationManager::mSimulationManager;
BoostLogger SimulationManager::mBoostLogger;  // initialize the static variables
SimulationManager::_Init SimulationManager::_initializer;
//-------------------------------------------------------------------------------------
SimulationManager::SimulationManager(void) :
		mStateHandler(), mInputHandler(), mCameraHandler(this), mSdlWindow(
		NULL) {
	// Initialize the singleton
	mSimulationManager = this;
	// main frame timer initialization
	mStart = boost::posix_time::microsec_clock::local_time();
	mNow = boost::posix_time::microsec_clock::local_time();
	mRuntime = mNow - mStart;
}
//-------------------------------------------------------------------------------------
SimulationManager::~SimulationManager(void) {

	// tear down the scene
	destroyScene();

	// destroy the ogre renderer system
	CEGUI::OgreRenderer::destroySystem();
}

//-------------------------------------------------------------------------------------
void SimulationManager::createFrameListener(void) {

	// request a state change saying that the GUI is shown
	mStateHandler.requestStateChange(StateHandler::GUI);

	// Set initial mouse clipping size
	windowResized(mWindow);

	// Register as a Window and Frame listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);
}

//-------------------------------------------------------------------------------------
/**
 * Creates the scene of the application.
 */
void SimulationManager::createScene(void) {

	// Initialize the logger
	Logger::init("minemonics.log",LoggerConfiguration::LOGGING_LEVEL);
	Logger::initTermSink();

	// initialize random number generator
	boost::posix_time::time_duration duration(mNow.time_of_day());

	// Set render target with the current application name
	Ogre::RenderTarget* renderTarget = mRoot->getRenderTarget(
			ApplicationConfiguration::APPLICATION_TITLE);

	//################################################################
	//TODO: Camera must be handled within view controller #############
	// Create the camera controlling node
	Ogre::SceneNode *camNode =
			mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1",
					Ogre::Vector3(0, 500, 400));
	mCamera->setPosition(0, 500, 400);
	camNode->lookAt(Ogre::Vector3(0, 300, -4000), Ogre::Node::TS_WORLD);
	camNode->attachObject(mCamera);
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(12000);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(
			Ogre::RSC_INFINITE_FAR_PLANE)) {
		mCamera->setFarClipDistance(0); // enable infinite far clip distance if we can
	}

	// Populate the camera container
	mCameraHandler.setCamNode(mCamera->getParentSceneNode());

	// initialize GUI and views
	mViewController.initialize(this, renderTarget, &mStateHandler);

	//################################################################

	// initialize the simulation's debug drawer
	mDebugDrawer.initialize(mSceneMgr, false);
	mDebugDrawer.setDebugDrawingEnabled(false);
	mDebugDrawer.setDrawWireframe(true);
	mDebugDrawer.setDrawConstraints(true);
	mDebugDrawer.setDrawConstraintLimits(true);
	mDebugDrawer.setDrawContactPoints(true);
	mDebugDrawer.setDrawNormals(true);

	// ###################
	// We create the evaluation scene defined by the planet to be evaluated
	// ###################
	BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Setup evaluation environment...";

	// Set default ambient light
	mSceneMgr->setAmbientLight(
			Ogre::ColourValue(EnvironmentConfiguration::AMBIENT_R,
					EnvironmentConfiguration::AMBIENT_G,
					EnvironmentConfiguration::AMBIENT_B));

	// either create a skydome or a skyplane
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8, 4000, true);

//  Create skyplane
//	Ogre::Plane plane;
//	plane.d = 100;
//	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
//	mSceneMgr->setSkyPlane(true, plane, "Examples/CloudySky", 500, 20, true,
//			0.5, 150, 150);

	//Set fog color and fading function
	Ogre::ColourValue fadeColour(0, 0, 0);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 17000, 30000);
//	mSceneMgr->setFog(Ogre::FOG_EXP, fadeColour, 0.002);
//	mSceneMgr->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);

	// ###################
	// We initialize the universe
	// ###################
//	detectChildren(mSceneMgr->getRootSceneNode());
//	detectChildren((Ogre::SceneNode*)mSceneMgr->getRootSceneNode()->getChild("CamNode1"));
	mUniverse.initialize(this,
			EvaluationConfiguration::DEFAULT_PARALLEL_EVALUATION);

	// create a planet called earth
	Planet* earth = new Planet(Environment::PLANE, &mDebugDrawer, 10);

	// add earth to universe
	mUniverse.addPlanet(earth);

	// create a population
	Population* earthPopulation = new Population();
	earthPopulation->initialize(earth, 10, Ogre::Vector3(0, 300, -4000));

	// add earth population to earth
	earth->addPopulation(earthPopulation);

//	// create a population
//	Population* earth2Population = new Population();
//	earth2Population->initialize(earth, this, 100,
//			Ogre::Vector3(0, 300, -4000));
//
//	// add earth population to earth
//	earth->addPopulation(earth2Population);

	//perform embryogenesis on every creature that is not developed yet.
	//TODO: The whole planet embryogenesis still causes issues that give us capsules sticking to the camera.
//	earth->performEmbryogenesis();

	//TODO::Make it work with multiple planets
//	// create a planet called mars
//	Planet* mars = new Planet();
//	mars->initialize(this, Environment::PLANE, &mDebugDrawer, 10);
//
//	// add mars to universe
//	mUniverse.addPlanet(mars);
//
//	// create a population
//	Population* marsPopulation = new Population();
//	marsPopulation->initialize(mars, this, 3);
//
//	// add earth population to earth
//	mars->addPopulation(marsPopulation);

	BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Setup evaluation environment...done.";
}

/**
 * Called after all render targets have had their rendering commands
 * issued, but before render windows have been asked to flip their
 * buffers over.
 * @param evt
 * 		Information about the frame rendering.
 * @remarks
 * 		The usefulness of this event comes from the fact that rendering
 * commands are queued for the GPU to process. These can take a little
 * while to finish, and so while that is happening the CPU can be doing
 * useful things. Once the request to 'flip buffers' happens, the thread
 * requesting it will block until the GPU is ready, which can waste CPU
 * cycles. Therefore, it is often a good idea to use this callback to
 * perform per-frame processing. Of course because the frame's rendering
 * commands have already been issued, any changes you make will only
 * take effect from the next frame, but in most cases that's not noticeable.
 * @return
 * 		True to continue rendering, false to drop out of the rendering loop.
 */
bool SimulationManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {

	// update main frame timer
	mNow = boost::posix_time::microsec_clock::local_time();
	mRuntime = mNow - mStart;

	// shutdown the application if the application has initiated shutdown
	if (mWindow->isClosed()
			|| mStateHandler.getCurrentState() == StateHandler::SHUTDOWN) {

		//shutdown the video writer if it is still running
		if (mVideoWriter.isInitialized()) {
			mVideoWriter.close();
		}
		return false;
	}

	// Inject input into handlers
	mInputHandler.injectInput();

	// reposition the camera
	mCameraHandler.reposition(evt.timeSinceLastFrame);

	// step the physics forward
	mUniverse.stepPhysics(evt.timeSinceLastFrame);

	// update the universe
	mUniverse.update(evt.timeSinceLastFrame);

	// draw the debug output if enabled
	mUniverse.drawDebugWorld();

	// update the information in the panels on screen
	updatePanels(evt.timeSinceLastFrame);

	// update view
	mViewController.update(evt.timeSinceLastFrame);

	return true;
}

void SimulationManager::updatePanels(Ogre::Real timeSinceLastFrame) {

	std::vector<MathGLPanel*>::iterator it =
			mViewController.getGraphWindows().begin();
	for (; it != mViewController.getGraphWindows().end(); it++) {
		(*it)->update(timeSinceLastFrame);
	}

	if (mViewController.getFpsPanel() != NULL) {
		if (mViewController.getFpsPanel()->isVisible()) // if fps panel is visible, then update its contents
		{
			if (mViewController.getFpsPanel()->size() == 3) {
				mViewController.getFpsPanel()->setParamValue(0,
						Ogre::StringConverter::toString(mWindow->getLastFPS()),
						true);
				mViewController.getFpsPanel()->setParamValue(1,
						Ogre::StringConverter::toString(
								mRuntime.total_milliseconds()), true);

//				if (mTerrain->mEnvironmentType == Environment::HILLS) {
//					if (((HillsO3D*) mTerrain)->mTerrainGroup->isDerivedDataUpdateInProgress()) {
//						if (((HillsO3D*) mTerrain)->mTerrainsImported) {
//							mFpsPanel->setParamValue(2,
//									"Building terrain, please wait...", true);
//						} else {
//							mFpsPanel->setParamValue(2,
//									"Updating textures, patience...", true);
//						}
//					} else {
//						mFpsPanel->setParamValue(2, "Idle.", true);
//						if (((HillsO3D*) mTerrain)->mTerrainsImported) {
//							((HillsO3D*) mTerrain)->mTerrainGroup->saveAllTerrains(
//									true);
//							((HillsO3D*) mTerrain)->mTerrainsImported = false;
//						}
//					}
//				}
			} else {
				Ogre::RenderTarget::FrameStats fs = mWindow->getStatistics();
				mViewController.getFpsPanel()->setParamValue(0,
						Ogre::StringConverter::toString(fs.lastFPS), false);
				mViewController.getFpsPanel()->setParamValue(1,
						Ogre::StringConverter::toString(fs.avgFPS), false);
				mViewController.getFpsPanel()->setParamValue(2,
						Ogre::StringConverter::toString(fs.bestFPS), false);
				mViewController.getFpsPanel()->setParamValue(3,
						Ogre::StringConverter::toString(fs.worstFPS), false);
				mViewController.getFpsPanel()->setParamValue(4,
						Ogre::StringConverter::toString(fs.triangleCount),
						false);
				mViewController.getFpsPanel()->setParamValue(5,
						Ogre::StringConverter::toString(fs.batchCount), true);
			}
		}
	}
	if (mViewController.getDetailsPanel() != NULL) {
		if (mViewController.getDetailsPanel()->isVisible()) // if details panel is visible, then update its contents
		{
			mViewController.getDetailsPanel()->setParamValue(0,
					Ogre::StringConverter::toString(
							mCamera->getDerivedPosition().x), false);
			mViewController.getDetailsPanel()->setParamValue(1,
					Ogre::StringConverter::toString(
							mCamera->getDerivedPosition().y), false);
			mViewController.getDetailsPanel()->setParamValue(2,
					Ogre::StringConverter::toString(
							mCamera->getDerivedPosition().z), false);
			mViewController.getDetailsPanel()->setParamValue(4,
					Ogre::StringConverter::toString(
							mCamera->getDerivedOrientation().w), false);
			mViewController.getDetailsPanel()->setParamValue(5,
					Ogre::StringConverter::toString(
							mCamera->getDerivedOrientation().x), false);
			mViewController.getDetailsPanel()->setParamValue(6,
					Ogre::StringConverter::toString(
							mCamera->getDerivedOrientation().y), false);
			mViewController.getDetailsPanel()->setParamValue(7,
					Ogre::StringConverter::toString(
							mCamera->getDerivedOrientation().z), true);
		}
	}
}

//void SimulationManager::updatePhysics(double timeSinceLastFrame) {
//
//	std::vector<RagDoll*>::iterator it = mRagdolls.begin();
//	for (; it != mRagdolls.end(); it++) {
//		(*it)->update();
//	}
//
//	const int numObjects =
//			mPhysicsController.getDynamicsWorld()->getNumCollisionObjects();
//
//	for (int i = 0; i < numObjects; i++) {
//		btCollisionObject* colObj =
//				mPhysicsController.getDynamicsWorld()->getCollisionObjectArray()[i];
//		btRigidBody* body = btRigidBody::upcast(colObj);
//
//		if (body) {
//
//			btVector3 Point = body->getCenterOfMassPosition();
//
//			std::string text;
//			text.append("O: ");
//			text.append(boost::lexical_cast<std::string>((int)Point[0]));
//			text.append(",");
//			text.append(boost::lexical_cast<std::string>((int)Point[1]));
//			text.append(",");
//			text.append(boost::lexical_cast<std::string>((int)Point[2]));
//			text.append(",\n");
//			text.append("------");
//			InfoOverlayData* data = new InfoOverlayData(
//					Ogre::Vector3((float) Point[0], (float) Point[1],
//							(float) Point[2]), text);
//			mInfoOverlay.addInfo(data);
//
//			// Get the Orientation of the rigidbody as a bullet Quaternion
//			// Convert it to an Ogre quaternion
//			btQuaternion btq = body->getOrientation();
//			Ogre::Quaternion quart = Ogre::Quaternion(btq.w(), btq.x(), btq.y(),
//					btq.z());
//		}
//	}
//}

//TODO: Use for creature evolution, but clean up afterwards
//void SimulationManager::updateEvolution() {
//
//	//
//	// maximization task
//	//
//	int i = 0;
//	parents.setMaximize();
//	offsprings.setMaximize();
//
//	//
//	// initialize all chromosomes of parent population
//	//
//	for (i = 0; i < parents.size(); ++i)
//		parents[i][0].initialize();
//
//	//
//	// evaluate parents (only needed for elitist strategy)
//	//
//	if (EvolutionConfiguration::EVOLUTION_SELECTION_ELITISTS_QTY > 0)
//		for (i = 0; i < parents.size(); ++i) {
//			jury.setEvaluationSubject(parents[i][0]);
//			jury.evaluateFitness();
//			parents[i].setFitness(jury.getFitness());
//		}
//
//	//
//	// recombine by crossing over two parents
//	//
//	offsprings = parents;
//	for (i = 0; i < offsprings.size() - 1; i += 2)
//		if (Rng::coinToss(EvolutionConfiguration::CrossProb))
//			offsprings[i][0].crossover(offsprings[i + 1][0],
//					EvolutionConfiguration::CrossPoints);
//	//
//	// mutate by flipping bits
//	//
//	for (i = 0; i < offsprings.size(); ++i)
//		offsprings[i][0].flip(EvolutionConfiguration::FlipProb);
//	//
//	// evaluate objective function
//	//
//	for (i = 0; i < offsprings.size(); ++i) {
//		jury.setEvaluationSubject((offsprings[i][0]));
//		jury.evaluateFitness();
//		offsprings[i].setFitness(jury.getFitness());
//	}
//	//
//	// scale fitness values and use proportional selection
//	//
//	//offsprings.linearDynamicScaling(window, t);
//	parents.selectProportional(offsprings,
//			EvolutionConfiguration::EVOLUTION_SELECTION_ELITISTS_QTY);
//
//	t++;
//	//
//	// print out best value found so far
//	//
//	std::cout << "Generation " << t << "s best individual has fitness value "
//			<< "\t" << parents.best().fitnessValue() << std::endl;
//
//}

/**
 * What to do if the window is resized.
 * @param rw The handle of the render window that was resized.
 */
void SimulationManager::windowResized(Ogre::RenderWindow* rw) {
	//BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Repositioning CEGUI pointer...";
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	int x, y;
	SDL_GetMouseState(&x, &y);

	// Align CEGUI mouse with SDL2 mouse
	CEGUI::Vector2f mousePos =
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
			x - mousePos.d_x, y - mousePos.d_y);
	int w, h;
	SDL_GetWindowSize(mSdlWindow, &w, &h);
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	mWindow->resize(w, h);
#else
	mWindow->windowMovedOrResized();
#endif

	//BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info) << "Notifying CEGUI of resize....";
	mViewController.notifyDisplaySizeChanged(width, height);
}

/**
 * What to do if the window focus has changed.
 * @param rw The handle of the render window whose focus has changed.
 */
void SimulationManager::windowFocusChange(Ogre::RenderWindow* rw) {
	if (rw->isVisible()) {
		//BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info)<< "Window has gained focus...";

		// Align CEGUI mouse with SDL mouse
		mViewController.updateMousePosition(mInputHandler.getMousePositionX(),
				mInputHandler.getMousePositionY());
	} else {
		//BOOST_LOG_SEV(mBoostLogger, boost::log::trivial::info) << "Window has lost focus...";
	}
}
//-------------------------------------------------------------------------------------

/**
 * Destroy the scene if the application is quit.
 */
void SimulationManager::destroyScene(void) {
//	std::vector<RagDoll*>::iterator it = mRagdolls.begin();
//	for (; it != mRagdolls.end(); it++) {
//		(*it)->removeFromWorld();
//	}

	//mUniverse.removeFromWorld();

//TODO: Clean up universe properly
//	mPhysicsController.exitBulletPhysics();

}
//-------------------------------------------------------------------------------------

/**
 * Quit the application.
 * @return Returns true if the application could be closed normally.
 */
bool SimulationManager::quit() {
	mStateHandler.requestStateChange(StateHandler::SHUTDOWN);
	mShutDown = true;
	return true;
}

/**
 * Configure and set up the window and the render window for the simulator
 */
bool SimulationManager::configure(void) {

	mInputHandler.initialize(this);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Cannot initialize SDL2!", "GraphicsSystem::initialize");
	}

	// Show the configuration dialog and initialize the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if (!mRoot->restoreConfig()) {
		if (!mRoot->showConfigDialog()) {
			mStateHandler.requestStateChange(StateHandler::SHUTDOWN);
		}
	}

	Ogre::ConfigOptionMap& cfgOpts =
			mRoot->getRenderSystem()->getConfigOptions();

	mRoot->getRenderSystem()->setConfigOption("sRGB Gamma Conversion", "Yes");
	mRoot->initialise(false);

	//split resolution string
	std::vector<std::string> elems;
	std::stringstream ss(cfgOpts[OgreConf::VIDEO_MODE].currentValue);
	std::string item;
	while (std::getline(ss, item, 'x')) {
		elems.push_back(item);
	}

	//parse video resolution out of the configuration
	char* end;
	int width = strtol(elems.at(0).c_str(), &end, 10);
	int height = strtol(elems.at(1).c_str(), &end, 10);

	//find the position of the window
	int screen = 0;
	int posX = 0;
	int posY = 0;

	//is the configuration in full screen?
	bool fullscreen = (cfgOpts[OgreConf::FULL_SCREEN].currentValue
			== OgreConf::YES);

	//if full screen
	if (fullscreen) {
		// set the window position to full screen
		posX = SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen);
		posY = SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen);
	} else {
		//center the window on the screen
		posX = SDL_WINDOWPOS_CENTERED_DISPLAY(screen);
		posY = SDL_WINDOWPOS_CENTERED_DISPLAY(screen);
	}

	// create the SDL window
	mSdlWindow = SDL_CreateWindow(
			ApplicationConfiguration::APPLICATION_TITLE.c_str(), // window title
			posX,               // initial x position
			posY,               // initial y position
			width,              // width, in pixels
			height,             // height, in pixels
			SDL_WINDOW_SHOWN | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
					| SDL_WINDOW_RESIZABLE);

	//Get the native window handle
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE) {
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Couldn't get WM Info! (SDL2)", "GraphicsSystem::initialize");
	}

	Ogre::String winHandle;
	Ogre::NameValuePairList params;

	switch (wmInfo.subsystem) {
#ifdef WIN32
	case SDL_SYSWM_WINDOWS:
	// required to make OGRE work on Windows
	winHandle = Ogre::StringConverter::toString( (unsigned long)wmInfo.info.win.window );
	break;
#elif __MACOSX__
	case SDL_SYSWM_COCOA:
	//required to make OGRE work on MAX OSX
	params.insert( std::make_pair("macAPI", "cocoa") );
	params.insert( std::make_pair("macAPICocoaUseNSView", "true") );

	winHandle = Ogre::StringConverter::toString(WindowContentViewHandle(wmInfo));
	break;
#else
	case SDL_SYSWM_X11:
		//required to make OGRE work on Linux
		winHandle = Ogre::StringConverter::toString(
				(unsigned long) wmInfo.info.x11.window);
		break;
#endif
	default:
		// in case it can not recognize the windowing system
		OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
				"Unexpected WM! (SDL2)", "GraphicsSystem::initialize");
		break;
	}

	//put together the parameter list for the OGRE render window
	params.insert(
			std::make_pair("title",
					ApplicationConfiguration::APPLICATION_TITLE));
	params.insert(std::make_pair("gamma", "true"));
	params.insert(std::make_pair("FSAA", cfgOpts["FSAA"].currentValue));
	params.insert(std::make_pair("vsync", cfgOpts["VSync"].currentValue));

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	params.insert( std::make_pair("externalWindowHandle", winHandle) );
#else
	params.insert(std::make_pair("parentWindowHandle", winHandle));
#endif

	// create the OGRE render window
	mWindow = Ogre::Root::getSingleton().createRenderWindow(
			ApplicationConfiguration::APPLICATION_TITLE, width, height,
			fullscreen, &params);
	return true;
}

/**
 * The main function of the simulator.
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
		{
// Create application object
	SimulationManager app;

	try {
		app.go();
	} catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: "
				<< e.getFullDescription().c_str() << std::endl;
#endif
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
