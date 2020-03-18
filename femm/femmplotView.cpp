// femmplotView.cpp : implementation of the CFemmplotView class
//

#include "stdafx.h"
#include "femm.h"
#include <math.h>

#include "lua.h"
#include "luadebug.h"
#include "luaconsoledlg.h"

#include "femmplotDoc.h"
#include "femmplotView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL bLinehook;
/////////////////////////////////////////////////////////////////////////////
// CFemmplotView

IMPLEMENT_DYNCREATE(CFemmplotView, CView)

BEGIN_MESSAGE_MAP(CFemmplotView, CView)
	//{{AFX_MSG_MAP(CFemmplotView)
	ON_COMMAND(ID_FP_EDIT_COPY, OnEditCopy)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FP_COPY_METAFILE, OnEditCopyAsMetafile)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFemmplotView construction/destruction

CFemmplotView::CFemmplotView()
{
	// TODO: add construction code here

}

CFemmplotView::~CFemmplotView()
{
}

BOOL CFemmplotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFemmplotView drawing

void CFemmplotView::OnDraw(CDC* pDC)
{
	CFemmplotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	RECT r;
	GetClientRect(&r);

	if (pDC->IsPrinting()!=FALSE){
		CSize sz;
		RECT cr;
		int wd=pDC->GetDeviceCaps( HORZRES );
		int hd=pDC->GetDeviceCaps( VERTRES );
		int ww=r.right-r.left;
		int hw=r.bottom-r.top;
		int x=wd/ww;
		int y=hd/hw;
		if (y<x) x=y;
		pDC->SetMapMode(MM_ISOTROPIC);
		sz.cx=ww; sz.cy=hw;
		pDC->SetWindowExt(sz);
		sz.cx=x*ww; sz.cy=x*hw;
		pDC->SetViewportExt(sz);
		pDC->GetClipBox(&cr);
		pDC->IntersectClipRect(&r);
	}

	r.bottom--;
	r.right--;
	pDC->PlayMetaFile(pDoc->hMetaPlot,&r);


	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFemmplotView printing

BOOL CFemmplotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFemmplotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFemmplotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFemmplotView diagnostics

#ifdef _DEBUG
void CFemmplotView::AssertValid() const
{
	CView::AssertValid();
}

void CFemmplotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFemmplotDoc* CFemmplotView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFemmplotDoc)));
	return (CFemmplotDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFemmplotView message handlers

void CFemmplotView::OnEditCopyAsMetafile() 
{
    // puts a metafile drawing of the screen on the clipboard
    CMetaFileDC Meta;
    RECT r;
    CRgn R;

    Meta.CreateEnhanced(NULL,NULL,NULL,NULL);
    GetClientRect(&r);
    R.CreateRectRgnIndirect(&r);
    Meta.SelectClipRgn(&R);
    OnDraw(&Meta);
    HENHMETAFILE hMeta=Meta.CloseEnhanced();

    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_ENHMETAFILE,hMeta);
    CloseClipboard();
    
}

void CFemmplotView::OnEditCopy() 
{
    // puts a bitmap of the screen on the clipboard
    RECT r;
    CDC tempDC;
    CBitmap bitmap;
    CDC *pDC=GetDC();

    GetClientRect(&r);
    tempDC.CreateCompatibleDC(pDC);
    bitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
    tempDC.SelectObject(&bitmap);

    tempDC.BitBlt(0, 0, r.right, r.bottom, pDC, 0, 0, SRCCOPY);

    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, HBITMAP(bitmap));
    CloseClipboard();
}
void CFemmplotView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// Grab pointer to the Status Bar
	CMainFrame *MFrm;
	MFrm=(CMainFrame *)GetParentFrame();
	if (MFrm==NULL) MsgBox("Null GetParentFrame");
	else StatBar=(CStatusBar *)MFrm->GetMessageBar();
	
}

void CFemmplotView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	static char statmsg[256];
	double x,y,ww,wh,pw,ph,ox,oy;
	RECT r;
	CFemmplotDoc *pDoc=GetDocument();

	if(pDoc->HasBoundingBox==FALSE) return;

	// get size of current client area and convert mouse position into
	// position on the actual drawing.
	GetClientRect(&r);
	ww=fabs((double) (r.right-r.left))+1.;
	wh=fabs((double) (r.top-r.bottom))+1.;
	pw=pDoc->bb_right-pDoc->bb_left;
	ph=pDoc->bb_top-pDoc->bb_bottom;
	ox=pDoc->bb_left;
	oy=pDoc->bb_bottom;

	x=((double) point.x)*pw/ww + ox; 
	y=((double) (r.bottom-point.y-1))*ph/wh+oy;

	sprintf(statmsg,"(x=%g,y=%g)",x,y);
	StatBar->SetPaneText(0,statmsg,TRUE);

	CView::OnMouseMove(nFlags, point);
}

void CFemmplotView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate && pActivateView==this)
	{
		CMainFrame *MFrm;
		MFrm=(CMainFrame *)GetTopLevelFrame();
		if (!MFrm->IsIconic()){
			MFrm->SetBar(0);
			MFrm->m_dlgBar.ShowWindow(SW_HIDE);
		}
	}

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);	
}

BOOL CFemmplotView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (bLinehook!=FALSE) return TRUE;
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
