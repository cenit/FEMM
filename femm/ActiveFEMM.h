#if !defined(AFX_ACTIVEFEMM_H__B28DBF46_5DAD_4398_AD40_58F1D7107DFA__INCLUDED_)
#define AFX_ACTIVEFEMM_H__B28DBF46_5DAD_4398_AD40_58F1D7107DFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActiveFEMM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ActiveFEMM command target

class ActiveFEMM : public CCmdTarget {
  DECLARE_DYNCREATE(ActiveFEMM)

  DECLARE_OLETYPELIB(ActiveFemm)

  ActiveFEMM(); // protected constructor used by dynamic creation

  // Attributes
  public:
  // Operations
  public:
  static int lua_to_string(lua_State* L);
  static int lua_to_matlab(lua_State* L);
  void DoLuaCmd(CString strToLua);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(ActiveFEMM)
  public:
  virtual void OnFinalRelease();
  //}}AFX_VIRTUAL

  // Implementation
  protected:
  virtual ~ActiveFEMM();

  // Generated message map functions
  //{{AFX_MSG(ActiveFEMM)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
  DECLARE_OLECREATE(ActiveFEMM)

  // Generated OLE dispatch map functions
  //{{AFX_DISPATCH(ActiveFEMM)
  afx_msg BSTR call2femm(LPCTSTR luacmd);
  afx_msg BSTR mlab2femm(LPCTSTR luacmd);
  //}}AFX_DISPATCH
  DECLARE_DISPATCH_MAP()
  DECLARE_INTERFACE_MAP()

  virtual BOOL GetDispatchIID(IID* pIID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVEFEMM_H__B28DBF46_5DAD_4398_AD40_58F1D7107DFA__INCLUDED_)
