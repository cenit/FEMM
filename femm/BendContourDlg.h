#pragma once


// CBendContourDlg dialog

class CBendContourDlg : public CDialog
{
	DECLARE_DYNAMIC(CBendContourDlg)

public:
	CBendContourDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBendContourDlg();

// Dialog Data
	enum { IDD = IDD_BENDCONTOUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_angle;
	double m_anglestep;
	CLuaEdit m_IDC_angle;
	CLuaEdit m_IDC_anglestep;
};
