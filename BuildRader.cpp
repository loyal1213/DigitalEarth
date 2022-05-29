#include "StdAfx.h"
#include "BuildRader.h"
#include "BuildRaderCallback.h"

CBuildRader::CBuildRader()
{

}

CBuildRader::~CBuildRader()
{

}

osg::ref_ptr<osg::Geode> CBuildRader::BuildRader(float fRadius,float fHeight )
{
	buildRaderCallback =new CBuildRaderCallback(2,fRadius,fHeight);
	osg::ref_ptr<osg::Geode> rpGeode =new osg::Geode;
	osg::ref_ptr<osg::Geometry> rpGeom =new osg::Geometry;
	osg::ref_ptr<osg::TessellationHints> rpHints =new osg::TessellationHints;
	rpHints->setDetailRatio(0.5f);
	// 顶点数组
	osg::ref_ptr<osg::Vec3Array> rpVec3Array =new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> rpVec4Array =new osg::Vec4Array;
	rpVec3Array->push_back(osg::Vec3f(0,0,0));
	rpVec3Array->push_back(osg::Vec3f(0,0,-fHeight));
	rpVec3Array->push_back(osg::Vec3f(fRadius,0,-fHeight));
	// 设置顶点
	rpGeom->setVertexArray(rpVec3Array);//
	// 设置关联⽅式三⾓形
	rpGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,rpVec3Array->size()));//
	rpVec4Array->push_back(osg::Vec4f(1,0,0,0.5));
	// 设置顶点颜⾊
	rpGeom->setColorArray(rpVec4Array);//
	// 设置关联⽅式
	rpGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);//
	rpGeom->setDataVariance(osg::Object::DYNAMIC);
	rpGeom->setUseVertexBufferObjects(true);
	rpGeom->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	// 混合⾊
	rpGeom->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
	// 透明度	
	rpGeom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	rpGeode->addDrawable(rpGeom.get());
	// 扫描更新回调函数
	//
	rpGeode->addUpdateCallback(buildRaderCallback);
	return rpGeode;
}