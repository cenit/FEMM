//////////////////////////////////////////////////
// CLuaEdit is a specialized CEdit control that
// allows the selection to be evaluated by Lua
//////////////////////////////////////////////////

class CLuaEdit : public CEdit
{
private:
	using CEdit::Create;

public:
// Implementation
	BOOL IsSelection();
	void ParseWithLua();
	void Undo();
	void SelectAll();

protected:
	

	//{{AFX_MSG(CLuaEdit)
	afx_msg void OnChar(UINT, UINT, UINT); // check to see if we need to evaluate the selection
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};