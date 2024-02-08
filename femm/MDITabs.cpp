/****************************************************************************\
Datei  : MDITabs.h
Projekt: MDITabs, a tabcontrol for switching between MDI-views
Inhalt : CMDITabs implementation
Datum  : 03.10.2001
Autor  : Christian Rodemeyer
Hinweis: © 2001 by Christian Rodemeyer
\****************************************************************************/

/****************************************************************************\
As described on http://www.codeproject.com/KB/tabs/mditabs.aspx,
MDITabls is distrbuted under the CPOL (http://www.codeproject.com/info/cpol10.aspx)
For convenience, the CPOL is reproduced here:

** The Code Project Open License (CPOL) 1.02 **

Preamble

This License governs Your use of the Work. This License is intended to
allow developers to use the Source Code and Executable Files provided as
part of the Work in any application in any form.

The main points subject to the terms of the License are:

* Source Code and Executable Files can be used in commercial applications;
* Source Code and Executable Files can be redistributed; and
* Source Code can be modified to create derivative works.
* No claim of suitability, guarantee, or any warranty whatsoever is provided.
* The software is provided "as-is".
* The Article accompanying the Work may not be distributed or republished
  without the Author's consent

This License is entered between You, the individual or other entity reading
or otherwise making use of the Work licensed pursuant to this License and
the individual or other entity which offers the Work under the terms of this
License ("Author").

License

THE WORK (AS DEFINED BELOW) IS PROVIDED UNDER THE TERMS OF THIS CODE PROJECT
OPEN LICENSE ("LICENSE"). THE WORK IS PROTECTED BY COPYRIGHT AND/OR OTHER
APPLICABLE LAW. ANY USE OF THE WORK OTHER THAN AS AUTHORIZED UNDER THIS
LICENSE OR COPYRIGHT LAW IS PROHIBITED.

BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HEREIN, YOU ACCEPT AND AGREE
TO BE BOUND BY THE TERMS OF THIS LICENSE. THE AUTHOR GRANTS YOU THE RIGHTS
CONTAINED HEREIN IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND
CONDITIONS. IF YOU DO NOT AGREE TO ACCEPT AND BE BOUND BY THE TERMS OF THIS
LICENSE, YOU CANNOT MAKE ANY USE OF THE WORK.

1.	Definitions.
  a) "Articles" means, collectively, all articles written by Author which
     describes how the Source Code and Executable Files for the Work may be
     used by a user.
  b) "Author" means the individual or entity that offers the Work under the
     terms of this License.
  c) "Derivative Work" means a work based upon the Work or upon the Work and
     other pre-existing works.
  d) "Executable Files" refer to the executables, binary files, configuration
     and any required data files included in the Work.
  e) "Publisher" means the provider of the website, magazine, CD-ROM, DVD or
     other medium from or by which the Work is obtained by You.
  f) "Source Code" refers to the collection of source code and configuration
     files used to create the Executable Files.
  g) "Standard Version" refers to such a Work if it has not been modified,
     or has been modified in accordance with the consent of the Author, such consent being in the full discretion of the Author.
  h) "Work" refers to the collection of files distributed by the Publisher,
     including the Source Code, Executable Files, binaries, data files,
     documentation, whitepapers and the Articles.
  i) "You" is you, an individual or entity wishing to use the Work and exercise
     your rights under this License.
2.	Fair Use/Fair Use Rights. Nothing in this License is intended to reduce, limit,
  or restrict any rights arising from fair use, fair dealing, first sale or other
  limitations on the exclusive rights of the copyright owner under copyright law
  or other applicable laws.
3.	License Grant. Subject to the terms and conditions of this License, the Author
  hereby grants You a worldwide, royalty-free, non-exclusive, perpetual
  (for the duration of the applicable copyright) license to exercise the
  rights in the Work as stated below:
  a) You may use the standard version of the Source Code or Executable Files
     in Your own applications.
  b) You may apply bug fixes, portability fixes and other modifications obtained
     from the Public Domain or from the Author. A Work modified in such a way
     shall still be considered the standard version and will be subject to
     this License.
  c) You may otherwise modify Your copy of this Work (excluding the Articles)
     in any way to create a Derivative Work, provided that You insert a
     prominent notice in each changed file stating how, when and where You
     changed that file.
  d) You may distribute the standard version of the Executable Files and Source
     Code or Derivative Work in aggregate with other (possibly commercial)
     programs as part of a larger (possibly commercial) software distribution.
       The Articles discussing the Work published in any form by the author may not be distributed or republished without the Author's consent. The author retains copyright to any such Articles. You may use the Executable Files and Source Code pursuant to this License but you may not repost or republish or otherwise distribute or make available the Articles, without the prior written consent of the Author.
  Any subroutines or modules supplied by You and linked into the Source Code or
  Executable Files of this Work shall not be considered part of this Work and
  will not be subject to the terms of this License.
4.	Patent License. Subject to the terms and conditions of this License, each
  Author hereby grants to You a perpetual, worldwide, non-exclusive, no-charge,
  royalty-free, irrevocable (except as stated in this section) patent license to
  make, have made, use, import, and otherwise transfer the Work.
5.	Restrictions. The license granted in Section 3 above is expressly made subject to and limited by the following restrictions:
  a) You agree not to remove any of the original copyright, patent, trademark,
     and attribution notices and associated disclaimers that may appear in the
     Source Code or Executable Files.
  b) You agree not to advertise or in any way imply that this Work is a product
     of Your own.
  c) The name of the Author may not be used to endorse or promote products
     derived from the Work without the prior written consent of the Author.
  d) You agree not to sell, lease, or rent any part of the Work. This does not
     restrict you from including the Work or any part of the Work inside a
     larger software distribution that itself is being sold. The Work by
     itself, though, cannot be sold, leased or rented.
  e) You may distribute the Executable Files and Source Code only under the
     terms of this License, and You must include a copy of, or the Uniform
     Resource Identifier for, this License with every copy of the Executable
     Files or Source Code You distribute and ensure that anyone receiving
     such Executable Files and Source Code agrees that the terms of this
     License apply to such Executable Files and/or Source Code. You may not
     offer or impose any terms on the Work that alter or restrict the terms
     of this License or the recipients' exercise of the rights granted
     hereunder. You may not sublicense the Work. You must keep intact all
     notices that refer to this License and to the disclaimer of warranties.
       You may not distribute the Executable Files or Source Code with any
     technological measures that control access or use of the Work in a
     manner inconsistent with the terms of this License.
  f) You agree not to use the Work for illegal, immoral or improper purposes,
     or on pages containing illegal, immoral or improper material. The Work
     is subject to applicable export laws. You agree to comply with all such
     laws and regulations that may apply to the Work after Your receipt of
     the Work.
6.	Representations, Warranties and Disclaimer. THIS WORK IS PROVIDED "AS IS",
  "WHERE IS" AND "AS AVAILABLE", WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES OR
  CONDITIONS OR GUARANTEES. YOU, THE USER, ASSUME ALL RISK IN ITS USE, INCLUDING
  COPYRIGHT INFRINGEMENT, PATENT INFRINGEMENT, SUITABILITY, ETC. AUTHOR EXPRESSLY
  DISCLAIMS ALL EXPRESS, IMPLIED OR STATUTORY WARRANTIES OR CONDITIONS, INCLUDING
  WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF MERCHANTABILITY, MERCHANTABLE
  QUALITY OR FITNESS FOR A PARTICULAR PURPOSE, OR ANY WARRANTY OF TITLE OR
  NON-INFRINGEMENT, OR THAT THE WORK (OR ANY PORTION THEREOF) IS CORRECT, USEFUL,
  BUG-FREE OR FREE OF VIRUSES. YOU MUST PASS THIS DISCLAIMER ON WHENEVER YOU
  DISTRIBUTE THE WORK OR DERIVATIVE WORKS.
7.	Indemnity. You agree to defend, indemnify and hold harmless the Author and the
  Publisher from and against any claims, suits, losses, damages, liabilities,
  costs, and expenses (including reasonable legal or attorneys’ fees) resulting
  from or relating to any use of the Work by You.
8.	Limitation on Liability. EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW,
  IN NO EVENT WILL THE AUTHOR OR THE PUBLISHER BE LIABLE TO YOU ON ANY LEGAL
  THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY
  DAMAGES ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK OR OTHERWISE,
  EVEN IF THE AUTHOR OR THE PUBLISHER HAS BEEN ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGES.
9.	Termination.
  a) This License and the rights granted hereunder will terminate automatically
     upon any breach by You of any term of this License. Individuals or entities
     who have received Derivative Works from You under this License, however,
     will not have their licenses terminated provided such individuals or
     entities remain in full compliance with those licenses. Sections 1, 2, 6, 7,
     8, 9, 10 and 11 will survive any termination of this License.
  b) If You bring a copyright, trademark, patent or any other infringement
     claim against any contributor over infringements You claim are made by the
     Work, your License from such contributor to the Work ends automatically.
  c) Subject to the above terms and conditions, this License is perpetual
     (for the duration of the applicable copyright in the Work). Notwithstanding
     the above, the Author reserves the right to release the Work under different
     license terms or to stop distributing the Work at any time; provided,
     however that any such election will not serve to withdraw this License
     (or any other license that has been, or is required to be, granted under
     the terms of this License), and this License will continue in full force
     and effect unless terminated as stated above.
10. Publisher. The parties hereby confirm that the Publisher shall not, under any
    circumstances, be responsible for and shall not have any liability in respect
  of the subject matter of this License. The Publisher makes no warranty
  whatsoever in connection with the Work and shall not be liable to You or any
  party on any legal theory for any damages whatsoever, including without
  limitation any general, special, incidental or consequential damages arising
  in connection to this license. The Publisher reserves the right to cease
  making the Work available to You at any time without notice
11.	Miscellaneous
  a) This License shall be governed by the laws of the location of the head
     office of the Author or if the Author is an individual, the laws of
     location of the principal place of residence of the Author.
  b) If any provision of this License is invalid or unenforceable under
     applicable law, it shall not affect the validity or enforceability of
     the remainder of the terms of this License, and without further action
     by the parties to this License, such provision shall be reformed to the
     minimum extent necessary to make such provision valid and enforceable.
  c) No term or provision of this License shall be deemed waived and no breach
     consented to unless such waiver or consent shall be in writing and signed
     by the party to be charged with such waiver or consent.
  d) This License constitutes the entire agreement between the parties with
     respect to the Work licensed herein. There are no understandings,
     agreements or representations with respect to the Work not specified
     herein. The Author shall not be bound by any additional provisions that
     may appear in any communication from You. This License may not be modified
     without the mutual written agreement of the Author and You.
\****************************************************************************/

#include "stdafx.h"
#include "MDITabs.h"
#include <AFXPRIV.H>
#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabs

CMDITabs::CMDITabs()
{
  m_mdiClient = NULL;
  m_minViews = 0;
  m_bImages = false;
  m_bTop = false;
}

BEGIN_MESSAGE_MAP(CMDITabs, CTabCtrl)
//{{AFX_MSG_MAP(CMDITabs)
ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
ON_WM_PAINT()
ON_WM_NCPAINT()
ON_WM_CONTEXTMENU()
ON_WM_LBUTTONDBLCLK()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDITabs message handlers

afx_msg LRESULT CMDITabs::OnSizeParent(WPARAM, LPARAM lParam)
{
  if (GetItemCount() < m_minViews) {
    ShowWindow(SW_HIDE);
  } else {
    AFX_SIZEPARENTPARAMS* pParams = reinterpret_cast<AFX_SIZEPARENTPARAMS*>(lParam);

    const int height = 26 + (m_bImages ? 1 : 0);
    const int offset = 2;

    m_height = height + offset;
    m_width = pParams->rect.right - pParams->rect.left;

    if (m_bTop) {
      pParams->rect.top += height;
      MoveWindow(pParams->rect.left, pParams->rect.top - height, m_width, m_height, true);
    } else {
      pParams->rect.bottom -= height;
      MoveWindow(pParams->rect.left, pParams->rect.bottom - offset, m_width, m_height, true);
    }
    ShowWindow(SW_NORMAL);
  }
  return 0;
}

void CMDITabs::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
  TCITEM item;
  item.mask = TCIF_PARAM;
  GetItem(GetCurSel(), &item);
  ::BringWindowToTop(HWND(item.lParam));
  *pResult = 0;
}

void CMDITabs::Update()
{
  SetRedraw(false);

  HWND active = ::GetTopWindow(m_mdiClient); // get active view window (actually the frame of the view)

  typedef std::vector<HWND> TWndVec;
  typedef TWndVec::iterator TWndIter;

  TWndVec vChild; // put all child windows in a list (actually a vector)
  for (HWND child = active; child; child = ::GetNextWindow(child, GW_HWNDNEXT)) {
    vChild.push_back(child);
  }

  TCITEM item;
  char text[256];
  item.pszText = text;

  int i;
  for (i = GetItemCount(); i--;) // for each tab
  {
    item.mask = TCIF_PARAM;
    GetItem(i, &item);

    TWndIter it = std::find(vChild.begin(), vChild.end(), HWND(item.lParam));
    if (it == vChild.end()) // associatete view does no longer exist, so delete the tab
    {
      DeleteItem(i);
      if (m_bImages)
        RemoveImage(i);
    } else // update the tab's text, image and selection state
    {
      item.mask = TCIF_TEXT;
      ::GetWindowText(*it, text, 256);
      if (m_bImages)
        m_images.Replace(i, (HICON)::GetClassLongPtr(*it, GCLP_HICON));
      SetItem(i, &item);
      if (*it == active)
        SetCurSel(i); // associated view is active => make it the current selection
      vChild.erase(it); // remove view from list
    }
  }

  // all remaining views in vChild have to be added as new tabs
  i = GetItemCount();
  for (TWndIter it = vChild.begin(), end = vChild.end(); it != end; ++it) {
    item.mask = TCIF_TEXT | TCIF_PARAM | TCIF_IMAGE;
    ::GetWindowText(*it, text, 256);
    // a bug in wine makes us want to add some extra, spurious tabs.
    // Since there's no text on these extra tabs, we can use this as
    // a discriminator to root them out.
    if (strlen(text) > 1) {
      if (m_bImages)
        m_images.Add((HICON)::GetClassLongPtr(*it, GCLP_HICON));
      item.iImage = i;
      item.lParam = LPARAM(*it);
      InsertItem(i, &item);
      if (*it == active)
        SetCurSel(i);
      ++i;
    }
  }

  // this removes the control when there are no tabs and shows it when there is at least one tab
  bool bShow = GetItemCount() >= m_minViews;
  if ((!bShow && IsWindowVisible()) || (bShow && !IsWindowVisible())) {
    static_cast<CMDIFrameWnd*>(FromHandlePermanent(::GetParent(m_mdiClient)))->RecalcLayout();
  }

  RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE);
  SetRedraw(true);
}

void CMDITabs::OnPaint()
{
  CPaintDC dc(this);

  if (GetItemCount() == 0)
    return; // do nothing

  // cache some system colors
  DWORD shadow = ::GetSysColor(COLOR_3DSHADOW);
  DWORD dark = ::GetSysColor(COLOR_3DDKSHADOW);
  DWORD hilight = ::GetSysColor(COLOR_3DHILIGHT);
  DWORD light = ::GetSysColor(COLOR_3DLIGHT);

  // Special preparations for spin-buttons (in case there are more tabs than fit into the window)
  // extend borders and prevent system from overdrawing our new pixels
  if (m_bTop) {
    ::SetPixel(dc, m_width - 5, m_height - 8, hilight);
    ::SetPixel(dc, m_width - 5, m_height - 7, light);
    ::SetPixel(dc, m_width - 6, m_height - 8, hilight);
    ::SetPixel(dc, m_width - 6, m_height - 7, light);
    ::ExcludeClipRect(dc, 0, m_height - 6, m_width, m_height - 2);
    ::ExcludeClipRect(dc, m_width - 6, m_height - 8, m_width - 2, m_height - 6);
  } else {
    ::SetPixel(dc, m_width - 5, 2, shadow);
    ::SetPixel(dc, m_width - 5, 3, dark);
    ::SetPixel(dc, m_width - 6, 2, shadow);
    ::SetPixel(dc, m_width - 6, 3, dark);
    ::ExcludeClipRect(dc, 0, 0, m_width, 2);
    ::ExcludeClipRect(dc, m_width - 6, 2, m_width - 2, 4);
  }

  // windows should draw the control as usual
  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
  pThreadState->m_lastSentMsg.wParam = WPARAM(HDC(dc));
  Default();

  // extend the horizontal border to the left margin
  if (m_bTop) {
    ::SetPixel(dc, 0, m_height - 8, hilight);
    ::SetPixel(dc, 0, m_height - 7, light);
  } else {
    ::SetPixel(dc, 0, 2, shadow);
  }

  // special drawing if the leftmost tab is selected
  CRect rect;
  GetItemRect(GetCurSel(), rect);
  if (rect.left == 2) // is at the leftmost position a tab selected?
  {
    // if yes, remove the leftmost white line and extend the bottom border of the tab
    int j = m_bImages ? 1 : 0;

    HPEN pen = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
    HGDIOBJ old = ::SelectObject(dc, pen);
    ::MoveToEx(dc, 0, 2, NULL);
    ::LineTo(dc, 0, 22 + j);
    ::MoveToEx(dc, 1, 2, NULL);
    ::LineTo(dc, 1, 22 + j);
    ::SelectObject(dc, old);
    ::DeleteObject(pen);

    if (m_bTop) {
      ::SetPixel(dc, 0, 0, hilight);
      ::SetPixel(dc, 1, 0, hilight);
      ::SetPixel(dc, 0, 1, light);
      ::SetPixel(dc, 1, 1, light);
    } else {
      ::SetPixel(dc, 0, 22 + j, shadow);
      ::SetPixel(dc, 1, 22 + j, shadow);
      ::SetPixel(dc, 0, 23 + j, dark);
      ::SetPixel(dc, 1, 23 + j, dark);
    }
  }
}

void CMDITabs::OnNcPaint()
{
  HDC hdc = ::GetWindowDC(m_hWnd);

  CRect rect;
  rect.left = 0;
  rect.top = m_bTop ? 0 : -2;
  rect.right = m_width;
  rect.bottom = m_height;

  HPEN pen = ::CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DFACE));
  HGDIOBJ old = ::SelectObject(hdc, pen);
  if (m_bTop) {
    DrawEdge(hdc, rect, EDGE_SUNKEN, BF_LEFT | BF_RIGHT | BF_TOP);
    ::MoveToEx(hdc, 2, m_height - 1, NULL);
    ::LineTo(hdc, m_width - 2, m_height - 1);
    ::MoveToEx(hdc, 2, m_height - 2, NULL);
    ::LineTo(hdc, m_width - 2, m_height - 2);
  } else {
    DrawEdge(hdc, rect, EDGE_SUNKEN, BF_LEFT | BF_RIGHT | BF_BOTTOM);
    ::MoveToEx(hdc, 2, 0, NULL);
    ::LineTo(hdc, m_width - 2, 0);
    ::MoveToEx(hdc, 2, 1, NULL);
    ::LineTo(hdc, m_width - 2, 1);
  }
  ::SelectObject(hdc, old);
  ::DeleteObject(pen);
  ::ReleaseDC(m_hWnd, hdc);
}

void CMDITabs::Create(CFrameWnd* pMainFrame, DWORD dwStyle)
{
  m_bTop = (dwStyle & MT_TOP);
  m_minViews = (dwStyle & MT_HIDEWLT2VIEWS) ? 2 : 1;

  CTabCtrl::Create(WS_CHILD | WS_VISIBLE | (m_bTop ? 0 : TCS_BOTTOM) | TCS_SINGLELINE | TCS_FOCUSNEVER | TCS_FORCEICONLEFT | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), pMainFrame, 42);
  ModifyStyleEx(0, WS_EX_CLIENTEDGE);
  SendMessage(WM_SETFONT, WPARAM(GetStockObject(DEFAULT_GUI_FONT)), 0);

  HWND wnd;
  for (wnd = ::GetTopWindow(*pMainFrame); wnd; wnd = ::GetNextWindow(wnd, GW_HWNDNEXT)) {
    char wndClass[32];
    ::GetClassName(wnd, wndClass, 32);
    if (strncmp(wndClass, "MDIClient", 32) == 0)
      break;
  }
  m_mdiClient = wnd;

  ASSERT(m_mdiClient); // Ooops, no MDIClient window?

  // manipulate Z-order so, that our tabctrl is above the mdi client, but below any status bar
  ::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  ::SetWindowPos(m_mdiClient, m_hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  m_bImages = (dwStyle & MT_IMAGES) != 0;
  if (m_bImages) {
    if (m_images.GetSafeHandle()) {
      m_images.SetImageCount(0);
    } else {
      m_images.Create(16, 16, ILC_COLORDDB | ILC_MASK, 1, 1);
    }
    SetImageList(&m_images);
  }

  // SetItemSize(CSize(50, 0)); // Fixed Width Experiment
}

void CMDITabs::OnContextMenu(CWnd* pWnd, CPoint point)
{
  TCHITTESTINFO hit;
  hit.pt = point;
  ScreenToClient(&hit.pt);
  int i = HitTest(&hit);
  if (i >= 0) {
    TCITEM item;
    item.mask = TCIF_PARAM;
    GetItem(i, &item);

    HWND hWnd = HWND(item.lParam);
    SetCurSel(i);
    ::BringWindowToTop(hWnd);

    HMENU menu = HMENU(::SendMessage(::GetTopWindow(hWnd), WM_GETTABSYSMENU, 0, 0));
    if (menu == 0)
      menu = ::GetSystemMenu(hWnd, FALSE);
    UINT cmd = ::TrackPopupMenu(menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_VCENTERALIGN, point.x, point.y, 0, m_hWnd, NULL);
    ::SendMessage(hWnd, WM_SYSCOMMAND, cmd, 0);
  }
}

void CMDITabs::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  int i = GetCurSel();
  if (i >= 0) {
    TCITEM item;
    item.mask = TCIF_PARAM;
    GetItem(i, &item);
    HWND hWnd = HWND(item.lParam);
    ::ShowWindow(hWnd, SW_MAXIMIZE);
  }
}
