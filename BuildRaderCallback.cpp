#include "StdAfx.h"
#include"BuildRaderCallback.h"
CBuildRaderCallback::CBuildRaderCallback(float fRotateSpeed,float fRotateRadius,float fRotateHeight )
{
	m_fSpeed = fRotateSpeed;
	m_fRadius = fRotateRadius;
	m_fHeight = fRotateHeight;
}
CBuildRaderCallback::~CBuildRaderCallback()
{
}
void CBuildRaderCallback::operator()( osg::Node* node,osg::NodeVisitor* nv )
{
	osg::Geode * pGeode =dynamic_cast<osg::Geode *>(node);
	osg::ref_ptr<osg::Geometry> rpGeo =dynamic_cast<osg::Geometry*>(pGeode->getDrawable(0));
	// 获取顶点数组，设置连接⽅式
	osg::ref_ptr<osg::Vec3Array> rpVertexArray =dynamic_cast<osg::Vec3Array*>(rpGeo->getVertexArray());
	// 获取当前运⾏时间
	double dRotateTime = nv->getFrameStamp()->getReferenceTime();//
	rpVertexArray->push_back(rpVertexArray->at(0));
	rpVertexArray->push_back(rpVertexArray->at(1));
	rpVertexArray->push_back(osg::Vec3(m_fRadius *cosf(dRotateTime * m_fSpeed),m_fRadius *sinf(dRotateTime * m_fSpeed),-m_fHeight));
	rpVertexArray->erase(rpVertexArray->begin());
	rpVertexArray->erase(rpVertexArray->begin());
	rpVertexArray->erase(rpVertexArray->begin());
	rpVertexArray->dirty();
	// 更新轨迹
	traverse(node,nv);
}
