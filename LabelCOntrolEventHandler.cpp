#include "StdAfx.h"
#include "LabelCOntrolEventHandler.h"
#include <osgEarth/Terrain>

CLabelControlEventHandler::CLabelControlEventHandler(osgEarth::MapNode* mapNode, osgEarth::Util::Controls::LabelControl* label):_mapNode(mapNode)
{
	_mapNodePath.push_back((osg::Node *)mapNode->getTerrainEngine());

	if (label)
	{
		label->setFont(osgText::readFontFile("simhei.ttf"));
		label->setEncoding(osgText::String::ENCODING_UTF8);
		label->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 1));
		label->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
		label->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);

		addCallback(new MouseCoordsLabelCallback(label));
	}
}

void CLabelControlEventHandler::addCallback(CLabelControlEventHandler::Callback* cb)
{
	_callbacks.push_back(cb);
}

bool CLabelControlEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getEventType() == ea.MOVE || ea.getEventType() == ea.DRAG || ea.getEventType() == ea.SCROLL)
	{
		osg::Vec3d world;
		if (_mapNode->getTerrain()->getWorldCoordsUnderMouse(aa.asView(), ea.getX(), ea.getY(), world))
		{
			osgEarth::GeoPoint map;
			map.fromWorld(_mapNode->getMapSRS(), world);

			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->set(map, aa.asView(), _mapNode);
		}
		else
		{
			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->reset(aa.asView(), _mapNode);
		}
	}

	return false;
}

//-----------------------------------------------------------------------

MouseCoordsLabelCallback::MouseCoordsLabelCallback(osgEarth::Util::Controls::LabelControl* label) :_label(label){}

void MouseCoordsLabelCallback::set(const osgEarth::GeoPoint& mapCoords, osg::View* view, osgEarth::MapNode* mapNode)
{
	if (_label.valid())
	{
		char wsrc[512];
		sprintf(wsrc, "%s:%.3f %s:%.3f %s:%.3f", "longitude",mapCoords.x(), "latitude", mapCoords.y(), "altitude",mapCoords.z());
		_label->setText(wsrc);
	}
}

void MouseCoordsLabelCallback::reset(osg::View* view, osgEarth::MapNode* mapNode)
{
	if (_label.valid())
	{
		_label->setText("");
	}
}