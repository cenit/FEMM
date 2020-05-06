// BHData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBHData dialog
#include <afxtempl.h>
#include "afxwin.h"

class CBHData : public CDialog {
  // Construction
  public:
  CBHData(CWnd* pParent = NULL); // standard constructor
  CArray<double, double> B;
  CArray<double, double> H;
  int BHpoints;
  double* slope;
  BOOL logplot;

  void StripBHData();
  void GetSlopes();
  double GetH(double x);

  // Dialog Data
  //{{AFX_DATA(CBHData)
  enum { IDD = IDD_BHCURVE };
  CString m_Bdata;
  CString m_Hdata;
  CString m_BHname;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CBHData)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  // Generated message map functions
  //{{AFX_MSG(CBHData)
  afx_msg void OnPlotBHcurve();
  afx_msg void OnLogPlotBHcurve();
  afx_msg void OnReadBhcurve();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  public:
  CLuaEdit m_IDC_BDATA;
  CLuaEdit m_IDC_HDATA;
};
