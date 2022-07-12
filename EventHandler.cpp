#include "StdAfx.h"
#include "EventHandler.h"


CEventHandler::CEventHandler(void)
{
}


CEventHandler::~CEventHandler(void)
{
}

bool CEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::Viewer *p_viewer = dynamic_cast<osgViewer::Viewer *>(&aa);
	if (NULL != p_viewer){
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (p_viewer->computeIntersections(ea.getX(),ea.getY(),intersections)){
			for (osgUtil::LineSegmentIntersector::Intersections::iterator hiter = intersections.begin();hiter!=intersections.end();hiter++){

			}
		}
		
	}
	return false;
}

osg::Node* CEventHandler::IntersectPoint(osg::Vec3 x_point, osg::Vec3 y_point, osg::ref_ptr<osg::Node> node, std::string name)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;
	osg::ref_ptr<osgUtil::LineSegmentIntersector> ls = new osgUtil::LineSegmentIntersector(x_point,y_point);
	osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor(ls);
	if (node){
		node->accept(*iv);
	}

	if (ls->containsIntersections()){

	}

	return node;
}
