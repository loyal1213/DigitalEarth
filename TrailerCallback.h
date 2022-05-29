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
class CTrailerCallback:public osg::NodeCallback
{
public:
	CTrailerCallback(osg::Geometry* ribbon,int size,int ribbonWidth);
	~CTrailerCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
private:
	osg::observer_ptr<osg::Geometry> m_opGeometryRibbon;
	int m_nsize;
	int m_nwidth;
};