#pragma once
#include"BuildRaderCallback.h"
class CBuildRader
{
public:
	CBuildRader();

	~CBuildRader();

public:
	// 创建雷达圆锥图形
	osg::ref_ptr<osg::Geode>BuildRader(float fRadius,float fHeight);
	float m_fHeight;
	float m_fRadius;
	CBuildRaderCallback * buildRaderCallback;
};