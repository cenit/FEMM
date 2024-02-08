// PtProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "beladrawDoc.h"
#include "bd_PtProp.h"
#include "bd_NodeProp.h"
#include "bd_MatDlg.h"
#include "bd_BdryDlg.h"
#include "bd_CircProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bdCPtProp dialog

bdCPtProp::bdCPtProp(CWnd* pParent /*=NULL*/)
    : CDialog(bdCPtProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(bdCPtProp)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void bdCPtProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(bdCPtProp)
  DDX_Control(pDX, IDC_NAME_LIST, m_namelist);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(bdCPtProp, CDialog)
//{{AFX_MSG_MAP(bdCPtProp)
ON_BN_CLICKED(IDC_ADD_PROP, OnAddProp)
ON_BN_CLICKED(IDC_DEL_PROP, OnDelProp)
ON_BN_CLICKED(IDC_MOD_PROP, OnModProp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bdCPtProp message handlers

void bdCPtProp::OnAddProp()
{

  // Code to deal with adding a Point Property.
  if (PropType == 0) {
    CPointProp PProp;
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    bdCNodeProp zDlg;

    for (int nn = 0; nn < nodeproplist.GetSize(); nn++)
      zDlg.namelist.Add(nodeproplist[nn].PointName);
    nodeproplist.Add(PProp);

    zDlg.m_vp = PProp.V;
    zDlg.m_qp = PProp.qp;
    zDlg.m_nodename = PProp.PointName;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)nodeproplist.GetSize() - 1;
      nodeproplist[k].PointName = zDlg.m_nodename;
      nodeproplist[k].V = zDlg.m_vp;
      nodeproplist[k].qp = zDlg.m_qp;
      m_namelist.AddString(nodeproplist[k].PointName);
      m_namelist.SetCurSel(k);
    } else
      nodeproplist.RemoveAt(nodeproplist.GetSize() - 1);
  }

  if (PropType == 3) {
    CCircuit CProp;
    CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
    bdCCircProp zDlg;
    for (int nn = 0; nn < circproplist.GetSize(); nn++)
      zDlg.namelist.Add(circproplist[nn].CircName);
    circproplist.Add(CProp);

    zDlg.m_circname = CProp.CircName;
    zDlg.m_circtype = CProp.CircType;
    zDlg.m_q = CProp.q;
    zDlg.m_v = CProp.V;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)circproplist.GetSize() - 1;
      circproplist[k].CircName = zDlg.m_circname;
      circproplist[k].CircType = zDlg.m_circtype;
      circproplist[k].q = zDlg.m_q;
      circproplist[k].V = zDlg.m_v;
      m_namelist.AddString(circproplist[k].CircName);
      m_namelist.SetCurSel(k);
    } else
      circproplist.RemoveAt(circproplist.GetSize() - 1);
  }

  // Code to deal with adding properties for segments...

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    bdCBdryDlg zDlg;
    for (int nn = 0; nn < lineproplist.GetSize(); nn++)
      zDlg.namelist.Add(lineproplist[nn].BdryName);
    CBoundaryProp MProp;
    lineproplist.Add(MProp);

    zDlg.m_v = MProp.V;
    zDlg.m_qs = MProp.qs;
    zDlg.m_c0 = MProp.c0;
    zDlg.m_c1 = MProp.c1;
    zDlg.BdryFormat = MProp.BdryFormat;
    zDlg.m_BdryName = MProp.BdryName;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)lineproplist.GetSize() - 1;
      lineproplist[k].BdryName = zDlg.m_BdryName;
      lineproplist[k].BdryFormat = zDlg.BdryFormat;
      lineproplist[k].V = zDlg.m_v;
      lineproplist[k].qs = zDlg.m_qs;
      lineproplist[k].c0 = zDlg.m_c0;
      lineproplist[k].c1 = zDlg.m_c1;

      m_namelist.AddString(lineproplist[k].BdryName);
      m_namelist.SetCurSel(k);
    } else
      lineproplist.RemoveAt(lineproplist.GetSize() - 1);
  }

  // Code to deal with adding properties for blocks...
  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    bdCMatDlg zDlg;
    for (int nn = 0; nn < blockproplist.GetSize(); nn++)
      zDlg.namelist.Add(blockproplist[nn].BlockName);
    CMaterialProp MProp;
    blockproplist.Add(MProp);
    zDlg.m_ex = MProp.ex;
    zDlg.m_ey = MProp.ey;
    zDlg.m_qv = MProp.qv;
    zDlg.m_BlockName = MProp.BlockName;

    if (ProblemType == 0) {
      zDlg.m_mu1label = "x";
      zDlg.m_mu2label = "y";
    } else {
      zDlg.m_mu1label = "r";
      zDlg.m_mu2label = "z";
    }

    if (zDlg.DoModal() == IDOK) {
      int k = (int)blockproplist.GetSize() - 1;
      blockproplist[k].BlockName = zDlg.m_BlockName;
      blockproplist[k].ex = zDlg.m_ex;
      blockproplist[k].ey = zDlg.m_ey;
      blockproplist[k].qv = zDlg.m_qv;

      m_namelist.AddString(blockproplist[k].BlockName);
      m_namelist.SetCurSel(k);
    } else
      blockproplist.RemoveAt(blockproplist.GetSize() - 1);
  }
}

void bdCPtProp::OnDelProp()
{
  // TODO: Add your control notification handler code here
  if (PropType == 0) {
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    int k = m_namelist.GetCurSel();
    if ((k != CB_ERR) && (nodeproplist.GetSize() != 0)) {
      nodeproplist.RemoveAt(k);
      m_namelist.DeleteString(k);
      m_namelist.SetCurSel(0);
    }
  }

  if (PropType == 3) {
    CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
    int k = m_namelist.GetCurSel();
    if ((k != CB_ERR) && (circproplist.GetSize() != 0)) {
      circproplist.RemoveAt(k);
      m_namelist.DeleteString(k);
      m_namelist.SetCurSel(0);
    }
  }

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    int k = m_namelist.GetCurSel();
    if ((k != CB_ERR) && (lineproplist.GetSize() != 0)) {
      lineproplist.RemoveAt(k);
      m_namelist.DeleteString(k);
      m_namelist.SetCurSel(0);
    }
  }

  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    int k = m_namelist.GetCurSel();
    if ((k != CB_ERR) && (blockproplist.GetSize() != 0)) {
      blockproplist.RemoveAt(k);
      m_namelist.DeleteString(k);
      m_namelist.SetCurSel(0);
    }
  }
}

void bdCPtProp::OnModProp()
{
  // TODO: Add your control notification handler code here
  if (PropType == 0) {
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    bdCNodeProp zDlg;

    if (nodeproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_vp = nodeproplist[k].V;
    zDlg.m_qp = nodeproplist[k].qp;
    zDlg.m_nodename = nodeproplist[k].PointName;

    for (int nn = 0; nn < nodeproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(nodeproplist[nn].PointName);

    if (zDlg.DoModal() == IDOK) {
      nodeproplist[k].PointName = zDlg.m_nodename;
      nodeproplist[k].V = zDlg.m_vp;
      nodeproplist[k].qp = zDlg.m_qp;
      m_namelist.InsertString(k, zDlg.m_nodename);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 3) {
    CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;

    if (circproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    bdCCircProp zDlg;

    zDlg.m_circname = circproplist[k].CircName;
    zDlg.m_circtype = circproplist[k].CircType;
    zDlg.m_q = circproplist[k].q;
    zDlg.m_v = circproplist[k].V;
    for (int nn = 0; nn < circproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(circproplist[nn].CircName);

    if (zDlg.DoModal() == IDOK) {
      circproplist[k].CircName = zDlg.m_circname;
      circproplist[k].CircType = zDlg.m_circtype;
      circproplist[k].q = zDlg.m_q;
      circproplist[k].V = zDlg.m_v;
      m_namelist.InsertString(k, zDlg.m_circname);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    bdCBdryDlg zDlg;

    if (lineproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_v = lineproplist[k].V;
    zDlg.m_qs = lineproplist[k].qs;
    zDlg.m_c0 = lineproplist[k].c0;
    zDlg.m_c1 = lineproplist[k].c1;
    zDlg.BdryFormat = lineproplist[k].BdryFormat;
    zDlg.m_BdryName = lineproplist[k].BdryName;
    for (int nn = 0; nn < lineproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(lineproplist[nn].BdryName);

    if (zDlg.DoModal() == IDOK) {
      lineproplist[k].BdryName = zDlg.m_BdryName;
      lineproplist[k].BdryFormat = zDlg.BdryFormat;
      lineproplist[k].V = zDlg.m_v;
      lineproplist[k].qs = zDlg.m_qs;
      lineproplist[k].c0 = zDlg.m_c0;
      lineproplist[k].c1 = zDlg.m_c1;
      m_namelist.InsertString(k, zDlg.m_BdryName);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    bdCMatDlg zDlg;

    if (blockproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_ex = blockproplist[k].ex;
    zDlg.m_ey = blockproplist[k].ey;
    zDlg.m_qv = blockproplist[k].qv;
    zDlg.m_BlockName = blockproplist[k].BlockName;
    for (int nn = 0; nn < blockproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(blockproplist[nn].BlockName);

    if (ProblemType == 0) {
      zDlg.m_mu1label = "x";
      zDlg.m_mu2label = "y";
    } else {
      zDlg.m_mu1label = "r";
      zDlg.m_mu2label = "z";
    }
    if (zDlg.DoModal() == IDOK) {
      blockproplist[k].BlockName = zDlg.m_BlockName;
      blockproplist[k].ex = zDlg.m_ex;
      blockproplist[k].ey = zDlg.m_ey;
      blockproplist[k].qv = zDlg.m_qv;

      m_namelist.InsertString(k, zDlg.m_BlockName);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }
}

void bdCPtProp::OnOK()
{
  // TODO: Add extra validation here

  CDialog::OnOK();
}

BOOL bdCPtProp::OnInitDialog()
{
  int i;

  CDialog::OnInitDialog();

  // TODO: Add extra initialization here

  if (PropType == 0) {
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    for (i = 0; i < nodeproplist.GetSize(); i++)
      m_namelist.AddString(nodeproplist[i].PointName);
    if (nodeproplist.GetSize() > 0)
      m_namelist.SetCurSel(0);
  }

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    for (i = 0; i < lineproplist.GetSize(); i++)
      m_namelist.AddString(lineproplist[i].BdryName);
    if (lineproplist.GetSize() > 0)
      m_namelist.SetCurSel(0);
  }

  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    for (i = 0; i < blockproplist.GetSize(); i++)
      m_namelist.AddString(blockproplist[i].BlockName);
    if (blockproplist.GetSize() > 0)
      m_namelist.SetCurSel(0);
  }

  if (PropType == 3) {
    CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
    for (i = 0; i < circproplist.GetSize(); i++)
      m_namelist.AddString(circproplist[i].CircName);
    if (circproplist.GetSize() > 0)
      m_namelist.SetCurSel(0);
  }

  return TRUE; // return TRUE unless you set the focus to a control
               // EXCEPTION: OCX Property Pages should return FALSE
}
