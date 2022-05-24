#pragma once

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
#include <string>

class cOSG
{
public:
    cOSG(HWND hWnd);
    ~cOSG();

    void InitOSG(std::string filename);
    void InitManipulators(void);
    void InitSceneGraph(void);
    void InitCameraConfig(void);
    void SetupWindow(void);
    void SetupCamera(void);
    void PreFrameUpdate(void);
    void PostFrameUpdate(void);
    void Done(bool value) { mDone = value; }
    bool Done(void) { return mDone; }
    //static void Render(void* ptr);
	// 设置机场
	void addAirport();

	void rmWorldBound();

	void addWorldBound();

	void InitOsgEarth();

    osgViewer::Viewer* getViewer() { return mViewer; }

	// 添加显示视点信息的控件
	void addViewPointLable();

public:
	void set_boundaries(double opt);
	double get_boundaries();

private:
    bool mDone;
    std::string m_ModelName;
    HWND m_hWnd;
    osgViewer::Viewer* mViewer;
    osg::ref_ptr<osg::Group> mRoot;
    osg::ref_ptr<osg::Node> mModel;
	osg::ref_ptr<osgEarth::MapNode> mapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> em;

private:
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
	osg::ref_ptr<osgEarth::ImageLayer> china_boundaries_;
	// 地表的矩阵操作
public:
	osg::ref_ptr<osg::CoordinateSystemNode> csn;
public:
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
