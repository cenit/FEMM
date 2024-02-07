// PtProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "hdrawDoc.h"
#include "hd_PtProp.h"
#include "hd_NodeProp.h"
#include "hd_MatDlg.h"
#include "hd_BdryDlg.h"
#include "hd_CircProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// hdCPtProp dialog

hdCPtProp::hdCPtProp(CWnd* pParent /*=NULL*/)
    : CDialog(hdCPtProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(hdCPtProp)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void hdCPtProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(hdCPtProp)
  DDX_Control(pDX, IDC_NAME_LIST, m_namelist);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(hdCPtProp, CDialog)
//{{AFX_MSG_MAP(hdCPtProp)
ON_BN_CLICKED(IDC_ADD_PROP, OnAddProp)
ON_BN_CLICKED(IDC_DEL_PROP, OnDelProp)
ON_BN_CLICKED(IDC_MOD_PROP, OnModProp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// hdCPtProp message handlers

void hdCPtProp::OnAddProp()
{

  // Code to deal with adding a Point Property.
  if (PropType == 0) {
    CPointProp PProp;
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    hdCNodeProp zDlg;

    for (int nn = 0; nn < nodeproplist.GetSize(); nn++)
      zDlg.namelist.Add(nodeproplist[nn].PointName);
    nodeproplist.Add(PProp);

    zDlg.m_vp = PProp.T;
    zDlg.m_qp = PProp.qp;
    zDlg.m_nodename = PProp.PointName;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)nodeproplist.GetSize() - 1;
      nodeproplist[k].PointName = zDlg.m_nodename;
      nodeproplist[k].T = zDlg.m_vp;
      nodeproplist[k].qp = zDlg.m_qp;
      m_namelist.AddString(nodeproplist[k].PointName);
      m_namelist.SetCurSel(k);
    } else
      nodeproplist.RemoveAt(nodeproplist.GetSize() - 1);
  }

  if (PropType == 3) {
    CCircuit CProp;
    CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
    hdCCircProp zDlg;
    for (int nn = 0; nn < circproplist.GetSize(); nn++)
      zDlg.namelist.Add(circproplist[nn].CircName);
    circproplist.Add(CProp);

    zDlg.m_circname = CProp.CircName;
    zDlg.m_circtype = CProp.CircType;
    zDlg.m_q = CProp.q;
    zDlg.m_v = CProp.T;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)circproplist.GetSize() - 1;
      circproplist[k].CircName = zDlg.m_circname;
      circproplist[k].CircType = zDlg.m_circtype;
      circproplist[k].q = zDlg.m_q;
      circproplist[k].T = zDlg.m_v;
      m_namelist.AddString(circproplist[k].CircName);
      m_namelist.SetCurSel(k);
    } else
      circproplist.RemoveAt(circproplist.GetSize() - 1);
  }

  // Code to deal with adding properties for segments...

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    hdCBdryDlg zDlg;
    for (int nn = 0; nn < lineproplist.GetSize(); nn++)
      zDlg.namelist.Add(lineproplist[nn].BdryName);
    CBoundaryProp MProp;
    lineproplist.Add(MProp);

    zDlg.m_Tset = MProp.Tset;
    zDlg.m_qs = MProp.qs;
    zDlg.m_htc = MProp.h;
    zDlg.m_To1 = MProp.Tinf;
    zDlg.m_beta = MProp.beta;
    zDlg.m_To2 = MProp.TinfRad;
    zDlg.BdryFormat = MProp.BdryFormat;
    zDlg.m_BdryName = MProp.BdryName;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)lineproplist.GetSize() - 1;
      lineproplist[k].BdryName = zDlg.m_BdryName;
      lineproplist[k].BdryFormat = zDlg.BdryFormat;
      lineproplist[k].Tset = zDlg.m_Tset;
      lineproplist[k].qs = zDlg.m_qs;
      lineproplist[k].h = zDlg.m_htc;
      lineproplist[k].beta = zDlg.m_beta;
      lineproplist[k].Tinf = zDlg.m_To1;
      lineproplist[k].TinfRad = zDlg.m_To2;

      m_namelist.AddString(lineproplist[k].BdryName);
      m_namelist.SetCurSel(k);
    } else
      lineproplist.RemoveAt(lineproplist.GetSize() - 1);
  }

  // Code to deal with adding properties for blocks...
  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    hdCMatDlg zDlg;
    for (int nn = 0; nn < blockproplist.GetSize(); nn++)
      zDlg.namelist.Add(blockproplist[nn].BlockName);
    CMaterialProp MProp;
    blockproplist.Add(MProp);
    zDlg.m_Kx = MProp.Kx;
    zDlg.m_Ky = MProp.Ky;
    zDlg.m_Kt = MProp.Kt;
    zDlg.m_qv = MProp.qv;
    zDlg.m_BlockName = MProp.BlockName;
    zDlg.m_npts = MProp.npts;
    for (int k = 0; k < MProp.npts; k++) {
      zDlg.m_Kn[k] = MProp.Kn[k];
    }

    if (ProblemType == 0) {
      zDlg.m_mu1label = "Kx";
      zDlg.m_mu2label = "Ky";
    } else {
      zDlg.m_mu1label = "Kr";
      zDlg.m_mu2label = "Kz";
    }

    if (zDlg.DoModal() == IDOK) {
      int k = (int)blockproplist.GetSize() - 1;
      blockproplist[k].BlockName = zDlg.m_BlockName;
      blockproplist[k].Kx = zDlg.m_Kx;
      blockproplist[k].Ky = zDlg.m_Ky;
      blockproplist[k].Kt = zDlg.m_Kt;
      blockproplist[k].qv = zDlg.m_qv;
      blockproplist[k].npts = zDlg.m_npts;
      for (int i = 0; i < zDlg.m_npts; i++) {
        blockproplist[k].Kn[i] = zDlg.m_Kn[i];
      }
      m_namelist.AddString(blockproplist[k].BlockName);
      m_namelist.SetCurSel(k);
    } else
      blockproplist.RemoveAt(blockproplist.GetSize() - 1);
  }
}

void hdCPtProp::OnDelProp()
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

void hdCPtProp::OnModProp()
{
  // TODO: Add your control notification handler code here
  if (PropType == 0) {
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    hdCNodeProp zDlg;

    if (nodeproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_vp = nodeproplist[k].T;
    zDlg.m_qp = nodeproplist[k].qp;
    zDlg.m_nodename = nodeproplist[k].PointName;

    for (int nn = 0; nn < nodeproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(nodeproplist[nn].PointName);

    if (zDlg.DoModal() == IDOK) {
      nodeproplist[k].PointName = zDlg.m_nodename;
      nodeproplist[k].T = zDlg.m_vp;
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

    hdCCircProp zDlg;

    zDlg.m_circname = circproplist[k].CircName;
    zDlg.m_circtype = circproplist[k].CircType;
    zDlg.m_q = circproplist[k].q;
    zDlg.m_v = circproplist[k].T;
    for (int nn = 0; nn < circproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(circproplist[nn].CircName);

    if (zDlg.DoModal() == IDOK) {
      circproplist[k].CircName = zDlg.m_circname;
      circproplist[k].CircType = zDlg.m_circtype;
      circproplist[k].q = zDlg.m_q;
      circproplist[k].T = zDlg.m_v;
      m_namelist.InsertString(k, zDlg.m_circname);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    hdCBdryDlg zDlg;

    if (lineproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_Tset = lineproplist[k].Tset;
    zDlg.m_qs = lineproplist[k].qs;
    zDlg.m_beta = lineproplist[k].beta;
    zDlg.m_htc = lineproplist[k].h;
    zDlg.m_To1 = lineproplist[k].Tinf;
    zDlg.m_To2 = lineproplist[k].TinfRad;
    zDlg.BdryFormat = lineproplist[k].BdryFormat;
    zDlg.m_BdryName = lineproplist[k].BdryName;
    for (int nn = 0; nn < lineproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(lineproplist[nn].BdryName);

    if (zDlg.DoModal() == IDOK) {
      lineproplist[k].BdryName = zDlg.m_BdryName;
      lineproplist[k].BdryFormat = zDlg.BdryFormat;
      lineproplist[k].Tset = zDlg.m_Tset;
      lineproplist[k].qs = zDlg.m_qs;
      lineproplist[k].beta = zDlg.m_beta;
      lineproplist[k].h = zDlg.m_htc;
      lineproplist[k].Tinf = zDlg.m_To1;
      lineproplist[k].TinfRad = zDlg.m_To2;
      m_namelist.InsertString(k, zDlg.m_BdryName);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    hdCMatDlg zDlg;

    if (blockproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_Kx = blockproplist[k].Kx;
    zDlg.m_Ky = blockproplist[k].Ky;
    zDlg.m_Kt = blockproplist[k].Kt;
    zDlg.m_qv = blockproplist[k].qv;
    zDlg.m_BlockName = blockproplist[k].BlockName;
    zDlg.m_npts = blockproplist[k].npts;
    for (int i = 0; i < blockproplist[k].npts; i++) {
      zDlg.m_Kn[i] = blockproplist[k].Kn[i];
    }

    for (int nn = 0; nn < blockproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(blockproplist[nn].BlockName);

    if (ProblemType == 0) {
      zDlg.m_mu1label = "Kx";
      zDlg.m_mu2label = "Ky";
    } else {
      zDlg.m_mu1label = "Kr";
      zDlg.m_mu2label = "Kz";
    }
    if (zDlg.DoModal() == IDOK) {
      blockproplist[k].BlockName = zDlg.m_BlockName;
      blockproplist[k].Kx = zDlg.m_Kx;
      blockproplist[k].Ky = zDlg.m_Ky;
      blockproplist[k].Kt = zDlg.m_Kt;
      blockproplist[k].qv = zDlg.m_qv;
      blockproplist[k].npts = zDlg.m_npts;
      for (int i = 0; i < blockproplist[k].npts; i++) {
        blockproplist[k].Kn[i] = zDlg.m_Kn[i];
      }
      m_namelist.InsertString(k, zDlg.m_BlockName);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }
}

void hdCPtProp::OnOK()
{
  // TODO: Add extra validation here

  CDialog::OnOK();
}

BOOL hdCPtProp::OnInitDialog()
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
