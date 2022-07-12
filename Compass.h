#ifndef COMPASS_H
#define COMPASS_H

#include <osgEarth/MapNode>

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>

#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/ImageOverlayEditor>

#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthUtil/RTTPicker>

#include <osgViewer/Viewer>
#include <osg/DrawPixels>

class Compass : public osg::Camera
{
public:
	Compass();
	Compass( const Compass& copy, osg::CopyOp copyop=osg::CopyOp::SHALLOW_COPY );
	META_Node( osg, Compass );


	void setPlate( osg::MatrixTransform* plate ) { _plateTransform = plate; }
	osg::MatrixTransform* getPlate() { return _plateTransform.get(); }
	const osg::MatrixTransform* getPlate() const { return _plateTransform.get(); }


	void setNeedle( osg::MatrixTransform* needle ) { _needleTransform = needle; }
	osg::MatrixTransform* getNeedle() { return _needleTransform.get(); }
	const osg::MatrixTransform* getNeedle() const { return _needleTransform.get(); }


	void setMainCamera( osg::Camera* camera ) { _mainCamera = camera; }
	osg::Camera* getMainCamera() { return _mainCamera.get(); }
	const osg::Camera* getMainCamera() const { return _mainCamera.get(); }
	void setWidthHeight(int x, int y, int width, int height){ m_xx = x; m_yy = y; m_width = width; m_height = height; };
	virtual void traverse( osg::NodeVisitor& nv );

protected:
	virtual ~Compass();
	int m_width, m_height;
	int m_x, m_y, m_xx, m_yy;
	osg::ref_ptr<osg::MatrixTransform> _plateTransform;
	osg::ref_ptr<osg::MatrixTransform> _needleTransform;
	osg::observer_ptr<osg::Camera> _mainCamera;
};

#endif // COMPASS_H