// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// DigitaleEarthView.cpp : CDigitaleEarthView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DigitaleEarth.h"
#endif

#include "DigitaleEarthDoc.h"
#include "DigitaleEarthView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitaleEarthView

IMPLEMENT_DYNCREATE(CDigitaleEarthView, CView)

BEGIN_MESSAGE_MAP(CDigitaleEarthView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_CHINA_BOUND, &CDigitaleEarthView::OnChinaBound)
	ON_COMMAND(ID_SET_BOUND, &CDigitaleEarthView::OnSetBound)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BOUND, &CDigitaleEarthView::OnUpdateShowBound)
	ON_COMMAND(ID_SHOW_BOUND, &CDigitaleEarthView::OnShowBound)
	ON_COMMAND(ID_BUTTON5, &CDigitaleEarthView::OnButton5)
	ON_COMMAND(ID_EDIT_LONGITUDE, &CDigitaleEarthView::OnEditLongitude)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LONGITUDE, &CDigitaleEarthView::OnUpdateEditLongitude)
	ON_COMMAND(ID_EDIT_LATITUDE, &CDigitaleEarthView::OnEditLatitude)
	ON_COMMAND(ID_EDIT_ALTITUDE, &CDigitaleEarthView::OnEditAltitude)
END_MESSAGE_MAP()

// CDigitaleEarthView 构造/析构

CDigitaleEarthView::CDigitaleEarthView(): mOSG(0L),isShowBound_(true),mThreadHandle(nullptr)
{
	// TODO: 在此处添加构造代码

}

CDigitaleEarthView::~CDigitaleEarthView()
{
}

BOOL CDigitaleEarthView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDigitaleEarthView 绘制

void CDigitaleEarthView::OnDraw(CDC* /*pDC*/)
{
	CDigitaleEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CDigitaleEarthView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDigitaleEarthView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifdef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDigitaleEarthView 诊断

#ifdef _DEBUG
void CDigitaleEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitaleEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitaleEarthDoc* CDigitaleEarthView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitaleEarthDoc)));
	return (CDigitaleEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitaleEarthView 消息处理程序


int CDigitaleEarthView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// Now that the window is created setup OSG
	mOSG = new cOSG(m_hWnd);

	return 1;
}


void CDigitaleEarthView::OnDestroy()
{
	// TODO: 在此处添加消息处理程序代码
	CView::OnDestroy();
	WaitForSingleObject(mThreadHandle, 1000);
	delete mThreadHandle;
	if(mOSG != 0){
		delete mOSG;
		mOSG = nullptr;
	}

}


void CDigitaleEarthView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// Close Window on Escape Key
	if(nChar == VK_ESCAPE){
		GetParent()->SendMessage(WM_CLOSE);
	}
	// CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CDigitaleEarthView::OnEraseBkgnd(CDC* pDC)
{
	if (0 == mOSG){
		return CView::OnEraseBkgnd(pDC);
	}else{
		return FALSE;
	}

	return CView::OnEraseBkgnd(pDC);
}


void CDigitaleEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	// Get Filename from DocumentOpen Dialog
	CString csFileName = GetDocument()->GetFileName();
	if (csFileName.IsEmpty()){
		csFileName = "feature_clip_plane.earth";
	}

	// Init the osg class
	mOSG->InitOSG(csFileName.GetString());

	// Start the thread to do OSG Rendering
	//mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG); 
	mThreadHandle = new CRenderingThread(mOSG);
	mThreadHandle->start();
}


void CDigitaleEarthView::OnChinaBound()
{
	// TODO: 在此添加命令处理程序代码
}


void CDigitaleEarthView::OnSetBound()
{
	// TODO: 在此添加命令处理程序代码
	CDigitaleEarthApp* p_app = (CDigitaleEarthApp*)AfxGetApp();
	CMainFrame* p_wnd = (CMainFrame*)p_app->GetMainWnd();
	CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_CHINA_BOUND));
	if (p_edit){
		CString str  = p_edit->GetEditText();
		std::string strTemp(str.GetBuffer());
		double opt = std::atof(strTemp.c_str());
		if (opt<0){
			AfxMessageBox("错误，透明度必须为正值",MB_OK,MB_ICONEXCLAMATION);
		}else{
			mOSG->set_boundaries(opt);
		}
	}
}


void CDigitaleEarthView::OnUpdateShowBound(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	/*pCmdUI->SetCheck(isShowBound_);
	if (isShowBound_){
	mOSG->addWorldBound();
	}else{
	mOSG->rmWorldBound();
	}*/
}


void CDigitaleEarthView::OnShowBound()
{
	// TODO: 在此添加命令处理程序代码
	// isShowBound_ = !isShowBound_;
}

// 飞往目的地
void CDigitaleEarthView::OnButton5()
{
	double longitude=0.0f, latitude=0.0f, altitude=0.0f;
	CDigitaleEarthApp* p_app = (CDigitaleEarthApp*)AfxGetApp();
	CMainFrame* p_wnd = (CMainFrame*)p_app->GetMainWnd();
	{  // 获取经度
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_EDIT_LONGITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			longitude = std::atof(strTemp.c_str());
			if ((longitude<-180) || (longitude>180)){
				AfxMessageBox("经度必须介于(-180,180)之间 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}

	}

	{	// 获取纬度
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_EDIT_LATITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			latitude = std::atof(strTemp.c_str());
			if ((latitude<-90) || (latitude>90)){
				AfxMessageBox("纬度必须介于（-90,90）之间 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// 获取高度
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_EDIT_ALTITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			altitude = std::atof(strTemp.c_str());
			if ((altitude<0)){
				AfxMessageBox("高度必须为正值 !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	mOSG->FlyTo(longitude,latitude,altitude);
}

void CDigitaleEarthView::OnEditLongitude()
{
	// TODO: 在此添加命令处理程序代码
}


void CDigitaleEarthView::OnUpdateEditLongitude(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDigitaleEarthView::OnEditLatitude()
{
	// TODO: 在此添加命令处理程序代码
}


void CDigitaleEarthView::OnEditAltitude()
{
	// TODO: 在此添加命令处理程序代码
}
