// femmplot.h : main header file for the FEMMPLOT application
//

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h" // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFemmplotApp:
// See femmplot.cpp for the implementation of this class
//

class CFemmplotApp : public CWinApp {
  public:
  CFemmplotApp();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFemmplotApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CFemmplotApp)
  afx_msg void OnAppAbout();
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
