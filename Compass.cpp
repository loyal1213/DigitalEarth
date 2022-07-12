#include "StdAfx.h"
#include "Compass.h"

#include "Compass.h"


Compass::Compass()
{
}

Compass::Compass( const Compass& copy, osg::CopyOp copyop )
	:   osg::Camera(copy, copyop),
	_plateTransform(copy._plateTransform),
	_needleTransform(copy._needleTransform),
	_mainCamera(copy._mainCamera)
{
}

Compass::~Compass()
{
}

void Compass::traverse( osg::NodeVisitor& nv )
{
	if ( _mainCamera.valid() && nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		osg::Matrix matrix = _mainCamera->getViewMatrix();
		matrix.setTrans( osg::Vec3() );

		osg::Vec3 northVec = osg::Z_AXIS * matrix;
		northVec.z() = 0.0f;
		northVec.normalize();

		osg::Vec3 axis = osg::Y_AXIS ^ northVec;
		float angle = atan2(axis.length(), osg::Y_AXIS*northVec);
		axis.normalize();

		if ( _plateTransform.valid() )
			_plateTransform->setMatrix( osg::Matrix::rotate(angle, axis) );

		if (m_x != _mainCamera->getViewport()->width() || _mainCamera->getViewport()->height() != m_y)
		{
			m_x = _mainCamera->getViewport()->width();
			m_y = _mainCamera->getViewport()->height();
			this->setViewport(_mainCamera->getViewport()->width()-m_width-m_xx, _mainCamera->getViewport()->height()-m_height-m_yy, m_width, m_height);
		}
	}


	_plateTransform->accept( nv );
	_needleTransform->accept( nv );
	osg::Camera::traverse( nv );
}