#pragma once
#include <string>
#include <typeinfo>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>
#include <osgEarthUtil/Controls>
#include <osg/Notify>
#include <osg/AnimationPath>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthSymbology/Color>
#include <osgEarthAnnotation/PlaceNode>


#include "DigitaleEarth.h"
#include "LabelCOntrolEventHandler.h"
#include "BuildRader.h"
#include "EventHandler.h"
#include "BuildRaderCallback.h"
using namespace osgEarth::Symbology;
using namespace osgEarth::Util::Controls;
class cOSG
{
public:
    cOSG(HWND hWnd);
    ~cOSG();

public:
	osgViewer::Viewer* getViewer() { return mViewer; }

	void Done(bool value) { mDone = value; }

	bool Done(void) { return mDone; }

    void InitOSG(std::string filename);
	//static void Render(void* ptr);

    void InitManipulators(void);

    void InitSceneGraph(void);

    void InitCameraConfig(void);

    void SetupWindow(void);

    void SetupCamera(void);

    void PreFrameUpdate(void);

    void PostFrameUpdate(void);


	// 设置机场
	void addAirport();

	void rmWorldBound();

	// 添加国界线和省界线
	void addWorldBound();

	// 添加地标
	void addEarthLabel();

	void InitOsgEarth();

	// 添加显示视点信息的控件
	void addViewPointLable();

	void CreateCompress();

	osg::MatrixTransform* CreateCompressPart(const std::string & image, float radius, float height);

public:
	void set_boundaries(double opt);
	double get_boundaries();

public:
	// 启动预设值路径
	void DoPreLineNow();

	void IsTrack(bool btrack);

	// Build 尾迹
	void BuildTail(osg::Vec3 position, osg::MatrixTransform *scalar);

	// 添加彩带
	void BuildRibbon(int size, osg::MatrixTransform* scalar);

	// 创建飞机历史航迹
	void BuildHistoryRoute(osg::MatrixTransform *scaler, float line_width);

	// 预设值路径飞行
	osg::ref_ptr<osg::AnimationPath> createAirLinePath(osg::Vec4Array * ctrl);

	// 设置一个预设值路径
	void DoAPreLine();

	// 两点间的距离公式
	double GetDis(osg::Vec3 form, osg::Vec3 to);
	
	// 计算两点间需要行驶的时间
	double GetRunTime(osg::Vec3 from, osg::Vec3 to, double speed);
	
public:
	// 飞行到指定位置
	void FlyTo(double longitude,double latitude,double altitude);

private:
    bool mDone;
    std::string m_ModelName;
    HWND m_hWnd;
    osgViewer::Viewer* mViewer;
    osg::ref_ptr<osg::Group> mRoot;
    osg::ref_ptr<osg::Node> mModel;
	osg::ref_ptr<osgEarth::MapNode> mapNode_;

	/*
	其中第一个translate将相机移动至世界坐标原点位置；
	第二个rotate表示相机绕北天东坐标系旋转；
	第三个rotate表示相机根据位置（经纬坐标）调整自己的视口，使相机移动时始终拥有固定的北天东偏转角度；
	第四个translate表示相机从球心至视点位置的矩阵；
	*/
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earth_manipulator_;
	CLabelControlEventHandler* label_event_;

	// 飞机
	osg::ref_ptr<osg::Node> fly_airport;
	osg::ref_ptr<osg::MatrixTransform> mtrix_fly_self;  // 矩阵
	osg::ref_ptr<osg::MatrixTransform> mtrix_fly_airport;
	osg::ref_ptr<osg::AnimationPath> apc_;

	

    CBuildRader  m_pBuildRader;
private:
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> canvas_;

	// 国界线图层
	osg::ref_ptr<osgEarth::ImageLayer> china_boundaries_;

	// 地标
	osg::ref_ptr<osg::Group> earth_label_;

	// 表盘，指针
	osg::MatrixTransform* _plate;
	osg::MatrixTransform* _needle;

public:
	osg::ref_ptr<osg::CoordinateSystemNode> coordinate_system_node_;// 地表的矩阵操作

public:
	// 机场
	osg::ref_ptr<osg::Node> airport;
	osg::ref_ptr<osg::MatrixTransform> mtAirport;
};

class CRenderingThread : public OpenThreads::Thread
{
public:
    CRenderingThread( cOSG* ptr );
    virtual ~CRenderingThread();

    virtual void run();

protected:
    cOSG* _ptr;
    bool _done;
};



/* osg 常用命令  
1. osgearth_cache --seed china-simple.earth --max-level 3






*/