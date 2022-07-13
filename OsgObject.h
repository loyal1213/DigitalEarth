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


	// ���û���
	void addAirport();

	void rmWorldBound();

	// ��ӹ����ߺ�ʡ����
	void addWorldBound();

	// ��ӵر�
	void addEarthLabel();

	void InitOsgEarth();

	// �����ʾ�ӵ���Ϣ�Ŀؼ�
	void addViewPointLable();

	void CreateCompress();

	osg::MatrixTransform* CreateCompressPart(const std::string & image, float radius, float height);

public:
	void set_boundaries(double opt);
	double get_boundaries();

public:
	// ����Ԥ��ֵ·��
	void DoPreLineNow();

	void IsTrack(bool btrack);

	// Build β��
	void BuildTail(osg::Vec3 position, osg::MatrixTransform *scalar);

	// ��Ӳʴ�
	void BuildRibbon(int size, osg::MatrixTransform* scalar);

	// �����ɻ���ʷ����
	void BuildHistoryRoute(osg::MatrixTransform *scaler, float line_width);

	// Ԥ��ֵ·������
	osg::ref_ptr<osg::AnimationPath> createAirLinePath(osg::Vec4Array * ctrl);

	// ����һ��Ԥ��ֵ·��
	void DoAPreLine();

	// �����ľ��빫ʽ
	double GetDis(osg::Vec3 form, osg::Vec3 to);
	
	// �����������Ҫ��ʻ��ʱ��
	double GetRunTime(osg::Vec3 from, osg::Vec3 to, double speed);
	
public:
	// ���е�ָ��λ��
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
	���е�һ��translate������ƶ�����������ԭ��λ�ã�
	�ڶ���rotate��ʾ����Ʊ��춫����ϵ��ת��
	������rotate��ʾ�������λ�ã���γ���꣩�����Լ����ӿڣ�ʹ����ƶ�ʱʼ��ӵ�й̶��ı��춫ƫת�Ƕȣ�
	���ĸ�translate��ʾ������������ӵ�λ�õľ���
	*/
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earth_manipulator_;
	CLabelControlEventHandler* label_event_;

	// �ɻ�
	osg::ref_ptr<osg::Node> fly_airport;
	osg::ref_ptr<osg::MatrixTransform> mtrix_fly_self;  // ����
	osg::ref_ptr<osg::MatrixTransform> mtrix_fly_airport;
	osg::ref_ptr<osg::AnimationPath> apc_;

	

    CBuildRader  m_pBuildRader;
private:
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> canvas_;

	// ������ͼ��
	osg::ref_ptr<osgEarth::ImageLayer> china_boundaries_;

	// �ر�
	osg::ref_ptr<osg::Group> earth_label_;

	// ���̣�ָ��
	osg::MatrixTransform* _plate;
	osg::MatrixTransform* _needle;

public:
	osg::ref_ptr<osg::CoordinateSystemNode> coordinate_system_node_;// �ر�ľ������

public:
	// ����
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



/* osg ��������  
1. osgearth_cache --seed china-simple.earth --max-level 3






*/