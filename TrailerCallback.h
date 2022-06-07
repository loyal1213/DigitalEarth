#pragma once
#include<osgViewer/Viewer>
#include<osgEarth/MapNode>
#include<osg/AnimationPath>
#include<osgEarth/Utils>
#include<osgEarthUtil/EarthManipulator>
#include<osgParticle/FireEffect>
#include<osg/Geometry>
#include<osg/Geode>
#include<osg/ShapeDrawable>
#include<osgGA/GUIEventHandler>
#include<math.h>
#include<iostream>
#include<fstream>

// TrailerCallback 主要是飞机飞⾏彩带

class CTrailerCallback:public osg::NodeCallback
{
public:
	CTrailerCallback(osg::Geometry* ribbon,int size,int ribbonWidth);
	~CTrailerCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
private:
	osg::observer_ptr<osg::Geometry> m_opGeometryRibbon;   // 观察者指针
	int m_nsize;
	int m_nwidth;
};