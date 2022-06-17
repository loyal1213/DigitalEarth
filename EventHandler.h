#pragma once
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/MapNode>

class CEventHandler : public osgGA::GUIEventHandler 
{
public:
	CEventHandler(void);
	~CEventHandler(void);

public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	// ���������ģ�� ������������ɵ��ߺ�ģ���Ƿ��н���
	osg::Node* IntersectPoint(osg::Vec3 x_point, osg::Vec3 y_point, osg::ref_ptr<osg::Node> node, std::string name);
};

