// MFC_OSG.cpp : implementation of the cOSG class
//
#include "stdafx.h"
#include "OsgObject.h"
#include <list>
cOSG::cOSG(HWND hWnd):m_hWnd(hWnd),label_event_(nullptr)
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
    InitManipulators(); // ������

    InitSceneGraph();

    InitCameraConfig();
	
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
	//mRoot->addChild(osgDB::readNodeFile("H:/002.OpenSceneGraph/019.Earth/003.������-VPB�÷�����볣�����⴦��/vpbtest/TestCommon10/output.ive"));

	china_boundaries_ = dynamic_cast<osgEarth::ImageLayer*>(mapNode->getMap()->getLayerByName("world_boundaries"));
	
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

	// Set the Scene Data
	mViewer->setSceneData(mRoot.get());

    // Add the Camera Manipulator to the Viewer
    mViewer->setCameraManipulator(keyswitchManipulator.get());
	// mViewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);

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
	coordinate_system_node_ = new osg::CoordinateSystemNode; // ��������ϵ�ڵ�
	coordinate_system_node_->setEllipsoidModel(new osg::EllipsoidModel()); // ������Բ��ģ��

	// ���ػ���
	airport = osgDB::readNodeFile("./data/airport/heinei_airport.ive"); // ��ȡ�����ļ�
	mtAirport = new osg::MatrixTransform; // ����任
	// mtAirport->setMatrix(osg::Matrix::scale(100,100,100)*osg::Matrixd::rotate(-1.57/2,osg::Vec3(0,0,1)));
	mtAirport->addChild(airport);
	mRoot->addChild(mtAirport);

	// ���û�������
	osg::Matrixd mtTemp;   // ����λ��  109.13 34.38 �߶ȣ�8434.96  ���Σ�390
	coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3762),osg::DegreesToRadians(109.1263),360,mtTemp);
	mtAirport->setMatrix(mtTemp);

	// ���طɻ�
	fly_airport = osgDB::readNodeFile("./data/airplane/F-16.ive"); // ��ȡ�ɻ��ļ�
	mtrix_fly_self = new osg::MatrixTransform();
	mtrix_fly_self->setMatrix(osg::Matrix::scale(10,10,10)*osg::Matrixd::rotate(-1.57/2,osg::Vec3(0,0,1)));

	mtrix_fly_self->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL,osg::StateAttribute::ON);// �������ԣ����շ���
	mtrix_fly_self->addChild(fly_airport);

	mtrix_fly_airport = new osg::MatrixTransform;
	mtrix_fly_airport->addChild(mtrix_fly_self);

	mRoot->addChild(mtrix_fly_airport);

	// ���÷ɻ�����
	coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3834),osg::DegreesToRadians(109.1347),437,mtTemp);
	mtrix_fly_airport->setMatrix(mtTemp);

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
	//��ʼ��������
	em_ = new osgEarth::Util::EarthManipulator;
	if (mapNode.valid())
	{
		em_->setNode(mapNode);
	}
	em_->getSettings()->setArcViewpointTransitions(true);

	mViewer->setCameraManipulator(em_);
	em_->setViewpoint(osgEarth::Viewpoint("view_point2",112.44,33.75,444.02,-15.84,-53.01,402812.75),5);

	////��ʼ�����
	/*osgEarth::Config skyConf;
	double hours = skyConf.value("hours", 12.0);
	osg::ref_ptr<osgEarth::Util::SkyNode> sky_node = new osgEarth::Util::SkyNode(mapNode->getMap());
	sky_node->setDateTime(2012, 1, 27, hours);
	sky_node->attach(mViewer, 1);
	// sky_node->setAmbientBrightness(1.0, mViewer);
	mRoot->addChild(sky_node);*/


	china_boundaries_ = dynamic_cast<osgEarth::ImageLayer*>(mapNode->getMap()->getLayerByName("world_boundaries"));

	mapNode->getBound();


	// ������ʾ�ӵ���Ϣ�Ŀؼ�
	addViewPointLable();

	// ��ӻ���
	addAirport();

}

void cOSG::addViewPointLable()
{
	if (mViewer==nullptr){
		TRACE(TEXT("mViewer is null, at addViewPointLable!"));
		return ;
	}
	canvas_ = ControlCanvas::getOrCreate(mViewer);

	// ��ӿؼ���������ʾ�ӵ���Ϣ
	osgEarth::Util::Controls::LabelControl* viewCoords = new osgEarth::Util::Controls::LabelControl(TEXT("TestViewPoint"),osg::Vec4(1.0,1.0,1.0,1.0));
	/*viewCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	viewCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	viewCoords->setBackColor(0,0,0,0.5);
	viewCoords->setSize(800,50);
	viewCoords->setMargin(10);*/
	canvas_->addControl(viewCoords);

	// ��ӿؼ���������ʾ�����Ϣ
	/*osgEarth::Util::Controls::LabelControl* mouseCoords = new osgEarth::Util::Controls::LabelControl(TEXT("TestViewPoint"),osg::Vec4(1.0,1.0,1.0,1.0));
	mouseCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouseCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
	mouseCoords->setBackColor(0,0,0,0.5);
	mouseCoords->setSize(400,50);
	mouseCoords->setMargin(10);
	canvas_->addControl(mouseCoords);*/

	if (label_event_ == 0){
		label_event_ = new CLabelControlEventHandler(mapNode,viewCoords);
	}
	mViewer->addEventHandler(label_event_);
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

// �����ľ��빫ʽ 
double cOSG::GetDis(osg::Vec3 form, osg::Vec3 to)
{
	return sqrt(pow((to.x() - form.x()), 2) + pow((to.y() - form.y()), 2) + pow((to.z() - form.z()), 2));
}

// �����һ���ʱ��
double cOSG::GetRunTime(osg::Vec3 from, osg::Vec3 to, double speed)
{
	double dist = GetDis(from, to);
	if (speed == 0){
		return 1000000000;
	}
	return dist / speed;
}

void cOSG::DoAPreLine()
{
	/*
		if 0����Ϊֱ�Ӽ��뾭γ�ߡ��ٶȣ����ܵ��·ɻ���̬��׼ȷ��
		else����Ϊ��0�ľ�γ�߻�����ÿ���ڵ��������20���ڵ㣬�������˻����ӱ��濿����
	*/
#if 0
	osg::ref_ptr<osg::Vec4Array> vaTemp = new osg::Vec4Array;
	vaTemp->push_back(osg::Vec4(109.1050, 34.3678, 413, 50));
	vaTemp->push_back(osg::Vec4(109.1101, 34.3717, 413, 140));
	vaTemp->push_back(osg::Vec4(109.1237, 34.3818, 450, 300));
	vaTemp->push_back(osg::Vec4(109.1455, 34.3985, 700, 500));

	vaTemp->push_back(osg::Vec4(109.2941, 34.4344, 1000, 800));
	vaTemp->push_back(osg::Vec4(109.1892, 34.4597, 2000, 1000));
	vaTemp->push_back(osg::Vec4(109.1439, 34.3993, 1500, 700));

	vaTemp->push_back(osg::Vec4(109.1291, 34.3861, 600, 500));
	vaTemp->push_back(osg::Vec4(109.1101, 34.3717, 413, 200));
	vaTemp->push_back(osg::Vec4(109.1050, 34.3678, 410, 100));
	vaTemp->push_back(osg::Vec4(109.1050, 34.3678, 0, 0));
	apc = createAirLinePath(vaTemp);
#else
	struct FlyNodeData{
		FlyNodeData(double _x,double _y,double _z,int _speed){
			x = _x;
			y = _y;
			z = _z;
			speed = _speed;
		}
		double x, y, z;
		int speed;
	};

	std::list<struct FlyNodeData> FlyNodeList;
	FlyNodeList.push_back(struct FlyNodeData(109.1050, 34.3678, 413, 100));
	FlyNodeList.push_back(struct FlyNodeData(109.1101, 34.3717, 500, 200));
	FlyNodeList.push_back(struct FlyNodeData(109.1237, 34.3818, 666, 500));
	FlyNodeList.push_back(struct FlyNodeData(109.1455, 34.3985, 700, 600));
	FlyNodeList.push_back(struct FlyNodeData(109.2941, 34.4344, 1000, 800));
	FlyNodeList.push_back(struct FlyNodeData(109.1892, 34.4597, 1300, 900));
	FlyNodeList.push_back(struct FlyNodeData(109.1439, 34.3993, 1000, 800));
	FlyNodeList.push_back(struct FlyNodeData(109.1455, 34.3985, 700, 600));
	FlyNodeList.push_back(struct FlyNodeData(109.1291, 34.3861, 666, 500));
	FlyNodeList.push_back(struct FlyNodeData(109.1101, 34.3717, 500, 200));
	FlyNodeList.push_back(struct FlyNodeData(109.1050, 34.3678, 413, 100));

	osg::ref_ptr<osg::Vec4Array> vaTemp = new osg::Vec4Array;
	for (std::list<struct FlyNodeData>::iterator it = FlyNodeList.begin(); it != FlyNodeList.end(); ++it){
		struct FlyNodeData data     = *it;		
		if (it == FlyNodeList.end()){
			vaTemp->push_back(osg::Vec4(data.x, data.y, data.z, data.speed));
			vaTemp->push_back(osg::Vec4(data.x, data.y, data.z+10, data.speed));
			break;
		}
		struct FlyNodeData dataNext = *(it);

		const int STEP = 20;
		double oneStepX = (dataNext.x - data.x) / STEP;
		double oneStepY = (dataNext.y - data.y) / STEP;
		double oneStepZ = (dataNext.z - data.z) / STEP;
		for (int index = 1; index <= STEP; ++index){
			double valueX = oneStepX * index;
			double valueY = oneStepY * index;
			double valueZ = oneStepZ * index;
			vaTemp->push_back(osg::Vec4(data.x + valueX, data.y + valueY, data.z + valueZ, data.speed));
		}
	}
	// ��������Ŀ��Ƶ㣬���һ��·��
	apc_ = createAirLinePath(vaTemp);

#endif
}

// ��������Ŀ��Ƶ㣬���һ��·�������Ƶ��ʽ�����ȣ�γ�ȣ��߶ȣ��ٶȣ�
osg::ref_ptr<osg::AnimationPath> cOSG::createAirLinePath(osg::Vec4Array * ctrl) // ���Ƶ�
{
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;		// ����·��
	animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);						// ������ѭ��

	
	osg::Vec3d curPosition,curNextPosition;
	double time = 0;

	osg::Matrix matrix;
	osg::Quat _rotation;

	_rotation.makeRotate(osg::DegreesToRadians(90.0),0.0,0.0,1.0);

	double hAngle = 0.0,vAngle = 0.0;
	for (osg::Vec4Array::iterator iterator = ctrl->begin(); iterator != ctrl->end(); ++iterator){
		osg::Vec4Array::iterator iterator2 = iterator; // iterator2 �� iterator ����һ����
		++iterator2;

		// ��Ҫ�ж��ǲ����Ѿ�����
		if (iterator2 == ctrl->end()) { break; }

		// ���㵱ǰ��͵�ǰ����һ��λ��:�ɾ�γ��תΪxyz
		coordinate_system_node_->getEllipsoidModel()->convertLatLongHeightToXYZ(
			osg::DegreesToRadians(iterator->y()),
			osg::DegreesToRadians(iterator->x()),
			//osg::DegreesToRadians(iterator->z()),
			iterator->z(),
			curPosition.x(), curPosition.y(), curPosition.z()
		);		
		coordinate_system_node_->getEllipsoidModel()->convertLatLongHeightToXYZ(
			osg::DegreesToRadians(iterator2->y()),
			osg::DegreesToRadians(iterator2->x()),
			//osg::DegreesToRadians(iterator2->z()),
			iterator2->z(),
			curNextPosition.x(), curNextPosition.y(), curNextPosition.z()
		);

		// ���㴹ֱ�н�  �߶���ͬ
		if (iterator->z() == iterator2->z())
		{
			vAngle = 0; // ˮƽ��
		}
		else
		{
			if (sqrt(pow(GetDis(curPosition, curNextPosition), 2)) - pow(iterator2->z() - iterator->z(), 2) == 0){
				vAngle = osg::PI_2;
			}else{
				vAngle = atan((iterator2->z() - iterator->z()) / sqrt(pow(GetDis(curPosition, curNextPosition), 2)) - pow((iterator2->z() - iterator->z()), 2)); // ���Ҷ���
			}

			if (vAngle >= osg::PI_2)// ��֤�ɻ����ܴ��
				vAngle = osg::PI_2;
			if (vAngle <= -osg::PI_2){
				vAngle = -osg::PI_2;
			}
		}

		// ��1 �� ֱ�߷ɣ� ���ҽǶȣ���γ�ȼн�   ���½Ƕ�

		// ˮƽ�нǣ� ������ͬ �н�90��   | γ����ͬ �н�0��180

		// ����ˮƽ�н�  ������ͬ ˮƽ�н�90��  
		if (iterator->x() == iterator2->x())
		{
			hAngle = osg::PI_2;
		}
		else
		{
			hAngle = (iterator2->y() - iterator->y()) / (iterator2->x() - iterator->x());

			if (iterator2->x() > iterator->x())
				hAngle += osg::PI;
		}

		// ��ɻ��ı任����
		coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(iterator->y()), osg::DegreesToRadians(iterator->x()), iterator->z(), matrix);
		_rotation.makeRotate(0, osg::Vec3(1.0, 0.0, 0.0), vAngle + osg::PI_2, osg::Vec3(0.0, 1.0, 0.0), hAngle, osg::Vec3(0.0, 0.0, 1.0));
		matrix.preMultRotate(_rotation);
		animationPath->insert(time, osg::AnimationPath::ControlPoint(curPosition, matrix.getRotate()));

		// ����һ�����ʱ�������
		time += GetRunTime(curPosition, curNextPosition, iterator2->w());
	}

	animationPath->insert(time, osg::AnimationPath::ControlPoint(curNextPosition, matrix.getRotate()));
	return animationPath.release();
}

void cOSG::DoPreLineNow()
{
	/*
	focalPoint	���㣺����Ľ���λ�ã���һ���������꣨���̣߳�
	range	���ࣺ���λ�õ�����ľ��룬��λ���ס�
	pitch	�����ǣ�-90��0��ֵ����λ�Ƕ�
	heading	ˮƽ��λ�ǣ�0-360��ֵ�����Ƶ�ͼˮƽ��ת����λ�Ƕȡ�
	*/
	mtrix_fly_airport->setUpdateCallback(new osg::AnimationPathCallback(apc_,0.0,1.0));
	// �ļ��� ����  γ�� �߶� ˮƽ��λ�� ��ֱ������ ���ӷ�Χ
	em_->setViewpoint(osgEarth::Viewpoint("view_point5",109.1347,34.3834,0,24.261,-21.6,1000),1);
	// em_->setNode(mtrix_fly_airport);
	// em_->setTetherNode(mtrix_fly_airport);
	// osgEarth2.10����setNode���setTetherNode�����ӵ����
	// em_->setNode(mtrix_fly_airport);

	//����������
	//��ȡ��ǰ���������ӵ㣬��ģ�ͷŽ�����ӽ��У�Ȼ����������ӽǵ�һЩ������������ĸ��ǶȺ;���۲�ģ�͡�Ȼ������ӵ�����Ϊ���������ӵ㡣
	osgEarth::Viewpoint vp = em_->getViewpoint();
	vp.setNode(mtrix_fly_airport);
	/*vp.name()._set("view_point5");
	vp.range()->set(250000.0, osgEarth::Units::METERS);//�۲�ľ���
	vp.pitch()->set(-45.0, osgEarth::Units::DEGREES);//�۲�ĽǶ�
	em->setViewpoint(vp, 1.0);*/
}

// ���ø���
void cOSG::IsTrack(bool btrack)
{
	if (btrack){
		em_->getViewpoint().setNode(mtrix_fly_airport);
	}else{
		em_->getViewpoint().setNode(0);
	}
}

void cOSG::FlyTo(double longitude,double latitude,double altitude){
	/*theApp.b_need_modify_ = true;
	while (!theApp.b_can_modify_){
		Sleep(1);
	}*/
	em_->setViewpoint(osgEarth::Viewpoint("viewer_point3",longitude,latitude,0,-60,0,altitude),2);
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
