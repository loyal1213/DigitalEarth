#pragma once
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/MapNode>
#include <osgEarthUtil/Controls>
#include <osgEarth/MapNode>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>


class CLabelControlEventHandler : public osgGA::GUIEventHandler
{
public:
	struct Callback : public osg::Referenced
	{
		virtual void set(const GeoPoint& coords, osg::View* view, MapNode* mapNode) = 0;

		virtual void reset(osg::View* view, MapNode* mapNode) = 0;

		virtual ~Callback() { }
	};

public:
	CLabelControlEventHandler(MapNode* mapNode, osgEarth::Util::Controls::LabelControl* label = 0L, Formatter* formatter = 0L);

	virtual ~CLabelControlEventHandler() { }

	void addCallback(Callback* callback);


public:

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	MapNode*      _mapNode;
	osg::NodePath _mapNodePath;
	typedef std::vector< osg::ref_ptr<Callback> > Callbacks;
	Callbacks _callbacks;
};


class MouseCoordsLabelCallback : public CLabelCOntrolEventHandler::Callback
{
public:
	MouseCoordsLabelCallback(osgEarth::Util::Controls::LabelControl* label, Formatter* formatter = 0L);

	virtual ~MouseCoordsLabelCallback() { }

	virtual void set(const GeoPoint& coords, osg::View* view, MapNode* mapNode);
	virtual void reset(osg::View* view, MapNode* mapNode);

protected:
	osg::observer_ptr<osgEarth::Util::Controls::LabelControl> _label;
	osg::ref_ptr<Formatter>         _formatter;
};