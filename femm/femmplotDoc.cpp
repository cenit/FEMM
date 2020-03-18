// femmplotDoc.cpp : implementation of the CFemmplotDoc class
//

#include "stdafx.h"
#include "femm.h"

#include "femmplotDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFemmplotDoc

IMPLEMENT_DYNCREATE(CFemmplotDoc, CDocument)

BEGIN_MESSAGE_MAP(CFemmplotDoc, CDocument)
	//{{AFX_MSG_MAP(CFemmplotDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFemmplotDoc construction/destruction

CFemmplotDoc::CFemmplotDoc()
{
	// TODO: add one-time construction code here
	hMetaPlot=NULL;
	HasBoundingBox=FALSE;
}

CFemmplotDoc::~CFemmplotDoc()
{
	DeleteEnhMetaFile(hMetaPlot);
}

BOOL CFemmplotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// clear out any old metafile;
	DeleteEnhMetaFile(hMetaPlot);
	HasBoundingBox=FALSE;

	// grab a new metafile from the clipboard;
	GrabMetafile();

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFemmplotDoc serialization

void CFemmplotDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFemmplotDoc diagnostics

#ifdef _DEBUG
void CFemmplotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFemmplotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFemmplotDoc commands

BOOL CFemmplotDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// clear out any old metafile;
	DeleteEnhMetaFile(hMetaPlot);
	HasBoundingBox=FALSE;
	HENHMETAFILE hMetaPlotFile = GetEnhMetaFile(lpszPathName);
	if (hMetaPlotFile!=NULL){
		DWORD len =(DWORD) GetEnhMetaFileBits(hMetaPlotFile,NULL,NULL);
		unsigned char *buff=(unsigned char *) malloc(len);
		if(GetEnhMetaFileBits(hMetaPlotFile,len,buff)==0)
			MsgBox("MetaFile not copied");
		hMetaPlot=SetEnhMetaFileBits(len,buff);
	}
	else MsgBox("Problem opening specified file");

	DeleteEnhMetaFile(hMetaPlotFile);

	// TODO: Add your specialized creation code here
	
	return TRUE;
}

BOOL CFemmplotDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL flag=TRUE;

	HENHMETAFILE hMetaCopy=CopyEnhMetaFile(hMetaPlot,lpszPathName);
	if (hMetaCopy==NULL){
		MsgBox("Problem writing file to disk");
		flag=FALSE;
	}
	DeleteEnhMetaFile(hMetaCopy);

	return flag; // CDocument::OnSaveDocument(lpszPathName);
}

BOOL CFemmplotDoc::GrabMetafile()
{
	if (IsClipboardFormatAvailable(CF_ENHMETAFILE)==FALSE){
		hMetaPlot=NULL;
		return FALSE;
	
	}
	if (OpenClipboard(NULL)==FALSE)
		MsgBox("Cannot access the Clipboard");
	else{
		HENHMETAFILE hMetaClip = (HENHMETAFILE) GetClipboardData(CF_ENHMETAFILE);
		HGLOBAL hBound = (HGLOBAL) GetClipboardData(CF_TEXT);
		if(hMetaClip==NULL)
		{
			MsgBox("Couldn't GetClipboardData");
		}
		else{
			DWORD len =(DWORD) GetEnhMetaFileBits(hMetaClip,NULL,NULL);
			unsigned char *buff=(unsigned char *) malloc(len);
			if(GetEnhMetaFileBits(hMetaClip,len,buff)==0)
				MsgBox("MetaFile not copied");
			hMetaPlot=SetEnhMetaFileBits(len,buff);
			if(hBound!=NULL){
				sscanf((char *) hBound,"%lf %lf %lf %lf",
					&bb_left,&bb_bottom,&bb_right,&bb_top);
				HasBoundingBox=TRUE;
			}
			EmptyClipboard();
		}

		CloseClipboard();
	}
	
	return TRUE;
}

