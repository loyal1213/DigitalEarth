#pragma once
#include"BuildRaderCallback.h"

// BuildRader/BuildRaderCallback 主要⽤来处理雷达相关操作
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