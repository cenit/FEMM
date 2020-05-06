#include "stdafx.h"
#include "MyRecentFileList.h"

// Header information required to use obscure MFC functions
UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);

// extra function cribbed from FILELIST.CPP in the MFC source
AFX_STATIC void AFXAPI _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName)
{
  int cchFullPath, cchFileName, cchVolName;
  const TCHAR* lpszCur;
  const TCHAR* lpszBase;
  const TCHAR* lpszFileName;

  lpszBase = lpszCanon;
  cchFullPath = lstrlen(lpszCanon);

  cchFileName = AfxGetFileName(lpszCanon, NULL, 0) - 1;
  lpszFileName = lpszBase + (cchFullPath - cchFileName);

  // If cchMax is more than enough to hold the full path name, we're done.
  // This is probably a pretty common case, so we'll put it first.
  if (cchMax >= cchFullPath)
    return;

  // If cchMax isn't enough to hold at least the basename, we're done
  if (cchMax < cchFileName) {
    lstrcpy(lpszCanon, (bAtLeastName) ? lpszFileName : "");
    return;
  }

  // Calculate the length of the volume name.  Normally, this is two characters
  // (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
  // "\\server\share").
  //
  // If cchMax isn't enough to hold at least <volume_name>\...\<base_name>, the
  // result is the base filename.

  lpszCur = lpszBase + 2; // Skip "C:" or leading "\\"

  if (lpszBase[0] == '\\' && lpszBase[1] == '\\') // UNC pathname
  {
    // First skip to the '\' between the server name and the share name,
    while (*lpszCur != '\\') {
      lpszCur = _tcsinc(lpszCur);
      ASSERT(*lpszCur != '\0');
    }
  }
  // if a UNC get the share name, if a drive get at least one directory
  ASSERT(*lpszCur == '\\');
  // make sure there is another directory, not just c:\filename.ext
  if (cchFullPath - cchFileName > 3) {
    lpszCur = _tcsinc(lpszCur);
    while (*lpszCur != '\\') {
      lpszCur = _tcsinc(lpszCur);
      ASSERT(*lpszCur != '\0');
    }
  }
  ASSERT(*lpszCur == '\\');

  cchVolName = (int)lpszCur - (int)lpszBase;
  if (cchMax < cchVolName + 5 + cchFileName) {
    lstrcpy(lpszCanon, lpszFileName);
    return;
  }

  // Now loop through the remaining directory components until something
  // of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
  //
  // Assert that the whole filename doesn't fit -- this should have been
  // handled earlier.

  ASSERT(cchVolName + (int)lstrlen(lpszCur) > cchMax);
  while (cchVolName + 4 + (int)lstrlen(lpszCur) > cchMax) {
    do {
      lpszCur = _tcsinc(lpszCur);
      ASSERT(*lpszCur != '\0');
    } while (*lpszCur != '\\');
  }

  // Form the resultant string and we're done.
  lpszCanon[cchVolName] = '\0';
  lstrcat(lpszCanon, _T("\\..."));
  lstrcat(lpszCanon, lpszCur);
}

BOOL MyCRecentFileList::GetDisplayName(CString& strName, int nIndex,
    LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName)
{
  ASSERT(lpszCurDir == NULL || AfxIsValidString(lpszCurDir, nCurDir));

  ASSERT(m_arrNames != NULL);
  ASSERT(nIndex < m_nSize);
  if (m_arrNames[nIndex].IsEmpty())
    return FALSE;

  LPTSTR lpch = strName.GetBuffer(_MAX_PATH);
  lstrcpy(lpch, m_arrNames[nIndex]);

  // abbreviate name based on what will fit in limited space
  _AfxAbbreviateName(lpch, m_nMaxDisplayLength, bAtLeastName);

  strName.ReleaseBuffer();

  return TRUE;
}

void MyCRecentFileList::UpdateMenu(CCmdUI* pCmdUI)
{
  ASSERT(m_arrNames != NULL);

  CMenu* pMenu = pCmdUI->m_pMenu;
  if (m_strOriginal.IsEmpty() && pMenu != NULL)
    pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);

  if (m_arrNames[0].IsEmpty()) {
    // no MRU files
    if (!m_strOriginal.IsEmpty())
      pCmdUI->SetText(m_strOriginal);
    pCmdUI->Enable(FALSE);
    return;
  }

  if (pCmdUI->m_pMenu == NULL)
    return;

  int iMRU;
  for (iMRU = 0; iMRU < m_nSize; iMRU++)
    pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

  TCHAR szCurDir[_MAX_PATH];
  GetCurrentDirectory(_MAX_PATH, szCurDir);
  int nCurDir = lstrlen(szCurDir);
  ASSERT(nCurDir >= 0);
  szCurDir[nCurDir] = '\\';
  szCurDir[++nCurDir] = '\0';

  CString strName;
  CString strTemp;
  for (iMRU = 0; iMRU < m_nSize; iMRU++) {
    if (!GetDisplayName(strName, iMRU, szCurDir, nCurDir))
      break;

    // double up any '&' characters so they are not underlined
    LPCTSTR lpszSrc = strName;
    LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength() * 2);
    while (*lpszSrc != 0) {
      if (*lpszSrc == '&')
        *lpszDest++ = '&';
      if (_istlead(*lpszSrc))
        *lpszDest++ = *lpszSrc++;
      *lpszDest++ = *lpszSrc++;
    }
    *lpszDest = 0;
    strTemp.ReleaseBuffer();

    // insert mnemonic + the file name
    TCHAR buf[10];
    wsprintf(buf, _T("&%d "), (iMRU + 1 + m_nStart) % 10);
    pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
        MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++,
        CString(buf) + strTemp);
  }

  // update end menu count
  pCmdUI->m_nIndex--; // point to last menu added
  pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

  pCmdUI->m_bEnableChanged = TRUE; // all the added items are enabled
}
