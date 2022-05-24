// MFC_OSG.cpp : implementation of the cOSG class
//
#include "stdafx.h"
#include "OsgObject.h"


cOSG::cOSG(HWND hWnd):m_hWnd(hWnd)
{
}

cOSG::~cOSG()
{
    mViewer->setDone(true);
    Sleep(1000);
    mViewer->stopThreading();

    delete mViewer;
}

void cOSG::InitOSG(std::string modelname)
{
    // Store the name of the model to load
    m_ModelName = modelname;

    // Init different parts of OSG
    InitManipulators(); // 操纵器
    InitSceneGraph();
    InitCameraConfig();
	// addAirport();
	InitOsgEarth();
}

void cOSG::InitManipulators(void)
{
    // Create a trackball manipulator
    trackball = new osgGA::TrackballManipulator();

    // Create a Manipulator Switcher
    keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

    // Add our trackball manipulator to the switcher
    keyswitchManipulator->addMatrixManipulator( '1', "Trackball", trackball.get());

    // Init the switcher to the first manipulator (in this case the only manipulator)
    keyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value
}


void cOSG::InitSceneGraph(void)
{
    // Init the main Root Node/Group
    mRoot  = new osg::Group;

    // Load the Model from the model name
    mModel = osgDB::readNodeFile(m_ModelName);
    if (!mModel) return;

    // Optimize the model
    osgUtil::Optimizer optimizer;
    optimizer.optimize(mModel.get());
    optimizer.reset();

    // Add the model to the scene
    mRoot->addChild(mModel.get());
	mapNode = dynamic_cast<osgEarth::MapNode*>(mModel.get());
	//mRoot->addChild(osgDB::readNodeFile("H:/002.OpenSceneGraph/019.Earth/003.第三讲-VPB用法详解与常见问题处理/vpbtest/TestCommon10/output.ive"));

}

void cOSG::InitCameraConfig(void)
{
    // Local Variable to hold window size data
    RECT rect;

    // Create the viewer for this window
    mViewer = new osgViewer::Viewer();

    // Add a Stats Handler to the viewer
    mViewer->addEventHandler(new osgViewer::StatsHandler);

    // Get the current window size
    ::GetWindowRect(m_hWnd, &rect);

    // Init the GraphicsContext Traits
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

    // Init the Windata Variable that holds the handle for the Window to display OSG in.
    osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

    // Setup the traits parameters
    traits->x = 0;
    traits->y = 0;
    traits->width = rect.right - rect.left;
    traits->height = rect.bottom - rect.top;
    traits->windowDecoration = false;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
    traits->setInheritedWindowPixelFormat = true;
    traits->inheritedWindowData = windata;

    // Create the Graphics Context
    osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

    // Init Master Camera for this View
    osg::ref_ptr<osg::Camera> camera = mViewer->getCamera();

    // Assign Graphics Context to the Camera
    camera->setGraphicsContext(gc);

    // Set the viewport for the Camera
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

    // set the draw and read buffers up for a double buffered window with rendering going to back buffer
    camera->setDrawBuffer(GL_BACK);
    camera->setReadBuffer(GL_BACK);

    // Set projection matrix and camera attribtues
    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);

    // Add the Camera to the Viewer
    //mViewer->addSlave(camera.get());
    mViewer->setCamera(camera.get());

    // Add the Camera Manipulator to the Viewer
    mViewer->setCameraManipulator(keyswitchManipulator.get());

    // Set the Scene Data
    mViewer->setSceneData(mRoot.get());

    // Realize the Viewer
    mViewer->realize();

    // Correct aspect ratio
    /*double fovy,aspectRatio,z1,z2;
    mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
    aspectRatio=double(traits->width)/double(traits->height);
    mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);*/
}

void cOSG::PreFrameUpdate()
{
    // Due any preframe updates in this routine
	while(theApp.b_need_modify_){Sleep(1);}
	theApp.b_can_modify_ = false;
}

void cOSG::PostFrameUpdate()
{
    // Due any postframe updates in this routine
	if (theApp.b_need_modify_){
		theApp.b_can_modify_ = true;
	}
}


void cOSG::addAirport()
{
	csn = new osg::CoordinateSystemNode;
	csn->setEllipsoidModel(new osg::EllipsoidModel());

	airport = osgDB::readNodeFile("../data/module/nanyuan.ive");
	 
	mtAirport = new osg::MatrixTransform;
	mtAirport->addChild(airport);
	mRoot->addChild(mtAirport);
	osg::Matrixd mtTemp;
	csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3762),osg::DegreesToRadians(109.1263),360,mtTemp);
	mtAirport->setMatrix(mtTemp);
}

void cOSG::rmWorldBound()
{
	theApp.b_need_modify_ = true;
	while(!theApp.b_can_modify_)Sleep(1);
	if (china_boundaries_)
	{
		mapNode->getMap()->removeLayer(china_boundaries_);
	}
	theApp.b_need_modify_ = false;
}

void cOSG::addWorldBound()
{
	theApp.b_need_modify_ = true;
	while(!theApp.b_can_modify_)Sleep(1);
	if (china_boundaries_)
	{
		mapNode->getMap()->addLayer(china_boundaries_);
	}
    theApp.b_need_modify_ = false;

}

void cOSG::InitOsgEarth()
{
	//初始化操作器
	em = new osgEarth::Util::EarthManipulator;
	if (mapNode.valid())
	{
		em->setNode(mapNode);
	}
	em->getSettings()->setArcViewpointTransitions(true);
	mViewer->setCameraManipulator(em);

	////初始化天空
	/*osgEarth::Config skyConf;
	double hours = skyConf.value("hours", 12.0);
	osg::ref_ptr<osgEarth::Util::SkyNode> sky_node = new osgEarth::Util::SkyNode(mapNode->getMap());
	sky_node->setDateTime(2012, 1, 27, hours);
	sky_node->attach(mViewer, 1);
	// sky_node->setAmbientBrightness(1.0, mViewer);
	mRoot->addChild(sky_node);*/


	china_boundaries_ = dynamic_cast<osgEarth::ImageLayer*>(mapNode->getMap()->getLayerByName("world_boundaries"));

	mapNode->getBound();

	// 新增显示视点信息的控件
	// addViewPointLable();

}

void cOSG::addViewPointLable()
{
	mRoot->addChild(osgEarth::Util::Controls::ControlCanvas::get(mViewer));
	osgEarth::Util::Controls::ControlCanvas* canvas = osgEarth::Util::Controls::ControlCanvas::get(mViewer);
	
	// 添加控件，用来显示视点信息
	osgEarth::Util::Controls::LabelControl* viewCoords = new osgEarth::Util::Controls::LabelControl(TEXT("TestViewPoint"),osg::Vec4(1.0,1.0,1.0,1.0));
	viewCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	viewCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	viewCoords->setBackColor(0,0,0,0.5);
	viewCoords->setSize(800,50);
	viewCoords->setMargin(10);
	canvas->addChild(viewCoords);

	// 添加控件，用来显示鼠标信息
	osgEarth::Util::Controls::LabelControl* mouseCoords = new osgEarth::Util::Controls::LabelControl(TEXT("TestViewPoint"),osg::Vec4(1.0,1.0,1.0,1.0));
	mouseCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouseCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
	mouseCoords->setBackColor(0,0,0,0.5);
	mouseCoords->setSize(400,50);
	mouseCoords->setMargin(10);
	canvas->addChild(mouseCoords);
}

void cOSG::set_boundaries(double opt)
{
	if (china_boundaries_){
		china_boundaries_->setOpacity(opt);
	}

}

double cOSG::get_boundaries()
{
	if (china_boundaries_){
		return china_boundaries_->getOpacity();
	}
	return 0.0f;
}

/*void cOSG::Render(void* ptr)
{
    cOSG* osg = (cOSG*)ptr;

    osgViewer::Viewer* viewer = osg->getViewer();

    // You have two options for the main viewer loop
    //      viewer->run()   or
    //      while(!viewer->done()) { viewer->frame(); }

    //viewer->run();
    while(!viewer->done())
    {
        osg->PreFrameUpdate();
        viewer->frame();
        osg->PostFrameUpdate();
        //Sleep(10);         // Use this command if you need to allow other processes to have cpu time
    }

    // For some reason this has to be here to avoid issue:
    // if you have multiple OSG windows up
    // and you exit one then all stop rendering
    AfxMessageBox("Exit Rendering Thread");

    _endthread();
}*/

CRenderingThread::CRenderingThread( cOSG* ptr )
:   OpenThreads::Thread(), _ptr(ptr), _done(false)
{
}

CRenderingThread::~CRenderingThread()
{
    _done = true;
    if (isRunning())
    {
        cancel();
        join();
    }
}

void CRenderingThread::run()
{
    if ( !_ptr )
    {
        _done = true;
        return;
    }

    osgViewer::Viewer* viewer = _ptr->getViewer();
    do
    {
        _ptr->PreFrameUpdate();
        viewer->frame();
        _ptr->PostFrameUpdate();
    } while ( !testCancel() && !viewer->done() && !_done );
}
