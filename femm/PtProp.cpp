// PtProp.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "femmeDoc.h"
#include "PtProp.h"
#include "NodeProp.h"
#include "MatDlg.h"
#include "BdryDlg.h"
#include "CircProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPtProp dialog

CPtProp::CPtProp(CWnd* pParent /*=NULL*/)
    : CDialog(CPtProp::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPtProp)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CPtProp::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPtProp)
  DDX_Control(pDX, IDC_NAME_LIST, m_namelist);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPtProp, CDialog)
//{{AFX_MSG_MAP(CPtProp)
ON_BN_CLICKED(IDC_ADD_PROP, OnAddProp)
ON_BN_CLICKED(IDC_DEL_PROP, OnDelProp)
ON_BN_CLICKED(IDC_MOD_PROP, OnModProp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPtProp message handlers

void CPtProp::OnAddProp()
{

  // Code to deal with adding a Point Property.
  if (PropType == 0) {
    CPointProp PProp;
    CNodeProp zDlg;
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;

    for (int nn = 0; nn < nodeproplist.GetSize(); nn++)
      zDlg.namelist.Add(nodeproplist[nn].PointName);

    nodeproplist.Add(PProp);

    zDlg.m_ap = PProp.Ap;
    zDlg.m_jp = PProp.Jp;
    zDlg.m_nodename = PProp.PointName;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)nodeproplist.GetSize() - 1;
      nodeproplist[k].PointName = zDlg.m_nodename;
      nodeproplist[k].Ap = zDlg.m_ap;
      nodeproplist[k].Jp = zDlg.m_jp;
      m_namelist.AddString(nodeproplist[k].PointName);
      m_namelist.SetCurSel(k);
    } else
      nodeproplist.RemoveAt(nodeproplist.GetSize() - 1);
  }

  if (PropType == 3) {
    CCircuit CProp;
    CArray<CCircuit, CCircuit&>& circproplist = *pcircproplist;
    CCircProp zDlg;
    for (int nn = 0; nn < circproplist.GetSize(); nn++)
      zDlg.namelist.Add(circproplist[nn].CircName);
    circproplist.Add(CProp);

    zDlg.m_circname = CProp.CircName;
    zDlg.m_circtype = CProp.CircType;
    zDlg.m_totcurrent = CProp.Amps;

    if (zDlg.DoModal() == IDOK) {
      int k = (int)circproplist.GetSize() - 1;
      circproplist[k].CircName = zDlg.m_circname;
      circproplist[k].CircType = zDlg.m_circtype;
      circproplist[k].Amps = zDlg.m_totcurrent;
      m_namelist.AddString(circproplist[k].CircName);
      m_namelist.SetCurSel(k);
    } else
      circproplist.RemoveAt(circproplist.GetSize() - 1);
  }

  // Code to deal with adding properties for segments...

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    CBdryDlg zDlg;
    for (int nn = 0; nn < lineproplist.GetSize(); nn++)
      zDlg.namelist.Add(lineproplist[nn].BdryName);
    CBoundaryProp MProp;
    lineproplist.Add(MProp);

    zDlg.m_A0 = MProp.A0;
    zDlg.m_A1 = MProp.A1;
    zDlg.m_A2 = MProp.A2;
    zDlg.m_Phi = MProp.phi;
    zDlg.m_Mu = MProp.Mu;
    zDlg.m_Sig = MProp.Sig;
    zDlg.m_c0 = MProp.c0;
    zDlg.m_c1 = MProp.c1;
    zDlg.BdryFormat = MProp.BdryFormat;
    zDlg.m_BdryName = MProp.BdryName;
    zDlg.m_innerangle = MProp.InnerAngle;
    zDlg.m_outerangle = MProp.OuterAngle;
    if (zDlg.DoModal() == IDOK) {
      int k = (int)lineproplist.GetSize() - 1;
      lineproplist[k].BdryName = zDlg.m_BdryName;
      lineproplist[k].BdryFormat = zDlg.BdryFormat;
      lineproplist[k].A0 = zDlg.m_A0;
      lineproplist[k].A1 = zDlg.m_A1;
      lineproplist[k].A2 = zDlg.m_A2;
      lineproplist[k].phi = zDlg.m_Phi;
      lineproplist[k].Mu = zDlg.m_Mu;
      lineproplist[k].Sig = zDlg.m_Sig;
      lineproplist[k].c0 = zDlg.m_c0;
      lineproplist[k].c1 = zDlg.m_c1;
      lineproplist[k].InnerAngle = zDlg.m_innerangle;
      lineproplist[k].OuterAngle = zDlg.m_outerangle;

      m_namelist.AddString(lineproplist[k].BdryName);
      m_namelist.SetCurSel(k);
    } else
      lineproplist.RemoveAt(lineproplist.GetSize() - 1);
  }

  // Code to deal with adding properties for blocks...
  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    CMatDlg zDlg;
    for (int nn = 0; nn < blockproplist.GetSize(); nn++)
      zDlg.namelist.Add(blockproplist[nn].BlockName);
    CMaterialProp MProp;
    blockproplist.Add(MProp);
    zDlg.m_mu_x = MProp.mu_x;
    zDlg.m_mu_y = MProp.mu_y;
    zDlg.m_H_c = MProp.H_c;
    zDlg.m_Jsrc = MProp.Jsrc;
    zDlg.m_Cduct = MProp.Cduct;
    zDlg.m_Lam_d = MProp.Lam_d;
    zDlg.m_Theta_hn = MProp.Theta_hn;
    zDlg.m_Theta_hx = MProp.Theta_hx;
    zDlg.m_Theta_hy = MProp.Theta_hy;
    zDlg.m_BlockName = MProp.BlockName;
    zDlg.m_WireD = MProp.WireD;
    zDlg.m_NStrands = MProp.NStrands;

    zDlg.m_lam_fill = MProp.LamFill;

    zDlg.LamType = MProp.LamType;

    zDlg.m_nlflag = FALSE;
    MProp.BHDataToCString(zDlg.BData, zDlg.HData);

    if (ProblemType == 0) {
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
      int k = (int)blockproplist.GetSize() - 1;
      blockproplist[k].BlockName = zDlg.m_BlockName;
      blockproplist[k].mu_x = zDlg.m_mu_x;
      blockproplist[k].mu_y = zDlg.m_mu_y;
      blockproplist[k].H_c = zDlg.m_H_c;
      blockproplist[k].Jsrc = zDlg.m_Jsrc;
      blockproplist[k].Cduct = zDlg.m_Cduct;
      blockproplist[k].Lam_d = zDlg.m_Lam_d;
      blockproplist[k].Theta_hn = zDlg.m_Theta_hn;
      blockproplist[k].Theta_hx = zDlg.m_Theta_hx;
      blockproplist[k].Theta_hy = zDlg.m_Theta_hy;

      if (zDlg.m_nlflag == FALSE) {
        zDlg.BData.Empty();
        zDlg.HData.Empty();
      }
      blockproplist[k].StripBHData(zDlg.BData, zDlg.HData);

      blockproplist[k].LamFill = zDlg.m_lam_fill;
      blockproplist[k].LamType = zDlg.LamType;
      blockproplist[k].NStrands = zDlg.m_NStrands;
      blockproplist[k].WireD = zDlg.m_WireD;
      m_namelist.AddString(blockproplist[k].BlockName);
      m_namelist.SetCurSel(k);
    } else
      blockproplist.RemoveAt(blockproplist.GetSize() - 1);
  }
}

void CPtProp::OnDelProp()
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

void CPtProp::OnModProp()
{
  // TODO: Add your control notification handler code here
  if (PropType == 0) {
    CArray<CPointProp, CPointProp&>& nodeproplist = *pnodeproplist;
    CNodeProp zDlg;

    if (nodeproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_ap = nodeproplist[k].Ap;
    zDlg.m_jp = nodeproplist[k].Jp;
    zDlg.m_nodename = nodeproplist[k].PointName;

    for (int nn = 0; nn < nodeproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(nodeproplist[nn].PointName);

    if (zDlg.DoModal() == IDOK) {
      nodeproplist[k].PointName = zDlg.m_nodename;
      nodeproplist[k].Ap = zDlg.m_ap;
      nodeproplist[k].Jp = zDlg.m_jp;
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

    CCircProp zDlg;

    zDlg.m_circname = circproplist[k].CircName;
    zDlg.m_circtype = circproplist[k].CircType;
    zDlg.m_totcurrent = circproplist[k].Amps;
    for (int nn = 0; nn < circproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(circproplist[nn].CircName);

    if (zDlg.DoModal() == IDOK) {
      circproplist[k].CircName = zDlg.m_circname;
      circproplist[k].CircType = zDlg.m_circtype;
      circproplist[k].Amps = zDlg.m_totcurrent;
      m_namelist.InsertString(k, zDlg.m_circname);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 1) {
    CArray<CBoundaryProp, CBoundaryProp&>& lineproplist = *plineproplist;
    CBdryDlg zDlg;

    if (lineproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_A0 = lineproplist[k].A0;
    zDlg.m_A1 = lineproplist[k].A1;
    zDlg.m_A2 = lineproplist[k].A2;
    zDlg.m_Phi = lineproplist[k].phi;
    zDlg.m_Mu = lineproplist[k].Mu;
    zDlg.m_Sig = lineproplist[k].Sig;
    zDlg.m_c0 = lineproplist[k].c0;
    zDlg.m_c1 = lineproplist[k].c1;
    zDlg.BdryFormat = lineproplist[k].BdryFormat;
    zDlg.m_BdryName = lineproplist[k].BdryName;
    zDlg.m_innerangle = lineproplist[k].InnerAngle;
    zDlg.m_outerangle = lineproplist[k].OuterAngle;
    for (int nn = 0; nn < lineproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(lineproplist[nn].BdryName);

    if (zDlg.DoModal() == IDOK) {
      lineproplist[k].BdryName = zDlg.m_BdryName;
      lineproplist[k].BdryFormat = zDlg.BdryFormat;
      lineproplist[k].A0 = zDlg.m_A0;
      lineproplist[k].A1 = zDlg.m_A1;
      lineproplist[k].A2 = zDlg.m_A2;
      lineproplist[k].phi = zDlg.m_Phi;
      lineproplist[k].Mu = zDlg.m_Mu;
      lineproplist[k].Sig = zDlg.m_Sig;
      lineproplist[k].c0 = zDlg.m_c0;
      lineproplist[k].c1 = zDlg.m_c1;
      lineproplist[k].InnerAngle = zDlg.m_innerangle;
      lineproplist[k].OuterAngle = zDlg.m_outerangle;
      m_namelist.InsertString(k, zDlg.m_BdryName);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }

  if (PropType == 2) {
    CArray<CMaterialProp, CMaterialProp&>& blockproplist = *pblockproplist;
    CMatDlg zDlg;

    if (blockproplist.GetSize() == 0)
      return;
    int k = m_namelist.GetCurSel();
    if (k == CB_ERR)
      return;

    zDlg.m_mu_x = blockproplist[k].mu_x;
    zDlg.m_mu_y = blockproplist[k].mu_y;
    zDlg.m_H_c = blockproplist[k].H_c;
    zDlg.m_Jsrc = blockproplist[k].Jsrc;
    zDlg.m_Cduct = blockproplist[k].Cduct;
    zDlg.m_Lam_d = blockproplist[k].Lam_d;
    zDlg.m_Theta_hn = blockproplist[k].Theta_hn;
    zDlg.m_Theta_hx = blockproplist[k].Theta_hx;
    zDlg.m_Theta_hy = blockproplist[k].Theta_hy;
    zDlg.m_WireD = blockproplist[k].WireD;
    zDlg.m_NStrands = blockproplist[k].NStrands;
    zDlg.m_BlockName = blockproplist[k].BlockName;
    zDlg.LamType = blockproplist[k].LamType;
    zDlg.m_lam_fill = blockproplist[k].LamFill;
    for (int nn = 0; nn < blockproplist.GetSize(); nn++)
      if (nn != k)
        zDlg.namelist.Add(blockproplist[nn].BlockName);

    if (blockproplist[k].BHpoints == 0)
      zDlg.m_nlflag = FALSE;
    else {
      zDlg.m_nlflag = TRUE;
      blockproplist[k].BHDataToCString(zDlg.BData, zDlg.HData);
    }

    if (ProblemType == 0) {
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
      blockproplist[k].BlockName = zDlg.m_BlockName;
      blockproplist[k].mu_x = zDlg.m_mu_x;
      blockproplist[k].mu_y = zDlg.m_mu_y;
      blockproplist[k].H_c = zDlg.m_H_c;
      blockproplist[k].Jsrc = zDlg.m_Jsrc;
      blockproplist[k].Cduct = zDlg.m_Cduct;
      blockproplist[k].Lam_d = zDlg.m_Lam_d;
      blockproplist[k].Theta_hn = zDlg.m_Theta_hn;
      blockproplist[k].Theta_hx = zDlg.m_Theta_hx;
      blockproplist[k].Theta_hy = zDlg.m_Theta_hy;
      blockproplist[k].NStrands = zDlg.m_NStrands;
      blockproplist[k].WireD = zDlg.m_WireD;
      blockproplist[k].LamType = zDlg.LamType;
      blockproplist[k].LamFill = zDlg.m_lam_fill;

      if (zDlg.m_nlflag == FALSE) {
        zDlg.BData.Empty();
        zDlg.HData.Empty();
      }
      blockproplist[k].StripBHData(zDlg.BData, zDlg.HData);

      m_namelist.InsertString(k, zDlg.m_BlockName);
      m_namelist.DeleteString(k + 1);
      m_namelist.SetCurSel(k);
    }
  }
}

void CPtProp::OnOK()
{
  // TODO: Add extra validation here

  CDialog::OnOK();
}

BOOL CPtProp::OnInitDialog()
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
