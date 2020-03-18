#pragma once

#include "ResizableDialog.h"

// treetestDlg.h : header file
//

#if !defined(AFX_TREETESTDLG_H__5B8113A6_69AB_44C9_A4F6_E868EE363479__INCLUDED_)
#define AFX_TREETESTDLG_H__5B8113A6_69AB_44C9_A4F6_E868EE363479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// hdCLibDlg dialog

class hdCLibDlg : public CResizableDialog
{
// Construction
public:
	hdCLibDlg(CWnd* pParent = NULL);	// standard constructor
	
	ChdrawDoc *pDoc;
	CArray <CMaterialProp,CMaterialProp&> LibProps;
	CArray <CFolderProp,CFolderProp&> FoldProps;
	CArray <HTREEITEM,HTREEITEM&> Ancestors;
	HTREEITEM Parent;
	HTREEITEM ModelParent;
	HTREEITEM LibParent;
	BOOL bDragFromLibrary;
	CString BinDir;
	CString VendorURL;
	CString VendorName;
	int Focus;

	int ParseLine(char *s, FILE *fp, CMaterialProp &MProp);
	char* StripKey(char *c);
	BOOL IsFolder(HTREEITEM hItem);
	HTREEITEM CopyItemToLibrary(HTREEITEM hItem, HTREEITEM hTarget);
	HTREEITEM CopyItemToModel(HTREEITEM hItem, HTREEITEM hTarget);
	void CopyItemToDisk(HTREEITEM hItem, FILE *fp);
	void VendorLink();
	void Zappit();
	void MouseModify();
	void AddNewProperty();
	void AddNewFolder();
	void ImportMaterials();
	void ModifyMaterial(int k);
	BOOL IsTailChasing(HTREEITEM hSource, HTREEITEM hDest);

// Dialog Data
	//{{AFX_DATA(hdCLibDlg)
	enum { IDD = IDD_TREETEST_DIALOG };
	CTreeCtrl	m_mylist;
	CTreeCtrl	m_mytree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(hdCLibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON m_hIcon;

	// stuff needed to do CTreeCtrl properly
    CImageList  m_imageList;
    BOOL        m_bIsDragging;
    HTREEITEM   m_dragItem;
	HTREEITEM	m_dragTargetTree;
	HTREEITEM	m_dragTargetList;
	
	// Generated message map functions
	//{{AFX_MSG(hdCLibDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBegindragMytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragMylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickMylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickMytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkMytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkMylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownMylist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownMytree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREETESTDLG_H__5B8113A6_69AB_44C9_A4F6_E868EE363479__INCLUDED_)





