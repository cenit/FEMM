// femmDoc.cpp : implementation of the CFemmDoc class
//

#include "stdafx.h"
#include "femm.h"

#include "femmDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFemmDoc

IMPLEMENT_DYNCREATE(CFemmDoc, CDocument)

BEGIN_MESSAGE_MAP(CFemmDoc, CDocument)
//{{AFX_MSG_MAP(CFemmDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFemmDoc construction/destruction

CFemmDoc::CFemmDoc()
{
  // TODO: add one-time construction code here
}

CFemmDoc::~CFemmDoc()
{
}

BOOL CFemmDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFemmDoc serialization

void CFemmDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) {
    // TODO: add storing code here
  } else {
    // TODO: add loading code here
  }
}

/////////////////////////////////////////////////////////////////////////////
// CFemmDoc diagnostics

#ifdef _DEBUG
void CFemmDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CFemmDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFemmDoc commands
