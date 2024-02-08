// treetestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "FemmeDoc.h"
#include "MatDlg.h"
#include "fe_libdlg.h"
#include "libfolderinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// fe_CLibDlg dialog

// CLuaConsoleDlg dialog

fe_CLibDlg::fe_CLibDlg(CWnd* pParent /*=NULL*/)
    : CResizableDialog(fe_CLibDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(fe_CLibDlg)
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  // m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void fe_CLibDlg::DoDataExchange(CDataExchange* pDX)
{
  CResizableDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(fe_CLibDlg)
  DDX_Control(pDX, IDC_MYLIST, m_mylist);
  DDX_Control(pDX, IDC_MYTREE, m_mytree);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(fe_CLibDlg, CResizableDialog)
//{{AFX_MSG_MAP(fe_CLibDlg)
ON_WM_QUERYDRAGICON()
ON_NOTIFY(TVN_BEGINDRAG, IDC_MYTREE, OnBegindragMytree)
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_NOTIFY(TVN_BEGINDRAG, IDC_MYLIST, OnBegindragMylist)
ON_NOTIFY(NM_RCLICK, IDC_MYLIST, OnRclickMylist)
ON_NOTIFY(NM_RCLICK, IDC_MYTREE, OnRclickMytree)
ON_NOTIFY(NM_DBLCLK, IDC_MYTREE, OnDblclkMytree)
ON_NOTIFY(NM_DBLCLK, IDC_MYLIST, OnDblclkMylist)
ON_NOTIFY(TVN_KEYDOWN, IDC_MYLIST, OnKeydownMylist)
ON_NOTIFY(TVN_KEYDOWN, IDC_MYTREE, OnKeydownMytree)
ON_WM_TIMER()
//}}AFX_MSG_MAP
ON_COMMAND(ID_EDIT_CUT, Zappit)
ON_COMMAND(ID_EDIT_COPY, AddNewProperty)
ON_COMMAND(ID_EDIT_PASTE, AddNewFolder)
ON_COMMAND(ID_EDIT_REPLACE, MouseModify)
ON_COMMAND(ID_EDIT_PASTE_LINK, VendorLink)
ON_COMMAND(ID_EDIT_FIND, ImportMaterials)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// fe_CLibDlg message handlers

BOOL fe_CLibDlg::OnInitDialog()
{
  CResizableDialog::OnInitDialog();

  // TODO: Add extra initialization here
  //////////////////////////////////////////////////////////////////

  // preset layout
  AddAnchor(IDOK, BOTTOM_RIGHT);
  AddAnchor(IDCANCEL, BOTTOM_RIGHT);
  AddAnchor(IDC_MYTREE, TOP_LEFT, BOTTOM_CENTER);
  AddAnchor(IDC_MYLIST, TOP_CENTER, BOTTOM_RIGHT);

  CString LibName = BinDir + "matlib.dat";
  FILE* fp;
  CMaterialProp MProp;
  CFolderProp FProp;
  char s[1024];
  int i, k;

  Ancestors.RemoveAll();
  Parent = NULL;
  FProp.FolderName = "New Folder";
  FProp.FolderURL = "";
  FProp.FolderVendor = "";

  // set up materials library tree control
  m_bIsDragging = FALSE;
  m_dragTargetTree = NULL;
  m_dragTargetList = NULL;
  m_dragItem = NULL;
  m_imageList.Create(IDB_TREE, 16, 1, RGB(255, 0, 0));
  m_mytree.SetImageList(&m_imageList, TVSIL_NORMAL);
  m_mylist.SetImageList(&m_imageList, TVSIL_NORMAL);

  LibParent = m_mytree.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_STATE,
      "Library Materials", 1, 1, TVIS_EXPANDED, TVIS_EXPANDED, -9999, NULL, NULL);
  Parent = LibParent;
  ModelParent = m_mylist.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_STATE,
      "Model Materials", 1, 1, TVIS_EXPANDED, TVIS_EXPANDED, -9999, NULL, NULL);

  // Put Model's properties into the InModel edit box;
  LibProps.RemoveAll();
  HTREEITEM hResult;
  CArray<CMaterialProp, CMaterialProp&>& blockproplist = pDoc->blockproplist;
  for (i = 0; i < blockproplist.GetSize(); i++) {
    LibProps.Add(blockproplist[i]);
    CopyBHdata(blockproplist[i], LibProps[i]);
    hResult = m_mylist.InsertItem(LibProps[i].BlockName, 2, 2, ModelParent);
    m_mylist.SetItemData(hResult, i);
  }

  // read in materials library;
  if ((fp = fopen(LibName, "rt")) == NULL)
    return TRUE;

  while (fgets(s, 1024, fp) != NULL) {

    switch (ParseLine(s, fp, MProp)) {
    case 1:

      // add a material to library
      k = (int)LibProps.GetSize();
      LibProps.Add(MProp);
      m_mytree.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT,
          MProp.BlockName, 2, 2, NULL, NULL, k, Parent, NULL);
      MProp.BHpoints = 0;

      break;

    case 2:

      // open a new folder
      if (Parent != NULL)
        Ancestors.Add(Parent);
      k = (int)FoldProps.GetSize();
      FoldProps.Add(FProp);
      Parent = m_mytree.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT,
          FProp.FolderName, 0, 1, NULL, NULL, k, Parent, NULL);

      break;

    case 3:

      // close a folder
      k = (int)Ancestors.GetUpperBound();
      if (k >= 0) {
        Parent = Ancestors[k];
        Ancestors.RemoveAt(k);
      }
      break;

    default:

      break;
    }
  }

  MProp.BHpoints = 0;

  fclose(fp);

  /*
    // tutorial example
      HTREEITEM hChapter;
      hChapter = m_mytree.InsertItem( "Chapter 1", 0, 1 );
      m_mytree.InsertItem( "What", 2, 2, hChapter );
  */

  Focus = NULL;

  return TRUE; // return TRUE  unless you set the focus to a control
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR fe_CLibDlg::OnQueryDragIcon()
{
  return (HCURSOR)m_hIcon;
}

///////////////////////////////////////////////////
//  Implementation of library-specific file I/O
///////////////////////////////////////////////////

char* fe_CLibDlg::StripKey(char* c)
{
  char* d;
  int i, k;

  k = (int)strlen(c);

  for (i = 0; i < k; i++) {
    if (c[i] == '=') {
      d = c + i + 1;
      return d;
    }
  }

  return c + k;
}

int fe_CLibDlg::ParseLine(char* s, FILE* fp, CMaterialProp& MProp)
{
  char q[1024];
  char* v;
  int i, j, k;

  if (sscanf(s, "%s", q) == EOF)
    return FALSE;

  // Library Hierarchy
  if (_strnicmp(q, "<beginfolder>", 13) == 0) {
    q[0] = NULL;
    return 2;
  }

  if (_strnicmp(q, "<foldername>", 11) == 0) {
    v = StripKey(s);
    k = (int)strlen(v);
    for (i = 0; i < k; i++)
      if (v[i] == '\"') {
        v = v + i + 1;
        i = k;
      }
    k = (int)strlen(v);
    if (k > 0)
      for (i = k - 1; i >= 0; i--) {
        if (v[i] == '\"') {
          v[i] = 0;
          i = -1;
        }
      }
    FoldProps[FoldProps.GetUpperBound()].FolderName = v;
    m_mytree.SetItemText(Parent, v);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<folderurl>", 10) == 0) {
    v = StripKey(s);
    k = (int)strlen(v);
    for (i = 0; i < k; i++)
      if (v[i] == '\"') {
        v = v + i + 1;
        i = k;
      }
    k = (int)strlen(v);
    if (k > 0)
      for (i = k - 1; i >= 0; i--) {
        if (v[i] == '\"') {
          v[i] = 0;
          i = -1;
        }
      }
    FoldProps[FoldProps.GetUpperBound()].FolderURL = v;
    q[0] = NULL;
  }

  if (_strnicmp(q, "<foldervendor>", 14) == 0) {
    v = StripKey(s);
    k = (int)strlen(v);
    for (i = 0; i < k; i++)
      if (v[i] == '\"') {
        v = v + i + 1;
        i = k;
      }
    k = (int)strlen(v);
    if (k > 0)
      for (i = k - 1; i >= 0; i--) {
        if (v[i] == '\"') {
          v[i] = 0;
          i = -1;
        }
      }
    FoldProps[FoldProps.GetUpperBound()].FolderVendor = v;
    q[0] = NULL;
  }

  // Block Properties;
  if (_strnicmp(q, "<beginblock>", 12) == 0) {
    MProp.BlockName = "New Material";
    MProp.mu_x = 1.;
    MProp.mu_y = 1.; // permeabilities, relative
    MProp.H_c = 0.; // magnetization, A/m
    MProp.Jsrc = 0.; // applied current density, MA/m^2
    MProp.Cduct = 0.; // conductivity of the material, MS/m
    MProp.Lam_d = 0.; // lamination thickness, mm
    MProp.Theta_hn = 0.; // hysteresis angle, degrees
    MProp.Theta_hx = 0.;
    MProp.Theta_hy = 0.;
    MProp.NStrands = 0;
    MProp.WireD = 0;
    MProp.LamFill = 1.; // lamination fill factor;
    MProp.LamType = 0; // type of lamination;
    MProp.BHpoints = 0;
    MProp.BHdata = NULL;

    q[0] = NULL;
  }

  // Library Hierarchy
  if (_strnicmp(q, "<endfolder>", 11) == 0) {

    q[0] = NULL;
    return 3;
  }

  if (_strnicmp(q, "<blockname>", 10) == 0) {
    v = StripKey(s);
    k = (int)strlen(v);
    for (i = 0; i < k; i++)
      if (v[i] == '\"') {
        v = v + i + 1;
        i = k;
      }
    k = (int)strlen(v);
    if (k > 0)
      for (i = k - 1; i >= 0; i--) {
        if (v[i] == '\"') {
          v[i] = 0;
          i = -1;
        }
      }
    MProp.BlockName = v;
    q[0] = NULL;
  }

  if (_strnicmp(q, "<mu_x>", 6) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.mu_x);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<mu_y>", 6) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.mu_y);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<H_c>", 5) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.H_c);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<J_re>", 6) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Jsrc.re);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<J_im>", 6) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Jsrc.im);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<sigma>", 7) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Cduct);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<phi_h>", 7) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Theta_hn);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<phi_hx>", 8) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Theta_hx);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<phi_hy>", 8) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Theta_hy);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<d_lam>", 7) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.Lam_d);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<LamFill>", 8) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.LamFill);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<LamType>", 9) == 0) {
    v = StripKey(s);
    sscanf(v, "%i", &MProp.LamType);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<NStrands>", 10) == 0) {
    v = StripKey(s);
    sscanf(v, "%i", &MProp.NStrands);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<WireD>", 7) == 0) {
    v = StripKey(s);
    sscanf(v, "%lf", &MProp.WireD);
    q[0] = NULL;
  }

  if (_strnicmp(q, "<BHPoints>", 10) == 0) {
    v = StripKey(s);
    sscanf(v, "%i", &MProp.BHpoints);
    if (MProp.BHpoints > 0) {
      MProp.BHdata = (CComplex*)calloc(MProp.BHpoints, sizeof(CComplex));
      for (j = 0; j < MProp.BHpoints; j++) {
        fgets(s, 1024, fp);
        sscanf(s, "%lf	%lf", &MProp.BHdata[j].re, &MProp.BHdata[j].im);
      }
    }
    q[0] = NULL;
  }

  if (_strnicmp(q, "<endblock>", 9) == 0) {
    q[0] = NULL;
    return TRUE;
  }

  return FALSE;
}

////////////////////////////////////////////////////////////////
//  Drag-n-Drop Implementation

void fe_CLibDlg::OnBegindragMytree(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  m_dragItem = pNMTreeView->itemNew.hItem;

  if (m_dragItem != LibParent) // can't drag top-level folder
  {
    CImageList* pDragImage;
    pDragImage = m_mytree.CreateDragImage(m_dragItem);
    m_mytree.SelectItem(m_dragItem);
    pDragImage->BeginDrag(0, CPoint(0, -14));
    pDragImage->DragEnter(this, pNMTreeView->ptDrag);
    SetCapture();
    m_bIsDragging = TRUE;
    delete pDragImage;
    SetTimer(1, 75, NULL);
    bDragFromLibrary = TRUE;
  }

  *pResult = 0;
}

void fe_CLibDlg::OnBegindragMylist(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  m_dragItem = pNMTreeView->itemNew.hItem;

  if (m_dragItem != ModelParent) {
    CImageList* pDragImage;
    pDragImage = m_mylist.CreateDragImage(m_dragItem);
    m_mylist.SelectItem(m_dragItem);
    pDragImage->BeginDrag(0, CPoint(0, -14));
    pDragImage->DragEnter(this, pNMTreeView->ptDrag);
    SetCapture();
    m_bIsDragging = TRUE;
    delete pDragImage;
    bDragFromLibrary = FALSE;
    SetTimer(1, 75, NULL);
  }

  *pResult = 0;
}

void fe_CLibDlg::OnMouseMove(UINT nFlags, CPoint point)
{
  if (m_bIsDragging != FALSE) {
    CPoint ptDrag(point);
    CPoint ptTree(point);
    CPoint ptList(point);
    UINT uHitTest = TVHT_ONITEM;

    MapWindowPoints(this, &ptDrag, 1);
    MapWindowPoints(&m_mytree, &ptTree, 1);
    MapWindowPoints(&m_mylist, &ptList, 1);

    CImageList::DragMove(ptDrag);
    m_dragTargetTree = m_mytree.HitTest(ptTree, &uHitTest);
    m_dragTargetList = m_mylist.HitTest(ptList, &uHitTest);
  }

  CResizableDialog::OnMouseMove(nFlags, point);
}

void fe_CLibDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (m_bIsDragging != FALSE) {
    KillTimer(1);
    CImageList::DragLeave(&m_mytree);
    CImageList::EndDrag();
    ReleaseCapture();
    m_bIsDragging = FALSE;

    // Do some logic to drop onto the main tree if the property is
    // dropped onto the tree's window, but not exactly on the list
    if ((m_dragTargetTree == NULL) && (m_dragTargetList == NULL)) {
      POINT point;
      GetCursorPos(&point);
      void* pVoid = (void*)GetDesktopWindow()->WindowFromPoint(point);
      if ((void*)&m_mytree == pVoid)
        m_dragTargetTree = LibParent;
      if ((void*)&m_mylist == pVoid)
        m_dragTargetList = ModelParent;
    }

    // Drop onto the Library tree
    if (m_dragTargetTree != NULL) {
      if (!IsTailChasing(m_dragItem, m_dragTargetTree)) {
        CopyItemToLibrary(m_dragItem, m_dragTargetTree);
        if (bDragFromLibrary) {
          m_mytree.DeleteItem(m_dragItem);
        }
      }
    }

    // Drop onto the model materials
    if (m_dragTargetList != NULL) {
      CopyItemToModel(m_dragItem, m_dragTargetList);
      if (!bDragFromLibrary)
        m_mylist.DeleteItem(m_dragItem);
    }
  } else
    CResizableDialog::OnLButtonUp(nFlags, point);
}

BOOL fe_CLibDlg::IsFolder(HTREEITEM hItem)
{
  int nImage1, nImage2, nSelImage;
  m_mytree.GetItemImage(hItem, nImage1, nSelImage);
  m_mylist.GetItemImage(hItem, nImage2, nSelImage);
  if ((nImage1 != 2) && (nImage2 != 2))
    return TRUE;
  return FALSE;
}

HTREEITEM fe_CLibDlg::CopyItemToLibrary(HTREEITEM hItem, HTREEITEM hTarget)
{
  HTREEITEM hResult, hParent, hChild;

  CString szLabel;
  int ItemData;
  if (!bDragFromLibrary) {
    szLabel = m_mylist.GetItemText(hItem);
    ItemData = (int)m_mylist.GetItemData(hItem);
  } else {
    szLabel = m_mytree.GetItemText(hItem);
    ItemData = (int)m_mytree.GetItemData(hItem);
  }

  if (IsFolder(hTarget)) {
    if (IsFolder(hItem)) {
      hResult = m_mytree.InsertItem(szLabel, 0, 1, hTarget, TVI_LAST);
      m_mytree.SetItemData(hResult, ItemData);

      hChild = m_mytree.GetChildItem(hItem);
      while (hChild != NULL) {
        CopyItemToLibrary(hChild, hResult);
        hChild = m_mytree.GetNextSiblingItem(hChild);
      }

    } else {
      if (!bDragFromLibrary) {
        CMaterialProp MProp = LibProps[ItemData];
        MProp.BHdata = NULL;
        MProp.BHpoints = NULL;
        LibProps.Add(MProp);
        int NewItemData = (int)LibProps.GetUpperBound();
        CopyBHdata(LibProps[ItemData], LibProps[NewItemData]);
        ItemData = NewItemData;
      }
      hResult = m_mytree.InsertItem(szLabel, 2, 2, hTarget, TVI_LAST);
      m_mytree.SetItemData(hResult, ItemData);
    }
  } else {
    hParent = m_mytree.GetParentItem(hTarget);

    if (IsFolder(hItem)) {
      hResult = m_mytree.InsertItem(szLabel, 0, 1, hParent, hTarget);
      m_mytree.SetItemData(hResult, ItemData);

      hChild = m_mytree.GetChildItem(hItem);
      while (hChild != NULL) {
        CopyItemToLibrary(hChild, hResult);
        hChild = m_mytree.GetNextSiblingItem(hChild);
      }
    } else {
      if (!bDragFromLibrary) {
        CMaterialProp MProp = LibProps[ItemData];
        MProp.BHdata = NULL;
        MProp.BHpoints = NULL;
        LibProps.Add(MProp);
        int NewItemData = (int)LibProps.GetUpperBound();
        CopyBHdata(LibProps[ItemData], LibProps[NewItemData]);
        ItemData = NewItemData;
      }
      hResult = m_mytree.InsertItem(szLabel, 2, 2, hParent, hTarget);
      m_mytree.SetItemData(hResult, ItemData);
    }
  }

  return hResult;
}

HTREEITEM fe_CLibDlg::CopyItemToModel(HTREEITEM hItem, HTREEITEM hTarget)
{
  HTREEITEM hResult, hParent, hChild;

  if (IsFolder(hItem)) {
    hChild = m_mytree.GetChildItem(hItem);
    hParent = hTarget;
    while (hChild != NULL) {
      hParent = CopyItemToModel(hChild, hParent);
      hChild = m_mytree.GetNextSiblingItem(hChild);
    }
    hResult = hParent;
  } else {
    CString szLabel;
    int ItemData;
    if (!bDragFromLibrary) {
      szLabel = m_mylist.GetItemText(hItem);
      ItemData = (int)m_mylist.GetItemData(hItem);
    } else {
      szLabel = m_mytree.GetItemText(hItem);
      ItemData = (int)m_mytree.GetItemData(hItem);
    }

    if (bDragFromLibrary) {
      CMaterialProp MProp = LibProps[ItemData];
      MProp.BHdata = NULL;
      MProp.BHpoints = NULL;
      LibProps.Add(MProp);
      int NewItemData = (int)LibProps.GetUpperBound();
      CopyBHdata(LibProps[ItemData], LibProps[NewItemData]);
      ItemData = NewItemData;
    }

    hParent = m_mylist.GetParentItem(hTarget);
    if (hParent != NULL)
      hResult = m_mylist.InsertItem(szLabel, 2, 2, ModelParent, hTarget);
    else
      hResult = m_mylist.InsertItem(szLabel, 2, 2, ModelParent, TVI_LAST);
    m_mylist.SetItemData(hResult, ItemData);
  }

  return hResult;
}

void fe_CLibDlg::AddNewProperty()
{
  HTREEITEM hParent, hResult;

  if (m_dragTargetList != NULL) {
    hParent = m_mylist.GetParentItem(m_dragTargetList);
    if (hParent != NULL)
      hResult = m_mylist.InsertItem("New Material", 2, 2, ModelParent, m_dragTargetList);
    else
      hResult = m_mylist.InsertItem("New Material", 2, 2, ModelParent, TVI_LAST);

    CMaterialProp MProp;
    LibProps.Add(MProp);
    m_mylist.SetItemData(hResult, LibProps.GetUpperBound());
  }

  if (m_dragTargetTree != NULL) {
    if (IsFolder(m_dragTargetTree))
      hParent = m_dragTargetTree;
    else
      hParent = m_mytree.GetParentItem(m_dragTargetTree);
    if (hParent != NULL)
      hResult = m_mytree.InsertItem("New Material", 2, 2, hParent, m_dragTargetTree);
    else
      hResult = m_mytree.InsertItem("New Material", 2, 2, LibParent, TVI_LAST);

    CMaterialProp MProp;
    LibProps.Add(MProp);
    m_mytree.SetItemData(hResult, LibProps.GetUpperBound());
  }
}

void fe_CLibDlg::AddNewFolder()
{
  HTREEITEM hParent, hResult;

  if (m_dragTargetTree != NULL) {
    if (IsFolder(m_dragTargetTree))
      hParent = m_dragTargetTree;
    else
      hParent = m_mytree.GetParentItem(m_dragTargetTree);
    if (hParent != NULL)
      hResult = m_mytree.InsertItem("New Folder", 0, 1, hParent, m_dragTargetTree);
    else
      hResult = m_mytree.InsertItem("New Folder", 0, 1, LibParent, TVI_LAST);

    CFolderProp FProp;
    FProp.FolderName = "New Folder";
    FProp.FolderURL = "";
    FProp.FolderVendor = "";
    FoldProps.Add(FProp);
    m_mytree.SetItemData(hResult, FoldProps.GetUpperBound());
  }
}

void fe_CLibDlg::Zappit()
{
  if ((m_dragTargetTree != NULL) && (m_dragTargetTree != LibParent))
    m_mytree.DeleteItem(m_dragTargetTree);
  if ((m_dragTargetList != NULL) && (m_dragTargetList != ModelParent))
    m_mylist.DeleteItem(m_dragTargetList);
}

void fe_CLibDlg::OnRclickMytree(NMHDR* pNMHDR, LRESULT* pResult)
{
  CMenu* pMenu = NULL;
  try {
    pMenu = new CMenu;
  } catch (...) {
    pMenu = NULL;
  }
  ASSERT(pMenu);

  POINT point;
  GetCursorPos(&point);
  CPoint ptTree(point);
  UINT uHitTest = TVHT_ONITEM;
  GetDesktopWindow()->MapWindowPoints(&m_mytree, &ptTree, 1);
  m_dragTargetTree = m_mytree.HitTest(ptTree, &uHitTest);
  m_dragTargetList = NULL;

  if (m_dragTargetTree == NULL)
    m_dragTargetTree = LibParent;

  VERIFY(pMenu->CreatePopupMenu());
  if (m_dragTargetTree != LibParent)
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_CUT, "Delete Selected"));
  else
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_FIND, "Import Materials"));
  VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_COPY, "Add New Material"));
  VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_PASTE, "Add New Folder"));

  if (m_dragTargetTree != NULL) {
    if (!IsFolder(m_dragTargetTree)) {
      VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_REPLACE, "Modify Material"));
      HTREEITEM hParent = m_mytree.GetParentItem(m_dragTargetTree);
      if (hParent != LibParent) {
        int k = (int)m_mytree.GetItemData(hParent);
        if (FoldProps[k].FolderVendor != "") {
          CString VendorBlurb;
          VendorBlurb = "Visit " + FoldProps[k].FolderVendor;
          VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_PASTE_LINK, VendorBlurb));
          VendorURL = FoldProps[k].FolderURL;
          VendorName = FoldProps[k].FolderVendor;
        }
      }
    } else if (m_dragTargetTree != LibParent) {
      VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_REPLACE, "Edit Folder Info"));
      int k = (int)m_mytree.GetItemData(m_dragTargetTree);
      if (FoldProps[k].FolderVendor != "") {
        CString VendorBlurb;
        VendorBlurb = "Visit " + FoldProps[k].FolderVendor;
        VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_PASTE_LINK, VendorBlurb));
        VendorURL = FoldProps[k].FolderURL;
        VendorName = FoldProps[k].FolderVendor;
      }
    }
  }

  pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);

  try {
    delete pMenu;
  } catch (...) {
    ASSERT(FALSE);
    TRACE("fe_CLibDlg::OnRclickMytree failed to destroy menu resource\n");
    pMenu = FALSE;
  }

  *pResult = 0;
}

void fe_CLibDlg::OnRclickMylist(NMHDR* pNMHDR, LRESULT* pResult)
{
  CMenu* pMenu = NULL;
  try {
    pMenu = new CMenu;
  } catch (...) {
    pMenu = NULL;
  }
  ASSERT(pMenu);

  POINT point;
  GetCursorPos(&point);
  CPoint ptList(point);
  UINT uHitTest = TVHT_ONITEM;
  GetDesktopWindow()->MapWindowPoints(&m_mylist, &ptList, 1);
  m_dragTargetList = m_mylist.HitTest(ptList, &uHitTest);
  m_dragTargetTree = NULL;

  if (m_dragTargetList == NULL)
    m_dragTargetList = ModelParent;

  VERIFY(pMenu->CreatePopupMenu());
  if (m_dragTargetList != ModelParent)
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_CUT, "Delete Selected"));
  else
    VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_FIND, "Import Materials"));
  VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_COPY, "Add New Material"));

  if (m_dragTargetList != NULL) {
    if (!IsFolder(m_dragTargetList))
      VERIFY(pMenu->AppendMenu(MF_STRING, ID_EDIT_REPLACE, "Modify Material"));
  }

  pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);

  try {
    delete pMenu;
  } catch (...) {
    ASSERT(FALSE);
    TRACE("fe_CLibDlg::OnRclickMylist failed to destroy menu resource\n");
    pMenu = FALSE;
  }

  *pResult = 0;
}

void fe_CLibDlg::CopyBHdata(CMaterialProp& source, CMaterialProp& dest)
{
  // copy BH data from source to destination
  dest.BHpoints = source.BHpoints;
  if (source.BHpoints == 0)
    return;
  dest.BHdata = (CComplex*)calloc(source.BHpoints, sizeof(CComplex));
  for (int k = 0; k < source.BHpoints; k++)
    dest.BHdata[k] = source.BHdata[k];
}

void fe_CLibDlg::OnDblclkMylist(NMHDR* pNMHDR, LRESULT* pResult)
{
  POINT point;
  GetCursorPos(&point);
  CPoint ptList(point);
  UINT uHitTest = TVHT_ONITEM;
  GetDesktopWindow()->MapWindowPoints(&m_mylist, &ptList, 1);
  m_dragTargetList = m_mylist.HitTest(ptList, &uHitTest);
  m_dragTargetTree = NULL;

  if (m_dragTargetList != NULL) {
    if (!IsFolder(m_dragTargetList)) {
      int k = (int)m_mylist.GetItemData(m_dragTargetList);
      ModifyMaterial(k);
      m_mylist.SetItemText(m_dragTargetList, LibProps[k].BlockName);
    }
  }

  *pResult = 0;
}

void fe_CLibDlg::OnDblclkMytree(NMHDR* pNMHDR, LRESULT* pResult)
{
  POINT point;
  GetCursorPos(&point);
  CPoint ptList(point);
  UINT uHitTest = TVHT_ONITEM;
  GetDesktopWindow()->MapWindowPoints(&m_mytree, &ptList, 1);
  m_dragTargetTree = m_mytree.HitTest(ptList, &uHitTest);
  m_dragTargetList = NULL;

  if (m_dragTargetTree != NULL) {
    if (!IsFolder(m_dragTargetTree)) {
      int k = (int)m_mytree.GetItemData(m_dragTargetTree);
      ModifyMaterial(k);
      m_mytree.SetItemText(m_dragTargetTree, LibProps[k].BlockName);
    }
  }

  *pResult = 0;
}

void fe_CLibDlg::ModifyMaterial(int k)
{
  CMatDlg zDlg;

  zDlg.m_mu_x = LibProps[k].mu_x;
  zDlg.m_mu_y = LibProps[k].mu_y;
  zDlg.m_H_c = LibProps[k].H_c;
  zDlg.m_Jsrc = LibProps[k].Jsrc;
  zDlg.m_Cduct = LibProps[k].Cduct;
  zDlg.m_Lam_d = LibProps[k].Lam_d;
  zDlg.m_Theta_hn = LibProps[k].Theta_hn;
  zDlg.m_Theta_hx = LibProps[k].Theta_hx;
  zDlg.m_Theta_hy = LibProps[k].Theta_hy;
  zDlg.m_WireD = LibProps[k].WireD;
  zDlg.m_NStrands = LibProps[k].NStrands;
  zDlg.m_BlockName = LibProps[k].BlockName;
  zDlg.LamType = LibProps[k].LamType;
  zDlg.m_lam_fill = LibProps[k].LamFill;
  //	for(int nn=0;nn<LibProps.GetSize();nn++)
  //		if(nn!=k) zDlg.namelist.Add(LibProps[nn].BlockName);
  if (LibProps[k].BHpoints == 0)
    zDlg.m_nlflag = FALSE;
  else {
    zDlg.m_nlflag = TRUE;
    LibProps[k].BHDataToCString(zDlg.BData, zDlg.HData);
  }
  if (pDoc->ProblemType == 0) {
    zDlg.m_mu1label = "x";
    zDlg.m_mu2label = "y";
    zDlg.m_h1label = "hx";
    zDlg.m_h2label = "hy";
  } else {
    zDlg.m_mu1label = "r";
    zDlg.m_mu2label = "z";
    zDlg.m_h1label = "hr";
    zDlg.m_h2label = "hz";
  }
  if (zDlg.DoModal() == IDOK) {
    LibProps[k].BlockName = zDlg.m_BlockName;
    LibProps[k].mu_x = zDlg.m_mu_x;
    LibProps[k].mu_y = zDlg.m_mu_y;
    LibProps[k].H_c = zDlg.m_H_c;
    LibProps[k].Jsrc = zDlg.m_Jsrc;
    LibProps[k].Cduct = zDlg.m_Cduct;
    LibProps[k].Lam_d = zDlg.m_Lam_d;
    LibProps[k].Theta_hn = zDlg.m_Theta_hn;
    LibProps[k].Theta_hx = zDlg.m_Theta_hx;
    LibProps[k].Theta_hy = zDlg.m_Theta_hy;
    LibProps[k].NStrands = zDlg.m_NStrands;
    LibProps[k].WireD = zDlg.m_WireD;
    LibProps[k].LamType = zDlg.LamType;
    LibProps[k].LamFill = zDlg.m_lam_fill;
    if (zDlg.m_nlflag == FALSE) {
      zDlg.BData.Empty();
      zDlg.HData.Empty();
    }
    LibProps[k].StripBHData(zDlg.BData, zDlg.HData);
  }
}

void fe_CLibDlg::MouseModify()
{
  if (m_dragTargetList != NULL) {
    if (!IsFolder(m_dragTargetList)) {
      int k = (int)m_mylist.GetItemData(m_dragTargetList);
      ModifyMaterial(k);
      m_mylist.SetItemText(m_dragTargetList, LibProps[k].BlockName);
    }
  }

  if (m_dragTargetTree != NULL) {
    if (!IsFolder(m_dragTargetTree)) {
      int k = (int)m_mytree.GetItemData(m_dragTargetTree);
      ModifyMaterial(k);
      m_mytree.SetItemText(m_dragTargetTree, LibProps[k].BlockName);
    } else {
      CLibFolderInfo dlg;

      int k = (int)m_mytree.GetItemData(m_dragTargetTree);
      dlg.m_foldername = FoldProps[k].FolderName;
      dlg.m_folderurl = FoldProps[k].FolderURL;
      dlg.m_foldervendor = FoldProps[k].FolderVendor;
      if (dlg.DoModal() == IDOK) {
        m_mytree.SetItemText(m_dragTargetTree, dlg.m_foldername);
        FoldProps[k].FolderName = dlg.m_foldername;
        FoldProps[k].FolderURL = dlg.m_folderurl;
        FoldProps[k].FolderVendor = dlg.m_foldervendor;
      }
    }
  }
}

void fe_CLibDlg::OnOK()
{
  HTREEITEM hChild;
  CArray<CMaterialProp, CMaterialProp&>& blockproplist = pDoc->blockproplist;
  int k;

  // copy Model Materials into the model
  blockproplist.RemoveAll();
  hChild = m_mylist.GetChildItem(ModelParent);
  while (hChild != NULL) {
    k = (int)m_mylist.GetItemData(hChild);
    blockproplist.Add(LibProps[k]);
    CopyBHdata(LibProps[k], blockproplist[blockproplist.GetUpperBound()]);
    hChild = m_mylist.GetNextSiblingItem(hChild);
  }

  // write Library Materials to disk

  CString LibName = BinDir + "matlib.dat";
  FILE* fp;

  if ((fp = fopen(LibName, "wt")) == NULL)
    return;

  hChild = m_mytree.GetChildItem(LibParent);
  while (hChild != NULL) {
    CopyItemToDisk(hChild, fp);
    hChild = m_mytree.GetNextSiblingItem(hChild);
  }

  fclose(fp);

  CResizableDialog::OnOK();
}

void fe_CLibDlg::CopyItemToDisk(HTREEITEM hItem, FILE* fp)
{
  HTREEITEM hChild;
  int i = (int)m_mytree.GetItemData(hItem);

  if (IsFolder(hItem)) {
    fprintf(fp, "<BeginFolder>\n");
    fprintf(fp, "<FolderName> = \"%s\"\n", (const char*)FoldProps[i].FolderName);
    if (FoldProps[i].FolderURL != "")
      fprintf(fp, "<FolderURL> = \"%s\"\n", (const char*)FoldProps[i].FolderURL);
    if (FoldProps[i].FolderVendor != "")
      fprintf(fp, "<FolderVendor> = \"%s\"\n", (const char*)FoldProps[i].FolderVendor);

    hChild = m_mytree.GetChildItem(hItem);
    while (hChild != NULL) {
      CopyItemToDisk(hChild, fp);
      hChild = m_mytree.GetNextSiblingItem(hChild);
    }

    fprintf(fp, "<EndFolder>\n");
  } else {
    fprintf(fp, "<BeginBlock>\n");
    fprintf(fp, "<BlockName> = \"%s\"\n", (const char*)LibProps[i].BlockName);
    fprintf(fp, "<Mu_x> = %.17g\n", LibProps[i].mu_x);
    fprintf(fp, "<Mu_y> = %.17g\n", LibProps[i].mu_y);
    fprintf(fp, "<H_c> = %.17g\n", LibProps[i].H_c);
    fprintf(fp, "<H_cAngle> = %.17g\n", LibProps[i].Theta_m);
    fprintf(fp, "<J_re> = %.17g\n", LibProps[i].Jsrc.re);
    fprintf(fp, "<J_im> = %.17g\n", LibProps[i].Jsrc.im);
    fprintf(fp, "<Sigma> = %.17g\n", LibProps[i].Cduct);
    fprintf(fp, "<d_lam> = %.17g\n", LibProps[i].Lam_d);
    fprintf(fp, "<Phi_h> = %.17g\n", LibProps[i].Theta_hn);
    fprintf(fp, "<Phi_hx> = %.17g\n", LibProps[i].Theta_hx);
    fprintf(fp, "<Phi_hy> = %.17g\n", LibProps[i].Theta_hy);
    fprintf(fp, "<LamType> = %i\n", LibProps[i].LamType);
    fprintf(fp, "<LamFill> = %.17g\n", LibProps[i].LamFill);
    fprintf(fp, "<NStrands> = %i\n", LibProps[i].NStrands);
    fprintf(fp, "<WireD> = %.17g\n", LibProps[i].WireD);
    fprintf(fp, "<BHPoints> = %i\n", LibProps[i].BHpoints);
    for (int j = 0; j < LibProps[i].BHpoints; j++)
      fprintf(fp, "	%.17g	%.17g\n", LibProps[i].BHdata[j].re, LibProps[i].BHdata[j].im);
    fprintf(fp, "<EndBlock>\n\n");
  }
}

void fe_CLibDlg::VendorLink()
{
  if (VendorURL != "")
    ShellExecute(m_hWnd, "open", VendorURL, "", "", SW_SHOWMAXIMIZED);
  else
    MsgBox("No URL available for %s", VendorName);
}

void fe_CLibDlg::ImportMaterials()
{
  CFileDialog* fname_dia;
  CString SourceFile;

  fname_dia = new CFileDialog(
      TRUE,
      "fem | * ",
      NULL,
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
      "Magnetostatic Input File (*.fem) | *.fem; *.FEM | All Files (*.*) | *.*||",
      NULL);

  if (fname_dia->DoModal() == IDCANCEL) {
    delete[] fname_dia;
    return;
  }

  SourceFile = fname_dia->GetPathName();
  delete[] fname_dia;

  // Add selected properties to the library;
  FILE* fp;
  CMaterialProp MProp;
  CFolderProp FProp;
  char s[1024];
  int k;
  BOOL bToLibrary = TRUE;

  if ((fp = fopen(SourceFile, "rt")) == NULL)
    return;

  Ancestors.RemoveAll();
  if (m_dragTargetTree != NULL) {
    // importing into the library
    // put the imported materials in a new folder
    Parent = m_mytree.InsertItem("Imported Materials", 0, 1, LibParent, TVI_FIRST);
    FProp.FolderName = "Imported Materials";
    FProp.FolderURL = "";
    FProp.FolderVendor = "";
    FoldProps.Add(FProp);
    m_mytree.SetItemData(Parent, FoldProps.GetUpperBound());
  } else if (m_dragTargetList != NULL) {
    Parent = ModelParent;
    bToLibrary = FALSE;
  } else
    return;

  while (fgets(s, 1024, fp) != NULL) {
    switch (ParseLine(s, fp, MProp)) {
    case 1:

      // add a material to library
      k = (int)LibProps.GetSize();
      LibProps.Add(MProp);
      if (bToLibrary)
        m_mytree.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT,
            MProp.BlockName, 2, 2, NULL, NULL, k, Parent, NULL);
      else
        m_mylist.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT,
            MProp.BlockName, 2, 2, NULL, NULL, k, Parent, NULL);
      MProp.BHpoints = 0;

      break;

    case 2:
      if (bToLibrary) {
        // open a new folder
        if (Parent != NULL)
          Ancestors.Add(Parent);
        k = (int)FoldProps.GetSize();
        FoldProps.Add(FProp);
        Parent = m_mytree.InsertItem(TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT,
            FProp.FolderName, 0, 1, NULL, NULL, k, Parent, NULL);
      }

      break;

    case 3:
      if (bToLibrary) {
        // close a folder
        k = (int)Ancestors.GetUpperBound();
        if (k >= 0) {
          Parent = Ancestors[k];
          Ancestors.RemoveAt(k);
        }
      }
      break;

    default:

      break;
    }
  }

  MProp.BHpoints = 0;

  fclose(fp);
}

BOOL fe_CLibDlg::IsTailChasing(HTREEITEM hSource, HTREEITEM hDest)
{
  HTREEITEM hItem;

  hItem = hDest;
  while (hItem != NULL) {
    if (hSource == hItem)
      return TRUE;
    hItem = m_mytree.GetParentItem(hItem);
  }

  return FALSE;
}

void fe_CLibDlg::OnKeydownMylist(NMHDR* pNMHDR, LRESULT* pResult)
{
  TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

  if (pTVKeyDown->wVKey == VK_DELETE) {
    HTREEITEM hItem = m_mylist.GetSelectedItem();
    if ((hItem != NULL) && (hItem != ModelParent))
      m_mylist.DeleteItem(hItem);
  }

  if (pTVKeyDown->wVKey == VK_INSERT) {
    m_dragTargetList = m_mylist.GetSelectedItem();
    m_dragTargetTree = NULL;
    AddNewProperty();
  }

  *pResult = 0;
}

void fe_CLibDlg::OnKeydownMytree(NMHDR* pNMHDR, LRESULT* pResult)
{
  TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

  if (pTVKeyDown->wVKey == VK_DELETE) {
    HTREEITEM hItem = m_mytree.GetSelectedItem();
    if ((hItem != NULL) && (hItem != LibParent))
      m_mytree.DeleteItem(hItem);
  }

  if (pTVKeyDown->wVKey == VK_INSERT) {
    m_dragTargetTree = m_mytree.GetSelectedItem();
    m_dragTargetList = NULL;
    AddNewProperty();
  }

  *pResult = 0;
}

void fe_CLibDlg::OnTimer(UINT_PTR nIDEvent)
{
  POINT pt;
  GetCursorPos(&pt);
  RECT rect;

  // check if Library needs to be scrolled and do it;
  m_mytree.GetClientRect(&rect);
  m_mytree.ClientToScreen(&rect);
  if ((pt.x > rect.left) && (pt.x < rect.right)) {
    if (pt.y < rect.top + 10) {
      CImageList::DragShowNolock(FALSE);
      m_mytree.SendMessage(WM_VSCROLL, SB_LINEUP);
      CImageList::DragShowNolock(TRUE);
    } else {
      if (pt.y > rect.bottom - 10) {
        CImageList::DragShowNolock(FALSE);
        m_mytree.SendMessage(WM_VSCROLL, SB_LINEDOWN);
        CImageList::DragShowNolock(TRUE);
      }
    }
  }

  // check if Model Materials needs to be scrolled and do it;
  m_mylist.GetClientRect(&rect);
  m_mylist.ClientToScreen(&rect);
  if ((pt.x > rect.left) && (pt.x < rect.right)) {
    if (pt.y < rect.top + 10) {
      CImageList::DragShowNolock(FALSE);
      m_mylist.SendMessage(WM_VSCROLL, SB_LINEUP);
      CImageList::DragShowNolock(TRUE);
    } else {
      if (pt.y > rect.bottom - 10) {
        CImageList::DragShowNolock(FALSE);
        m_mylist.SendMessage(WM_VSCROLL, SB_LINEDOWN);
        CImageList::DragShowNolock(TRUE);
      }
    }
  }

  CResizableDialog::OnTimer(nIDEvent);
}
