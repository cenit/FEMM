#ifndef _MYRECENTFILELIST_H_
#define _MYRECENTFILELIST_H_

#include < afxadv.h>  // for CRecentFileList

class MyCRecentFileList : public CRecentFileList {
public:
        MyCRecentFileList(UINT nStart, LPCTSTR lpszSection,
                LPCTSTR lpszEntryFormat, int nSize,
                int nMaxDispLen = AFX_ABBREV_FILENAME_LEN)
                : CRecentFileList(nStart, lpszSection,lpszEntryFormat, nSize,
                nMaxDispLen) {}
        virtual BOOL MyCRecentFileList::GetDisplayName(CString& strName, int nIndex,
	LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName= TRUE);
		virtual void UpdateMenu(CCmdUI* pCmdUI);
};  // class VIRecentFileList

#endif  // _MYRECENTFILELIST_H_