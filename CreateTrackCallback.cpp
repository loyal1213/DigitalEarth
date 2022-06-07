#include "StdAfx.h"
#include "CreateTrackCallback.h"
#include <osg/MatrixTransform>

CreateTrackCallback::CreateTrackCallback(osg::Group* root,osg::MatrixTransform* scaler,float lineWidth){
	m_proot = root;
	m_nwidth = lineWidth;
	m_rpmtFly = scaler;
}

CreateTrackCallback::~CreateTrackCallback(){
}

void CreateTrackCallback::operator()( osg::Node* node,osg::NodeVisitor* nv ){
	osg::MatrixTransform* pmtTrans =dynamic_cast<osg::MatrixTransform*>(node);
	//   osg::MatrixTransform* pmtTrans = new osg::MatrixTransform;
	// pmtTrans->setMatrix()
	if(pmtTrans)
	{
		osg::Matrix mtx = pmtTrans->getMatrix();
		m_Vec3CurPosition = mtx.getTrans();
		m_proot->addChild(BuildTrack(m_Vec3LastPosition,m_Vec3CurPosition));
	}
	traverse(node,nv);
	m_Vec3LastPosition = m_Vec3CurPosition;
}

osg::ref_ptr<osg::Geode> CreateTrackCallback::BuildTrack(osg::Vec3 m_Vec3LatPoint,osg::Vec3 m_Vec3CurPoint){

	osg::ref_ptr<osg::Geode> rpGeode =new osg::Geode;
	osg::ref_ptr<osg::Geometry> rpGeom =new osg::Geometry;
	osg::ref_ptr<osg::TessellationHints> rpHints =new osg::TessellationHints;

	rpHints->setDetailRatio(0.5f);

	// 顶点数组
	osg::ref_ptr<osg::Vec3Array> rpVec3Array =new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> rpVec4Array =new osg::Vec4Array;
	rpVec3Array->push_back(m_Vec3LatPoint);
	rpVec3Array->push_back(m_Vec3CurPoint);

	// 设置顶点
	rpGeom->setVertexArray(rpVec3Array);

	// 设置关联⽅式线段
	rpGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,rpVec3Array->size()));
	rpVec4Array->push_back(osg::Vec4f(1,0,0,1.0));

	// 设置顶点颜⾊
	rpGeom->setColorArray(rpVec4Array);

	// 设置关联⽅式
	rpGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	rpGeom->setDataVariance(osg::Object::DYNAMIC);
	rpGeom->setUseVertexBufferObjects(true);

	// 设置线宽
	//
	osg::ref_ptr<osg::LineWidth> lw =new osg::LineWidth(m_nwidth);
	rpGeom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);
	rpGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	// 混合⾊
	rpGeom->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);

	// 透明度
	rpGeom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	rpGeode->addDrawable(rpGeom.get());
	return rpGeode;
}