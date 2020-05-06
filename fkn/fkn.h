// fkn.h : main header file for the FKN application
//

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h" // main symbols

void old_main(void* inptr);

/////////////////////////////////////////////////////////////////////////////
// CFknApp:
// See fkn.cpp for the implementation of this class
//

class CFknApp : public CWinApp {
  public:
  CFknApp();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFknApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CFknApp)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
