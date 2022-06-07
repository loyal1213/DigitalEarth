#pragma once
#include<osgViewer/Viewer>
#include<osgEarth/MapNode>
#include<osg/AnimationPath>
#include<osgEarth/Utils>
#include<osgEarthUtil/EarthManipulator>
#include<osgParticle/FireEffect>
#include<osg/Node>
#include<osg/Geometry>
#include<osg/Geode>
#include<osg/ShapeDrawable>
#include<osgGA/GUIEventHandler>
#include<math.h>
#include<iostream>
#include<fstream>
#include<osg/LineWidth>

// ⾃定义轨道回调类    CreateTrackCallback 主要是⽤来实时绘制飞机历史轨迹
class CreateTrackCallback : public osg::NodeCallback
{
public:
	CreateTrackCallback(osg::Group* root,osg::MatrixTransform* scaler,float ribbonWidth);
	~CreateTrackCallback();

public:
	osg::ref_ptr<osg::Geode> BuildTrack(osg::Vec3 m_Vec3LatPoint,osg::Vec3 m_Vec3CurPoint);
	
	virtual void operator() (osg::Node* node,osg::NodeVisitor* nv);
	
public:
	// 上⼀帧模型位置坐标点
	osg::Vec3 m_Vec3LastPosition;
	// 当前模型位置坐标点
	osg::Vec3 m_Vec3CurPosition;
	osg::observer_ptr<osg::Geometry> m_opGeometryRibbon;
	osg::ref_ptr<osg::MatrixTransform> m_rpmtFly;
	osg::Group* m_proot;
	int m_nsize;
	int m_nwidth;
};
