#pragma once
#include"BuildRaderCallback.h"
class CBuildRader
{
public:
	CBuildRader();

	~CBuildRader();

public:
	// �����״�Բ׶ͼ��
	osg::ref_ptr<osg::Geode>BuildRader(float fRadius,float fHeight);
	float m_fHeight;
	float m_fRadius;
	CBuildRaderCallback * buildRaderCallback;
};