// BendContourDlg.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include "BendContourDlg.h"

// CBendContourDlg dialog

IMPLEMENT_DYNAMIC(CBendContourDlg, CDialog)
CBendContourDlg::CBendContourDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBendContourDlg::IDD, pParent)
    , m_angle(0)
    , m_anglestep(1)
{
}

CBendContourDlg::~CBendContourDlg()
{
}

void CBendContourDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_BENDANGLE, m_angle);
  DDV_MinMaxDouble(pDX, m_angle, -180, 180);
  DDX_Text(pDX, IDC_BENDSEGMENT, m_anglestep);
  DDV_MinMaxDouble(pDX, m_anglestep, 0, 180);
  DDX_Control(pDX, IDC_BENDANGLE, m_IDC_angle);
  DDX_Control(pDX, IDC_BENDSEGMENT, m_IDC_anglestep);
}

BEGIN_MESSAGE_MAP(CBendContourDlg, CDialog)
END_MESSAGE_MAP()

// CBendContourDlg message handlers
