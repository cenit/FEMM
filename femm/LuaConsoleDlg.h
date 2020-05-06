#pragma once

#include "ResizableDialog.h"

// CLuaConsoleDlg dialog

class CLuaConsoleDlg : public CResizableDialog {
  //	DECLARE_DYNAMIC(CLuaConsoleDlg)

  public:
  CLuaConsoleDlg(CWnd* pParent = NULL); // standard constructor
  virtual ~CLuaConsoleDlg();

  // Dialog Data
  enum { IDD = IDD_LUACONSOLE };

  protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
  virtual BOOL OnInitDialog();

  DECLARE_MESSAGE_MAP()
  public:
  CString outbuffer;
  CString inbuffer;
  afx_msg void OnBnClickedClearInput();
  afx_msg void OnBnClickedClearOutput();

  void ToOutput(CString str);
  static int lua_Print(lua_State* L);
  static int lua_showconsole(lua_State* L);
  static int lua_hideconsole(lua_State* L);
  static int lua_clearconsole(lua_State* L);

  CLuaEdit inbufferctrl;
  CEdit ConsoleOutput;

  afx_msg void OnBnClickedEvaluate();
};
