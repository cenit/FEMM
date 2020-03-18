// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IActiveFEMM wrapper class

#include <afx.h>
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes


class IActiveFEMM : public COleDispatchDriver
{
public:
	IActiveFEMM() {}		// Calls COleDispatchDriver default constructor
	IActiveFEMM(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IActiveFEMM(const IActiveFEMM& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString call2femm(LPCTSTR luacmd);
	CString mlab2femm(LPCTSTR luacmd);
};
