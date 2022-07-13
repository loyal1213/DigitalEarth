// MFC_OSG.cpp : implementation of the cOSG class
//
#include "stdafx.h"
#include "OsgObject.h"
#include <list>
#include <osg/Math>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthFeatures/FeatureSource>
#include <osg/Notify>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthFeatures/GeometryUtils>
#include <osgEarthFeatures/FeatureCursor>
#include <osgEarthAnnotation/LabelNode>

#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthSymbology/GeometryFactory>
#include <osgViewer/Viewer>

#include "StringConvert.h"
#include "CreateTrackCallback.h"
#include "TrailerCallback.h"
#include "Compass.h"

using namespace osg;
using namespace osgEarth;
using namespace osgEarth::Features;
using namespace osgEarth::Drivers;
using namespace osgEarth::Symbology;
using namespace osgEarth::Annotation;
using namespace osgEarth::Util;

cOSG::cOSG(HWND hWnd):m_hWnd(hWnd),label_event_(nullptr){

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



	// 初始化 earth 操作器
	earth_manipulator_ = new osgEarth::Util::EarthManipulator;
	if (mapNode_.valid()){
		earth_manipulator_->setNode(mapNode_);
	}
	earth_manipulator_->getSettings()->setArcViewpointTransitions(true);

}


std::string attributeTypeToString( AttributeType type )
{
	switch (type)
	{
	case ATTRTYPE_BOOL: return "Boolean";
	case ATTRTYPE_DOUBLE: return "Double";
	case ATTRTYPE_INT: return "Integer";
	case ATTRTYPE_STRING: return "String";
	default:  return "Unspecified";
	}
}


void printStats(FeatureSource* features)
{
	// std::cout << "Feature Count:  " << features->getFeatureCount() << std::endl;
	// std::cout << "Geometry Type:  " << osgEarth::Symbology::Geometry::toString( features->getGeometryType() ) << std::endl;
	// TRACE2(TEXT("Feature Count: %d, Geometry Type: %s \n"),features->getFeatureCount(), osgEarth::Symbology::Geometry::toString( features->getGeometryType() ) );
	char buffer[520] = {0};
	sprintf(buffer,TEXT("Feature Count: %d, Geometry Type: %s \n"),
		features->getFeatureCount(), osgEarth::Symbology::Geometry::toString( features->getGeometryType() )
		);
	OutputDebugString(buffer);

	//Print the schema
	const FeatureSchema schema = features->getSchema();
	// std::cout << "Schema:" << std::endl;
	TRACE(TEXT("Schema: \n"));
	for (FeatureSchema::const_iterator itr = schema.begin(); itr != schema.end(); ++itr)
	{
		// std::cout << indent << itr->first << ": " << attributeTypeToString(itr->second) << std::endl;
		char buffer[520] = {0};
		//sprintf(buffer,TEXT("\t\t %s: %s"),itr->first ,attributeTypeToString(itr->second));
		OutputDebugString("\t\t");
		OutputDebugString(itr->first.c_str());
		OutputDebugString("\t");
		OutputDebugString(attributeTypeToString(itr->second).c_str());
		OutputDebugString("\n");
		// TRACE2(TEXT("\t\t %s:  %s\n"), itr->first ,attributeTypeToString(itr->second));
	}
	// std::cout << std::endl;
	TRACE(TEXT("\n"));
}

void printFeature( Feature* feature )
{
	// std::cout << "FID: " << feature->getFID() << std::endl;
	TRACE1(_T("FID: %ul \n"), feature->getFID());
	for (AttributeTable::const_iterator itr = feature->getAttrs().begin(); itr != feature->getAttrs().end(); ++itr){
		/*std::cout 
			<< indent 
			<< itr->first << "=" << itr->second.getString() << " ("
			<< (itr->second.first == ATTRTYPE_INT?    "integer" :
			itr->second.first == ATTRTYPE_DOUBLE? "double" :
			itr->second.first == ATTRTYPE_BOOL?   "bool" :
			"string")
			<< ")" << std::endl;*/
		// RACE2(TEXT("\t\t %s = %s"),itr->first,itr->second.getString());
		OutputDebugString("\t\t");
		OutputDebugString(itr->first.c_str());
		OutputDebugString("\t");
		OutputDebugString(itr->second.getString().c_str());
		OutputDebugString("\t");
		OutputDebugString((itr->second.first == ATTRTYPE_INT?    "integer" :
			itr->second.first == ATTRTYPE_DOUBLE? "double" :
			itr->second.first == ATTRTYPE_BOOL?   "bool" :
			"string"));
		OutputDebugString("\n");
	}

	//Print out the geometry
	osgEarth::Geometry* geom = feature->getGeometry();
	if (geom){
		std::cout << "\t\t" << GeometryUtils::geometryToWKT( geom ) << std::endl;
	}
	std::cout << std::endl;
}

void printAllFeatures(FeatureSource* features)
{
	osg::ref_ptr< FeatureCursor > cursor = features->createFeatureCursor(0L);
	while (cursor.valid() && cursor->hasMore())
	{
		osg::ref_ptr< Feature > feature = cursor->nextFeature();
		printFeature( feature.get() );
	}
}


void cOSG::InitSceneGraph(void)
{
	// Init the main Root Node/Group
	mRoot  = new osg::Group;

	// 构造MapNode，arguments里面有earth文件的路径
	mModel = osgDB::readNodeFile(m_ModelName);
	if (!mModel) return;

	/******************
	
	//创建节点,读取SHP文件
	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("./data/china_data/county_total.shp");

	//Open the feature source
	OGRFeatureOptions featureOpt;
	featureOpt.url() = TEXT("./data/china_data/county_total.shp");
	featureOpt.openWrite() = false; 

	osgEarth::Drivers::OGRFeatureOptions ogr_option;
	ogr_option.url() = TEXT("./data/china_data/county_total.shp");
	// osg::ref_ptr<osgEarth::Features::FeatureSource> features = osgEarth::Features::FeatureSourceFactory::create(featureOpt);


	osg::ref_ptr< FeatureSource > features = FeatureSourceFactory::create( featureOpt );
	Status s = features->open();
	if (s.isError())
		return ;
	// features->initialize();

	

	//Print out feature info
	// printStats( features.get() );


	// printAllFeatures( features.get() );
	
	******************/

	// Optimize the model
	osgUtil::Optimizer optimizer;
	optimizer.optimize(mModel.get());
	optimizer.reset();

	// Add the model to the scene
	mRoot->addChild(mModel.get());
	mapNode_ = dynamic_cast<osgEarth::MapNode*>(mModel.get());
	//mRoot->addChild(osgDB::readNodeFile("H:/002.OpenSceneGraph/019.Earth/003.第三讲-VPB用法详解与常见问题处理/vpbtest/TestCommon10/output.ive"));

	earth_label_ = new osg::Group();

	china_boundaries_ = dynamic_cast<osgEarth::ImageLayer*>(mapNode_->getMap()->getLayerByName("GlobeBoundary"));

}

void cOSG::InitCameraConfig(void)
{
	// Local Variable to hold window size data
	RECT rect;

	// osg 的场景
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

	// 将组节点设置为场景节点
	mViewer->setSceneData(mRoot.get());

	// 设置earth操作器
	// mViewer->setCameraManipulator(keyswitchManipulator.get());
	// mViewer->setCameraManipulator(new osgEarth::Util::EarthManipulator);
	mViewer->setCameraManipulator(earth_manipulator_);

	earth_manipulator_->setViewpoint(osgEarth::Viewpoint("view_point2",112.44,33.75,444.02,-15.84,-53.01,402812.75),5);

	// Realize the Viewer
	mViewer->realize();

	// Correct aspect ratio
	/*double fovy,aspectRatio,z1,z2;
	mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
	aspectRatio=double(traits->width)/double(traits->height);
	mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);*/
}

void cOSG::InitOsgEarth()
{
	////初始化天空
	/*osgEarth::Config skyConf;
	double hours = skyConf.value("hours", 12.0);
	osg::ref_ptr<osgEarth::Util::SkyNode> sky_node = new osgEarth::Util::SkyNode(mapNode->getMap());
	sky_node->setDateTime(2012, 1, 27, hours);
	sky_node->attach(mViewer, 1);
	// sky_node->setAmbientBrightness(1.0, mViewer);
	mRoot->addChild(sky_node);*/

	mapNode_->getBound();


	// 新增显示视点信息的控件
	addViewPointLable();

	// 添加机场
	addAirport();

	// 添加地标
	addEarthLabel();

	// 读取临时路径   暂时在这里调用 
	DoAPreLine();

	// CreateCompress();
}

void cOSG::PreFrameUpdate()
{
	// Due any preframe updates in this routine
	/*while(theApp.b_need_modify_){Sleep(1);}
	theApp.b_can_modify_ = false;*/
}

void cOSG::PostFrameUpdate()
{
	// Due any postframe updates in this routine
	/*if (theApp.b_need_modify_){
	theApp.b_can_modify_ = true;
	}*/
}


void cOSG::addAirport()
{
	coordinate_system_node_ = new osg::CoordinateSystemNode; // 创建坐标系节点
	coordinate_system_node_->setEllipsoidModel(new osg::EllipsoidModel()); // 设置椭圆体模型

	// 加载机场
	airport = osgDB::readNodeFile("./data/airport/heinei_airport.ive"); // 读取机场文件
	mtAirport = new osg::MatrixTransform; // 矩阵变换
	mtAirport->addChild(airport);
	mRoot->addChild(mtAirport);

	// 设置机场矩阵
	osg::Matrixd mtTemp;   // 机场位置  109.13 34.38 高度：8434.96  海拔：390
	coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 460, mtTemp);
	mtAirport->setMatrix(mtTemp);

	// 加载飞机
	osg::Matrixd::value_type plane_angle = osg::PI_4f*1.6554;  //正值： 逆时针  

	fly_airport = osgDB::readNodeFile("./data/airplane/F-16.ive"); // 读取飞机文件
	fly_airport->setName(TEXT("F16"));
	mtrix_fly_self = new osg::MatrixTransform();
	mtrix_fly_self->setMatrix(osg::Matrix::scale(10,10,10)
		* osg::Matrixd::rotate(osg::DegreesToRadians(75.0f), osg::Vec3(0,0,1))
		// * osg::Matrix::translate(osg::Vec3f(0, 0, 0))
		); // -(osg::PI_2/2*10)

	mtrix_fly_self->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL,osg::StateAttribute::ON);// 设置属性，光照法线
	mtrix_fly_self->addChild(fly_airport);

	
	// mtrix_fly_self->addChild(m_pBuildRader->BuildRader(500,300).get());
	mtrix_fly_airport = new osg::MatrixTransform;
	mtrix_fly_airport->addChild(mtrix_fly_self);

	mRoot->addChild(mtrix_fly_airport);

	BuildHistoryRoute(mtrix_fly_self, 10.0f);

	// 设置飞机矩阵
	coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.376128), osg::DegreesToRadians(109.125682), 537, mtTemp);
	mtrix_fly_airport->setMatrix(mtTemp);

}

void cOSG::rmWorldBound()
{
	if (china_boundaries_){
		mapNode_->getMap()->removeLayer(china_boundaries_);
	}

}

void cOSG::addWorldBound()
{
	if (china_boundaries_){
		mapNode_->getMap()->addLayer(china_boundaries_);
	}
}


void cOSG::addEarthLabel()
{
	// Group to hold all our annotation elements.
	osg::Group* annoGroup = new osg::Group();
	MapNode::get(mapNode_)->addChild( annoGroup );

	// Make a group for labels
	osg::Group* labelGroup = new osg::Group();
	annoGroup->addChild( labelGroup );

	osg::Group* editGroup = new osg::Group();
	MapNode::get(mapNode_)->addChild( editGroup );

	// Style our labels:
	Style labelStyle;
	labelStyle.getOrCreate<TextSymbol>()->alignment() = TextSymbol::ALIGN_CENTER_CENTER;
	labelStyle.getOrCreate<TextSymbol>()->fill()->color() = Color::Yellow;

	// A lat/long SRS for specifying points.
	const SpatialReference* geoSRS = mapNode_->getMapSRS()->getGeographicSRS();

	//--------------------------------------------------------------------

	// A series of place nodes (an icon with a text label)
	{
		osgEarth::Style pm;
		pm.getOrCreate<IconSymbol>()->url()->setLiteral( "../data/placemark32.png" );
		pm.getOrCreate<IconSymbol>()->declutter() = true;
		pm.getOrCreate<TextSymbol>()->halo() = Color("#5f5f5f");

		// bunch of pins:
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 116.3979471 ,   39.9081726) ,  "BeiJing"     , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 121.4692688 ,   31.2381763) ,  "ShangHai"    , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 117.2523808 ,   39.1038561) ,  "TianJin"     , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 106.548425  ,   29.5549144) ,  "ChongQing"   , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 114.4897766 ,   38.0451279) ,  "HeiBei"      , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 112.5223053 ,   37.8357424) ,  "ShanXi"      , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 123.4116821 ,   41.7966156) ,  "LiaoNing"    , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 125.3154297 ,   43.8925629) ,  "JiLin"       , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 126.6433411 ,   45.7414932) ,  "HeiLongJang" , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 120.1592484 ,   30.265995 ) ,  "ZheJiang"    , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 119.2978134 ,   26.0785904) ,  "FuJian"      , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 117.0056	,   36.6670723) ,  "ShanDong"    , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 113.6500473 ,   34.7570343) ,  "HeNan"       , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 114.2919388 ,   30.5675144) ,  "HuBei"       , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 112.9812698 ,   28.2008247) ,  "HuNan"       , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 113.2614288 ,   23.1189117) ,  "Guangdong"   , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 110.3465118 ,   20.0317936) ,  "HaiNan"      , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 104.0817566 ,   30.6610565) ,  "SiChuan"     , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 106.7113724 ,   26.5768738) ,  "GuiZhou"     , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 102.704567  ,   25.0438442) ,  "YunNan"      , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 115.8999176 ,   28.6759911) ,  "JiangXi"     , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 108.949028  ,   34.2616844) ,  "ShanXi"      , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 101.7874527 ,   36.6094475) ,  "QingHai"     , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 103.7500534 ,   36.0680389) ,  "GanSu"       , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 108.3117676 ,   22.8065434) ,  "GuangXi"     , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 87.6061172  ,   43.7909393) ,  "XinJiang"    , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 111.6632996 ,   40.8209419) ,  "NeiMengGu"   , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 91.1320496  ,   29.657589 ) ,  "XiZang"      , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 106.2719421 ,   38.4680099) ,  "NingXia"     , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 120.9581316 ,   23.8516062) ,  "TaiWan"      , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 114.139452  ,   22.391577 ) ,  "XiangGang"   , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 113.5678411 ,   22.167654 ) ,  "AoMen"       , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 117.2757034 ,   31.8632545) ,  "AnHui"       , pm)); 
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 118.7727814 ,   32.0476151) ,  "JangSu"      , pm)); 


		// test with an LOD:
		osg::LOD* lod = new osg::LOD();
		lod->addChild( new PlaceNode(GeoPoint(geoSRS, 14.68, 50.0), "Prague", pm), 0.0, 2e6);
		labelGroup->addChild( lod );

		// absolute altitude:
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, -87.65, 41.90, 1000, ALTMODE_ABSOLUTE), "Chicago", pm));
	}

	//--------------------------------------------------------------------

	// a box that follows lines of latitude (rhumb line interpolation, the default)
	// and flashes on and off using a cull callback.
	{
		struct C : public osg::NodeCallback {
			void operator()(osg::Node* n, osg::NodeVisitor* nv) {
				static int i=0;
				i++;
				if (i % 100 < 50)
					traverse(n, nv);
			}
		};

		osgEarth::Geometry* geom = new osgEarth::Polygon();
		geom->push_back( osg::Vec3d(0,   40, 0) );
		geom->push_back( osg::Vec3d(-60, 40, 0) );
		geom->push_back( osg::Vec3d(-60, 60, 0) );
		geom->push_back( osg::Vec3d(0,   60, 0) );

		Feature* feature = new Feature(geom, geoSRS);
		feature->geoInterp() = GEOINTERP_RHUMB_LINE;

		Style geomStyle;
		geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Cyan;
		geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 5.0f;
		geomStyle.getOrCreate<LineSymbol>()->tessellationSize() = 75000;
		geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;

		FeatureNode* fnode = new FeatureNode(feature, geomStyle);

		fnode->addCullCallback(new C());

		annoGroup->addChild( fnode );

		LabelNode* label = new LabelNode("Rhumb line polygon", labelStyle);
		label->setPosition(GeoPoint(geoSRS, -30, 50));
		labelGroup->addChild(label);
	}

	//--------------------------------------------------------------------

	// another rhumb box that crosses the antimeridian
	{
		osgEarth::Geometry* geom = new osgEarth::Polygon();
		geom->push_back( -160., -30. );
		geom->push_back(  150., -20. );
		geom->push_back(  160., -45. );
		geom->push_back( -150., -40. );
		Style geomStyle;

		Feature* feature = new Feature(geom, geoSRS);
		feature->geoInterp() = GEOINTERP_RHUMB_LINE;

		geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Lime;
		geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 3.0f;
		geomStyle.getOrCreate<LineSymbol>()->tessellationSize() = 75000;
		geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;

		FeatureNode* gnode = new FeatureNode(feature, geomStyle);
		annoGroup->addChild( gnode );

		LabelNode* label = new LabelNode("Antimeridian polygon", labelStyle);
		label->setPosition(GeoPoint(geoSRS, -175, -35));
		labelGroup->addChild(label);
	}

	//--------------------------------------------------------------------



	// A path using great-circle interpolation.
	// Keep a pointer to it so we can modify it later on.
	FeatureNode* pathNode = 0;
	{
		osgEarth::Geometry* path = new osgEarth::LineString();
		path->push_back( osg::Vec3d(-74, 40.714, 0) );   // New York
		path->push_back( osg::Vec3d(139.75, 35.68, 0) ); // Tokyo

		Feature* pathFeature = new Feature(path, geoSRS);
		pathFeature->geoInterp() = GEOINTERP_GREAT_CIRCLE;

		Style pathStyle;
		pathStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::White;
		pathStyle.getOrCreate<LineSymbol>()->stroke()->width() = 1.0f;
		pathStyle.getOrCreate<LineSymbol>()->stroke()->smooth() = true;
		pathStyle.getOrCreate<LineSymbol>()->tessellationSize() = 75000;
		pathStyle.getOrCreate<PointSymbol>()->size() = 8;
		pathStyle.getOrCreate<PointSymbol>()->fill()->color() = Color::Red;
		pathStyle.getOrCreate<PointSymbol>()->smooth() = true;
		pathStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		pathStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;
		pathStyle.getOrCreate<RenderSymbol>()->depthOffset()->enabled() = true;

		//OE_INFO << "Path extent = " << pathFeature->getExtent().toString() << std::endl;

		pathNode = new FeatureNode(pathFeature, pathStyle);
		annoGroup->addChild( pathNode );

		LabelNode* label = new LabelNode("Great circle path", labelStyle);
		label->setPosition(GeoPoint(geoSRS,-170, 61.2));
		labelGroup->addChild(label);
	}

	//--------------------------------------------------------------------

	// Two circle segments around New Orleans.
	{
		Style circleStyle;
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Cyan, 0.5);
		circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

		CircleNode* circle = new CircleNode();
		circle->set(
			GeoPoint(geoSRS, -90.25, 29.98, 1000., ALTMODE_RELATIVE),
			Distance(300, Units::KILOMETERS),
			circleStyle, 
			Angle(-45.0, Units::DEGREES),
			Angle(45.0, Units::DEGREES),
			true);

		annoGroup->addChild( circle );

		editGroup->addChild( new CircleNodeEditor(circle) );
	}

	{
		Style circleStyle;
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Red, 0.5);
		circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

		CircleNode* circle = new CircleNode();
		circle->set(
			GeoPoint(geoSRS, -90.25, 29.98, 1000., ALTMODE_RELATIVE),
			Distance(300, Units::KILOMETERS),
			circleStyle,
			Angle(45.0, Units::DEGREES),
			Angle(360.0 - 45.0, Units::DEGREES),
			true);

		annoGroup->addChild( circle );

		editGroup->addChild( new CircleNodeEditor(circle) );
	}

	//--------------------------------------------------------------------

	// An extruded ellipse around Miami.
	{
		Style ellipseStyle;
		ellipseStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Orange, 0.75);
		ellipseStyle.getOrCreate<ExtrusionSymbol>()->height() = 250000.0; // meters MSL
		EllipseNode* ellipse = new EllipseNode();
		ellipse->set(
			GeoPoint(geoSRS, -80.28, 25.82, 0.0, ALTMODE_RELATIVE),
			Distance(250, Units::MILES),
			Distance(100, Units::MILES),
			Angle   (0, Units::DEGREES),
			ellipseStyle,
			Angle(45.0, Units::DEGREES),
			Angle(360.0 - 45.0, Units::DEGREES), 
			true);
		annoGroup->addChild( ellipse );

		editGroup->addChild( new EllipseNodeEditor(ellipse) );
	}
	{
		Style ellipseStyle;
		ellipseStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Blue, 0.75);
		ellipseStyle.getOrCreate<ExtrusionSymbol>()->height() = 250000.0; // meters MSL
		EllipseNode* ellipse = new EllipseNode();
		ellipse->set(
			GeoPoint(geoSRS, -80.28, 25.82, 0.0, ALTMODE_RELATIVE),
			Distance(250, Units::MILES),
			Distance(100, Units::MILES),
			Angle   (0, Units::DEGREES),
			ellipseStyle, 
			Angle(-40.0, Units::DEGREES), 
			Angle(40.0, Units::DEGREES), 
			true);
		annoGroup->addChild( ellipse );

		editGroup->addChild( new EllipseNodeEditor(ellipse) );
	}

	//--------------------------------------------------------------------

	{
		// A rectangle around San Diego
		Style rectStyle;
		rectStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Green, 0.5);
		rectStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		rectStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;
		RectangleNode* rect = new RectangleNode(
			GeoPoint(geoSRS, -117.172, 32.721),
			Distance(300, Units::KILOMETERS ),
			Distance(600, Units::KILOMETERS ),
			rectStyle);
		annoGroup->addChild( rect );

		editGroup->addChild( new RectangleNodeEditor(rect) );
	}    

	//--------------------------------------------------------------------

	// An extruded polygon roughly the shape of Utah. Here we demonstrate the
	// FeatureNode, where you create a geographic geometry and use it as an
	// annotation.
	{
		osgEarth::Geometry* utah = new osgEarth::Polygon();
		utah->push_back( -114.052, 37.0   );
		utah->push_back( -109.054, 37.0   );
		utah->push_back( -109.054, 41.0   );
		utah->push_back( -111.040, 41.0   );
		utah->push_back( -111.080, 42.059 );
		utah->push_back( -114.080, 42.024 );

		Style utahStyle;
		utahStyle.getOrCreate<ExtrusionSymbol>()->height() = 250000.0; // meters MSL
		utahStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::White, 0.8);

		Feature*     utahFeature = new Feature(utah, geoSRS);
		FeatureNode* featureNode = new FeatureNode(utahFeature, utahStyle);

		annoGroup->addChild( featureNode );
	}

	//--------------------------------------------------------------------

	// an image overlay.
	{
		ImageOverlay* imageOverlay = 0L;
		osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile( "../data/USFLAG.TGA" );
		if (image.valid())
		{
			imageOverlay = new ImageOverlay(mapNode_, image.get());
			imageOverlay->setBounds( Bounds( -100.0, 35.0, -90.0, 40.0) );
			annoGroup->addChild( imageOverlay );

			editGroup->addChild( new ImageOverlayEditor(imageOverlay) );
		}
	}

	//--------------------------------------------------------------------

	// a model node with auto scaling.
	{
		Style style;
		style.getOrCreate<ModelSymbol>()->autoScale() = true;
		style.getOrCreate<ModelSymbol>()->url()->setLiteral("../data/red_flag.osg.50.scale");
		ModelNode* modelNode = new ModelNode(mapNode_, style); 
		modelNode->setPosition(GeoPoint(geoSRS, -100, 52));
		annoGroup->addChild(modelNode);
	}

	//--------------------------------------------------------------------

	// mRoot->addChild(earth_label_);
}


void cOSG::addViewPointLable()
{
	if (mViewer==nullptr){
		TRACE(TEXT("mViewer is null, at addViewPointLable!"));
		return ;
	}
	canvas_ = ControlCanvas::getOrCreate(mViewer);

	// 添加控件，用来显示视点信息
	osgEarth::Util::Controls::LabelControl* viewCoords = new osgEarth::Util::Controls::LabelControl(TEXT("TestViewPoint"),osg::Vec4(1.0,1.0,1.0,1.0));
	/*viewCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	viewCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	viewCoords->setBackColor(0,0,0,0.5);
	viewCoords->setSize(800,50);
	viewCoords->setMargin(10);*/
	canvas_->addControl(viewCoords);

	// 添加控件，用来显示鼠标信息
	/*osgEarth::Util::Controls::LabelControl* mouseCoords = new osgEarth::Util::Controls::LabelControl(TEXT("TestViewPoint"),osg::Vec4(1.0,1.0,1.0,1.0));
	mouseCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouseCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
	mouseCoords->setBackColor(0,0,0,0.5);
	mouseCoords->setSize(400,50);
	mouseCoords->setMargin(10);
	canvas_->addControl(mouseCoords);*/




	if (label_event_ == 0){
		label_event_ = new CLabelControlEventHandler(mapNode_,viewCoords);
	}
	mViewer->addEventHandler(label_event_);
}

void cOSG::CreateCompress()
{
	osg::ref_ptr<Compass> compass = new Compass();
	compass->setProjectionMatrix(osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0));
	compass->setPlate(CreateCompressPart("d:/data/image/compass_plate.jpeg", 1.5f, -1.0f)); //圆盘图片
	// compass->setNeedle(CreateCompressPart("d:/data/image/needle.png", 1.5f, 0.0f));//指针图片
	compass->setWidthHeight(100,100,100,100); //起始点、宽高
	compass->setMainCamera(mViewer->getCamera());

	compass->setRenderOrder(osg::Camera::POST_RENDER);
	compass->setClearMask(GL_DEPTH_BUFFER_BIT);
	compass->setAllowEventFocus(false);
	compass->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	compass->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	compass->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	mRoot->addChild(compass); //加入跟节点


}

osg::MatrixTransform* cOSG::CreateCompressPart(const std::string &image, float radius, float height)
{
	osg::Vec3 center(-radius, -radius, height);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(createTexturedQuadGeometry(center, osg::Vec3(radius*2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, radius*2.0f, 0.0f)));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(image));

	osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
	part->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	part->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	part->addChild(geode.get());
	return part.release();
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

// 两点间的距离公式 
double cOSG::GetDis(osg::Vec3 form, osg::Vec3 to)
{
	return sqrt(pow((to.x() - form.x()), 2) + pow((to.y() - form.y()), 2) + pow((to.z() - form.z()), 2));
}

// 求出下一点的时间
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
	if 0代码为直接加入经纬高、速度，可能导致飞机姿态不准确。
	else代码为在0的经纬高基础上每个节点多生成了20个节点，导致无人机更加逼真靠近。
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
	FlyNodeList.push_back(struct FlyNodeData(109.124955, 34.376215, 550, 50));
	FlyNodeList.push_back(struct FlyNodeData(109.091485, 34.384483, 550, 100));
	FlyNodeList.push_back(struct FlyNodeData(109.072182, 34.389523, 700, 100));
	FlyNodeList.push_back(struct FlyNodeData(109.056278, 34.384264, 1300, 100));
	FlyNodeList.push_back(struct FlyNodeData(109.058372, 34.439908, 2000, 100));
	FlyNodeList.push_back(struct FlyNodeData(109.117206, 34.437226, 3000, 100));
	FlyNodeList.push_back(struct FlyNodeData(109.126324, 34.376233, 530, 50));

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
	// 根据输入的控制点，输出一个路径
	apc_ = createAirLinePath(vaTemp);

#endif
}

// 根据输入的控制点，输出一个路径，控制点格式（经度，纬度，高度，速度）
osg::ref_ptr<osg::AnimationPath> cOSG::createAirLinePath(osg::Vec4Array * ctrl) // 控制点
{
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;		// 动画路径
	animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);						// 动画不循环

	double time = 0;
	osg::Vec3d curPosition,curNextPosition;

	osg::Matrix matrix;
	osg::Quat _rotation;

	double hAngle = 0.0,vAngle = 0.0;
	for (osg::Vec4Array::iterator iterator = ctrl->begin(); iterator != ctrl->end(); ++iterator){
		osg::Vec4Array::iterator iterator2 = iterator;
		++iterator2;

		// 需要判断是不是已经到顶
		if (iterator2 == ctrl->end()) { break; }

		// 计算当前点和当前的下一点位置:由经纬高转为xyz
		coordinate_system_node_->getEllipsoidModel()->convertLatLongHeightToXYZ
			(
			osg::DegreesToRadians(iterator->y()),
			osg::DegreesToRadians(iterator->x()),
			// osg::DegreesToRadians(iterator->z()),
			iterator->z(),
			curPosition.x(), curPosition.y(), curPosition.z()
			);		
		coordinate_system_node_->getEllipsoidModel()->convertLatLongHeightToXYZ
			(
			osg::DegreesToRadians(iterator2->y()),
			osg::DegreesToRadians(iterator2->x()),
			// osg::DegreesToRadians(iterator2->z()),
			iterator2->z(),
			curNextPosition.x(), curNextPosition.y(), curNextPosition.z()
			);

		// 计算垂直夹角
		if (iterator->z() == iterator2->z()){
			vAngle = 0;
		}else{
			if (sqrt(pow(GetDis(curPosition, curNextPosition), 2)) - pow(iterator2->z() - iterator->z(), 2) == 0){
				vAngle = osg::PI_2;
			}else{
				vAngle = atan((iterator2->z() - iterator->z()) / sqrt(pow(GetDis(curPosition, curNextPosition), 2)) - pow((iterator2->z() - iterator->z()), 2));
			}

			if (vAngle >= osg::PI_2)
				vAngle = osg::PI_2;
			if (vAngle <= -osg::PI_2){
				vAngle = -osg::PI_2;
			}
		}

		// 计算水平夹角
		if (iterator->x() == iterator2->x()){
			hAngle = osg::PI_2;
		}else{
			hAngle = (iterator2->y() - iterator->y()) / (iterator2->x() - iterator->x());

			if (iterator2->x() > iterator->x())
				hAngle += osg::PI;
		}

		//求飞机的变换矩阵  vAngle + osg::PI_2
		coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(iterator->y()), osg::DegreesToRadians(iterator->x()), iterator->z(), matrix);
		_rotation.makeRotate(0, osg::Vec3(1.0, 0.0, 0.0), 0, osg::Vec3(0.0, 1.0, 0.0), hAngle, osg::Vec3(0.0, 0.0, 1.0));
		matrix.preMultRotate(_rotation);
		animationPath->insert(time, osg::AnimationPath::ControlPoint(curPosition, matrix.getRotate()));

		//把下一个点的时间求出来
		time += GetRunTime(curPosition, curNextPosition, iterator2->w());
	}

	animationPath->insert(time, osg::AnimationPath::ControlPoint(curNextPosition, matrix.getRotate()));
	return animationPath.release();
}

void cOSG::DoPreLineNow()
{
	/*
	focalPoint	焦点：相机的焦点位置，是一个地理坐标（含高程）
	range	焦距：相机位置到焦点的距离，单位是米。
	pitch	俯仰角：-90至0的值，单位是度
	heading	水平方位角：0-360的值，控制地图水平旋转，单位是度。
	*/
	mtrix_fly_airport->setUpdateCallback(new osg::AnimationPathCallback(apc_,0.0,1.0));

	// em_->setNode(mtrix_fly_airport);
	// em_->setTetherNode(mtrix_fly_airport);
	// osgEarth2.10中用setNode替代setTetherNode设置视点跟踪
	// em_->setNode(mtrix_fly_airport);

	//下面是区别
	//获取当前操作器的视点，将模型放进这个视角中，然后设置这个视角的一些参数，比如从哪个角度和距离观察模型。然后将这个视点设置为操作器的视点。
	osgEarth::Viewpoint vp = earth_manipulator_->getViewpoint();
	vp.setNode(mtrix_fly_airport);
	// vp.name()._set("view_point5");
	vp.range()->set(3000.0, osgEarth::Units::METERS);//观察的距离
	vp.pitch()->set(-30.0f, osgEarth::Units::DEGREES);//观察的角度
	

	// 加载尾迹
	// BuildTail(osg::Vec3(0,0,0),mtrix_fly_self);
	// BuildRibbon(512, mtrix_fly_self);
	// osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 460, mtTemp);
	// 文件名 经度  纬度 高度 水平方位角 垂直俯仰角 可视范围
	// earth_manipulator_->setViewpoint(osgEarth::Viewpoint("view_point5", 109.126324, 34.376233, 4000, -60, -90, 1000),1);

	earth_manipulator_->setViewpoint(vp, 1.0);
}

// 设置跟踪
void cOSG::IsTrack(bool btrack)
{
	if (btrack){
		// 设置视⾓跟踪
		earth_manipulator_->setViewpoint(osgEarth::Viewpoint(TEXT("v6"),109.1263, 34.3762, 0, 24.261, -21.6, 3000),5);
		earth_manipulator_->getViewpoint().setNode(mtrix_fly_airport);
	}else{
		earth_manipulator_->getViewpoint().setNode(0);
	}
}

void cOSG::BuildTail(osg::Vec3 position, osg::MatrixTransform *scalar)
{
	osg::ref_ptr<osgParticle::FireEffect> fire = new osgParticle::FireEffect(position,10);
	fire->setUseLocalParticleSystem(false);		// 不使用世界坐标系统
	fire->getEmitter()->setEndless(false);		// 发射器周期
	fire->getEmitter()->setLifeTime(1);			// 设置效果生命周期为无限
	scalar->addChild(fire);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(fire->getParticleSystem());

	mRoot->addChild(geode);
}


void cOSG::BuildRibbon(int size, osg::MatrixTransform* scalar)
{
	osg::ref_ptr<osg::Geometry> rpgeom =new osg::Geometry;
	// 设置顶点
	osg::ref_ptr<osg::Vec3Array> rpvec3Vertex =new osg::Vec3Array(size);
	// 设置颜⾊
	osg::ref_ptr<osg::Vec4Array> rpvec4Color =new osg::Vec4Array(size);

	for(int i = 0 ;i < size-1 ; i += 2){
		(*rpvec3Vertex)[i]= osg::Vec3(0,0,0);
		(*rpvec3Vertex)[i+1]= osg::Vec3(0,0,0);
		float falpha = sinf(osg::PI *(float)i /(float)size);
		(*rpvec4Color)[i]= osg::Vec4(osg::Vec3(1.0,0.0,1.0), falpha );//m_vec3RibbonColor,falpha);
		(*rpvec4Color)[i+1]= osg::Vec4(osg::Vec3(1.0,0.0,1.0), falpha);//m_vec3RibbonColor,falpha);
	}
	// 场景数据动态改变
	rpgeom->setDataVariance(osg::Object::DYNAMIC);
	// 禁⽤显⽰列表，动态更新不安全
	rpgeom->setUseDisplayList(false);
	// 使⽤VBO模式
		
		
	rpgeom->setUseVertexBufferObjects(true);
	rpgeom->setVertexArray(rpvec3Vertex);
	rpgeom->setColorArray(rpvec4Color);
	rpgeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	rpgeom->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP,0,size));
	osg::ref_ptr<osg::Geode> rpgeode =new osg::Geode;
	rpgeode->addDrawable(rpgeom);
	
	//灯光、透明度
	rpgeom->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	rpgeom->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
	rpgeom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	scalar->addUpdateCallback(new CTrailerCallback(rpgeom,size,10));
	
	mRoot->addChild(rpgeode);
}

// 创建飞机历史航迹
void cOSG::BuildHistoryRoute(osg::MatrixTransform *scaler, float line_width)
{
	osg::ref_ptr<osg::Group> group = new osg::Group();

	scaler->addUpdateCallback(new CreateTrackCallback(group,scaler,line_width));

	mRoot->addChild(group);
}

void cOSG::FlyTo(double longitude,double latitude,double altitude){

	/*setViewpoint参数:
	1:视点
	1:视点名称
	2、3、4:视点经纬高度
	5:水平方位角：0-360的值，控制地图水平旋转，单位是度
	6:俯仰角:-90至0的值，单位是度
	7:焦距:相机位置到焦点的距离，单位是米
	2:飞行时间(s)
	*/
	earth_manipulator_->setViewpoint(osgEarth::Viewpoint("viewer_point3", longitude, latitude, altitude, -60, -45, 1000), 2);
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
