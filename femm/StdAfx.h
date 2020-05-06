// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__744766B7_69F6_473E_A1A8_F1100DCD5ECC__INCLUDED_)
#define AFX_STDAFX_H__744766B7_69F6_473E_A1A8_F1100DCD5ECC__INCLUDED_

#define _CRT_SECURE_NO_WARNINGS

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers

#include <afxwin.h> // MFC core and standard components
#include <afxext.h> // MFC extensions
#include <afxdisp.h> // MFC Automation classes
#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h> // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// This macro is the same as IMPLEMENT_OLECREATE, except it passes TRUE
//  for the bMultiInstance parameter to the COleObjectFactory constructor.
//  We want a separate instance of this application to be launched for
//  each automation proxy object requested by automation controllers.
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
  AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid,                              \
      RUNTIME_CLASS(class_name), TRUE, _T(external_name));                                         \
  const AFX_DATADEF GUID class_name::guid = { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };
#endif // IMPLEMENT_OLECREATE2

// need this for CArray support
#include <afx.h>
#include <afxtempl.h>

#include <vector>
#include <math.h>
#define PI 3.141592653589793238462643383
#define muo 1.2566370614359173e-6
#define eo 8.85418781762e-12
#define DEGREE 0.017453292519943295
#define round(x) floor(x + 0.5)

#define PLANAR 0
#define AXISYMMETRIC 1

// uncomment the following if you want to compile with MathLink support.
// #define MATHLINK

// commend the following line if you don't want a tabbed MDI interface.
#define MDITAB

#include "complex.h"

// kludge to put lua support into all edit boxes
#include "luaDDX.h"
#define DDX_Text Lua_DDX_Text
#include "LuaEdit.h"
#include "OutBox.h"
class CZPix {
  public:
  int x, y;
  COLORREF c;
};

class CPixel {
  public:
  int z;
  COLORREF c;
};

class CFolderProp {
  public:
  CString FolderName;
  CString FolderURL;
  CString FolderVendor;

  private:
};

int MsgBox(PSTR sz, ...);
int MsgBox(CString s);

double sq(double x);
CString ToString(CComplex x);
CString ToString(CComplex x, int n);

// preprocessor tolerance
#define CLOSE_ENOUGH 1.e-06

// Default mesh size is the diagonal of the geometry's
// bounding box divided by BoundingBoxFraction
#define BoundingBoxFraction 100.0
#define LineFraction 500.0
#define DEFAULT_MINIMUM_ANGLE 30.0
// kludge to make mesh from Triangle 1.6 have about the
// same mesh density as Triangle 1.3 for consistent "smartmesh" results
#define MINANGLE_BUMP 3
#define MINANGLE_MAX 33.8

// definition needed for PtLoc
enum LocateResult { eOnVertex,
  eOnEdge,
  eInTriangle,
  eOutSide };

// different sorts of bLinehook cases
#define NormalLua 1
#define HiddenLua 2
#define ImportDXF 3
#define BuildMask 4

//////////////////////////////
// Default Colors           //
//////////////////////////////

// Greyscale Colormap
#define dGrey00 RGB(55, 55, 55)
#define dGrey01 RGB(65, 65, 65)
#define dGrey02 RGB(75, 75, 75)
#define dGrey03 RGB(85, 85, 85)
#define dGrey04 RGB(95, 95, 95)
#define dGrey05 RGB(105, 105, 105)
#define dGrey06 RGB(115, 115, 115)
#define dGrey07 RGB(125, 125, 125)
#define dGrey08 RGB(135, 135, 135)
#define dGrey09 RGB(145, 145, 145)
#define dGrey10 RGB(155, 155, 155)
#define dGrey11 RGB(165, 165, 165)
#define dGrey12 RGB(175, 175, 175)
#define dGrey13 RGB(185, 185, 185)
#define dGrey14 RGB(195, 195, 195)
#define dGrey15 RGB(205, 205, 205)
#define dGrey16 RGB(215, 215, 215)
#define dGrey17 RGB(225, 225, 225)
#define dGrey18 RGB(235, 235, 235)
#define dGrey19 RGB(245, 245, 245)

// BELA Colormap
#define dColor19 RGB(0, 255, 255)
#define dColor18 RGB(37, 255, 195)
#define dColor17 RGB(69, 255, 147)
#define dColor16 RGB(98, 255, 108)
#define dColor15 RGB(123, 255, 76)
#define dColor14 RGB(148, 255, 51)
#define dColor13 RGB(171, 255, 31)
#define dColor12 RGB(194, 255, 16)
#define dColor11 RGB(217, 255, 6)
#define dColor10 RGB(242, 255, 1)
#define dColor09 RGB(255, 242, 1)
#define dColor08 RGB(255, 217, 6)
#define dColor07 RGB(255, 194, 16)
#define dColor06 RGB(255, 171, 31)
#define dColor05 RGB(255, 148, 51)
#define dColor04 RGB(255, 123, 76)
#define dColor03 RGB(255, 98, 108)
#define dColor02 RGB(255, 69, 147)
#define dColor01 RGB(255, 37, 195)
#define dColor00 RGB(255, 0, 255)

// Other colors
#define dSelColor RGB(255, 0, 0)
#define dMeshColor RGB(213, 228, 20)
#define dBlockColor RGB(0, 125, 0)
#define dLineColor RGB(0, 0, 255)
#define dRegionColor RGB(0, 255, 0)
#define dGridColor RGB(0, 0, 255)
#define dBackColor RGB(255, 255, 255)
#define dNodeColor RGB(0, 0, 0)
#define dTextColor RGB(0, 0, 0)
#define dRealFluxLineColor RGB(0, 0, 0)
#define dImagFluxLineColor RGB(128, 128, 128)
#define dNameColor RGB(0, 0, 0)
#define dMaskLineColor RGB(255, 128, 64)
#define dRealVectorColor RGB(0, 0, 0)
#define dImagVectorColor RGB(128, 128, 128)

#define lua_todouble(lua, n) Re(lua_tonumber(lua, n))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__744766B7_69F6_473E_A1A8_F1100DCD5ECC__INCLUDED_)
