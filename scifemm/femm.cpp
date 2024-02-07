// Machine generated IDispatch wrapper class(es) created with ClassWizard

// #include "stdafx.h"
#include "femm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IActiveFEMM properties

/////////////////////////////////////////////////////////////////////////////
// IActiveFEMM operations

CString IActiveFEMM::call2femm(LPCTSTR luacmd)
{
  CString result;
  static BYTE parms[] = VTS_BSTR;
  InvokeHelper(0x1, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      luacmd);
  return result;
}

CString IActiveFEMM::mlab2femm(LPCTSTR luacmd)
{
  CString result;
  static BYTE parms[] = VTS_BSTR;
  InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
      luacmd);
  return result;
}
