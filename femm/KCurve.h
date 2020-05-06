// TKData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTKData dialog
#include <afxtempl.h>
#include "afxwin.h"

class CTKData : public CDialog {
  // Construction
  public:
  CTKData(CWnd* pParent = NULL); // standard constructor
  CArray<double, double> T;
  CArray<double, double> K;
  int npts;

  BOOL logplot;

  void StripTKData();
  double GetK(double T);
  void SortCurve();

  // Dialog Data
  //{{AFX_DATA(CTKData)
  enum { IDD = IDD_KCURVE };
  CString m_Bdata;
  CString m_Hdata;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTKData)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CTKData)
  afx_msg void OnPlotBHcurve();
  afx_msg void OnLogPlotBHcurve();
  afx_msg void OnReadBhcurve();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_BDATA;
  CLuaEdit m_IDC_HDATA;
};
