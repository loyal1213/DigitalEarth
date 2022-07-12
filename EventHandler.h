#pragma once
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/MapNode>

#include <osgGA/CameraManipulator>

class CEventHandler : public osgGA::GUIEventHandler 
{
public:
	CEventHandler(void);
	~CEventHandler(void);

public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	// ���������ģ�� ������������ɵ��ߺ�ģ���Ƿ��н���
	osg::Node* IntersectPoint(osg::Vec3 x_point, osg::Vec3 y_point, osg::ref_ptr<osg::Node> node, std::string name);


	/** set the position of the matrix manipulator using a 4x4 Matrix.*/
	void setByMatrix(const osg::Matrixd& matrix){}

	/** set the position of the matrix manipulator using a 4x4 Matrix.*/
	void setByInverseMatrix(const osg::Matrixd& matrix) {}

	/** get the position of the manipulator as 4x4 Matrix.*/
	osg::Matrixd getMatrix() const{
		return osg::Matrix();
	}

	/** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
	osg::Matrixd getInverseMatrix() const {
		return osg::Matrix();
	}

private:
	// ���̣�ָ��
	osg::MatrixTransform* _plate;
	osg::MatrixTransform* _needle;
};

