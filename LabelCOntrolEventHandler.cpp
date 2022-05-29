#include "StdAfx.h"
#include "LabelCOntrolEventHandler.h"
#include <osgEarth/Terrain>
#include <osgEarthUtil/EarthManipulator>

CLabelControlEventHandler::CLabelControlEventHandler(MapNode* mapNode, osgEarth::Util::Controls::LabelControl* label, Formatter* formatter) :_mapNode(mapNode)
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

	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer *>(&aa);
	if(ea.getEventType() ==  ea.MOVE || ea.getEventType() == ea.DRAG || ea.getEventType() == ea.SCROLL){ /*ea.FRAME )*/
		osg::Vec3d world;
		if (_mapNode->getTerrain()->getWorldCoordsUnderMouse(aa.asView(), ea.getX(), ea.getY(), world)){
			osgEarth::GeoPoint map;
			map.fromWorld(_mapNode->getMapSRS(), world);

			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->set(map, aa.asView(), _mapNode);
		}else{
			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->reset(aa.asView(), _mapNode);
		}
	}

	{	
		osgEarth::Util::EarthManipulator * em = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
		osgEarth::Viewpoint view_point = em->getViewpoint();
		double fx = view_point.focalPoint().mutable_value().x();
		double fy = view_point.focalPoint().mutable_value().y();
		double fz = view_point.focalPoint().mutable_value().z();
		double heading_deg = view_point.getHeading();
		double pitch = view_point.getPitch();
		double rangle = view_point.getRange();

		if (ea.getEventType() == ea.KEYDOWN){
			if (ea.getKey() == 'X'){		// focalPoint	焦点：相机的焦点位置，是一个地理坐标（含高程）
				fx++;	
			}else if(ea.getKey() == 'x'){
				fx--;
			}else if (ea.getKey() == 'Y'){
				fy++;
			}else if (ea.getKey() == 'y'){
				fy--;
			}else if (ea.getKey() == 'Z'){
				fz++;
			}else if (ea.getKey() == 'z'){
				fz--;
			}else if (ea.getKey() == 'H'){ // heading	水平方位角：0-360的值，控制地图水平旋转，单位是度。
				heading_deg++;
			}else if (ea.getKey() == 'h'){
				heading_deg--;
			}else if (ea.getKey() == 'P'){ // pitch	俯仰角：-90至0的值，单位是度
				pitch++;
			}else if (ea.getKey() == 'p'){
				pitch--;
			}else if (ea.getKey() == 'R'){ // range	焦距：相机位置到焦点的距离，单位是米。
				rangle+=1000;
			}else if (ea.getKey() == 'r'){
				rangle-=1000;
			}
			// 112.44 33.75 444.02 -15.84 -53.01 402812.75
			em->setViewpoint(osgEarth::Viewpoint("viepoint_1",fx,fy,fz,heading_deg,pitch,rangle));
		}
	}

	return false;
}

//-----------------------------------------------------------------------

MouseCoordsLabelCallback::MouseCoordsLabelCallback(osgEarth::Util::Controls::LabelControl* label, Formatter* formatter) :_label(label),_formatter(formatter){}

void MouseCoordsLabelCallback::set(const osgEarth::GeoPoint& mapCoords, osg::View* view, osgEarth::MapNode* mapNode)
{
	if (_label.valid())
	{
		char wsrc[512];
		sprintf(wsrc, "%s:%.6f %s:%.6f %s:%.6f", "longitude",mapCoords.x(), "latitude", mapCoords.y(), "altitude",mapCoords.z());
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