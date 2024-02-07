// femmviewDoc.cpp : implementation of the CFemmviewDoc class
//

#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "problem.h"
#include "femm.h"
#include "xyplot.h"
#include "femmviewDoc.h"
#include "femmviewView.h"
#include "lua.h"

extern lua_State* lua;
extern void* pFemmviewdoc;
extern CLuaConsoleDlg* LuaConsole;
extern BOOL bLinehook;

extern void lua_baselibopen(lua_State* L);
extern void lua_iolibopen(lua_State* L);
extern void lua_strlibopen(lua_State* L);
extern void lua_mathlibopen(lua_State* L);
extern void lua_dblibopen(lua_State* L);

char* ParseDbl(char* t, double* f);
char* ParseString(char* t, CString* s);
char* ParseInt(char* t, int* f);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFemmviewDoc

IMPLEMENT_DYNCREATE(CFemmviewDoc, CDocument)

BEGIN_MESSAGE_MAP(CFemmviewDoc, CDocument)
//{{AFX_MSG_MAP(CFemmviewDoc)
//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFemmviewDoc construction/destruction

double sqr(double x)
{
  return x * x;
}

CFemmviewDoc::CFemmviewDoc()
{
  // set some default values for problem definition
  d_LineIntegralPoints = 400;
  d_ShiftH = TRUE;
  d_WeightingScheme = 0;
  Frequency = 0.;
  Depth = 1 / 0.0254;
  LengthUnits = 0;
  ProblemType = FALSE;
  ProblemNote = "Add comments here.";
  PrevSoln = "";
  PrevType = 0;
  bIncremental = FALSE;
  FirstDraw = -1;
  A_High = 0.;
  A_Low = 0.;
  A_lb = 0.;
  A_ub = 0.;
  extRo = extRi = extZo = 0;
  Smooth = TRUE;
  NumList = NULL;
  ConList = NULL;
  WeightingScheme = 0;
  bHasMask = FALSE;
  LengthConv = (double*)calloc(6, sizeof(double));
  LengthConv[0] = 0.0254; // inches
  LengthConv[1] = 0.001; // millimeters
  LengthConv[2] = 0.01; // centimeters
  LengthConv[3] = 1.; // meters
  LengthConv[4] = 2.54e-05; // mils
  LengthConv[5] = 1.e-06; // micrometers
  Coords = FALSE;

  for (int i = 0; i < 10; i++)
    d_PlotBounds[i][0] = d_PlotBounds[i][1] = PlotBounds[i][0] = PlotBounds[i][1] = 0;

  // determine path to bin directory
  BinDir = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  ScanPreferences();

  WeightingScheme = d_WeightingScheme;

  // lua initialization stuff
  initalise_lua();
}

CFemmviewDoc::~CFemmviewDoc()
{
  int i;
  free(LengthConv);
  for (i = 0; i < meshnode.GetSize(); i++)
    if (ConList[i] != NULL)
      free(ConList[i]);
  free(ConList);
  free(NumList);
  if (pFemmviewdoc == this)
    pFemmviewdoc = NULL;

  for (i = 0; i < agelist.GetSize(); i++) {
    if (agelist[i].qp != NULL)
      free(agelist[i].qp); // list boundary nodes & weights
    if (agelist[i].btc != NULL)
      free(agelist[i].btc);
    if (agelist[i].bts != NULL)
      free(agelist[i].bts);
    if (agelist[i].brc != NULL)
      free(agelist[i].brc);
    if (agelist[i].brs != NULL)
      free(agelist[i].brs);
    if (agelist[i].nh != NULL)
      free(agelist[i].nh);
    if (agelist[i].btcPrev != NULL)
      free(agelist[i].btcPrev);
    if (agelist[i].btsPrev != NULL)
      free(agelist[i].btsPrev);
    if (agelist[i].brcPrev != NULL)
      free(agelist[i].brcPrev);
    if (agelist[i].brsPrev != NULL)
      free(agelist[i].brsPrev);

    if (agelist[i].brPrev != NULL)
      free(agelist[i].brPrev);
    if (agelist[i].btPrev != NULL)
      free(agelist[i].btPrev);
    if (agelist[i].br != NULL)
      free(agelist[i].br);
    if (agelist[i].bt != NULL)
      free(agelist[i].bt);
  }
}

BOOL CFemmviewDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)
  int i;

  // clear out all current lines, nodes, and block labels
  if (NumList != NULL) {
    for (i = 0; i < meshnode.GetSize(); i++)
      if (ConList[i] != NULL)
        free(ConList[i]);
    free(ConList);
    ConList = NULL;
    free(NumList);
    NumList = NULL;
  }
  nodelist.RemoveAll();
  linelist.RemoveAll();
  blocklist.RemoveAll();
  arclist.RemoveAll();
  nodeproplist.RemoveAll();
  lineproplist.RemoveAll();
  blockproplist.RemoveAll();
  circproplist.RemoveAll();
  meshnode.RemoveAll();
  meshelem.RemoveAll();
  contour.RemoveAll();

  for (i = 0; i < agelist.GetSize(); i++) {
    if (agelist[i].qp != NULL)
      free(agelist[i].qp); // list of nod
    if (agelist[i].btc != NULL)
      free(agelist[i].btc);
    if (agelist[i].bts != NULL)
      free(agelist[i].bts);
    if (agelist[i].brc != NULL)
      free(agelist[i].brc);
    if (agelist[i].brs != NULL)
      free(agelist[i].brs);
    if (agelist[i].nh != NULL)
      free(agelist[i].nh);
    if (agelist[i].btcPrev != NULL)
      free(agelist[i].btcPrev);
    if (agelist[i].btsPrev != NULL)
      free(agelist[i].btsPrev);
    if (agelist[i].brcPrev != NULL)
      free(agelist[i].brcPrev);
    if (agelist[i].brsPrev != NULL)
      free(agelist[i].brsPrev);

    if (agelist[i].brPrev != NULL)
      free(agelist[i].brPrev);
    if (agelist[i].btPrev != NULL)
      free(agelist[i].btPrev);
    if (agelist[i].br != NULL)
      free(agelist[i].br);
    if (agelist[i].bt != NULL)
      free(agelist[i].bt);
  }
  agelist.RemoveAll();

  // set problem attributes to generic ones;
  Frequency = 0.;
  LengthUnits = 0;
  ProblemType = FALSE;
  ProblemNote = "Add comments here.";
  bHasMask = FALSE;
  extRo = extRi = extZo = 0;

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFemmviewDoc serialization

void CFemmviewDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) {
    // TODO: add storing code here
  } else {
    // TODO: add loading code here
  }
}

/////////////////////////////////////////////////////////////////////////////
// CFemmviewDoc diagnostics

#ifdef _DEBUG
void CFemmviewDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CFemmviewDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFemmviewDoc commands

BOOL CFemmviewDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  FILE* fp;
  int i, j, k, t;
  char s[1024], q[1024];
  char* v;
  double b, bi, br;
  double zr, zi;
  BOOL flag = FALSE;
  CPointProp PProp;
  CBoundaryProp BProp;
  CMaterialProp MProp;
  CCircuit CProp;
  CNode node;
  CSegment segm;
  CArcSegment asegm;
  CElement elm;
  CBlockLabel blk;
  CMeshNode mnode;
  CPoint mline;

  // make sure old document is cleared out...
  if (NumList != NULL) {
    for (i = 0; i < meshnode.GetSize(); i++)
      if (ConList[i] != NULL)
        free(ConList[i]);
    free(ConList);
    ConList = NULL;
    free(NumList);
    NumList = NULL;
  }
  nodelist.RemoveAll();
  linelist.RemoveAll();
  blocklist.RemoveAll();
  arclist.RemoveAll();
  nodeproplist.RemoveAll();
  lineproplist.RemoveAll();
  blockproplist.RemoveAll();
  circproplist.RemoveAll();
  meshnode.RemoveAll();
  meshelem.RemoveAll();
  contour.RemoveAll();

  if ((fp = fopen(lpszPathName, "rt")) == NULL) {
    MsgBox("Couldn't read from specified .poly file");
    return FALSE;
  }

  // define some defaults
  Frequency = 0.;
  ProblemType = 0;
  Coords = 0;
  ProblemNote = "";
  bHasMask = FALSE;
  Depth = -1;

  // parse the file
  while (flag == FALSE) {
    fgets(s, 1024, fp);
    sscanf(s, "%s", q);

    // Deal with flag for file format version
    if (_strnicmp(q, "[format]", 8) == 0) {
      double vers;
      v = StripKey(s);
      sscanf(v, "%lf", &vers);
      vers = 10. * vers + 0.5;
      if (((int)vers) != 40) {
        MsgBox("This file is from a different version of FEMM\nRe-analyze the problem using the current version.");
        fclose(fp);
        return FALSE;
      }
      q[0] = NULL;
    }

    // Frequency of the problem
    if (_strnicmp(q, "[frequency]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &Frequency);
      Frequency = fabs(Frequency);
      q[0] = NULL;
    }

    // Frequency of the problem
    if (_strnicmp(q, "[depth]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &Depth);
      q[0] = NULL;
    }

    // Units of length used by the problem
    if (_strnicmp(q, "[lengthunits]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%s", q);
      if (_strnicmp(q, "inches", 6) == 0)
        LengthUnits = 0;
      else if (_strnicmp(q, "millimeters", 11) == 0)
        LengthUnits = 1;
      else if (_strnicmp(q, "centimeters", 1) == 0)
        LengthUnits = 2;
      else if (_strnicmp(q, "mils", 4) == 0)
        LengthUnits = 4;
      else if (_strnicmp(q, "microns", 6) == 0)
        LengthUnits = 5;
      else if (_strnicmp(q, "meters", 6) == 0)
        LengthUnits = 3;
      q[0] = NULL;
    }

    // Problem Type (planar or axisymmetric)
    if (_strnicmp(q, "[problemtype]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%s", q);
      if (_strnicmp(q, "planar", 6) == 0)
        ProblemType = 0;
      if (_strnicmp(q, "axisymmetric", 3) == 0)
        ProblemType = 1;
      q[0] = NULL;
    }

    // Coordinates (cartesian or polar)
    if (_strnicmp(q, "[coordinates]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%s", q);
      if (_strnicmp(q, "cartesian", 4) == 0)
        Coords = 0;
      if (_strnicmp(q, "polar", 5) == 0)
        Coords = 1;
      q[0] = NULL;
    }

    // Comments
    if (_strnicmp(q, "[comment]", 9) == 0) {
      v = StripKey(s);
      // put in carriage returns;
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if ((v[i] == '\\') && (v[i + 1] == 'n')) {
          v[i] = 13;
          v[i + 1] = 10;
        }

      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      ProblemNote = v;
      q[0] = NULL;
    }

    // properties for axisymmetric external region
    if (_strnicmp(q, "[extzo]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &extZo);
      q[0] = NULL;
    }

    if (_strnicmp(q, "[extro]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &extRo);
      q[0] = NULL;
    }

    if (_strnicmp(q, "[extri]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &extRi);
      q[0] = NULL;
    }

    // name of previous solution file for AC incremental permeability solution
    // Previous Solution File
    if (_strnicmp(q, "[prevsoln]", 10) == 0) {
      int i;
      v = StripKey(s);

      // have to do this carefully to accept a filename with spaces
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }

      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }

      PrevSoln = v;
      if (PrevSoln.GetLength() > 0)
        bIncremental = PrevType;
      else
        bIncremental = FALSE;
      q[0] = NULL;
    }

    if (_strnicmp(q, "[prevtype]", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &PrevType);
      q[0] = NULL;
      // 0 == None
      // 1 == Incremental
      // 2 == Frozen
    }

    // Point Properties
    if (_strnicmp(q, "<beginpoint>", 11) == 0) {
      PProp.PointName = "New Point Property";
      PProp.Jr = 0.;
      PProp.Ji = 0.;
      PProp.Ar = 0.;
      PProp.Ai = 0.;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<pointname>", 11) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      PProp.PointName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<A_re>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.Ar);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<A_im>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.Ai);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<I_re>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.Jr);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<I_im>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.Ji);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endpoint>", 9) == 0) {
      nodeproplist.Add(PProp);
      q[0] = NULL;
    }

    // Boundary Properties;
    if (_strnicmp(q, "<beginbdry>", 11) == 0) {
      BProp.BdryName = "New Boundary";
      BProp.BdryFormat = 0;
      BProp.A0 = 0.;
      BProp.A1 = 0.;
      BProp.A2 = 0.;
      BProp.phi = 0.;
      BProp.Mu = 0.;
      BProp.Sig = 0.;
      BProp.c0 = 0.;
      BProp.c1 = 0.;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<bdryname>", 10) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      BProp.BdryName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<bdrytype>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &BProp.BdryFormat);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<mu_ssd>", 8) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.Mu);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<sigma_ssd>", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.Sig);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<A_0>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.A0);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<A_1>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.A1);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<A_2>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.A2);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<phi>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.phi);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c0>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c0.re);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c1>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c1.re);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c0i>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c0.im);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c1i>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c1.im);
      q[0] = NULL;
    }
    if (_strnicmp(q, "<endbdry>", 9) == 0) {
      lineproplist.Add(BProp);
      q[0] = NULL;
    }

    // Block Properties;
    if (_strnicmp(q, "<beginblock>", 12) == 0) {
      MProp.BlockName = "New Material";
      MProp.mu_x = 1.;
      MProp.mu_y = 1.; // permeabilities, relative
      MProp.H_c = 0.; // magnetization, A/m
      MProp.Jr = 0.;
      MProp.Ji = 0.; // applied current density, MA/m^2
      MProp.Cduct = 0.; // conductivity of the material, MS/m
      MProp.Lam_d = 0.; // lamination thickness, mm
      MProp.Theta_hn = 0.; // hysteresis angle, degrees
      MProp.Theta_hx = 0.; // hysteresis angle, degrees
      MProp.Theta_hy = 0.; // hysteresis angle, degrees
      MProp.NStrands = 0;
      MProp.WireD = 0;
      MProp.LamFill = 1.; // lamination fill factor;
      MProp.LamType = 0; // type of lamination;
      MProp.BHpoints = 0;
      MProp.MuMax = 0;
      MProp.Frequency = Frequency;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<blockname>", 10) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      MProp.BlockName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<mu_x>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.mu_x);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<mu_y>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.mu_y);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<H_c>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.H_c);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<J_re>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Jr);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<J_im>", 6) == 0) {
      v = StripKey(s);
      if (Frequency != 0)
        sscanf(v, "%lf", &MProp.Ji);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<sigma>", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Cduct);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<phi_h>", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Theta_hn);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<phi_hx>", 8) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Theta_hx);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<phi_hy>", 8) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Theta_hy);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<d_lam>", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Lam_d);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<LamFill>", 8) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.LamFill);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<LamType>", 9) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &MProp.LamType);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<NStrands>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &MProp.NStrands);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<WireD>", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.WireD);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<BHPoints>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &MProp.BHpoints);
      if (MProp.BHpoints > 0) {
        MProp.Hdata = (CComplex*)calloc(MProp.BHpoints, sizeof(CComplex));
        MProp.Bdata = (double*)calloc(MProp.BHpoints, sizeof(double));
        for (j = 0; j < MProp.BHpoints; j++) {
          fgets(s, 1024, fp);
          MProp.Hdata[j] = 0;
          sscanf(s, "%lf	%lf", &MProp.Bdata[j], &MProp.Hdata[j].re);
        }
      }
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endblock>", 9) == 0) {
      if (MProp.BHpoints > 0) {
        if (bIncremental != 0) {
          // first time through was just to get MuMax from AC curve...
          CComplex* tmpHdata = (CComplex*)calloc(MProp.BHpoints, sizeof(CComplex));
          double* tmpBdata = (double*)calloc(MProp.BHpoints, sizeof(double));
          for (i = 0; i < MProp.BHpoints; i++) {
            tmpHdata[i] = MProp.Hdata[i];
            tmpBdata[i] = MProp.Bdata[i];
          }
          MProp.GetSlopes(Frequency * 2. * PI);
          for (i = 0; i < MProp.BHpoints; i++) {
            MProp.Hdata[i] = tmpHdata[i];
            MProp.Bdata[i] = tmpBdata[i];
          }
          free(tmpHdata);
          free(tmpBdata);
          free(MProp.slope);
          MProp.slope = NULL;

          // set a flag for DC incremental permeability problems
          if ((bIncremental == TRUE) && (Frequency == 0))
            MProp.MuMax = 1;

          // second time through is to get the DC curve
          MProp.GetSlopes(0);
        } else {
          MProp.GetSlopes(Frequency * 2. * PI);
          MProp.MuMax = 0; // this is the hint to the materials prop that this is _not_ incremental
        }
      }
      blockproplist.Add(MProp);
      MProp.BHpoints = 0;
      MProp.Bdata = NULL;
      MProp.Hdata = NULL;
      MProp.slope = NULL;
      q[0] = NULL;
    }

    // Circuit Properties
    if (_strnicmp(q, "<begincircuit>", 14) == 0) {
      CProp.CircName = "New Circuit";
      CProp.CircType = 0;
      CProp.Amps = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<circuitname>", 13) == 0) {
      v = StripKey(s);
      k = (int)strlen(v);
      for (i = 0; i < k; i++)
        if (v[i] == '\"') {
          v = v + i + 1;
          i = k;
        }
      k = (int)strlen(v);
      if (k > 0)
        for (i = k - 1; i >= 0; i--) {
          if (v[i] == '\"') {
            v[i] = 0;
            i = -1;
          }
        }
      CProp.CircName = v;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<totalamps_re>", 14) == 0) {
      double inval;
      v = StripKey(s);
      sscanf(v, "%lf", &inval);
      CProp.Amps += inval;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<totalamps_im>", 14) == 0) {
      double inval;
      v = StripKey(s);
      sscanf(v, "%lf", &inval);
      if (Frequency != 0)
        CProp.Amps += (I * inval);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<circuittype>", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &CProp.CircType);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endcircuit>", 12) == 0) {
      circproplist.Add(CProp);
      q[0] = NULL;
    }

    // Points list;
    if (_strnicmp(q, "[numpoints]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);
        sscanf(s, "%lf	%lf	%i\n", &node.x, &node.y, &t);
        node.BoundaryMarker = t - 1;
        nodelist.Add(node);
      }
      q[0] = NULL;
    }

    // read in segment list
    if (_strnicmp(q, "[numsegments]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);
        sscanf(s, "%i	%i	%lf %i	%i	%i\n", &segm.n0, &segm.n1, &segm.MaxSideLength, &t, &segm.Hidden, &segm.InGroup);
        segm.BoundaryMarker = t - 1;
        linelist.Add(segm);
      }
      q[0] = NULL;
    }

    // read in arc segment list
    if (_strnicmp(q, "[numarcsegments]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);
        sscanf(s, "%i	%i	%lf	%lf %i	%i	%i	%lf\n", &asegm.n0, &asegm.n1,
            &asegm.ArcLength, &asegm.MaxSideLength, &t, &asegm.Hidden, &asegm.InGroup, &b);
        asegm.BoundaryMarker = t - 1;
        if (b > 0)
          asegm.MaxSideLength = b; // use as-meshed max side length for display purposes
        arclist.Add(asegm);
      }
      q[0] = NULL;
    }

    // read in list of holes;
    if (_strnicmp(q, "[numholes]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0) {
        blk.BlockType = -1;
        blk.MaxArea = 0;
        for (i = 0; i < k; i++) {
          fgets(s, 1024, fp);
          sscanf(s, "%lf	%lf\n", &blk.x, &blk.y);
          //	blocklist.Add(blk);
          //  don't add holes to the list
          //  of block labels because it messes up the
          //  number of block labels.
        }
      }
      q[0] = NULL;
    }

    // read in regional attributes
    if (_strnicmp(q, "[numblocklabels]", 13) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);

        // some defaults
        blk.MaxArea = 0.;
        blk.MagDir = 0.;
        blk.MagDirFctn.Empty();
        blk.Turns = 1;
        blk.InCircuit = 0;
        blk.InGroup = 0;
        blk.IsExternal = 0;
        blk.IsDefault = 0;

        // scan in data
        v = ParseDbl(s, &blk.x);
        v = ParseDbl(v, &blk.y);
        v = ParseInt(v, &blk.BlockType);
        v = ParseDbl(v, &blk.MaxArea);
        v = ParseInt(v, &blk.InCircuit);
        v = ParseDbl(v, &blk.MagDir);
        v = ParseInt(v, &blk.InGroup);
        v = ParseInt(v, &blk.Turns);
        v = ParseInt(v, &blk.IsExternal);
        blk.IsDefault = blk.IsExternal & 2;
        blk.IsExternal = blk.IsExternal & 1;
        v = ParseString(v, &blk.MagDirFctn);

        if (blk.MaxArea < 0)
          blk.MaxArea = 0;
        else
          blk.MaxArea = PI * blk.MaxArea * blk.MaxArea / 4.;
        blk.BlockType -= 1;
        blk.InCircuit -= 1;
        blocklist.Add(blk);
      }
      q[0] = NULL;
    }

    if (_strnicmp(q, "[solution]", 10) == 0) {
      flag = TRUE;
      q[0] = NULL;
    }
  }

  // read in meshnodes;
  fscanf(fp, "%i\n", &k);
  meshnode.SetSize(k);
  for (i = 0; i < k; i++) {
    fgets(s, 1024, fp);
    if (Frequency != 0) {
      int bc;

      if (!bIncremental)
        sscanf(s, "%lf	%lf	%lf	%lf", &mnode.x, &mnode.y, &mnode.A.re, &mnode.A.im);
      else
        sscanf(s, "%lf	%lf	%lf	%lf	%i	%lf", &mnode.x, &mnode.y, &mnode.A.re, &mnode.A.im, &bc, &mnode.Aprev);
    } else {
      int bc;

      if (!bIncremental)
        sscanf(s, "%lf	%lf	%lf", &mnode.x, &mnode.y, &mnode.A.re);
      else
        sscanf(s, "%lf	%lf	%lf	%i	%lf", &mnode.x, &mnode.y, &mnode.A.re, &bc, &mnode.Aprev);
      mnode.A.im = 0;
    }
    meshnode.SetAt(i, mnode);
  }

  // read in elements;
  fscanf(fp, "%i\n", &k);
  meshelem.SetSize(k);
  for (i = 0; i < k; i++) {
    fgets(s, 1024, fp);
    if (!bIncremental)
      sscanf(s, "%i	%i	%i	%i", &elm.p[0], &elm.p[1], &elm.p[2], &elm.lbl);
    else
      sscanf(s, "%i	%i	%i	%i	%lf", &elm.p[0], &elm.p[1], &elm.p[2], &elm.lbl, &elm.Jp);
    elm.blk = blocklist[elm.lbl].BlockType;
    meshelem.SetAt(i, elm);
  }

  // read in circuit data;
  fscanf(fp, "%i\n", &k);
  for (i = 0; i < k; i++) {
    fgets(s, 1024, fp);
    if (Frequency == 0) {
      sscanf(s, "%i	%lf", &j, &zr);
      blocklist[i].Case = j;
      if (j == 0)
        blocklist[i].dVolts = zr;
      else
        blocklist[i].J = zr;
    } else {
      sscanf(s, "%i	%lf	%lf", &j, &zr, &zi);
      blocklist[i].Case = j;
      if (j == 0)
        blocklist[i].dVolts = zr + I * zi;
      else
        blocklist[i].J = zr + I * zi;
    }
  }

  // femmview doesn't actively use PBC data, but it needs to read it to get to the
  // air gap element data beyond
  if (fgets(s, 1024, fp) != NULL) {
    sscanf(s, "%i", &k);
    for (i = 0; i < k; i++)
      fgets(s, 1024, fp);
  }

  // Read in Air Gap Element information
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);
  for (i = 0; i < k; i++) {
    CAirGapElement age;

    fgets(s, 1024, fp);
    age.BdryName = s;
    age.BdryName.Replace("\"", "");
    age.BdryName.Replace("\n", "");
    fgets(s, 1024, fp);
    sscanf(s, "%i %lf %lf %lf %lf %lf %lf %lf %i %lf %lf",
        &age.BdryFormat, &age.InnerAngle, &age.OuterAngle,
        &age.ri, &age.ro, &age.totalArcLength,
        &age.agc.re, &age.agc.im, &age.totalArcElements,
        &age.InnerShift, &age.OuterShift);

    age.ri *= LengthConv[LengthUnits];
    age.ro *= LengthConv[LengthUnits];

    // allocate space
    if (age.totalArcElements > 0) {
      j = age.totalArcElements + 1;
      age.qp = (CQuadPoint*)calloc(j, sizeof(CQuadPoint)); // list of nodes on inner radius
    }

    for (j = 0; j <= age.totalArcElements; j++) {
      CQuadPoint q;

      fgets(s, 1024, fp);
      sscanf(s, "%i %lf %i %lf %i %lf %i %lf",
          &q.n0, &q.w0,
          &q.n1, &q.w1,
          &q.n2, &q.w2,
          &q.n3, &q.w3);
      age.qp[j] = q;
    }
    if (age.totalArcElements > 0)
      agelist.Add(age);
  }

  fclose(fp);

  // figure out amplitudes of harmonics for AGE boundary conditions
  for (i = 0; i < agelist.GetSize(); i++) {
    int m;
    double tta, R, dr, ri, ro, n, dt;
    CComplex brc, brs, btc, bts;
    double brcPrev, brsPrev, btcPrev, btsPrev;

    R = (agelist[i].ri + agelist[i].ro) / 2.;
    dr = (agelist[i].ro - agelist[i].ri);
    ri = agelist[i].ri / R;
    ro = agelist[i].ro / R;
    dt = (PI / 180.) * agelist[i].totalArcLength / ((double)agelist[i].totalArcElements);

    if (agelist[i].BdryFormat == 0) {
      agelist[i].nn = (agelist[i].totalArcElements / 2) + 1; // periodic AGE
      m = (int)round(360. / agelist[i].totalArcLength);
    } else {
      agelist[i].nn = (agelist[i].totalArcElements + 1) / 2; // antiperiodic AGE
      m = (int)round(180. / agelist[i].totalArcLength);
    }

    // for present solution
    agelist[i].brc = (CComplex*)calloc(agelist[i].nn, sizeof(CComplex));
    agelist[i].brs = (CComplex*)calloc(agelist[i].nn, sizeof(CComplex));
    agelist[i].btc = (CComplex*)calloc(agelist[i].nn, sizeof(CComplex));
    agelist[i].bts = (CComplex*)calloc(agelist[i].nn, sizeof(CComplex));
    agelist[i].br = (CComplex*)calloc(agelist[i].totalArcElements, sizeof(CComplex));
    agelist[i].bt = (CComplex*)calloc(agelist[i].totalArcElements, sizeof(CComplex));
    agelist[i].nh = (int*)calloc(agelist[i].nn, sizeof(int));

    // for previous solution;
    if (!bIncremental) {
      agelist[i].brcPrev = NULL;
      agelist[i].brsPrev = NULL;
      agelist[i].btcPrev = NULL;
      agelist[i].btsPrev = NULL;
      agelist[i].brPrev = NULL;
      agelist[i].btPrev = NULL;
    } else {
      agelist[i].brcPrev = (double*)calloc(agelist[i].nn, sizeof(double));
      agelist[i].brsPrev = (double*)calloc(agelist[i].nn, sizeof(double));
      agelist[i].btcPrev = (double*)calloc(agelist[i].nn, sizeof(double));
      agelist[i].btsPrev = (double*)calloc(agelist[i].nn, sizeof(double));
      agelist[i].brPrev = (double*)calloc(agelist[i].totalArcElements, sizeof(double));
      agelist[i].btPrev = (double*)calloc(agelist[i].totalArcElements, sizeof(double));
    }

    // compute A and B at center of each gap element
    agelist[i].aco = 0;
    for (k = 0; k < agelist[i].totalArcElements; k++) {
      int nn[10];
      double ww[10];
      int kk;
      CComplex a[10];
      CComplex ac;

      double ci = agelist[i].InnerShift;
      double co = agelist[i].OuterShift;

      // inner nodes
      if ((k - 1) < 0) {
        nn[0] = agelist[i].qp[agelist[i].totalArcElements - 1].n0;
        ww[0] = agelist[i].qp[agelist[i].totalArcElements - 1].w0;
      } else {
        nn[0] = agelist[i].qp[k - 1].n0;
        ww[0] = agelist[i].qp[k - 1].w0;
      }

      nn[1] = agelist[i].qp[k].n0;
      nn[2] = agelist[i].qp[k].n1;
      nn[3] = agelist[i].qp[k + 1].n1;
      ww[1] = agelist[i].qp[k].w0;
      ww[2] = agelist[i].qp[k].w1;
      ww[3] = agelist[i].qp[k + 1].w1;

      if ((k + 2) > agelist[i].totalArcElements) {
        nn[4] = agelist[i].qp[1].n1;
        ww[4] = agelist[i].qp[1].w1;
      } else {
        nn[4] = agelist[i].qp[k + 2].n1;
        ww[4] = agelist[i].qp[k + 2].w1;
      }

      // outer nodes
      if ((k - 1) < 0) {
        nn[5] = agelist[i].qp[agelist[i].totalArcElements - 1].n2;
        ww[5] = agelist[i].qp[agelist[i].totalArcElements - 1].w2;
      } else {
        nn[5] = agelist[i].qp[k - 1].n2;
        ww[5] = agelist[i].qp[k - 1].w2;
      }

      nn[6] = agelist[i].qp[k].n2;
      nn[7] = agelist[i].qp[k].n3;
      nn[8] = agelist[i].qp[k + 1].n3;
      ww[6] = agelist[i].qp[k].w2;
      ww[7] = agelist[i].qp[k].w3;
      ww[8] = agelist[i].qp[k + 1].w3;

      if ((k + 2) > agelist[i].totalArcElements) {
        nn[9] = agelist[i].qp[1].n3;
        ww[9] = agelist[i].qp[1].w3;
      } else {
        nn[9] = agelist[i].qp[k + 2].n3;
        ww[9] = agelist[i].qp[k + 2].w3;
      }

      // fix antiperiodic weights...
      if ((k == 0) && (agelist[i].BdryFormat == 1)) {
        ww[0] = -ww[0];
        ww[5] = -ww[5];
      }
      if (((k + 1) == agelist[i].totalArcElements) && (agelist[i].BdryFormat == 1)) {
        ww[4] = -ww[4];
        ww[9] = -ww[9];
      }

      for (kk = 0; kk < 10; kk++)
        a[kk] = meshnode[nn[kk]].A * ww[kk];

      // A at the center of the element
      if (agelist[i].BdryFormat == 0) {
        ac = (2 * a[2] + 2 * a[3] + 2 * a[7] + 2 * a[8] + a[1] * ci + (a[2] - a[3] - a[4]) * ci - (a[0] - 3 * a[1] + a[2] + 3 * a[3] - 2 * a[4]) * ci * ci + (a[0] - 2 * a[1] + 2 * a[3] - a[4]) * ci * ci * ci + (a[6] + a[7] - a[8] - a[9]) * co - (a[5] - 3 * a[6] + a[7] + 3 * a[8] - 2 * a[9]) * co * co + (a[5] - 2 * a[6] + 2 * a[8] - a[9]) * co * co * co) / 8.;
        agelist[i].aco += ac / ((double)agelist[i].totalArcElements);
      }

      // flux density for this element
      agelist[i].br[k] = (-(ci * a[1]) - 2 * a[2] + 2 * a[3] + ci * (a[2] + a[3] - a[4]) - ci * ci * ci * (a[0] - 4 * a[1] + 6 * a[2] - 4 * a[3] + a[4]) + ci * ci * (a[0] - 5 * a[1] + 9 * a[2] - 7 * a[3] + 2 * a[4]) - 2 * a[7] + 2 * a[8] + co * (-a[6] + a[7] + a[8] - a[9]) - co * co * co * (a[5] - 4 * a[6] + 6 * a[7] - 4 * a[8] + a[9]) + co * co * (a[5] - 5 * a[6] + 9 * a[7] - 7 * a[8] + 2 * a[9])) / (4 * dt * R);
      agelist[i].bt[k] = (ci * a[1] + 2 * a[2] + 2 * a[3] - ci * ci * (a[0] - 3 * a[1] + a[2] + 3 * a[3] - 2 * a[4]) + ci * (a[2] - a[3] - a[4]) + ci * ci * ci * (a[0] - 2 * a[1] + 2 * a[3] - a[4]) - co * a[6] + (-2 + co) * (1 + co) * a[7] - 2 * a[8] + co * (a[8] + co * (a[5] - 3 * a[6] + 3 * a[8] - 2 * a[9]) + a[9] + co * co * (-a[5] + 2 * a[6] - 2 * a[8] + a[9]))) / (4 * dr);
      if (bIncremental) {
        for (kk = 0; kk < 10; kk++)
          a[kk] = meshnode[nn[kk]].Aprev * ww[kk];
        agelist[i].brPrev[k] = Re((-(ci * a[1]) - 2 * a[2] + 2 * a[3] + ci * (a[2] + a[3] - a[4]) - ci * ci * ci * (a[0] - 4 * a[1] + 6 * a[2] - 4 * a[3] + a[4]) + ci * ci * (a[0] - 5 * a[1] + 9 * a[2] - 7 * a[3] + 2 * a[4]) - 2 * a[7] + 2 * a[8] + co * (-a[6] + a[7] + a[8] - a[9]) - co * co * co * (a[5] - 4 * a[6] + 6 * a[7] - 4 * a[8] + a[9]) + co * co * (a[5] - 5 * a[6] + 9 * a[7] - 7 * a[8] + 2 * a[9])) / (4 * dt * R));
        agelist[i].btPrev[k] = Re((ci * a[1] + 2 * a[2] + 2 * a[3] - ci * ci * (a[0] - 3 * a[1] + a[2] + 3 * a[3] - 2 * a[4]) + ci * (a[2] - a[3] - a[4]) + ci * ci * ci * (a[0] - 2 * a[1] + 2 * a[3] - a[4]) - co * a[6] + (-2 + co) * (1 + co) * a[7] - 2 * a[8] + co * (a[8] + co * (a[5] - 3 * a[6] + 3 * a[8] - 2 * a[9]) + a[9] + co * co * (-a[5] + 2 * a[6] - 2 * a[8] + a[9]))) / (4 * dr));
      }
    }

    // Convolve with sines and cosines to get amplitudes of each harmonic
    for (j = 0; j < agelist[i].nn; j++) {
      if (agelist[i].BdryFormat == 0)
        agelist[i].nh[j] = m * j;
      else
        agelist[i].nh[j] = m * (2 * j + 1);

      n = agelist[i].nh[j];
      brc = 0;
      brs = 0;
      btc = 0;
      bts = 0;
      brcPrev = 0;
      brsPrev = 0;
      btcPrev = 0;
      btsPrev = 0;
      for (k = 0; k < agelist[i].totalArcElements; k++) {
        tta = (((double)k) + 0.5) * dt;
        tta *= n; // multiply times # of harmonic under consideration

        brc += agelist[i].br[k] * cos(tta);
        brs += agelist[i].br[k] * sin(tta);
        btc += agelist[i].bt[k] * cos(tta);
        bts += agelist[i].bt[k] * sin(tta);

        if (bIncremental) {
          brcPrev += agelist[i].brPrev[k] * cos(tta);
          brsPrev += agelist[i].brPrev[k] * sin(tta);
          btcPrev += agelist[i].btPrev[k] * cos(tta);
          btsPrev += agelist[i].btPrev[k] * sin(tta);
        }
      }

      if ((agelist[i].nh[j] == 0) || (((j == (agelist[i].nn - 1)) && (agelist[i].BdryFormat == 0)) && ((agelist[i].totalArcElements % 2) == 0))) {
        brc /= agelist[i].totalArcElements;
        brs /= agelist[i].totalArcElements;
        btc /= agelist[i].totalArcElements;
        bts /= agelist[i].totalArcElements;
        brcPrev /= agelist[i].totalArcElements;
        brsPrev /= agelist[i].totalArcElements;
        btcPrev /= agelist[i].totalArcElements;
        btsPrev /= agelist[i].totalArcElements;
      } else {
        brc /= ((double)agelist[i].totalArcElements) / 2.;
        brs /= ((double)agelist[i].totalArcElements) / 2.;
        btc /= ((double)agelist[i].totalArcElements) / 2.;
        bts /= ((double)agelist[i].totalArcElements) / 2.;
        brcPrev /= ((double)agelist[i].totalArcElements) / 2.;
        brsPrev /= ((double)agelist[i].totalArcElements) / 2.;
        btcPrev /= ((double)agelist[i].totalArcElements) / 2.;
        btsPrev /= ((double)agelist[i].totalArcElements) / 2.;
      }

      agelist[i].brc[j] = brc;
      agelist[i].brs[j] = brs;
      agelist[i].btc[j] = btc;
      agelist[i].bts[j] = bts;

      if (bIncremental) {
        agelist[i].brcPrev[j] = brcPrev;
        agelist[i].brsPrev[j] = brsPrev;
        agelist[i].btcPrev[j] = btcPrev;
        agelist[i].btsPrev[j] = btsPrev;
      }
    }
  }

  // scale depth to meters for internal computations;
  if (Depth == -1)
    Depth = 1;
  else
    Depth *= LengthConv[LengthUnits];

  // element centroids and radii;
  for (i = 0; i < meshelem.GetSize(); i++) {
    meshelem[i].ctr = Ctr(i);
    for (j = 0, meshelem[i].rsqr = 0; j < 3; j++) {
      b = sqr(meshnode[meshelem[i].p[j]].x - meshelem[i].ctr.re) + sqr(meshnode[meshelem[i].p[j]].y - meshelem[i].ctr.im);
      if (b > meshelem[i].rsqr)
        meshelem[i].rsqr = b;
    }
  }

  // Compute magnetization direction in each element
  lua_State* LocalLua = lua_open(4096);
  lua_baselibopen(LocalLua);
  lua_strlibopen(LocalLua);
  lua_mathlibopen(LocalLua);
  for (i = 0; i < meshelem.GetSize(); i++) {
    if (blocklist[meshelem[i].lbl].MagDirFctn.GetLength() == 0) {
      meshelem[i].magdir = blocklist[meshelem[i].lbl].MagDir;
    } else {
      CString str;
      CComplex X;
      X = meshelem[i].ctr;
      str.Format("x=%.17g\ny=%.17g\nr=x\nz=y\ntheta=%.17g\nR=%.17g\nreturn %s",
          X.re, X.im, arg(X) * 180 / PI, abs(X), blocklist[meshelem[i].lbl].MagDirFctn);
      lua_dostring(LocalLua, str);
      meshelem[i].magdir = Re(lua_tonumber(LocalLua, -1));
      lua_pop(LocalLua, 1);
    }
  }
  lua_close(LocalLua);

  // Find flux density in each element;
  for (i = 0; i < meshelem.GetSize(); i++)
    GetElementB(meshelem[i]);

  // Find extreme values of A;
  A_Low = meshnode[0].A.re;
  A_High = meshnode[0].A.re;
  for (i = 1; i < meshnode.GetSize(); i++) {
    if (meshnode[i].A.re > A_High)
      A_High = meshnode[i].A.re;
    if (meshnode[i].A.re < A_Low)
      A_Low = meshnode[i].A.re;

    if (Frequency != 0) {
      if (meshnode[i].A.im < A_Low)
        A_Low = meshnode[i].A.im;
      if (meshnode[i].A.im > A_High)
        A_High = meshnode[i].A.im;
    }
  }
  // save default values for extremes of A
  A_lb = A_Low;
  A_ub = A_High;

  if (Frequency != 0) { // compute frequency-dependent permeabilities for linear blocks;

    CComplex deg45;
    deg45 = 1 + I;
    CComplex K, halflag;
    double ds;
    double w = 2. * PI * Frequency;

    for (k = 0; k < blockproplist.GetSize(); k++) {
      if (blockproplist[k].LamType == 0) {
        blockproplist[k].mu_fdx = blockproplist[k].mu_x * exp(-I * blockproplist[k].Theta_hx * PI / 180.);
        blockproplist[k].mu_fdy = blockproplist[k].mu_y * exp(-I * blockproplist[k].Theta_hy * PI / 180.);

        if (blockproplist[k].Lam_d != 0) {
          halflag = exp(-I * blockproplist[k].Theta_hx * PI / 360.);
          ds = sqrt(2. / (0.4 * PI * w * blockproplist[k].Cduct * blockproplist[k].mu_x));
          K = halflag * deg45 * blockproplist[k].Lam_d * 0.001 / (2. * ds);
          if (blockproplist[k].Cduct != 0) {
            blockproplist[k].mu_fdx = (blockproplist[k].mu_fdx * tanh(K) / K) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
          } else {
            blockproplist[k].mu_fdx = (blockproplist[k].mu_fdx) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
          }

          halflag = exp(-I * blockproplist[k].Theta_hy * PI / 360.);
          ds = sqrt(2. / (0.4 * PI * w * blockproplist[k].Cduct * blockproplist[k].mu_y));
          K = halflag * deg45 * blockproplist[k].Lam_d * 0.001 / (2. * ds);
          if (blockproplist[k].Cduct != 0) {
            blockproplist[k].mu_fdy = (blockproplist[k].mu_fdy * tanh(K) / K) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
          } else {
            blockproplist[k].mu_fdy = (blockproplist[k].mu_fdy) * blockproplist[k].LamFill + (1. - blockproplist[k].LamFill);
          }
        }
      }
    }
  }

  // compute fill factor associated with each block label
  for (k = 0; k < blocklist.GetSize(); k++) {
    GetFillFactor(k);
  }

  // build list of elements connected to each node;
  // allocate connections list;
  NumList = (int*)calloc(meshnode.GetSize(), sizeof(int));
  ConList = (int**)calloc(meshnode.GetSize(), sizeof(int*));
  // find out number of connections to each node;
  for (i = 0; i < meshelem.GetSize(); i++)
    for (j = 0; j < 3; j++)
      NumList[meshelem[i].p[j]]++;
  // allocate space for connections lists;
  for (i = 0; i < meshnode.GetSize(); i++)
    ConList[i] = (int*)calloc(NumList[i], sizeof(int));
  // build list;
  for (i = 0; i < meshnode.GetSize(); i++)
    NumList[i] = 0;
  for (i = 0; i < meshelem.GetSize(); i++)
    for (j = 0; j < 3; j++) {
      k = meshelem[i].p[j];
      ConList[k][NumList[k]] = i;
      NumList[k]++;
    }

  // find extreme values of J;
  {
    CComplex Jelm[3], Aelm[3];

    double J_Low, J_High;
    double Jr_Low, Jr_High;
    double Ji_Low, Ji_High;

    GetJA(0, Jelm, Aelm);
    Jr_Low = fabs(Jelm[0].re);
    Jr_High = Jr_Low;
    Ji_Low = fabs(Jelm[0].im);
    Ji_High = Ji_Low;
    J_Low = abs(Jelm[0]);
    J_High = J_Low;
    for (i = 0; i < meshelem.GetSize(); i++) {
      GetJA(i, Jelm, Aelm);
      for (j = 0; j < 3; j++) {
        br = fabs(Jelm[j].re);
        bi = fabs(Jelm[j].im);
        b = abs(Jelm[j]);

        if (b > J_High)
          J_High = b;
        if (b < J_Low)
          J_Low = b;
        if (br > Jr_High)
          Jr_High = br;
        if (br < Jr_Low)
          Jr_Low = br;
        if (bi > Ji_High)
          Ji_High = bi;
        if (bi < Ji_Low)
          Ji_Low = bi;
      }
    }

    J_Low *= 1.e-6;
    J_High *= 1e-6;
    Jr_Low *= 1.e-6;
    Jr_High *= 1e-6;
    Ji_Low *= 1.e-6;
    Ji_High *= 1e-6;

    if (Frequency == 0) {
      d_PlotBounds[2][0] = PlotBounds[2][0] = J_Low;
      d_PlotBounds[2][1] = PlotBounds[2][1] = J_High;
    } else {
      d_PlotBounds[6][0] = PlotBounds[6][0] = J_Low;
      d_PlotBounds[6][1] = PlotBounds[6][1] = J_High;
      d_PlotBounds[7][0] = PlotBounds[7][0] = Jr_Low;
      d_PlotBounds[7][1] = PlotBounds[7][1] = Jr_High;
      d_PlotBounds[8][0] = PlotBounds[8][0] = Ji_Low;
      d_PlotBounds[8][1] = PlotBounds[8][1] = Ji_High;
    }
  }

  // Find extreme values of B and H;
  {
    double Br_Low, Br_High;
    double Bi_Low, Bi_High;
    double H_Low;
    double Hr_Low, Hr_High;
    double Hi_Low, Hi_High;
    double logB_Low, logB_High;
    double a0, a1;
    CComplex h1, h2;

    // Do a little bit of work to exclude external region from the extreme value calculation
    // Otherwise, flux in the external regions can give a spurious indication of limits
    short* isExt = (short*)calloc(meshelem.GetSize(), sizeof(short));
    CString myBlockName;
    for (i = 0, j = 0; i < meshelem.GetSize(); i++) {
      if (blocklist[meshelem[i].lbl].IsExternal == TRUE)
        isExt[i] = TRUE;
      myBlockName = blockproplist[meshelem[i].blk].BlockName;
      if ((myBlockName[0] == 'u') && (myBlockName.GetLength() > 1)) {
        for (k = 1; k < 10; k++) {
          if (myBlockName[1] == ('0' + k)) {
            isExt[i] = TRUE;
            break;
          }
        }
      }
      if (isExt[i] == TRUE)
        j++;
    }

    // catch the special case where _every_ element seems to be in an external region...
    if (j == meshelem.GetSize())
      for (i = 0; i < meshelem.GetSize(); i++)
        isExt[i] = FALSE;

    i = 0;
    while (isExt[i])
      i++;
    Br_Low = sqrt(sqr(meshelem[i].B1.re) + sqr(meshelem[i].B2.re));
    Br_High = Br_Low;
    Bi_Low = sqrt(sqr(meshelem[i].B1.im) + sqr(meshelem[i].B2.im));
    Bi_High = Bi_Low;
    B_Low = sqrt(Br_Low * Br_Low + Bi_Low * Bi_Low);
    B_High = B_Low;
    a0 = sqrt(meshelem[i].rsqr) * B_High * B_High;

    if (Frequency != 0)
      GetH(meshelem[i].B1, meshelem[i].B2, h1, h2, 0);
    else {
      h1 = 0;
      h2 = 0;
      GetH(meshelem[i].B1.re, meshelem[i].B2.re, h1.re, h2.re, 0);
    }

    Hr_Low = sqrt(sqr(h1.re) + sqr(h2.re));
    Hr_High = Hr_Low;
    Hi_Low = sqrt(sqr(h1.im) + sqr(h2.im));
    Hi_High = Hi_Low;
    H_Low = sqrt(Hr_Low * Hr_Low + Hi_Low * Hi_Low);
    H_High = H_Low;

    for (i = 0; i < meshelem.GetSize(); i++) {
      GetNodalB(meshelem[i].b1, meshelem[i].b2, meshelem[i]);
      for (j = 0; j < 3; j++) {
        br = sqrt(sqr(meshelem[i].b1[j].re) + sqr(meshelem[i].b2[j].re));
        bi = sqrt(sqr(meshelem[i].b1[j].im) + sqr(meshelem[i].b2[j].im));
        b = sqrt(br * br + bi * bi);

        // used to be: if(b>B_High)   B_High=b;
        // new form is a heuristic that discounts really small elements
        // with really high flux density, which sometimes happens in corners.
        a1 = sqrt(meshelem[i].rsqr) * b * b;
        if ((a1 > a0) && (!isExt[i])) {
          B_High = b;
          a0 = a1;
        }

        if (!isExt[i]) {
          if (b < B_Low)
            B_Low = b;
          if (br > Br_High)
            Br_High = br;
          if (br < Br_Low)
            Br_Low = br;
          if (bi > Bi_High)
            Bi_High = bi;
          if (bi < Bi_Low)
            Bi_Low = bi;
        }
      }

      // getting lazy--just consider element averages for H
      if (Frequency != 0)
        GetH(meshelem[i].B1, meshelem[i].B2, h1, h2, i);
      else
        GetH(meshelem[i].B1.re, meshelem[i].B2.re, h1.re, h2.re, i);

      if (!isExt[i]) {
        br = sqrt(sqr(h1.re) + sqr(h2.re));
        bi = sqrt(sqr(h1.im) + sqr(h2.im));
        b = sqrt(br * br + bi * bi);
        if (b > H_High)
          H_High = b;
        if (b < H_Low)
          H_Low = b;
        if (br > Hr_High)
          Hr_High = br;
        if (br < Hr_Low)
          Hr_Low = br;
        if (bi > Hi_High)
          Hi_High = bi;
        if (bi < Hi_Low)
          Hi_Low = bi;
      }
    }
    free(isExt);

    if (Frequency == 0) {
      d_PlotBounds[0][0] = PlotBounds[0][0] = B_Low;
      d_PlotBounds[0][1] = PlotBounds[0][1] = B_High;
      d_PlotBounds[1][0] = PlotBounds[1][0] = H_Low;
      d_PlotBounds[1][1] = PlotBounds[1][1] = H_High;

      // Do log(|B|) bounds...
      if (B_Low != 0)
        logB_Low = log10(B_Low);
      else
        logB_Low = -12.;
      if (logB_Low < -12.)
        logB_Low = -12.;
      logB_Low = floor(logB_Low * 10.) / 10.;

      if (B_High != 0)
        logB_High = log10(B_High);
      else
        logB_High = -12.;
      if (logB_High < -12.)
        logB_High = -12.;
      logB_High = ceil(logB_High * 10.) / 10.;

      d_PlotBounds[3][0] = PlotBounds[3][0] = logB_Low;
      d_PlotBounds[3][1] = PlotBounds[3][1] = logB_High;
    } else {
      d_PlotBounds[0][0] = PlotBounds[0][0] = B_Low;
      d_PlotBounds[0][1] = PlotBounds[0][1] = B_High;
      d_PlotBounds[1][0] = PlotBounds[1][0] = Br_Low;
      d_PlotBounds[1][1] = PlotBounds[1][1] = Br_High;
      d_PlotBounds[2][0] = PlotBounds[2][0] = Bi_Low;
      d_PlotBounds[2][1] = PlotBounds[2][1] = Bi_High;
      d_PlotBounds[3][0] = PlotBounds[3][0] = H_Low;
      d_PlotBounds[3][1] = PlotBounds[3][1] = H_High;
      d_PlotBounds[4][0] = PlotBounds[4][0] = Hr_Low;
      d_PlotBounds[4][1] = PlotBounds[4][1] = Hr_High;
      d_PlotBounds[5][0] = PlotBounds[5][0] = Hi_Low;
      d_PlotBounds[5][1] = PlotBounds[5][1] = Hi_High;

      // Do log(|B|) bounds...
      if (B_Low != 0)
        logB_Low = log10(B_Low);
      else
        logB_Low = -12.;
      if (logB_Low < -12.)
        logB_Low = -12.;
      logB_Low = floor(logB_Low * 10.) / 10.;

      if (B_High != 0)
        logB_High = log10(B_High);
      else
        logB_High = -12.;
      if (logB_High < -12.)
        logB_High = -12.;
      logB_High = ceil(logB_High * 10.) / 10.;

      d_PlotBounds[9][0] = PlotBounds[9][0] = logB_Low;
      d_PlotBounds[9][1] = PlotBounds[9][1] = logB_High;
    }
  }

  // Choose bounds based on the type of contour plot
  // currently in play
  POSITION pos = GetFirstViewPosition();
  CFemmviewView* theView = (CFemmviewView*)GetNextView(pos);

  if (Frequency == 0) {
    if (theView->DensityPlot == 2)
      theView->DensityPlot = 1;
    if (theView->DensityPlot > 1)
      theView->DensityPlot = 0;
  }

  // compute total resulting current for circuits with an a priori defined
  // voltage gradient;  Need this to display circuit results & impedance.
  for (i = 0; i < circproplist.GetSize(); i++) {
    CComplex Jelm[3], Aelm[3];
    double a;

    if (circproplist[i].CircType > 1)
      for (j = 0, circproplist[i].Amps = 0.; j < meshelem.GetSize(); j++) {
        if (blocklist[meshelem[j].lbl].InCircuit == i) {
          GetJA(j, Jelm, Aelm);
          a = ElmArea(j) * sqr(LengthConv[LengthUnits]);
          for (k = 0; k < 3; k++)
            circproplist[i].Amps += a * Jelm[k] / 3;
        }
      }
  }

  // Build adjacency information for each element.
  FindBoundaryEdges();

  // Check to see if any regions are multiply defined
  // (i.e. tagged by more than one block label). If so,
  // display an error message and mark the problem blocks.
  for (k = 0, bMultiplyDefinedLabels = FALSE; k < blocklist.GetSize(); k++) {
    if ((i = InTriangle(blocklist[k].x, blocklist[k].y)) >= 0) {
      if (meshelem[i].lbl != k) {
        blocklist[meshelem[i].lbl].IsSelected = TRUE;
        if (!bMultiplyDefinedLabels) {
          CString msg;
          msg = "Some regions in the problem have been defined\n";
          msg += "by more than one block label.  These potentially\n";
          msg += "problematic regions will appear as selected in\n";
          msg += "the initial view.";
          MsgBox(msg);
          bMultiplyDefinedLabels = TRUE;
        }
      }
    }
  }

  // Get some information needed to compute energy stored in
  // permanent magnets with a nonlinear demagnetization curve
  if (Frequency == 0) {
    for (k = 0; k < blockproplist.GetSize(); k++) {
      if ((blockproplist[k].H_c > 0) && (blockproplist[k].BHpoints > 0)) {
        blockproplist[k].Nrg = blockproplist[k].GetCoEnergy(blockproplist[k].GetB(blockproplist[k].H_c));
      }
    }
  }

  FirstDraw = TRUE;
  return TRUE;
}

int CFemmviewDoc::InTriangle(double x, double y)
{
  static int k;
  int j, hi, lo, sz;
  double z;

  sz = (int)meshelem.GetSize();
  if ((k < 0) || (k >= sz))
    k = 0;

  // In most applications, the triangle we're looking
  // for is nearby the last one we found.  Since the elements
  // are ordered in a banded structure, we want to check the
  // elements nearby the last one selected first.
  if (InTriangleTest(x, y, k))
    return k;

  hi = k;
  lo = k;

  for (j = 0; j < sz; j += 2) {
    hi++;
    if (hi >= sz)
      hi = 0;
    lo--;
    if (lo < 0)
      lo = sz - 1;

    z = (meshelem[hi].ctr.re - x) * (meshelem[hi].ctr.re - x) + (meshelem[hi].ctr.im - y) * (meshelem[hi].ctr.im - y);
    if (z <= meshelem[hi].rsqr) {
      if (InTriangleTest(x, y, hi)) {
        k = hi;
        return k;
      }
    }

    z = (meshelem[lo].ctr.re - x) * (meshelem[lo].ctr.re - x) + (meshelem[lo].ctr.im - y) * (meshelem[lo].ctr.im - y);
    if (z <= meshelem[lo].rsqr) {
      if (InTriangleTest(x, y, lo)) {
        k = lo;
        return k;
      }
    }
  }

  return (-1);
}

BOOL CFemmviewDoc::GetPointValues(double x, double y, CPointVals& u)
{
  int k;
  k = InTriangle(x, y);
  if (k < 0)
    return FALSE;
  GetPointValues(x, y, k, u);
  return TRUE;
}

BOOL CFemmviewDoc::GetPointValues(double x, double y, int k, CPointVals& u)
{
  int i, j, n[3], lbl;
  double a[3], b[3], c[3], da, ravg;

  for (i = 0; i < 3; i++)
    n[i] = meshelem[k].p[i];
  a[0] = meshnode[n[1]].x * meshnode[n[2]].y - meshnode[n[2]].x * meshnode[n[1]].y;
  a[1] = meshnode[n[2]].x * meshnode[n[0]].y - meshnode[n[0]].x * meshnode[n[2]].y;
  a[2] = meshnode[n[0]].x * meshnode[n[1]].y - meshnode[n[1]].x * meshnode[n[0]].y;
  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
  da = (b[0] * c[1] - b[1] * c[0]);
  ravg = LengthConv[LengthUnits] * (meshnode[n[0]].x + meshnode[n[1]].x + meshnode[n[2]].x) / 3.;

  GetPointB(x, y, u.B1, u.B2, meshelem[k]);

  u.Hc = 0;
  u.mu12 = 0;

  //	if(blockproplist[meshelem[k].blk].LamType>2)
  u.ff = blocklist[meshelem[k].lbl].FillFactor;
  //	else u.ff=-1;

  if (Frequency == 0) {
    u.A = 0;
    if (ProblemType == 0) {
      for (i = 0; i < 3; i++)
        u.A.re += meshnode[n[i]].A.re * (a[i] + b[i] * x + c[i] * y) / (da);
    } else {
      /* Old way that I interpolated potential in axi case:
        // interpolation from A from nodal points.
        // note that the potential that's actually stored
        // for axisymmetric problems is 2*Pi*r*A, so divide
        // by nodal r to get 2*Pi*A at the nodes.  Linearly
        // interpolate this, then multiply by r at the point
        // of interest to get back to 2*Pi*r*A.
        for(i=0,rp=0;i<3;i++){
          r=meshnode[n[i]].x;
          rp+=meshnode[n[i]].x*(a[i]+b[i]*x+c[i]*y)/da;
          if (r>1.e-6) u.A.re+=meshnode[n[i]].A.re*
            (a[i]+b[i]*x+c[i]*y)/(r*da);
        }
        u.A.re*=rp;
      */

      // a ``smarter'' interpolation.  One based on A can't
      // represent constant flux density very well.
      // This works, but I should re-write it in a more
      // efficient form--it's doing a lot of the work
      // twice, because the a-b-c stuff that's already
      // been computed is ignored.
      double v[6];
      double R[3];
      double Z[3];
      double p, q;

      for (i = 0; i < 3; i++) {
        R[i] = meshnode[n[i]].x;
        Z[i] = meshnode[n[i]].y;
      }

      // corner nodes
      v[0] = meshnode[n[0]].A.re;
      v[2] = meshnode[n[1]].A.re;
      v[4] = meshnode[n[2]].A.re;

      // construct values for mid-side nodes;
      if ((R[0] < 1.e-06) && (R[1] < 1.e-06))
        v[1] = (v[0] + v[2]) / 2.;
      else
        v[1] = (R[1] * (3. * v[0] + v[2]) + R[0] * (v[0] + 3. * v[2])) / (4. * (R[0] + R[1]));

      if ((R[1] < 1.e-06) && (R[2] < 1.e-06))
        v[3] = (v[2] + v[4]) / 2.;
      else
        v[3] = (R[2] * (3. * v[2] + v[4]) + R[1] * (v[2] + 3. * v[4])) / (4. * (R[1] + R[2]));

      if ((R[2] < 1.e-06) && (R[0] < 1.e-06))
        v[5] = (v[4] + v[0]) / 2.;
      else
        v[5] = (R[0] * (3. * v[4] + v[0]) + R[2] * (v[4] + 3. * v[0])) / (4. * (R[2] + R[0]));

      // compute location in element transformed onto
      // a unit triangle;
      p = (b[1] * x + c[1] * y + a[1]) / da;
      q = (b[2] * x + c[2] * y + a[2]) / da;

      // now, interpolate to get potential...
      u.A.re = v[0] - p * (3. * v[0] - 4. * v[1] + v[2]) + 2. * p * p * (v[0] - 2. * v[1] + v[2]) - q * (3. * v[0] + v[4] - 4. * v[5]) + 2. * q * q * (v[0] + v[4] - 2. * v[5]) + 4. * p * q * (v[0] - v[1] + v[3] - v[5]);

      /*		// "simple" way to do it...
          // problem is that this mucks up things
          // near the centerline, where things ought
          // to look pretty quadratic.
          for(i=0;i<3;i++)
            u.A.re+=meshnode[n[i]].A.re*(a[i]+b[i]*x+c[i]*y)/(da);
      */
    }

    // Need to catch bIncremental case here...
    u.mu1.im = 0;
    u.mu2.im = 0;
    u.mu12 = 0;
    if (!bIncremental) {
      GetMu(u.B1.re, u.B2.re, u.mu1.re, u.mu2.re, k);
      u.H1 = u.B1 / (Re(u.mu1) * muo);
      u.H2 = u.B2 / (Re(u.mu2) * muo);
    } else {
      double muinc, murel;
      double B, B1p, B2p;

      B1p = meshelem[k].B1p;
      B2p = meshelem[k].B2p;
      B = sqrt(B1p * B1p + B2p * B2p);

      GetMu(B1p, B2p, muinc, murel, k);
      if (B == 0) {
        // Catch the special case where B=0 to avoid a possible divide by zero
        u.mu1 = muinc;
        u.mu2 = muinc;
        u.mu12 = 0;
      } else if (bIncremental == 1) {
        // For "incremental" problem, permeability is linearized about the previous soluiton.
        u.mu1 = (B1p * B1p * muinc + B2p * B2p * murel) / (B * B);
        u.mu12 = (B1p * B2p * (muinc - murel)) / (B * B);
        u.mu2 = (B2p * B2p * muinc + B1p * B1p * murel) / (B * B);
      } else { // bIncremental==2
        // For "frozen" permeability, same permeability as previous problem
        u.mu1 = murel;
        u.mu2 = murel;
        u.mu12 = 0;
      }

      u.H1 = (u.B2 * u.mu12 - u.B1 * u.mu2) / (u.mu12 * u.mu12 - u.mu1 * u.mu2);
      u.H2 = (u.B2 * u.mu1 - u.B1 * u.mu12) / (u.mu1 * u.mu2 - u.mu12 * u.mu12);
    }

    u.Je = 0;
    u.Js = blockproplist[meshelem[k].blk].Jr;
    lbl = meshelem[k].lbl;
    j = blocklist[lbl].InCircuit;
    if (j >= 0) {
      if (blocklist[lbl].Case == 0) {
        if (ProblemType == 0)
          u.Js -= Re(blocklist[meshelem[k].lbl].o) * blocklist[lbl].dVolts;
        else {

          int tn;
          double R[3];
          for (tn = 0; tn < 3; tn++) {
            R[tn] = meshnode[n[tn]].x;
            if (R[tn] < 1.e-6)
              R[tn] = ravg;
            else
              R[tn] *= LengthConv[LengthUnits];
          }
          for (ravg = 0., tn = 0; tn < 3; tn++)
            ravg += (1. / R[tn]) * (a[tn] + b[tn] * x + c[tn] * y) / (da);
          u.Js -= Re(blocklist[meshelem[k].lbl].o) * blocklist[lbl].dVolts * ravg;
        }
      } else
        u.Js += blocklist[lbl].J;
    }
    u.c = Re(blocklist[meshelem[k].lbl].o);
    u.E = blockproplist[meshelem[k].blk].DoEnergy(u.B1.re, u.B2.re);

    // correct H and energy stored in magnet for second-quadrant
    // representation of a PM.
    if (blockproplist[meshelem[k].blk].H_c != 0) {
      int bk = meshelem[k].blk;

      u.Hc = blockproplist[bk].H_c * exp(I * PI * meshelem[k].magdir / 180.);
      u.H1 = u.H1 - Re(u.Hc);
      u.H2 = u.H2 - Im(u.Hc);

      // in the linear case:
      if (blockproplist[bk].BHpoints == 0)
        u.E = 0.5 * muo * (u.mu1.re * u.H1.re * u.H1.re + u.mu2.re * u.H2.re * u.H2.re);
      else {
        u.E = u.E + blockproplist[bk].Nrg
            - blockproplist[bk].H_c * Re((u.B1.re + I * u.B2.re) / exp(I * PI * meshelem[k].magdir / 180.));
      }

      // If considering the magnet as an equivalent coil, add Hc to the demagnetizing field
      if (!d_ShiftH) {
        u.H1 = u.H1 + Re(u.Hc);
        u.H2 = u.H2 + Im(u.Hc);
        u.Hc = 0;
      }
    }

    // add in "local" stored energy for wound that would be subject to
    // prox and skin effect for nonzero frequency cases.
    if (blockproplist[meshelem[k].blk].LamType > 2) {
      CComplex J;
      J = u.Js * 1.e6;

      u.E += Re(J * J) * blocklist[meshelem[i].lbl].LocalEnergy / 2.;
    }

    u.Ph = 0;
    u.Pe = 0;
    return TRUE;
  }

  if (Frequency != 0) {
    u.A = 0;
    if (ProblemType == 0) {
      for (i = 0; i < 3; i++)
        u.A += meshnode[n[i]].A * (a[i] + b[i] * x + c[i] * y) / (da);
    } else {
      CComplex v[6];
      double R[3];
      double Z[3];
      double p, q;

      for (i = 0; i < 3; i++) {
        R[i] = meshnode[n[i]].x;
        Z[i] = meshnode[n[i]].y;
      }

      // corner nodes
      v[0] = meshnode[n[0]].A;
      v[2] = meshnode[n[1]].A;
      v[4] = meshnode[n[2]].A;

      // construct values for mid-side nodes;
      if ((R[0] < 1.e-06) && (R[1] < 1.e-06))
        v[1] = (v[0] + v[2]) / 2.;
      else
        v[1] = (R[1] * (3. * v[0] + v[2]) + R[0] * (v[0] + 3. * v[2])) / (4. * (R[0] + R[1]));

      if ((R[1] < 1.e-06) && (R[2] < 1.e-06))
        v[3] = (v[2] + v[4]) / 2.;
      else
        v[3] = (R[2] * (3. * v[2] + v[4]) + R[1] * (v[2] + 3. * v[4])) / (4. * (R[1] + R[2]));

      if ((R[2] < 1.e-06) && (R[0] < 1.e-06))
        v[5] = (v[4] + v[0]) / 2.;
      else
        v[5] = (R[0] * (3. * v[4] + v[0]) + R[2] * (v[4] + 3. * v[0])) / (4. * (R[2] + R[0]));

      // compute location in element transformed onto
      // a unit triangle;
      p = (b[1] * x + c[1] * y + a[1]) / da;
      q = (b[2] * x + c[2] * y + a[2]) / da;

      // now, interpolate to get potential...
      u.A = v[0] - p * (3. * v[0] - 4. * v[1] + v[2]) + 2. * p * p * (v[0] - 2. * v[1] + v[2]) - q * (3. * v[0] + v[4] - 4. * v[5]) + 2. * q * q * (v[0] + v[4] - 2. * v[5]) + 4. * p * q * (v[0] - v[1] + v[3] - v[5]);
    }

    // if bIncremental, need to get permeability about the DC
    // operating point, rather than usual DC permeability.
    if (!bIncremental) {
      GetMu(u.B1, u.B2, u.mu1, u.mu2, k);
      u.mu12 = 0;
      u.H1 = u.B1 / (u.mu1 * muo);
      u.H2 = u.B2 / (u.mu2 * muo);
    } else {
      CComplex muinc, murel;
      double B, B1p, B2p;

      B1p = meshelem[k].B1p;
      B2p = meshelem[k].B2p;
      B = sqrt(B1p * B1p + B2p * B2p);

      GetMu(B1p, B2p, muinc, murel, k);
      if (B == 0) {
        u.mu1 = murel;
        u.mu2 = murel;
        u.mu12 = 0;
      } else {
        u.mu1 = (B1p * B1p * muinc + B2p * B2p * murel) / (B * B);
        u.mu12 = (B1p * B2p * (muinc - murel)) / (B * B);
        u.mu2 = (B2p * B2p * muinc + B1p * B1p * murel) / (B * B);
      }

      u.H1 = (u.B2 * u.mu12 - u.B1 * u.mu2) / (u.mu12 * u.mu12 - u.mu1 * u.mu2);
      u.H2 = (u.B2 * u.mu1 - u.B1 * u.mu12) / (u.mu1 * u.mu2 - u.mu12 * u.mu12);
    }

    u.Js = blockproplist[meshelem[k].blk].Jr + I * blockproplist[meshelem[k].blk].Ji;
    lbl = meshelem[k].lbl;
    j = blocklist[lbl].InCircuit;
    if (j >= 0) {
      if (blocklist[lbl].Case == 0) {
        if (ProblemType == 0)
          u.Js -= blocklist[meshelem[k].lbl].o * blocklist[lbl].dVolts;
        else {

          int tn;
          double R[3];
          for (tn = 0; tn < 3; tn++) {
            R[tn] = meshnode[n[tn]].x;
            if (R[tn] < 1.e-6)
              R[tn] = ravg;
            else
              R[tn] *= LengthConv[LengthUnits];
          }
          for (ravg = 0., tn = 0; tn < 3; tn++)
            ravg += (1. / R[tn]) * (a[tn] + b[tn] * x + c[tn] * y) / (da);
          u.Js -= blocklist[meshelem[k].lbl].o * blocklist[lbl].dVolts * ravg;
        }
      } else
        u.Js += blocklist[lbl].J;
    }

    // report just loss-related part of conductivity.
    if (blockproplist[meshelem[k].blk].Cduct != 0)
      u.c = 1. / Re(1. / (blocklist[meshelem[k].lbl].o));
    else
      u.c = 0;

    if (blockproplist[meshelem[k].blk].Lam_d != 0)
      u.c = 0;

    // only add in eddy currents if the region is solid
    if (blocklist[meshelem[k].lbl].FillFactor < 0)
      u.Je = -I * Frequency * 2. * PI * u.c * u.A;

    if (ProblemType != 0) {
      if (x != 0)
        u.Je /= (2. * PI * x * LengthConv[LengthUnits]);
      else
        u.Je = 0;
    }

    CComplex z;
    z = (u.H1 * u.B1.Conj()) + (u.H2 * u.B2.Conj());
    u.E = 0.25 * z.re;

    // add in "local" stored energy for wound that would be subject to
    // prox and skin effect for nonzero frequency cases.
    if (blockproplist[meshelem[k].blk].LamType > 2) {
      CComplex J;
      J = u.Js * 1.e6;
      u.E += Re(J * conj(J)) * blocklist[meshelem[k].lbl].LocalEnergy / 4.;
    }

    u.Ph = Frequency * PI * z.im;
    u.Pe = 0;
    if (u.c != 0) {
      z = u.Js + u.Je;
      u.Pe = 1.e06 * (z.re * z.re + z.im * z.im) / (u.c * 2.);
    }

    return TRUE;
  }

  return FALSE;
}

void CFemmviewDoc::GetPointB(double x, double y, CComplex& B1, CComplex& B2,
    CElement& elm)
{
  // elm is a reference to the element that contains the point of interest.
  int i, n[3];
  double da, a[3], b[3], c[3];

  if (Smooth == FALSE) {
    B1 = elm.B1;
    B2 = elm.B2;
    return;
  }

  for (i = 0; i < 3; i++)
    n[i] = elm.p[i];
  a[0] = meshnode[n[1]].x * meshnode[n[2]].y - meshnode[n[2]].x * meshnode[n[1]].y;
  a[1] = meshnode[n[2]].x * meshnode[n[0]].y - meshnode[n[0]].x * meshnode[n[2]].y;
  a[2] = meshnode[n[0]].x * meshnode[n[1]].y - meshnode[n[1]].x * meshnode[n[0]].y;
  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
  da = (b[0] * c[1] - b[1] * c[0]);

  B1.Set(0, 0);
  B2.Set(0, 0);
  for (i = 0; i < 3; i++) {
    B1 += (elm.b1[i] * (a[i] + b[i] * x + c[i] * y) / da);
    B2 += (elm.b2[i] * (a[i] + b[i] * x + c[i] * y) / da);
  }
}

void CFemmviewDoc::GetNodalB(CComplex* b1, CComplex* b2, CElement& elm)
{
  // elm is a reference to the element that contains the point of interest.
  CComplex p;
  CComplex tn, bn, bt, btu, btv, u1, u2, v1, v2;
  int i, j, k, l, q, m, pt, nxt;
  double r, R, z;
  CElement* e;
  int flag;

  // find nodal values of flux density via a patch method.
  for (i = 0; i < 3; i++) {

    k = elm.p[i];
    p.Set(meshnode[k].x, meshnode[k].y);
    b1[i].Set(0, 0);
    b2[i].Set(0, 0);
    for (j = 0, m = 0; j < NumList[k]; j++)
      if (elm.lbl == meshelem[ConList[k][j]].lbl)
        m++;
      else {
        if (Frequency == 0) {
          if ((blockproplist[elm.blk].mu_x == blockproplist[meshelem[ConList[k][j]].blk].mu_x) && (blockproplist[elm.blk].mu_y == blockproplist[meshelem[ConList[k][j]].blk].mu_y) && (blockproplist[elm.blk].H_c == blockproplist[meshelem[ConList[k][j]].blk].H_c) && (elm.magdir == meshelem[ConList[k][j]].magdir))
            m++;
          else if ((elm.blk == meshelem[ConList[k][j]].blk) && (elm.magdir == meshelem[ConList[k][j]].magdir))
            m++;
        } else if ((blockproplist[elm.blk].mu_fdx == blockproplist[meshelem[ConList[k][j]].blk].mu_fdx) && (blockproplist[elm.blk].mu_fdy == blockproplist[meshelem[ConList[k][j]].blk].mu_fdy))
          m++;
      }

    if (m == NumList[k]) // normal smoothing method for points
    { // away from any boundaries
      for (j = 0, R = 0; j < NumList[k]; j++) {
        m = ConList[k][j];
        z = 1. / abs(p - Ctr(m));
        R += z;
        b1[i] += (z * meshelem[m].B1);
        b2[i] += (z * meshelem[m].B2);
      }
      b1[i] /= R;
      b2[i] /= R;
    }

    else {
      R = 0;
      v1 = 0;
      v2 = 0;

      // scan ccw for an interface...
      e = &elm;
      for (q = 0; q < NumList[k]; q++) {
        // find ccw side of the element;
        for (j = 0; j < 3; j++)
          if (e->p[j] == k)
            pt = j;
        pt--;
        if (pt < 0)
          pt = 2;
        pt = e->p[pt];

        // scan to find element adjacent to this side;
        for (j = 0, nxt = -1; j < NumList[k]; j++) {
          if (&meshelem[ConList[k][j]] != e) {
            for (l = 0; l < 3; l++)
              if (meshelem[ConList[k][j]].p[l] == pt)
                nxt = ConList[k][j];
          }
        }

        if (nxt == -1) {
          // a special-case punt
          q = NumList[k];
          b1[i] = (e->B1);
          b2[i] = (e->B2);
          v1 = 1;
          v2 = 1;
        } else if (elm.lbl != meshelem[nxt].lbl) {
          // we have found two elements on either side of the interface
          // now, we take contribution from B at the center of the
          // interface side
          tn.Set(meshnode[pt].x - meshnode[k].x,
              meshnode[pt].y - meshnode[k].y);
          r = (meshnode[pt].x + meshnode[k].x) * LengthConv[LengthUnits] / 2.;
          bn = (meshnode[pt].A - meshnode[k].A) / (abs(tn) * LengthConv[LengthUnits]);
          if (ProblemType == 1) {
            bn /= (-2. * PI * r);
          }
          z = 0.5 / abs(tn);
          tn /= abs(tn);

          // for the moment, kludge with bt...
          bt = e->B1 * tn.re + e->B2 * tn.im;

          R += z;
          b1[i] += (z * tn.re * bt);
          b2[i] += (z * tn.im * bt);
          b1[i] += (z * tn.im * bn);
          b2[i] += (-z * tn.re * bn);
          v1 = tn;
          q = NumList[k];
        } else
          e = &meshelem[nxt];
      }

      // scan cw for an interface...
      if (v2 == 0) // catches the "special-case punt" where we have
      { // already set nodal B values....
        e = &elm;
        for (q = 0; q < NumList[k]; q++) {
          // find cw side of the element;
          for (j = 0; j < 3; j++)
            if (e->p[j] == k)
              pt = j;
          pt++;
          if (pt > 2)
            pt = 0;
          pt = e->p[pt];

          // scan to find element adjacent to this side;
          for (j = 0, nxt = -1; j < NumList[k]; j++) {
            if (&meshelem[ConList[k][j]] != e) {
              for (l = 0; l < 3; l++)
                if (meshelem[ConList[k][j]].p[l] == pt)
                  nxt = ConList[k][j];
            }
          }
          if (nxt == -1) {
            // a special-case punt
            q = NumList[k];
            b1[i] = (e->B1);
            b2[i] = (e->B2);
            v1 = 1;
            v2 = 1;
          } else if (elm.lbl != meshelem[nxt].lbl) {
            // we have found two elements on either side of the interface
            // now, we take contribution from B at the center of the
            // interface side
            tn.Set(meshnode[pt].x - meshnode[k].x,
                meshnode[pt].y - meshnode[k].y);
            r = (meshnode[pt].x + meshnode[k].x) * LengthConv[LengthUnits] / 2.;
            bn = (meshnode[pt].A - meshnode[k].A) / (abs(tn) * LengthConv[LengthUnits]);
            if (ProblemType == 1) {
              bn /= (-2. * PI * r);
            }
            z = 0.5 / abs(tn);
            tn /= abs(tn);

            // for the moment, kludge with bt...
            bt = e->B1 * tn.re + e->B2 * tn.im;

            R += z;
            b1[i] += (z * tn.re * bt);
            b2[i] += (z * tn.im * bt);
            b1[i] += (z * tn.im * bn);
            b2[i] += (-z * tn.re * bn);
            v2 = tn;
            q = NumList[k];
          } else
            e = &meshelem[nxt];
        }
        b1[i] /= R;
        b2[i] /= R;
      }

      // check to see if angle of corner is too sharp to apply
      // this rule; really only does right if the interface is flat;
      flag = FALSE;
      // if there is only one edge, approx is ok;
      if ((abs(v1) < 0.9) || (abs(v2) < 0.9))
        flag = TRUE;
      // if the interfaces make less than a 10 degree angle, things are ok;
      if ((-v1.re * v2.re - v1.im * v2.im) > 0.985)
        flag = TRUE;

      // Otherwise, punt...
      if (flag == FALSE) {
        bn = 0;
        k = elm.p[i];
        for (j = 0; j < NumList[k]; j++) {
          if (elm.lbl == meshelem[ConList[k][j]].lbl) {
            m = ConList[k][j];
            bt.re = sqrt(meshelem[m].B1.re * meshelem[m].B1.re + meshelem[m].B2.re * meshelem[m].B2.re);
            bt.im = sqrt(meshelem[m].B1.im * meshelem[m].B1.im + meshelem[m].B2.im * meshelem[m].B2.im);
            if (bt.re > bn.re)
              bn.re = bt.re;
            if (bt.im > bn.im)
              bn.im = bt.im;
          }
        }

        R = sqrt(elm.B1.re * elm.B1.re + elm.B2.re * elm.B2.re);
        if (R != 0) {
          b1[i].re = bn.re / R * elm.B1.re;
          b2[i].re = bn.re / R * elm.B2.re;
        } else {
          b1[i].re = 0;
          b2[i].re = 0;
        }

        R = sqrt(elm.B1.im * elm.B1.im + elm.B2.im * elm.B2.im);
        if (R != 0) {
          b1[i].im = bn.im / R * elm.B1.im;
          b2[i].im = bn.im / R * elm.B2.im;
        } else {
          b1[i].im = 0;
          b2[i].im = 0;
        }
      }
    }

    // check to see if the point has a point current; if so, just
    // use element average values;
    if (nodeproplist.GetSize() != 0)
      for (j = 0; j < nodelist.GetSize(); j++) {
        if (abs(p - (nodelist[j].x + nodelist[j].y * I)) < 1.e-08)
          if (nodelist[j].BoundaryMarker >= 0) {
            if ((nodeproplist[nodelist[j].BoundaryMarker].Jr != 0) || (nodeproplist[nodelist[j].BoundaryMarker].Ji != 0)) {
              b1[i] = elm.B1;
              b2[i] = elm.B2;
            }
          }
      }

    // check for special case of node on r=0 axisymmetric; set Br=0;
    if ((fabs(p.re) < 1.e-06) && (ProblemType == 1))
      b1[i].Set(0., 0);
  }
}

void CFemmviewDoc::GetElementB(CElement& elm)
{
  int i, n[3];
  double b[3], c[3], da;

  for (i = 0; i < 3; i++)
    n[i] = elm.p[i];

  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
  da = (b[0] * c[1] - b[1] * c[0]);

  if (ProblemType == 0) {
    elm.B1 = 0;
    elm.B2 = 0;
    elm.B1p = 0;
    elm.B2p = 0;
    for (i = 0; i < 3; i++) {
      elm.B1 += meshnode[n[i]].A * c[i] / (da * LengthConv[LengthUnits]);
      elm.B2 -= meshnode[n[i]].A * b[i] / (da * LengthConv[LengthUnits]);
    }

    if (bIncremental) {
      for (i = 0; i < 3; i++) {
        elm.B1p += meshnode[n[i]].Aprev * c[i] / (da * LengthConv[LengthUnits]);
        elm.B2p -= meshnode[n[i]].Aprev * b[i] / (da * LengthConv[LengthUnits]);
      }
    }

    return;
  } else {
    CComplex v[6], dp, dq;
    double R[3], Z[3], r;

    for (i = 0, r = 0; i < 3; i++) {
      R[i] = meshnode[n[i]].x;
      Z[i] = meshnode[n[i]].y;
      r += R[i] / 3.;
    }

    // corner nodes
    v[0] = meshnode[n[0]].A;
    v[2] = meshnode[n[1]].A;
    v[4] = meshnode[n[2]].A;

    // construct values for mid-side nodes;
    if ((R[0] < 1.e-06) && (R[1] < 1.e-06))
      v[1] = (v[0] + v[2]) / 2.;
    else
      v[1] = (R[1] * (3. * v[0] + v[2]) + R[0] * (v[0] + 3. * v[2])) / (4. * (R[0] + R[1]));

    if ((R[1] < 1.e-06) && (R[2] < 1.e-06))
      v[3] = (v[2] + v[4]) / 2.;
    else
      v[3] = (R[2] * (3. * v[2] + v[4]) + R[1] * (v[2] + 3. * v[4])) / (4. * (R[1] + R[2]));

    if ((R[2] < 1.e-06) && (R[0] < 1.e-06))
      v[5] = (v[4] + v[0]) / 2.;
    else
      v[5] = (R[0] * (3. * v[4] + v[0]) + R[2] * (v[4] + 3. * v[0])) / (4. * (R[2] + R[0]));

    // derivatives w.r.t. p and q:
    dp = (-v[0] + v[2] + 4. * v[3] - 4. * v[5]) / 3.;
    dq = (-v[0] - 4. * v[1] + 4. * v[3] + v[4]) / 3.;

    // now, compute flux.
    da *= 2. * PI * r * LengthConv[LengthUnits] * LengthConv[LengthUnits];
    elm.B1 = -(c[1] * dp + c[2] * dq) / da;
    elm.B2 = (b[1] * dp + b[2] * dq) / da;

    if (bIncremental) {
      // corner nodes
      v[0] = meshnode[n[0]].Aprev;
      v[2] = meshnode[n[1]].Aprev;
      v[4] = meshnode[n[2]].Aprev;

      // construct values for mid-side nodes;
      if ((R[0] < 1.e-06) && (R[1] < 1.e-06))
        v[1] = (v[0] + v[2]) / 2.;
      else
        v[1] = (R[1] * (3. * v[0] + v[2]) + R[0] * (v[0] + 3. * v[2])) / (4. * (R[0] + R[1]));

      if ((R[1] < 1.e-06) && (R[2] < 1.e-06))
        v[3] = (v[2] + v[4]) / 2.;
      else
        v[3] = (R[2] * (3. * v[2] + v[4]) + R[1] * (v[2] + 3. * v[4])) / (4. * (R[1] + R[2]));

      if ((R[2] < 1.e-06) && (R[0] < 1.e-06))
        v[5] = (v[4] + v[0]) / 2.;
      else
        v[5] = (R[0] * (3. * v[4] + v[0]) + R[2] * (v[4] + 3. * v[0])) / (4. * (R[2] + R[0]));

      // derivatives w.r.t. p and q:
      dp = (-v[0] + v[2] + 4. * v[3] - 4. * v[5]) / 3.;
      dq = (-v[0] - 4. * v[1] + 4. * v[3] + v[4]) / 3.;

      // now, compute flux.
      da = (b[0] * c[1] - b[1] * c[0]);
      da *= 2. * PI * r * LengthConv[LengthUnits] * LengthConv[LengthUnits];
      elm.B1p = Re(-(c[1] * dp + c[2] * dq) / da);
      elm.B2p = Re((b[1] * dp + b[2] * dq) / da);
    } else {
      elm.B1p = 0;
      elm.B2p = 0;
    }
    return;
  }
}

void CFemmviewDoc::OnReload()
{
  // TODO: Add your command handler code here
  CString pname = GetPathName();
  if (pname.GetLength() > 0) {
    OnNewDocument();
    SetPathName(pname, FALSE);
    OnOpenDocument(pname);
  }
}

int CFemmviewDoc::ClosestNode(double x, double y)
{
  int i, j;
  double d0, d1;

  if (nodelist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = nodelist[0].GetDistance(x, y);
  for (i = 0; i < nodelist.GetSize(); i++) {
    d1 = nodelist[i].GetDistance(x, y);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

void CFemmviewDoc::GetLineValues(CXYPlot& p, int PlotType, int NumPlotPoints)
{
  double *q, z, u, dz;
  CComplex pt, n, t;
  int i, j, k, m, elm;
  CPointVals v;
  BOOL flag;

  q = (double*)calloc(contour.GetSize(), sizeof(double));
  for (i = 1, z = 0.; i < contour.GetSize(); i++) {
    z += abs(contour[i] - contour[i - 1]);
    q[i] = z;
  }
  dz = z / (NumPlotPoints - 1);

  /*
    m_XYPlotType.AddString("Potential");
    m_XYPlotType.AddString("|B|        (Magnitude of flux density)");
    m_XYPlotType.AddString("B . n      (Normal flux density)");
    m_XYPlotType.AddString("B . t      (Tangential flux density)");
    m_XYPlotType.AddString("|H|        (Magnitude of field intensity)");
    m_XYPlotType.AddString("H . n      (Normal field intensity)");
    m_XYPlotType.AddString("H . t      (Tangential field intensity)");
    m_XYPlotType.AddString("J_eddy
  */

  if (Frequency == 0) {
    switch (PlotType) {
    case 0:
      p.Create(NumPlotPoints, 2);
      if (ProblemType == 0)
        strcpy(p.lbls[1], "Potential, Wb/m");
      else
        strcpy(p.lbls[1], "Flux, Wb");
      break;
    case 1:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|B|, Tesla");
      break;
    case 2:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "B.n, Tesla");
      break;
    case 3:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "B.t, Tesla");
      break;
    case 4:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|H|, Amp/m");
      break;
    case 5:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "H.n, Amp/m");
      break;
    case 6:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "H.t, Amp/m");
      break;
    default:
      p.Create(NumPlotPoints, 2);
      break;
    }
  } else {
    switch (PlotType) {
    case 0:
      p.Create(NumPlotPoints, 4);
      if (ProblemType == 0) {
        strcpy(p.lbls[1], "|A|, Wb/m");
        strcpy(p.lbls[2], "Re[A], Wb/m");
        strcpy(p.lbls[3], "Im[A], Wb/m");
      } else {
        strcpy(p.lbls[1], "|Flux|, Wb");
        strcpy(p.lbls[2], "Re[Flux], Wb");
        strcpy(p.lbls[3], "Im[Flux], Wb");
      }
      break;
    case 1:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|B|, Tesla");
      break;
    case 2:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|B.n|, Tesla");
      strcpy(p.lbls[2], "Re[B.n], Tesla");
      strcpy(p.lbls[3], "Im[B.n], Tesla");
      break;
    case 3:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|B.t|, Tesla");
      strcpy(p.lbls[2], "Re[B.t], Tesla");
      strcpy(p.lbls[3], "Im[B.t], Tesla");
      break;
    case 4:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|H|, Amp/m");
      break;
    case 5:
      p.Create(NumPlotPoints, 4);
      if (ProblemType == 0) {
        strcpy(p.lbls[1], "|H.n|, Amp/m");
        strcpy(p.lbls[2], "Re[H.n], Amp/m");
        strcpy(p.lbls[3], "Im[H.n], Amp/m");
      }
      break;
    case 6:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|H.t|, Amp/m");
      strcpy(p.lbls[2], "Re[H.t], Amp/m");
      strcpy(p.lbls[3], "Im[H.t], Amp/m");
      break;
    case 7:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|Je|, MA/m^2");
      strcpy(p.lbls[2], "Re[Je], MA/m^2");
      strcpy(p.lbls[3], "Im[Je], MA/m^2");
      break;
    case 8:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|Js+Je|, MA/m^2");
      strcpy(p.lbls[2], "Re[Js+Je], MA/m^2");
      strcpy(p.lbls[3], "Im[Js+Je], MA/m^2");
      break;
    default:
      p.Create(NumPlotPoints, 2);
      break;
    }
  }

  switch (LengthUnits) {
  case 1:
    strcpy(p.lbls[0], "Length, mm");
    break;
  case 2:
    strcpy(p.lbls[0], "Length, cm");
    break;
  case 3:
    strcpy(p.lbls[0], "Length, m");
    break;
  case 4:
    strcpy(p.lbls[0], "Length, mils");
    break;
  case 5:
    strcpy(p.lbls[0], "Length, um");
    break;
  default:
    strcpy(p.lbls[0], "Length, inches");
    break;
  }

  for (i = 0, k = 1, z = 0, elm = -1; i < NumPlotPoints; i++, z += dz) {
    while ((z > q[k]) && (k < (contour.GetSize() - 1)))
      k++;
    u = (z - q[k - 1]) / (q[k] - q[k - 1]);
    pt = contour[k - 1] + u * (contour[k] - contour[k - 1]);
    t = contour[k] - contour[k - 1];
    t /= abs(t);
    n = I * t;
    pt += (n * 1.e-06);

    if (elm < 0)
      elm = InTriangle(pt.re, pt.im);
    else if (InTriangleTest(pt.re, pt.im, elm) == FALSE) {
      flag = FALSE;
      for (j = 0; j < 3; j++)
        for (m = 0; m < NumList[meshelem[elm].p[j]]; m++) {
          elm = ConList[meshelem[elm].p[j]][m];
          if (InTriangleTest(pt.re, pt.im, elm) == TRUE) {
            flag = TRUE;
            m = 100;
            j = 3;
          }
        }
      if (flag == FALSE)
        elm = InTriangle(pt.re, pt.im);
    }
    if (elm >= 0)
      flag = GetPointValues(pt.re, pt.im, elm, v);
    else
      flag = FALSE;

    p.M[i][0] = z;
    if ((Frequency == 0) && (flag != FALSE)) {
      switch (PlotType) {
      case 0:
        p.M[i][1] = v.A.re;
        break;
      case 1:
        p.M[i][1] = sqrt(v.B1.Abs() * v.B1.Abs() + v.B2.Abs() * v.B2.Abs());
        break;
      case 2:
        p.M[i][1] = n.re * v.B1.re + n.im * v.B2.re;
        break;
      case 3:
        p.M[i][1] = t.re * v.B1.re + t.im * v.B2.re;
        break;
      case 4:
        p.M[i][1] = sqrt(v.H1.Abs() * v.H1.Abs() + v.H2.Abs() * v.H2.Abs());
        break;
      case 5:
        p.M[i][1] = n.re * v.H1.re + n.im * v.H2.re;
        break;
      case 6:
        p.M[i][1] = t.re * v.H1.re + t.im * v.H2.re;
        break;
      default:
        p.M[i][1] = 0;
        break;
      }
    } else if (flag != FALSE) {
      switch (PlotType) {
      case 0:
        p.M[i][1] = v.A.Abs();
        p.M[i][2] = v.A.re;
        p.M[i][3] = v.A.im;
        break;
      case 1:
        p.M[i][1] = sqrt(v.B1.Abs() * v.B1.Abs() + v.B2.Abs() * v.B2.Abs());
        break;
      case 2:
        p.M[i][2] = n.re * v.B1.re + n.im * v.B2.re;
        p.M[i][3] = n.re * v.B1.im + n.im * v.B2.im;
        p.M[i][1] = sqrt(p.M[i][2] * p.M[i][2] + p.M[i][3] * p.M[i][3]);
        break;
      case 3:
        p.M[i][2] = t.re * v.B1.re + t.im * v.B2.re;
        p.M[i][3] = t.re * v.B1.im + t.im * v.B2.im;
        p.M[i][1] = sqrt(p.M[i][2] * p.M[i][2] + p.M[i][3] * p.M[i][3]);
        break;
      case 4:
        p.M[i][1] = sqrt(v.H1.Abs() * v.H1.Abs() + v.H2.Abs() * v.H2.Abs());
        break;
      case 5:
        p.M[i][2] = n.re * v.H1.re + n.im * v.H2.re;
        p.M[i][3] = n.re * v.H1.im + n.im * v.H2.im;
        p.M[i][1] = sqrt(p.M[i][2] * p.M[i][2] + p.M[i][3] * p.M[i][3]);
        break;
      case 6:
        p.M[i][2] = t.re * v.H1.re + t.im * v.H2.re;
        p.M[i][3] = t.re * v.H1.im + t.im * v.H2.im;
        p.M[i][1] = sqrt(p.M[i][2] * p.M[i][2] + p.M[i][3] * p.M[i][3]);
        break;
      case 7:
        p.M[i][2] = v.Je.re;
        p.M[i][3] = v.Je.im;
        p.M[i][1] = abs(v.Je);
        break;
      case 8:
        p.M[i][2] = v.Je.re + v.Js.re;
        p.M[i][3] = v.Je.im + v.Js.im;
        p.M[i][1] = abs(v.Je + v.Js);
        break;
      default:
        p.M[i][1] = 0;
        break;
      }
    }
  }

  free(q);
}

BOOL CFemmviewDoc::InTriangleTest(double x, double y, int i)
{
  if ((i < 0) || (i >= meshelem.GetSize()))
    return FALSE;

  int j, k;
  double z;

  for (j = 0; j < 3; j++) {
    k = j + 1;
    if (k == 3)
      k = 0;
    // Case 1: p[k]>p[j]
    if (meshelem[i].p[k] > meshelem[i].p[j]) {
      z = (meshnode[meshelem[i].p[k]].x - meshnode[meshelem[i].p[j]].x) * (y - meshnode[meshelem[i].p[j]].y) - (meshnode[meshelem[i].p[k]].y - meshnode[meshelem[i].p[j]].y) * (x - meshnode[meshelem[i].p[j]].x);
      if (z < 0)
        return FALSE;
    }
    // Case 2: p[k]<p[j]
    else {
      z = (meshnode[meshelem[i].p[j]].x - meshnode[meshelem[i].p[k]].x) * (y - meshnode[meshelem[i].p[k]].y) - (meshnode[meshelem[i].p[j]].y - meshnode[meshelem[i].p[k]].y) * (x - meshnode[meshelem[i].p[k]].x);
      if (z > 0)
        return FALSE;
    }
  }

  return TRUE;
}

CPointVals::CPointVals()
{
  A = 0; // vector potential
  B1 = 0;
  B2 = 0; // flux density
  mu1 = 1;
  mu2 = 1; // permeability
  H1 = 0;
  H2 = 0; // field intensity
  Je = 0;
  Js = 0; // eddy current and source current densities
  c = 0; // conductivity
  E = 0; // energy stored in the magnetic field
  Ph = 0; // power dissipated by hysteresis
  Pe = 0;
  ff = 1;
}

CComplex CFemmviewDoc::Ctr(int i)
{
  CComplex p, c;
  int j;

  for (j = 0, c = 0; j < 3; j++) {
    p.Set(meshnode[meshelem[i].p[j]].x / 3., meshnode[meshelem[i].p[j]].y / 3.);
    c += p;
  }

  return c;
}

double CFemmviewDoc::ElmArea(int i)
{
  int j, n[3];
  double b0, b1, c0, c1;

  for (j = 0; j < 3; j++)
    n[j] = meshelem[i].p[j];

  b0 = meshnode[n[1]].y - meshnode[n[2]].y;
  b1 = meshnode[n[2]].y - meshnode[n[0]].y;
  c0 = meshnode[n[2]].x - meshnode[n[1]].x;
  c1 = meshnode[n[0]].x - meshnode[n[2]].x;
  return (b0 * c1 - b1 * c0) / 2.;
}

double CFemmviewDoc::ElmArea(CElement* elm)
{
  int j, n[3];
  double b0, b1, c0, c1;

  for (j = 0; j < 3; j++)
    n[j] = elm->p[j];

  b0 = meshnode[n[1]].y - meshnode[n[2]].y;
  b1 = meshnode[n[2]].y - meshnode[n[0]].y;
  c0 = meshnode[n[2]].x - meshnode[n[1]].x;
  c1 = meshnode[n[0]].x - meshnode[n[2]].x;
  return (b0 * c1 - b1 * c0) / 2.;
}

CComplex CFemmviewDoc::GetJA(int k, CComplex* J, CComplex* A)
{
  // returns current density with contribution from all sources in
  // units of MA/m^2

  int i, blk, lbl, crc;
  double r, c, rn;
  CComplex Javg;

  blk = meshelem[k].blk;
  lbl = meshelem[k].lbl;
  crc = blocklist[lbl].InCircuit;

  // first, get A
  for (i = 0; i < 3; i++) {
    if (ProblemType == 0)
      A[i] = (meshnode[meshelem[k].p[i]].A);
    else {
      rn = meshnode[meshelem[k].p[i]].x * LengthConv[LengthUnits];
      if (fabs(rn / LengthConv[LengthUnits]) < 1.e-06)
        A[i] = 0;
      else
        A[i] = (meshnode[meshelem[k].p[i]].A) / (2. * PI * rn);
    }
  }

  if (ProblemType == 1)
    r = Re(Ctr(k)) * LengthConv[LengthUnits];

  // contribution from explicitly specified J
  for (i = 0; i < 3; i++)
    J[i] = blockproplist[blk].Jr + I * blockproplist[blk].Ji;
  Javg = blockproplist[blk].Jr + I * blockproplist[blk].Ji;

  c = blockproplist[blk].Cduct;
  if ((blockproplist[blk].Lam_d != 0) && (blockproplist[blk].LamType == 0))
    c = 0;
  if (blocklist[lbl].FillFactor > 0)
    c = 0;

  // contribution from eddy currents;
  if (Frequency != 0)
    for (i = 0; i < 3; i++) {
      J[i] -= I * Frequency * 2. * PI * c * A[i];
      Javg -= I * Frequency * 2. * PI * c * A[i] / 3.;
    }

  // contribution from circuit currents //
  if (crc >= 0) {
    if (blocklist[lbl].Case == 0) { // specified voltage
      if (ProblemType == 0) {
        for (i = 0; i < 3; i++)
          J[i] -= c * blocklist[lbl].dVolts;
        Javg -= c * blocklist[lbl].dVolts;
      } else {
        for (i = 0; i < 3; i++) {
          rn = meshnode[meshelem[k].p[i]].x;
          if (fabs(rn / LengthConv[LengthUnits]) < 1.e-06)
            J[i] -= c * blocklist[lbl].dVolts / r;
          else
            J[i] -= c * blocklist[lbl].dVolts / (rn * LengthConv[LengthUnits]);
        }
        Javg -= c * blocklist[lbl].dVolts / r;
      }
    } else {
      for (i = 0; i < 3; i++)
        J[i] += blocklist[lbl].J; // specified current
      Javg += blocklist[lbl].J;
    }
  }

  // convert results to A/m^2
  for (i = 0; i < 3; i++)
    J[i] *= 1.e06;

  return (Javg * 1.e06);
}

CComplex CFemmviewDoc::PlnInt(double a, CComplex* u, CComplex* v)
{
  int i;
  CComplex z[3], x;

  z[0] = 2. * u[0] + u[1] + u[2];
  z[1] = u[0] + 2. * u[1] + u[2];
  z[2] = u[0] + u[1] + 2. * u[2];

  for (i = 0, x = 0; i < 3; i++)
    x += v[i] * z[i];
  return a * x / 12.;
}

CComplex CFemmviewDoc::AxiInt(double a, CComplex* u, CComplex* v, double* r)
{
  int i;
  static CComplex M[3][3];
  CComplex x, z[3];

  M[0][0] = 6. * r[0] + 2. * r[1] + 2. * r[2];
  M[0][1] = 2. * r[0] + 2. * r[1] + 1. * r[2];
  M[0][2] = 2. * r[0] + 1. * r[1] + 2. * r[2];
  M[1][1] = 2. * r[0] + 6. * r[1] + 2. * r[2];
  M[1][2] = 1. * r[0] + 2. * r[1] + 2. * r[2];
  M[2][2] = 2. * r[0] + 2. * r[1] + 6. * r[2];
  M[1][0] = M[0][1];
  M[2][0] = M[0][2];
  M[2][1] = M[1][2];

  for (i = 0; i < 3; i++)
    z[i] = M[i][0] * u[0] + M[i][1] * u[1] + M[i][2] * u[2];
  for (i = 0, x = 0; i < 3; i++)
    x += v[i] * z[i];
  return PI * a * x / 30.;
}

CComplex CFemmviewDoc::HenrotteVector(int k)
{
  int i, n[3];
  double b[3], c[3], da;
  CComplex v;

  for (i = 0; i < 3; i++)
    n[i] = meshelem[k].p[i];

  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
  da = (b[0] * c[1] - b[1] * c[0]);

  for (i = 0, v = 0; i < 3; i++)
    v -= meshnode[n[i]].msk * (b[i] + I * c[i]) / (da * LengthConv[LengthUnits]); // grad

  return v;
}

CComplex CFemmviewDoc::BlockIntegral(int inttype)
{
  int i, k;
  CComplex c, y, z, J, mu1, mu2, B1, B2, H1, H2, F1, F2;
  CComplex A[3], Jn[3], U[3], V[3];
  double a, sig, R, B1p, B2p, Jp;
  double r[3];

  // make weighted stress tensor mask for integrals where it's needed
#define WEIGHTED_INTEGRALS 9
  int mm[WEIGHTED_INTEGRALS] = { 18, 19, 20, 21, 22, 23, 25, 26, 27 };
  BOOL bMaskIntegral = FALSE;
  for (k = 0; k < WEIGHTED_INTEGRALS; k++) {
    if (mm[k] == inttype) {
      MakeMask();
      bMaskIntegral = TRUE;
      break;
    }
  }

  z = 0;
  for (i = 0; i < 3; i++)
    U[i] = 1.;

  if (inttype == 6)
    z = BlockIntegral(3) + BlockIntegral(4); // total losses
  else
    for (i = 0; i < meshelem.GetSize(); i++) {
      // Integrals performed only over the selected region (as opposed to Mask Integrals),
      // which are performed over the entire solution domain using the mask to weight the contribution
      // of the various elements)
      if ((blocklist[meshelem[i].lbl].IsSelected == TRUE) && (!bMaskIntegral)) {

        // compute some useful quantities employed by most integrals...
        J = GetJA(i, Jn, A);
        a = ElmArea(i) * pow(LengthConv[LengthUnits], 2.);
        if (ProblemType == 1) {
          for (k = 0; k < 3; k++)
            r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
          R = (r[0] + r[1] + r[2]) / 3.;
        }

        // now, compute the desired integral;
        switch (inttype) {
        case 0: //  A.J
          for (k = 0; k < 3; k++)
            V[k] = Jn[k].Conj();
          if (ProblemType == 0)
            y = PlnInt(a, A, V) * Depth;
          else
            y = AxiInt(a, A, V, r);
          z += y;

          break;

        case 11: // x (or r) direction Lorentz force, SS part.
          B2 = meshelem[i].B2;
          y = -(B2.re * J.re + B2.im * J.im);
          if (ProblemType == 1)
            y = 0;
          else
            y *= Depth;
          if (Frequency != 0)
            y *= 0.5;
          z += (a * y);
          break;

        case 12: // y (or z) direction Lorentz force, SS part.
          for (k = 0; k < 3; k++)
            V[k] = Re(meshelem[i].B1 * Jn[k].Conj());
          if (ProblemType == 0)
            y = PlnInt(a, U, V) * Depth;
          else
            y = AxiInt(-a, U, V, r);
          if (Frequency != 0)
            y *= 0.5;
          z += y;

          break;

        case 13: // x (or r) direction Lorentz force, 2x part.
          if ((Frequency != 0) && (ProblemType == 0)) {
            B2 = meshelem[i].B2;
            y = -(B2.re * J.re - B2.im * J.im) - I * (B2.re * J.im + B2.im * J.re);
            z += 0.5 * (a * y * Depth);
          }
          break;

        case 14: // y (or z) direction Lorentz force, 2x part.
          if (Frequency != 0) {
            B1 = meshelem[i].B1;
            B2 = meshelem[i].B2;
            y = (B1.re * J.re - B1.im * J.im) + I * (B1.re * J.im + B1.im * J.re);
            if (ProblemType == 1)
              y = (-y * 2. * PI * R);
            else
              y *= Depth;
            z += (a * y) / 2.;
          }
          break;

        case 16: // Lorentz Torque, 2x
          if ((Frequency != 0) && (ProblemType == 0)) {
            B1 = meshelem[i].B1;
            B2 = meshelem[i].B2;
            c = Ctr(i) * LengthConv[LengthUnits];
            y = c.re * ((B1.re * J.re - B1.im * J.im) + I * (B1.re * J.im + B1.im * J.re))
                + c.im * ((B2.re * J.re - B2.im * J.im) + I * (B2.re * J.im + B2.im * J.re));
            z += 0.5 * (a * y * Depth);
          }
          break;

        case 15: // Lorentz Torque, SS part.
          if (ProblemType == 0) {
            B1 = meshelem[i].B1;
            B2 = meshelem[i].B2;
            c = Ctr(i) * LengthConv[LengthUnits];
            y = c.im * (B2.re * J.re + B2.im * J.im) + c.re * (B1.re * J.re + B1.im * J.im);
            if (Frequency != 0)
              y *= 0.5;
            z += (a * y * Depth);
          }
          break;

        case 1: // integrate A over the element;
          if (ProblemType == 1)
            y = AxiInt(a, U, A, r);
          else
            for (k = 0, y = 0; k < 3; k++)
              y += a * Depth * A[k] / 3.;

          z += y;
          break;

        case 2: // stored energy
          if (ProblemType == 1)
            a *= (2. * PI * R);
          else
            a *= Depth;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          if (Frequency != 0) {
            // have to compute the energy stored in a special way for
            // wound regions subject to prox and skin effects
            if (blockproplist[meshelem[i].blk].LamType > 2) {
              CComplex mu;
              mu = muo * blocklist[meshelem[i].lbl].mu;
              double u = blocklist[meshelem[i].lbl].LocalEnergy;
              y = a * Re(B1 * conj(B1) + B2 * conj(B2)) * Re(1. / mu) / 4.;
              y += a * Re(J * conj(J)) * u / 4.;
            } else
              y = a * blockproplist[meshelem[i].blk].DoEnergy(B1, B2);
          } else {
            // correct H and energy stored in magnet for second-quadrant
            // representation of a PM.
            if (blockproplist[meshelem[i].blk].H_c != 0) {
              int bk = meshelem[i].blk;

              // in the linear case:
              if (blockproplist[bk].BHpoints == 0) {
                CComplex Hc;
                mu1 = blockproplist[bk].mu_x;
                mu2 = blockproplist[bk].mu_y;
                H1 = B1 / (mu1 * muo);
                H2 = B2 / (mu2 * muo);
                Hc = blockproplist[bk].H_c * exp(I * PI * meshelem[i].magdir / 180.);
                H1 = H1 - Re(Hc);
                H2 = H2 - Im(Hc);
                y = a * 0.5 * muo * (mu1.re * H1.re * H1.re + mu2.re * H2.re * H2.re);
              } else { // the material is nonlinear
                y = blockproplist[bk].DoEnergy(B1.re, B2.re);
                y = y + blockproplist[bk].Nrg
                    - blockproplist[bk].H_c * Re((B1.re + I * B2.re) / exp(I * PI * meshelem[i].magdir / 180.));
                y *= a;
              }
            } else
              y = a * blockproplist[meshelem[i].blk].DoEnergy(B1.re, B2.re);

            // add in "local" stored energy for wound that would be subject to
            // prox and skin effect for nonzero frequency cases.
            if (blockproplist[meshelem[i].blk].LamType > 2) {
              double u = blocklist[meshelem[i].lbl].LocalEnergy;
              y += a * Re(J * J) * u / 2.;
            }
          }
          y *= AECF(i); // correction for axisymmetric external region;

          z += y;
          break;

        case 3: // Hysteresis & Laminated eddy current losses
          if (Frequency != 0) {
            if (ProblemType == 1)
              a *= (2. * PI * R);
            else
              a *= Depth;
            B1 = meshelem[i].B1;
            B2 = meshelem[i].B2;
            GetMu(B1, B2, mu1, mu2, i);
            H1 = B1 / (mu1 * muo);
            H2 = B2 / (mu2 * muo);

            y = a * PI * Frequency * Im(H1 * B1.Conj() + H2 * B2.Conj());
            z += y;
          }
          break;

        case 4: // Resistive Losses
          if (abs(blocklist[meshelem[i].lbl].o) == 0)
            sig = 0;
          else
            sig = 1.e06 / Re(1. / blocklist[meshelem[i].lbl].o);
          if ((blockproplist[meshelem[i].blk].Lam_d != 0) && (blockproplist[meshelem[i].blk].LamType == 0))
            sig = 0;
          if (sig != 0) {

            if (ProblemType == 0) {
              for (k = 0; k < 3; k++)
                V[k] = Jn[k].Conj() / sig;
              y = PlnInt(a, Jn, V) * Depth;
            }

            if (ProblemType == 1)
              y = 2. * PI * R * a * J * conj(J) / sig;

            if (Frequency != 0)
              y /= 2.;
            z += y;
          }
          break;

        case 5: // cross-section area
          z += a;
          break;

        case 10: // volume
          if (ProblemType == 1)
            a *= (2. * PI * R);
          else
            a *= Depth;
          z += a;
          break;

        case 7: // total current in block;
          z += a * J;

          break;

        case 8: // integrate x or r part of b over the block
          if (ProblemType == 1)
            a *= (2. * PI * R);
          else
            a *= Depth;
          z += (a * meshelem[i].B1);
          break;

        case 9: // integrate y or z part of b over the block
          if (ProblemType == 1)
            a *= (2. * PI * R);
          else
            a *= Depth;
          z += (a * meshelem[i].B2);
          break;

        case 17: // Coenergy
          if (ProblemType == 1)
            a *= (2. * PI * R);
          else
            a *= Depth;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          if (Frequency != 0) {
            // have to compute the energy stored in a special way for
            // wound regions subject to prox and skin effects
            if (blockproplist[meshelem[i].blk].LamType > 2) {
              CComplex mu;
              mu = muo * blocklist[meshelem[i].lbl].mu;
              double u = blocklist[meshelem[i].lbl].LocalEnergy;
              y = a * Re(B1 * conj(B1) + B2 * conj(B2)) * Re(1. / mu) / 4.;
              y += a * Re(J * conj(J)) * u / 4.;
            } else
              y = a * blockproplist[meshelem[i].blk].DoCoEnergy(B1, B2);
          } else {
            y = a * blockproplist[meshelem[i].blk].DoCoEnergy(B1.re, B2.re);

            // add in "local" stored energy for wound that would be subject to
            // prox and skin effect for nonzero frequency cases.
            if (blockproplist[meshelem[i].blk].LamType > 2) {
              double u = blocklist[meshelem[i].lbl].LocalEnergy;
              y += a * Re(J * J) * u / 2.;
            }
          }
          y *= AECF(i); // correction for axisymmetric external region;

          z += y;
          break;

        case 24: // Moment of Inertia-like integral

          // For axisymmetric problems, compute the moment
          // of inertia about the r=0 axis.
          if (ProblemType == 1) {
            for (k = 0; k < 3; k++)
              V[k] = r[k];
            y = AxiInt(a, V, V, r);
          }

          // For planar problems, compute the moment of
          // inertia about the z=axis.
          else {
            for (k = 0; k < 3; k++) {
              U[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
              V[k] = meshnode[meshelem[i].p[k]].y * LengthConv[LengthUnits];
            }
            y = U[0] * U[0] + U[1] * U[1] + U[2] * U[2];
            y += U[0] * U[1] + U[0] * U[2] + U[1] * U[2];
            y += V[0] * V[0] + V[1] * V[1] + V[2] * V[2];
            y += V[0] * V[1] + V[0] * V[2] + V[1] * V[2];
            y *= (a * Depth / 6.);
          }

          z += y;
          break;

        case 28: // x (or r) direction Lorentz force, 1X part for incremental AC problems
          B2 = meshelem[i].B2;
          B2p = meshelem[i].B2p;
          Jp = meshelem[i].Jp;
          y = -(B2p * J + B2 * Jp);
          if (ProblemType == 1)
            y = 0;
          else
            y *= Depth;
          z += (a * y);
          break;

        case 29: // y (or z) direction Lorentz force, 1X part for incremental AC problems
          B1 = meshelem[i].B1;
          B1p = meshelem[i].B1p;
          Jp = meshelem[i].Jp;
          for (k = 0; k < 3; k++) {
            U[k] = 1;
            V[k] = (B1p * J + B1 * Jp);
          }
          if (ProblemType == 0)
            y = PlnInt(a, U, V) * Depth;
          else
            y = AxiInt(-a, U, V, r);

          z += y;

          break;

        case 30: // Lorentz Torque, 1X part for incremental AC problems
          if (ProblemType == 0) {
            B1 = meshelem[i].B1;
            B2 = meshelem[i].B2;
            B1p = meshelem[i].B1p;
            B2p = meshelem[i].B2p;
            Jp = meshelem[i].Jp;
            c = Ctr(i) * LengthConv[LengthUnits];
            y = c.im * (B2 * Jp + B2p * J) + c.re * (B1 * Jp + B1p * J);
            z += (a * y * Depth);
          }
          break;
        default:
          break;
        }
      }

      // integrals that need to be evaluated over all elements,
      // regardless of which elements are actually selected.
      if (bMaskIntegral) {
        a = ElmArea(i) * pow(LengthConv[LengthUnits], 2.);
        if (ProblemType == 1) {
          for (k = 0; k < 3; k++)
            r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
          R = (r[0] + r[1] + r[2]) / 3.;
          a *= (2. * PI * R);
        } else
          a *= Depth;

        switch (inttype) {

        case 18: // x (or r) direction Henrotte force, SS part.
          if (ProblemType != 0)
            break;

          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          c = HenrotteVector(i);
          y = (((B1 * conj(B1)) - (B2 * conj(B2))) * Re(c) + 2. * Re(B1 * conj(B2)) * Im(c)) / (2. * muo);
          if (Frequency != 0)
            y /= 2.;

          y *= AECF(i); // correction for axisymmetric external region;

          z += (a * y);
          break;

        case 19: // y (or z) direction Henrotte force, SS part.

          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          c = HenrotteVector(i);

          y = (((B2 * conj(B2)) - (B1 * conj(B1))) * Im(c) + 2. * Re(B1 * conj(B2)) * Re(c)) / (2. * muo);

          y *= AECF(i); // correction for axisymmetric external region;

          if (Frequency != 0)
            y /= 2.;
          z += (a * y);

          break;

        case 20: // x (or r) direction Henrotte force, 2x part.

          if (ProblemType != 0)
            break;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          c = HenrotteVector(i);
          z += a * ((((B1 * B1) - (B2 * B2)) * Re(c) + 2. * B1 * B2 * Im(c)) / (4. * muo)) * AECF(i);

          break;

        case 21: // y (or z) direction Henrotte force, 2x part.

          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          c = HenrotteVector(i);
          z += a * ((((B2 * B2) - (B1 * B1)) * Im(c) + 2. * B1 * B2 * Re(c)) / (4. * muo)) * AECF(i);

          break;

        case 22: // Henrotte torque, SS part.
          if (ProblemType != 0)
            break;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          c = HenrotteVector(i);

          F1 = (((B1 * conj(B1)) - (B2 * conj(B2))) * Re(c) + 2. * Re(B1 * conj(B2)) * Im(c)) / (2. * muo);
          F2 = (((B2 * conj(B2)) - (B1 * conj(B1))) * Im(c) + 2. * Re(B1 * conj(B2)) * Re(c)) / (2. * muo);

          for (c = 0, k = 0; k < 3; k++)
            c += meshnode[meshelem[i].p[k]].CC() * LengthConv[LengthUnits] / 3.;

          y = Re(c) * F2 - Im(c) * F1;
          if (Frequency != 0)
            y /= 2.;
          y *= AECF(i);
          z += (a * y);

          break;

        case 23: // Henrotte torque, 2x part.

          if (ProblemType != 0)
            break;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          c = HenrotteVector(i);
          F1 = (((B1 * B1) - (B2 * B2)) * Re(c) + 2. * B1 * B2 * Im(c)) / (4. * muo);
          F2 = (((B2 * B2) - (B1 * B1)) * Im(c) + 2. * B1 * B2 * Re(c)) / (4. * muo);

          for (c = 0, k = 0; k < 3; k++)
            c += meshnode[meshelem[i].p[k]].CC() * LengthConv[LengthUnits] / 3;

          z += a * (Re(c) * F2 - Im(c) * F1) * AECF(i);

          break;

        case 25: // x (or r) direction Henrotte force, 1x part for incremental AC problems

          if (ProblemType != 0)
            break;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          B1p = meshelem[i].B1p;
          B2p = meshelem[i].B2p;
          c = HenrotteVector(i);
          z += a * ((B1 * B1p - B2 * B2p) * Re(c) + (B1p * B2 + B1 * B2p) * Im(c)) / muo * AECF(i);

          break;

        case 26: // y (or z) direction Henrotte force, 1x part for incremental AC problems

          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          B1p = meshelem[i].B1p;
          B2p = meshelem[i].B2p;
          c = HenrotteVector(i);
          z += a * ((B1p * B2 + B1 * B2p) * Re(c) + (-(B1 * B1p) + B2 * B2p) * Im(c)) / muo * AECF(i);

          break;

        case 27: // Henrotte torque, 1x part for incremental AC problems

          if (ProblemType != 0)
            break;
          B1 = meshelem[i].B1;
          B2 = meshelem[i].B2;
          B1p = meshelem[i].B1p;
          B2p = meshelem[i].B2p;
          c = HenrotteVector(i);
          F1 = ((B1 * B1p - B2 * B2p) * Re(c) + (B1p * B2 + B1 * B2p) * Im(c)) / muo;
          F2 = ((B1p * B2 + B1 * B2p) * Re(c) + (-(B1 * B1p) + B2 * B2p) * Im(c)) / muo;

          for (c = 0, k = 0; k < 3; k++)
            c += meshnode[meshelem[i].p[k]].CC() * LengthConv[LengthUnits] / 3;

          z += a * (Re(c) * F2 - Im(c) * F1) * AECF(i);

          break;

        default:
          break;
        }
      }
    }

  return z;
}

void CFemmviewDoc::LineIntegral(int inttype, CComplex* z)
{
  // inttype	Integral
  //		0	B.n
  //		1	H.t
  //		2	Contour length
  //		3	Stress Tensor Force
  //		4	Stress Tensor Torque
  //		5	(B.n)^2

  // inttype==0 => B.n
  if (inttype == 0) {
    CComplex a0, a1;
    CPointVals u;
    double l;
    int i, k;

    k = (int)contour.GetSize();
    GetPointValues(contour[0].re, contour[0].im, u);
    a0 = u.A;
    GetPointValues(contour[k - 1].re, contour[k - 1].im, u);
    a1 = u.A;
    if (ProblemType == 0) {
      for (i = 0, l = 0; i < k - 1; i++)
        l += abs(contour[i + 1] - contour[i]);
      l *= LengthConv[LengthUnits];
      z[0] = (a0 - a1) * Depth;
      if (l != 0)
        z[1] = z[0] / (l * Depth);
    } else {
      for (i = 0, l = 0; i < k - 1; i++)
        l += (PI * (contour[i].re + contour[i + 1].re) * abs(contour[i + 1] - contour[i]));
      l *= pow(LengthConv[LengthUnits], 2.);
      z[0] = a1 - a0;
      if (l != 0)
        z[1] = z[0] / l;
    }
  }

  // inttype==1 => H.t
  if (inttype == 1) {
    CComplex n, t, pt, Ht;
    CPointVals v;
    double dz, u, l;
    int i, j, k, m, elm;
    int NumPlotPoints = d_LineIntegralPoints;
    BOOL flag;

    z[0] = 0;
    for (k = 1; k < contour.GetSize(); k++) {
      dz = abs(contour[k] - contour[k - 1]) / ((double)NumPlotPoints);
      for (i = 0, elm = -1; i < NumPlotPoints; i++) {
        u = (((double)i) + 0.5) / ((double)NumPlotPoints);
        pt = contour[k - 1] + u * (contour[k] - contour[k - 1]);
        t = contour[k] - contour[k - 1];
        t /= abs(t);
        n = I * t;
        pt += n * 1.e-06;

        if (elm < 0)
          elm = InTriangle(pt.re, pt.im);
        else if (InTriangleTest(pt.re, pt.im, elm) == FALSE) {
          flag = FALSE;
          for (j = 0; j < 3; j++)
            for (m = 0; m < NumList[meshelem[elm].p[j]]; m++) {
              elm = ConList[meshelem[elm].p[j]][m];
              if (InTriangleTest(pt.re, pt.im, elm) == TRUE) {
                flag = TRUE;
                m = 100;
                j = 3;
              }
            }
          if (flag == FALSE)
            elm = InTriangle(pt.re, pt.im);
        }
        if (elm >= 0)
          flag = GetPointValues(pt.re, pt.im, elm, v);
        else
          flag = FALSE;

        if (flag == TRUE) {
          Ht = t.re * v.H1 + t.im * v.H2;
          z[0] += (Ht * dz * LengthConv[LengthUnits]);
        }
      }

      for (i = 0, l = 0; i < contour.GetSize() - 1; i++)
        l += abs(contour[i + 1] - contour[i]);
      l *= LengthConv[LengthUnits];
      if (l != 0)
        z[1] = z[0] / l;
    }
  }

  // inttype==2 => Contour Length
  if (inttype == 2) {
    int i, k;
    k = (int)contour.GetSize();
    for (i = 0, z[0].re = 0; i < k - 1; i++)
      z[0].re += abs(contour[i + 1] - contour[i]);
    z[0].re *= LengthConv[LengthUnits];

    if (ProblemType == 1) {
      for (i = 0, z[0].im = 0; i < k - 1; i++)
        z[0].im += (PI * (contour[i].re + contour[i + 1].re) * abs(contour[i + 1] - contour[i]));
      z[0].im *= pow(LengthConv[LengthUnits], 2.);
    } else {
      z[0].im = z[0].re * Depth;
    }
  }

  // inttype==3 => Stress Tensor Force
  if (inttype == 3) {
    CComplex n, t, pt, Hn, Bn, BH, dF1, dF2;
    CPointVals v;
    double dz, dza, u;
    int i, j, k, m, elm;
    int NumPlotPoints = d_LineIntegralPoints;
    BOOL flag;

    for (i = 0; i < 4; i++)
      z[i] = 0;

    for (k = 1; k < contour.GetSize(); k++) {
      dz = abs(contour[k] - contour[k - 1]) / ((double)NumPlotPoints);
      for (i = 0, elm = -1; i < NumPlotPoints; i++) {
        u = (((double)i) + 0.5) / ((double)NumPlotPoints);
        pt = contour[k - 1] + u * (contour[k] - contour[k - 1]);
        t = contour[k] - contour[k - 1];
        t /= abs(t);
        n = I * t;
        pt += n * 1.e-06;

        if (elm < 0)
          elm = InTriangle(pt.re, pt.im);
        else if (InTriangleTest(pt.re, pt.im, elm) == FALSE) {
          flag = FALSE;
          for (j = 0; j < 3; j++)
            for (m = 0; m < NumList[meshelem[elm].p[j]]; m++) {
              elm = ConList[meshelem[elm].p[j]][m];
              if (InTriangleTest(pt.re, pt.im, elm) == TRUE) {
                flag = TRUE;
                m = 100;
                j = 3;
              }
            }
          if (flag == FALSE)
            elm = InTriangle(pt.re, pt.im);
        }
        if (elm >= 0)
          flag = GetPointValues(pt.re, pt.im, elm, v);
        else
          flag = FALSE;

        if (flag == TRUE)
          if (Frequency == 0) {
            Hn = n.re * v.H1 + n.im * v.H2;
            Bn = n.re * v.B1 + n.im * v.B2;
            BH = v.B1 * v.H1 + v.B2 * v.H2;
            dF1 = v.H1 * Bn + v.B1 * Hn - n.re * BH;
            dF2 = v.H2 * Bn + v.B2 * Hn - n.im * BH;

            dza = dz * LengthConv[LengthUnits];
            if (ProblemType == 1) {
              dza *= 2. * PI * pt.re * LengthConv[LengthUnits];
              dF1 = 0;
            } else
              dza *= Depth;

            z[0] += (dF1 * dza / 2.);
            z[1] += (dF2 * dza / 2.);
          } else {
            Hn = n.re * v.H1 + n.im * v.H2;
            Bn = n.re * v.B1 + n.im * v.B2;
            BH = v.B1 * v.H1 + v.B2 * v.H2;
            dF1 = v.H1 * Bn + v.B1 * Hn - n.re * BH;
            dF2 = v.H2 * Bn + v.B2 * Hn - n.im * BH;

            dza = dz * LengthConv[LengthUnits];
            if (ProblemType == 1) {
              dza *= 2. * PI * pt.re * LengthConv[LengthUnits];
              dF1 = 0;
            } else
              dza *= Depth;

            z[0] += (dF1 * dza / 4.);
            z[1] += (dF2 * dza / 4.);

            BH = v.B1 * v.H1.Conj() + v.B2 * v.H2.Conj();

            if (ProblemType != 1)
              dF1 = v.H1 * Bn.Conj() + v.B1 * Hn.Conj() - n.re * BH;
            dF2 = v.H2 * Bn.Conj() + v.B2 * Hn.Conj() - n.im * BH;

            z[2] += (dF1 * dza / 4.);
            z[3] += (dF2 * dza / 4.);
          }
      }
    }
  }

  // inttype==4 => Stress Tensor Torque
  if (inttype == 4) {
    CComplex n, t, pt, Hn, Bn, BH, dF1, dF2, dT;
    CPointVals v;
    double dz, dza, u;
    int i, j, k, m, elm;
    int NumPlotPoints = d_LineIntegralPoints;
    BOOL flag;

    for (i = 0; i < 2; i++)
      z[i].Set(0, 0);

    for (k = 1; k < contour.GetSize(); k++) {
      dz = abs(contour[k] - contour[k - 1]) / ((double)NumPlotPoints);
      for (i = 0, elm = -1; i < NumPlotPoints; i++) {
        u = (((double)i) + 0.5) / ((double)NumPlotPoints);
        pt = contour[k - 1] + u * (contour[k] - contour[k - 1]);
        t = contour[k] - contour[k - 1];
        t /= abs(t);
        n = I * t;
        pt += n * 1.e-6;

        if (elm < 0)
          elm = InTriangle(pt.re, pt.im);
        else if (InTriangleTest(pt.re, pt.im, elm) == FALSE) {
          flag = FALSE;
          for (j = 0; j < 3; j++)
            for (m = 0; m < NumList[meshelem[elm].p[j]]; m++) {
              elm = ConList[meshelem[elm].p[j]][m];
              if (InTriangleTest(pt.re, pt.im, elm) == TRUE) {
                flag = TRUE;
                m = 100;
                j = 3;
              }
            }
          if (flag == FALSE)
            elm = InTriangle(pt.re, pt.im);
        }
        if (elm >= 0)
          flag = GetPointValues(pt.re, pt.im, elm, v);
        else
          flag = FALSE;

        if (flag == TRUE)
          if (Frequency == 0) {
            Hn = n.re * v.H1 + n.im * v.H2;
            Bn = n.re * v.B1 + n.im * v.B2;
            BH = v.B1 * v.H1 + v.B2 * v.H2;
            dF1 = v.H1 * Bn + v.B1 * Hn - n.re * BH;
            dF2 = v.H2 * Bn + v.B2 * Hn - n.im * BH;
            dT = pt.re * dF2 - dF1 * pt.im;
            dza = dz * LengthConv[LengthUnits] * LengthConv[LengthUnits];

            z[0] += (dT * dza * Depth / 2.);
          } else {
            Hn = n.re * v.H1 + n.im * v.H2;
            Bn = n.re * v.B1 + n.im * v.B2;
            BH = v.B1 * v.H1 + v.B2 * v.H2;
            dF1 = v.H1 * Bn + v.B1 * Hn - n.re * BH;
            dF2 = v.H2 * Bn + v.B2 * Hn - n.im * BH;
            dT = pt.re * dF2 - dF1 * pt.im;
            dza = dz * LengthConv[LengthUnits] * LengthConv[LengthUnits];

            z[0] += (dT * dza * Depth / 4.);

            BH = v.B1 * v.H1.Conj() + v.B2 * v.H2.Conj();
            dF1 = v.H1 * Bn.Conj() + v.B1 * Hn.Conj() - n.re * BH;
            dF2 = v.H2 * Bn.Conj() + v.B2 * Hn.Conj() - n.im * BH;
            dT = pt.re * dF2 - dF1 * pt.im;

            z[1] += (dT * dza * Depth / 4.);
          }
      }
    }
  }

  // inttype==5 => (B.n)^2
  if (inttype == 5) {
    CComplex n, t, pt, Ht;
    CPointVals v;
    double dz, u, l;
    int i, j, k, m, elm;
    int NumPlotPoints = d_LineIntegralPoints;
    BOOL flag;

    z[0] = 0;
    for (k = 1; k < contour.GetSize(); k++) {
      dz = abs(contour[k] - contour[k - 1]) / ((double)NumPlotPoints);
      for (i = 0, elm = -1; i < NumPlotPoints; i++) {
        u = (((double)i) + 0.5) / ((double)NumPlotPoints);
        pt = contour[k - 1] + u * (contour[k] - contour[k - 1]);
        t = contour[k] - contour[k - 1];
        t /= abs(t);
        n = I * t;
        pt += n * 1.e-06;

        if (elm < 0)
          elm = InTriangle(pt.re, pt.im);
        else if (InTriangleTest(pt.re, pt.im, elm) == FALSE) {
          flag = FALSE;
          for (j = 0; j < 3; j++)
            for (m = 0; m < NumList[meshelem[elm].p[j]]; m++) {
              elm = ConList[meshelem[elm].p[j]][m];
              if (InTriangleTest(pt.re, pt.im, elm) == TRUE) {
                flag = TRUE;
                m = 100;
                j = 3;
              }
            }
          if (flag == FALSE)
            elm = InTriangle(pt.re, pt.im);
        }
        if (elm >= 0)
          flag = GetPointValues(pt.re, pt.im, elm, v);
        else
          flag = FALSE;

        if (flag == TRUE) {
          Ht = n.re * v.B1 + n.im * v.B2;
          z[0] += (Ht * Ht.Conj() * dz * LengthConv[LengthUnits]);
        }
      }

      for (i = 0, l = 0; i < contour.GetSize() - 1; i++)
        l += abs(contour[i + 1] - contour[i]);
      l *= LengthConv[LengthUnits];
      if (l != 0)
        z[1] = z[0] / l;
    }
  }

  return;
}

int CFemmviewDoc::ClosestArcSegment(double x, double y)
{
  double d0, d1;
  int i, j;

  if (arclist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = ShortestDistanceFromArc(CComplex(x, y), arclist[0]);
  for (i = 0; i < arclist.GetSize(); i++) {
    d1 = ShortestDistanceFromArc(CComplex(x, y), arclist[i]);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

void CFemmviewDoc::GetCircle(CArcSegment& arc, CComplex& c, double& R)
{
  CComplex a0, a1, t;
  double d, tta;

  a0.Set(nodelist[arc.n0].x, nodelist[arc.n0].y);
  a1.Set(nodelist[arc.n1].x, nodelist[arc.n1].y);
  d = abs(a1 - a0); // distance between arc endpoints

  // figure out what the radius of the circle is...
  t = (a1 - a0) / d;
  tta = arc.ArcLength * PI / 180.;
  R = d / (2. * sin(tta / 2.));
  c = a0 + (d / 2. + I * sqrt(R * R - d * d / 4.)) * t; // center of the arc segment's circle...
}

double CFemmviewDoc::ShortestDistanceFromArc(CComplex p, CArcSegment& arc)
{
  double R, d, l, z;
  CComplex a0, a1, c, t;

  a0.Set(nodelist[arc.n0].x, nodelist[arc.n0].y);
  a1.Set(nodelist[arc.n1].x, nodelist[arc.n1].y);
  GetCircle(arc, c, R);
  d = abs(p - c);

  if (d == 0)
    return R;

  t = (p - c) / d;
  l = abs(p - c - R * t);
  z = arg(t / (a0 - c)) * 180 / PI;
  if ((z > 0) && (z < arc.ArcLength))
    return l;

  z = abs(p - a0);
  l = abs(p - a1);
  if (z < l)
    return z;
  return l;
}

double CFemmviewDoc::ShortestDistanceFromSegment(double p, double q, int segm)
{
  double t, x[3], y[3];

  x[0] = nodelist[linelist[segm].n0].x;
  y[0] = nodelist[linelist[segm].n0].y;
  x[1] = nodelist[linelist[segm].n1].x;
  y[1] = nodelist[linelist[segm].n1].y;

  t = ((p - x[0]) * (x[1] - x[0]) + (q - y[0]) * (y[1] - y[0])) / ((x[1] - x[0]) * (x[1] - x[0]) + (y[1] - y[0]) * (y[1] - y[0]));

  if (t > 1.)
    t = 1.;
  if (t < 0.)
    t = 0.;

  x[2] = x[0] + t * (x[1] - x[0]);
  y[2] = y[0] + t * (y[1] - y[0]);

  return sqrt((p - x[2]) * (p - x[2]) + (q - y[2]) * (q - y[2]));
}

BOOL CFemmviewDoc::ScanPreferences()
{
  FILE* fp;
  CString fname;

  fname = BinDir + "femmview.cfg";

  fp = fopen(fname, "rt");
  if (fp != NULL) {
    BOOL flag = FALSE;
    char s[1024];
    char q[1024];
    char* v;

    // parse the file
    while (fgets(s, 1024, fp) != NULL) {
      sscanf(s, "%s", q);

      if (_strnicmp(q, "<LineIntegralPoints>", 20) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_LineIntegralPoints);
        q[0] = NULL;
      }

      if (_strnicmp(q, "<WeightingScheme>", 12) == 0) {
        v = StripKey(s);
        int lastd_WeightingScheme = d_WeightingScheme;
        sscanf(v, "%i", &d_WeightingScheme);
        // update weighting scheme if the weighting scheme was changed in the prefs dialog.
        if (lastd_WeightingScheme != d_WeightingScheme) {
          WeightingScheme = d_WeightingScheme;
          bHasMask = FALSE;
        }
        q[0] = NULL;
      }

      if (_strnicmp(q, "<ShiftH>", 8) == 0) {
        v = StripKey(s);
        sscanf(v, "%i", &d_ShiftH);
        q[0] = NULL;
      }
    }
    fclose(fp);
  } else
    return FALSE;

  return TRUE;
}

void CFemmviewDoc::BendContour(double angle, double anglestep)
{
  if (angle == 0)
    return;
  if (anglestep == 0)
    anglestep = 1;

  int k, n;
  double d, tta, dtta, R;
  CComplex c, a0, a1;

  // check to see if there are at least enough
  // points to have made one line;
  k = (int)contour.GetSize() - 1;
  if (k < 1)
    return;

  // restrict the angle of the contour to 180 degrees;
  if ((angle < -180.) || (angle > 180.))
    return;
  n = (int)ceil(fabs(angle / anglestep));
  tta = angle * PI / 180.;
  dtta = tta / ((double)n);

  // pop last point off of the contour;
  a1 = contour[k];
  contour.RemoveAt(k);
  a0 = contour[k - 1];

  // compute location of arc center;
  // and radius of the circle that the
  // arc lives on.
  d = abs(a1 - a0);
  R = d / (2. * sin(fabs(tta / 2.)));
  if (tta > 0)
    c = a0 + (R / d) * (a1 - a0) * exp(I * (PI - tta) / 2.);
  else
    c = a0 + (R / d) * (a1 - a0) * exp(-I * (PI + tta) / 2.);

  // add the points on the contour
  for (k = 1; k <= n; k++)
    contour.Add(c + (a0 - c) * exp(k * I * dtta));
}

BOOL CFemmviewDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;
  return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CComplex CFemmviewDoc::GetStrandedVoltageDrop(int lbl)
{
  // Derive the voltage drop associated with a stranded and
  // current-carrying region.

  int i, k;
  CComplex dVolts, rho;
  CComplex A[3], J[3], U[3], V[3];
  double a, atot;
  double r[3];

  U[0] = 1;
  U[1] = 1;
  U[2] = 1;

  for (i = 0, dVolts = 0, atot = 0; i < meshelem.GetSize(); i++) {
    if (meshelem[i].lbl == lbl) {
      rho = blocklist[meshelem[i].lbl].o * 1.e6;
      if (Frequency == 0)
        rho = Re(rho);
      if (rho != 0)
        rho = (1. / rho);

      GetJA(i, J, A);
      a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];
      atot += a;

      if (ProblemType == AXISYMMETRIC) {
        for (k = 0; k < 3; k++)
          r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
      }

      for (k = 0; k < 3; k++)
        V[k] = (2. * PI * I * Frequency * A[k] + rho * J[k]);
      if (ProblemType == PLANAR)
        dVolts += PlnInt(a, V, U) * Depth;
      else
        dVolts += AxiInt(a, V, U, r);
    }
  }
  dVolts *= (((double)blocklist[lbl].Turns) / atot);

  return dVolts;
}

void CFemmviewDoc::GetFillFactor(int lbl)
{
  // Get the fill factor associated with a stranded and
  // current-carrying region.  For AC problems, also compute
  // the apparent conductivity and permeability for use in
  // post-processing the voltage.

  CMaterialProp* bp = &blockproplist[blocklist[lbl].BlockType];
  CBlockLabel* bl = &blocklist[lbl];
  double lc = LengthConv[LengthUnits] * LengthConv[LengthUnits];
  double atot, awire, w, d, o, fill, dd, W, R, c1, c2, c3, c4;
  int i, wiretype;
  CComplex ufd, ueff, ofd;

  // default values
  if (abs(bl->Turns) > 1)
    bl->FillFactor = 1;
  else
    bl->FillFactor = -1;
  bl->o = bp->Cduct;
  bl->mu = 0.;

  if (blockproplist[blocklist[lbl].BlockType].LamType < 3)
    return;

  // compute total area of associated block
  for (i = 0, atot = 0; i < meshelem.GetSize(); i++)
    if (meshelem[i].lbl == lbl)
      atot += ElmArea(i) * lc;
  if (atot == 0)
    return;

  wiretype = bp->LamType - 3;
  // wiretype = 0 for magnet wire
  // wiretype = 1 for stranded but non-litz wire
  // wiretype = 2 for litz wire
  // wiretype = 3 for rectangular wire
  // wiretype = 4 for 10% CCA
  // wiretype = 5 for 15% CCA

  if (wiretype == 3) // rectangular wire
  {
    w = 2. * PI * Frequency;
    d = bp->WireD * 0.001;
    bl->FillFactor = fabs(d * d * ((double)bl->Turns) / atot);
    dd = d / sqrt(bl->FillFactor); // foil pitch
    fill = d / dd; // fill for purposes of equivalent foil analysis
    o = bp->Cduct * fill * 1.e6; // effective foil conductivity in S/m
    W = w * o * muo * d * d * fill / 4.;
    if ((Frequency == 0) || (bp->Cduct == 0)) {
      bl->o = bp->Cduct * bl->FillFactor;
      bl->LocalEnergy = (dd - d) * dd * muo / 6.;
      bl->mu = 1;
      return;
    }

    // effective permeability for the equivalent foil.  Note that this is
    // the same equation as effective permeability of a lamination...
    ufd = muo * tanh(sqrt(I * W)) / sqrt(I * W);
    ueff = (fill * ufd + (1. - fill) * muo);
    bl->o = 1. / (muo / (fill * o * ufd) + I * dd * dd * (1. - fill) * muo * w / 4. - I * dd * dd * ueff * w / 12.);
    bl->o *= 1.e-6; // represent conductivity in units of MS/m for consistency with other parts of code.
    bl->mu = ueff / muo;

    // Treat local stored energy separately because it becomes ill-posed as frequency goes to zero.
    // use an approximate expression at low frequency.

    if (W > 0.1) {
      // normal higher-frequency regime
      bl->LocalEnergy = (1.e-6) * Im(1 / bl->o) / w;
    } else {
      // low-frequency asymptotic expression from Mathematica series expansion
      bl->LocalEnergy = muo * ((dd - d) * dd / 6. + (2. / 189.) * d * dd * W * W);
    }
    return;
  }

  // procedure for round wires;

  switch (wiretype) {
  // wiretype = 1 for stranded but non-litz wire
  case 1:
    R = bp->WireD * 0.0005 * sqrt((double)bp->NStrands);
    awire = PI * R * R * ((double)bl->Turns);
    break;

  // wiretype = 2 for magnet wire, litz wire, CCA
  default:
    R = bp->WireD * 0.0005;
    awire = PI * R * R * ((double)bp->NStrands) * ((double)bl->Turns);
    break;
  }
  bl->FillFactor = fabs(awire / atot);
  fill = bl->FillFactor;

  // preliminary definitions
  w = 2. * PI * Frequency; // frequency in rad/s
  o = bp->Cduct * 1.e6; // conductivity in S/m
  W = w * o * muo * R * R / 2.; // non-dimensionalized frequency
  dd = (1.6494541661869013 * R) / sqrt(fill); // foil pitch in equivalent foil geometry

  // curve fits to FEA data for frequency-dependent permeabilty and conductivity
  switch (wiretype) {
  case 0: // magnet wire
  case 1: // plain stranded
  case 2: // litz

    c1 = 0.7756067409818643 + fill * (0.6873854335408803 + fill * (0.06841584481674128 - 0.07143732702512284 * fill));
    c2 = 1.5 * fill / c1;
    c3 = 0.8824642871525136 + fill * (-0.008605512994838827 + fill * (0.7223208744682307 - 0.2157183942377177 * fill));
    c4 = log(1.5299240194394943 / sqrt(fill)) - c3 / 3.;
    break;

  case 4: // 10% CCA
    c1 = 0.7270741505617485 + 0.8902950067721367 * fill + 0.11894736885885195 * fill * fill - 0.12247276254503957 * fill * fill * fill;
    c2 = 0.006784920229549677 + 1.8942880489198526 * fill - 1.3631438759519217 * fill * fill + 0.504431701685587 * fill * fill * fill;
    c3 = 0.7879151117538901 - 0.7216375417404758 * fill + 2.266562622770727 * fill * fill - 1.1349603497265566 * fill * fill * fill;
    c4 = 1.6190358786364027 - 3.967461431323324 * fill + 4.068458362148543 * fill * fill - 1.7975534375694646 * fill * fill * fill;
    break;

  case 5: // 15% CCA
    c1 = 0.7486913529860821 + 0.9042845510838825 * fill + 0.1361040321433224 * fill * fill - 0.10652380745682069 * fill * fill * fill;
    c2 = 0.006790468527313965 + 1.8945509985370095 * fill - 1.3643501010185972 * fill * fill + 0.5036765577982594 * fill * fill * fill;
    c3 = 0.7519323306379909 - 0.7184130853903536 * fill + 2.2551599408130594 * fill * fill - 1.1439852083597326 * fill * fill * fill;
    c4 = 1.6204876714534728 - 3.966250827502031 * fill + 4.064413095233383 * fill * fill - 1.7940297437703776 * fill * fill * fill;
    break;
  }

  // can punch out early if no eddy current effects
  if ((Frequency == 0) || (bp->Cduct == 0)) {
    bl->o = bp->Cduct * fill;
    bl->LocalEnergy = muo * (2 * (c3 + 3 * c4) * R * R / fill - dd * dd) / 12;
    bl->mu = 1.; // relative permeability of the block.
    return;
  }

  // fit for frequency-dependent conductivity....
  ufd = c2 * (tanh(sqrt(c1 * I * W)) / sqrt(c1 * I * W)) + (1. - c2); // relative frequency-dependent permeability
  bl->mu = ufd;
  ofd = o * fill / (I * c4 * W + sqrt(I * c3 * W) * (1. / tanh(sqrt(I * c3 * W)))); // fit to curves in Skin4.nb
  ofd = 1. / (1. / ofd - I * w * ufd * muo * dd * dd / 12.); // don't double-book local stored energy;
  bl->o = ofd * 1.e-6; // return frequency-dependent conductivity in MS/m

  // Treat local stored energy separately because it becomes ill-posed as frequency goes to zero.
  // use an approximate expression at low frequency.
  if (W > 0.1) {
    // normal higher-frequency regime
    bl->LocalEnergy = (1.e-6) * Im(1 / bl->o) / w;
  } else {
    // low-frequency asymptotic expression from Mathematica series expansion
    bl->LocalEnergy = (muo * (21 * dd * dd * fill * (-15 + 2 * c1 * c1 * c2 * W * W) + 2 * R * R * (315 * (c3 + 3 * c4) - 2 * c3 * c3 * c3 * W * W))) / (3780. * fill);
  }
}

CComplex CFemmviewDoc::GetStrandedLinkage(int lbl)
{
  // This is a routine for the special case of determining
  // the flux linkage of a stranded conductor at zero frequency
  // when the conductor is carrying zero current.
  int i, k;
  CComplex FluxLinkage;
  CComplex A[3], J[3], U[3], V[3];
  double a, atot;
  double r[3];

  U[0] = 1;
  U[1] = 1;
  U[2] = 1;

  for (i = 0, FluxLinkage = 0, atot = 0; i < meshelem.GetSize(); i++) {
    if (meshelem[i].lbl == lbl) {
      GetJA(i, J, A);
      a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];
      atot += a;

      if (ProblemType == AXISYMMETRIC) {
        for (k = 0; k < 3; k++)
          r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
      }

      if (ProblemType == PLANAR)
        FluxLinkage += PlnInt(a, A, U) * Depth;
      else
        FluxLinkage += AxiInt(a, A, U, r);
    }
  }
  FluxLinkage *= (((double)blocklist[lbl].Turns) / atot);

  return FluxLinkage;
}

CComplex CFemmviewDoc::GetSolidAxisymmetricLinkage(int lbl)
{
  // This is a routine for the special case of determining
  // the flux linkage of a solid and axisymmetric conductor
  // t zero frequency when the conductor is carrying zero
  // current.  The trick here is to take account of the distribution
  // of the current that would be there, if there was any current.
  // In solid axisymmetric regions, the inner radius of the
  // region tends to carry a higher current density that the outer
  // edges, because the length of conductor that the current has
  // to traverse is smaller on the inner edge.

  int i, k;
  CComplex FluxLinkage;
  CComplex Aa, A[3], J[3], U[3], V[3];
  double a, atot, R;
  double r[3];

  U[0] = 1;
  U[1] = 1;
  U[2] = 1;

  for (i = 0, FluxLinkage = 0, atot = 0; i < meshelem.GetSize(); i++) {
    if (meshelem[i].lbl == lbl) {
      GetJA(i, J, A);
      Aa = (A[0] + A[1] + A[2]) / 3.;
      a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];

      for (k = 0; k < 3; k++)
        r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
      R = (r[0] + r[1] + r[2]) / 3.;

      atot += a / R;
      FluxLinkage += 2. * PI * R * a * (Aa / R);
    }
  }
  FluxLinkage *= (((double)blocklist[lbl].Turns) / atot);

  return FluxLinkage;
}

CComplex CFemmviewDoc::GetParallelLinkage(int numcirc)
{
  // routine for deducing the flux linkage of a "parallel-connected"
  // "circuit" in the annoying special case in which the
  // current carried in the circuit is zero and the frequency is zero.
  // This routine takes care of the case in which the current is divvied
  // up based on the conductivity and size of the various regions

  int i, k;
  CComplex FluxLinkage;
  CComplex Aa, A[3], J[3], U[3], V[3];
  double a, atot, R, c;
  double r[3];

  U[0] = 1;
  U[1] = 1;
  U[2] = 1;

  for (i = 0, FluxLinkage = 0, atot = 0; i < meshelem.GetSize(); i++) {
    if (blocklist[meshelem[i].lbl].InCircuit == numcirc) {
      c = blockproplist[meshelem[i].blk].Cduct;
      GetJA(i, J, A);
      a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];

      if (ProblemType == AXISYMMETRIC) {
        for (k = 0; k < 3; k++)
          r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
        R = (r[0] + r[1] + r[2]) / 3.;
        Aa = (A[0] + A[1] + A[2]) / 3.;
      }

      if (ProblemType == PLANAR) {
        FluxLinkage += PlnInt(a, A, U) * Depth * c;
        atot += (a * c);
      } else {
        FluxLinkage += 2. * PI * R * c * (Aa / R);
        atot += (a * c / R);
      }
    }
  }
  FluxLinkage /= atot;

  return FluxLinkage;
}

CComplex CFemmviewDoc::GetParallelLinkageAlt(int numcirc)
{
  // routine for deducing the flux linkage of a "parallel-connected"
  // "circuit" in the annoying special case in which the
  // current carried in the circuit is zero and the frequency is zero.
  // This routine takes care of the "punt" case in which all regions
  // in the "circuit" have been assigned a zero conductivity.
  // In this case, an even current density is applied to all regions
  // that are marked with the circuit (for both axi and planar cases).

  int i, k;
  CComplex FluxLinkage;
  CComplex Aa, A[3], J[3], U[3], V[3];
  double a, atot, R, c;
  double r[3];

  U[0] = 1;
  U[1] = 1;
  U[2] = 1;

  for (i = 0, FluxLinkage = 0, atot = 0; i < meshelem.GetSize(); i++) {
    if (blocklist[meshelem[i].lbl].InCircuit == numcirc) {
      c = blockproplist[meshelem[i].blk].Cduct;
      GetJA(i, J, A);
      a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];
      atot += a;

      if (ProblemType == AXISYMMETRIC) {
        for (k = 0; k < 3; k++)
          r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
        R = (r[0] + r[1] + r[2]) / 3.;
        Aa = (A[0] + A[1] + A[2]) / 3.;
      }

      if (ProblemType == PLANAR)
        FluxLinkage += PlnInt(a, A, U) * Depth;
      else
        FluxLinkage += AxiInt(a, A, U, r);
    }
  }
  FluxLinkage /= atot;

  return FluxLinkage;
}

CComplex CFemmviewDoc::GetVoltageDrop(int circnum)
{
  int i;
  CComplex Volts;

  Volts = 0;

  // if the circuit is a "series" circuit...
  if (circproplist[circnum].CircType == 1) {
    for (i = 0; i < blocklist.GetSize(); i++) {
      if (blocklist[i].InCircuit == circnum) {
        // if this region is solid...
        if (blocklist[i].Case == 0) {
          // still need to multiply by turns, since it indicates
          // the direction of the current;
          if (ProblemType == AXISYMMETRIC)
            Volts -= (2. * PI * blocklist[i].dVolts * blocklist[i].Turns);
          else
            Volts -= (Depth * blocklist[i].dVolts * blocklist[i].Turns);
        }
        // or if this region is stranded
        else
          Volts += GetStrandedVoltageDrop(i);
      }
    }
  } else if (circproplist[circnum].CircType == 0) {
    // root through the block labels until
    // we find one that gives us the voltage drop.
    BOOL flag = FALSE;
    for (i = 0; i < blocklist.GetSize(); i++) {
      if ((blocklist[i].InCircuit == circnum) && (blocklist[i].Case == 0)) {
        if (ProblemType == AXISYMMETRIC)
          Volts -= (2. * PI * blocklist[i].dVolts);
        else
          Volts -= (Depth * blocklist[i].dVolts);
        flag = TRUE;
        i = (int)blocklist.GetSize();
      }
    }

    // but perhaps no voltage was found.  This could be a parallel circuit
    // in which the conductivity in every block in the circuit is equal
    // to zero.  We can still have flux linkage and voltage drop here.
    // have to root through things in a brute force way to get the voltage drop.
    if (flag == FALSE) {
      int k;
      CComplex FluxLinkage;
      CComplex A[3], J[3], U[3];
      double a, atot;
      double r[3];

      U[0] = 1;
      U[1] = 1;
      U[2] = 1;

      for (i = 0, FluxLinkage = 0, atot = 0; i < meshelem.GetSize(); i++) {
        if (blocklist[meshelem[i].lbl].InCircuit == circnum) {
          GetJA(i, J, A);
          a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];
          atot += a;

          if (ProblemType == AXISYMMETRIC) {
            for (k = 0; k < 3; k++)
              r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
          }
          if (ProblemType == PLANAR)
            FluxLinkage += PlnInt(a, A, U) * Depth;
          else
            FluxLinkage += AxiInt(a, A, U, r);
        }
      }
      Volts = (2. * PI * Frequency / atot) * FluxLinkage;
    }
  }

  return Volts;
}

CComplex CFemmviewDoc::GetFluxLinkage(int circnum)
{
  int i, k;
  CComplex FluxLinkage;
  CComplex A[3], J[3];
  double a, r[3];

  // in the "normal" case, we can just use Integral of A.J
  // and divide through by i.conj to get the flux linkage.
  if ((circproplist[circnum].Amps.re != 0) || (circproplist[circnum].Amps.im != 0)) {
    for (i = 0, FluxLinkage = 0; i < meshelem.GetSize(); i++) {
      if (blocklist[meshelem[i].lbl].InCircuit == circnum) {
        GetJA(i, J, A);
        a = ElmArea(i) * LengthConv[LengthUnits] * LengthConv[LengthUnits];
        if (ProblemType == AXISYMMETRIC) {
          for (k = 0; k < 3; k++)
            r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
        }

        // for a multiturn region, there can be some "local" flux linkage due to the complex-valued
        // part of the conductivity.
        for (k = 0; k < 3; k++)
          A[k] += J[k] * blocklist[meshelem[i].lbl].LocalEnergy;

        for (k = 0; k < 3; k++)
          J[k] = J[k].Conj();
        if (ProblemType == PLANAR)
          FluxLinkage += PlnInt(a, A, J) * Depth;
        else
          FluxLinkage += AxiInt(a, A, J, r);
      }
    }

    FluxLinkage /= conj(circproplist[circnum].Amps);
  } else {
    // Rats!  The circuit of interest is not carrying any current.
    // However, the circuit can still have a non-zero flux linkage.
    // due to mutual inductance. Now, we have to go through
    // some annoying manipulations to back out the flux linkage.

    // For a non-zero frequency, things aren't too bad.  Any
    // voltage that we have must be solely due to flux linkage.
    // To get the flux linkage, just divide the voltage by the frequency.
    if (Frequency != 0)
      FluxLinkage = GetVoltageDrop(circnum) / (2. * I * PI * Frequency);

    // The zero frequency case is the interesting one, because
    // there are lots of annoying special cases.
    else {
      // First, deal with "series" circuits...
      if (circproplist[circnum].CircType == 1) {
        for (i = 0, FluxLinkage = 0; i < blocklist.GetSize(); i++) {
          if (blocklist[i].InCircuit == circnum) {
            if ((blocklist[i].Case == 1) || (ProblemType == PLANAR))
              FluxLinkage += GetStrandedLinkage(i);
            // in solid, axisymmetric case, the current distribution
            // isn't even.  We have to do some extra work to get
            // the flux linkage in this case.
            else
              FluxLinkage += GetSolidAxisymmetricLinkage(i);
          }
        }
      } else {
        BOOL flag;

        // first, see whether some of the blocks have nonzero conductivity;
        // flag=TRUE means that at least one of the blocks has a
        // nonzero conductivity.
        for (i = 0, flag = FALSE; i < blocklist.GetSize(); i++)
          if ((blocklist[i].Case == 0) && (blocklist[i].InCircuit == circnum))
            flag = TRUE;

        // if there is at least one nonzero conductivity block, we can use
        // the GetParallelLinkage routine, which is more or less driving
        // all the blocks with a fictitious voltage gradient.
        if (flag)
          FluxLinkage = GetParallelLinkage(i);
        // otherwise, treat the "punt" case, where every part of the
        // parallel "circuit" is just assumed to have the same applied
        // current density;
        else
          FluxLinkage = GetParallelLinkageAlt(i);
      }
    }
  }

  return FluxLinkage;
}

void CFemmviewDoc::GetMagnetization(int n, CComplex& M1, CComplex& M2)
{
  // Puts the piece-wise constant magnetization for an element into
  // M1 and M2.  The magnetization could be useful for some kinds of
  // postprocessing, e.g. computation of field gradients by integrating
  // gradient contributions from each non-air element;

  CComplex mu1, mu2, Hc;
  CComplex b1, b2;

  b1 = meshelem[n].B1;
  b2 = meshelem[n].B2;
  Hc = 0;
  mu1 = 0;
  mu2 = 0;

  if (Frequency == 0) {
    GetMu(Re(b1), Re(b2), mu1.re, mu2.re, n);
    Hc = blockproplist[meshelem[n].blk].H_c * exp(I * meshelem[n].magdir * PI / 180.);
  } else
    GetMu(b1, b2, mu1, mu2, n);

  M1 = b1 * (mu1 - 1) / (mu1 * muo) + Re(Hc);
  M2 = b2 * (mu2 - 1) / (mu2 * muo) + Im(Hc);
}

double CFemmviewDoc::AECF(int k)
{
  // Computes the permeability correction factor for axisymmetric
  // external regions.  This is sort of a kludge, but it's the best
  // way I could fit it in.  The structure of the code wasn't really
  // designed to have a permeability that varies with position in a
  // continuous way.

  if (!ProblemType)
    return 1.; // no correction for planar problems
  if (!blocklist[meshelem[k].lbl].IsExternal)
    return 1; // only need to correct for external regions

  double r = abs(meshelem[k].ctr - I * extZo);
  return (r * r * extRi) / (extRo * extRo * extRo); // permeability gets divided by this factor;
}

// versions of GetMu that sort out whether or not the AECF should be applied,
// as well as the corrections required for wound regions.
void CFemmviewDoc::GetMu(CComplex b1, CComplex b2, CComplex& mu1, CComplex& mu2, int i)
{
  if (blockproplist[meshelem[i].blk].LamType > 2) // is a region subject to prox effects
  {
    mu1 = blocklist[meshelem[i].lbl].mu;
    mu2 = mu1;
  } else
    blockproplist[meshelem[i].blk].GetMu(b1, b2, mu1, mu2);

  double aecf = AECF(i);
  mu1 /= aecf;
  mu2 /= aecf;
}

void CFemmviewDoc::GetMu(double b1, double b2, double& mu1, double& mu2, int i)
{
  blockproplist[meshelem[i].blk].GetMu(b1, b2, mu1, mu2);
  double aecf = AECF(i);
  mu1 /= aecf;
  mu2 /= aecf;
}

void CFemmviewDoc::GetH(double b1, double b2, double& h1, double& h2, int k)
{
  double mu1, mu2;
  CComplex Hc;

  GetMu(b1, b2, mu1, mu2, k);
  h1 = b1 / (mu1 * muo);
  h2 = b2 / (mu2 * muo);
  if ((d_ShiftH) && (blockproplist[meshelem[k].blk].H_c != 0)) {
    Hc = blockproplist[meshelem[k].blk].H_c * exp(I * PI * meshelem[k].magdir / 180.);
    h1 = h1 - Re(Hc);
    h2 = h2 - Im(Hc);
  }
}

void CFemmviewDoc::GetH(CComplex b1, CComplex b2, CComplex& h1, CComplex& h2, int k)
{
  CComplex mu1, mu2;

  GetMu(b1, b2, mu1, mu2, k);
  h1 = b1 / (mu1 * muo);
  h2 = b2 / (mu2 * muo);
}

void CFemmviewDoc::FindBoundaryEdges()
{
  int i, j;
  static int plus1mod3[3] = { 1, 2, 0 };
  static int minus1mod3[3] = { 2, 0, 1 };

  // Init all elements' neigh to be unfinished.
  for (i = 0; i < meshelem.GetSize(); i++) {
    for (j = 0; j < 3; j++)
      meshelem[i].n[j] = 0;
  }

  int orgi, desti;
  int ei, ni;
  BOOL done;

  // Loop all elements, to find and set there neighs.
  for (i = 0; i < meshelem.GetSize(); i++) {
    for (j = 0; j < 3; j++) {
      if (meshelem[i].n[j] == 0) {
        // Get this edge's org and dest node index,
        orgi = meshelem[i].p[plus1mod3[j]];
        desti = meshelem[i].p[minus1mod3[j]];
        done = FALSE;
        // Find this edge's neigh from the org node's list
        for (ni = 0; ni < NumList[orgi]; ni++) {
          // Find a Element around org node contained dest node of this edge.
          ei = ConList[orgi][ni];
          if (ei == i)
            continue; // Skip myself.
          // Check this Element's 3 vert to see if there exist dest node.
          if (meshelem[ei].p[0] == desti) {
            done = TRUE;
            break;
          } else if (meshelem[ei].p[1] == desti) {
            done = TRUE;
            break;
          } else if (meshelem[ei].p[2] == desti) {
            done = TRUE;
            break;
          }
        }
        if (!done) {
          // This edge must be a Boundary Edge.
          meshelem[i].n[j] = 1;
        }
      } // Finish One Edge
    } // End of One Element Loop
  } // End of Main Loop
}

double CFemmviewDoc::ShortestDistance(double p, double q, int segm)
{
  double t, x[3], y[3];

  x[0] = nodelist[linelist[segm].n0].x;
  y[0] = nodelist[linelist[segm].n0].y;
  x[1] = nodelist[linelist[segm].n1].x;
  y[1] = nodelist[linelist[segm].n1].y;

  t = ((p - x[0]) * (x[1] - x[0]) + (q - y[0]) * (y[1] - y[0])) / ((x[1] - x[0]) * (x[1] - x[0]) + (y[1] - y[0]) * (y[1] - y[0]));

  if (t > 1.)
    t = 1.;
  if (t < 0.)
    t = 0.;

  x[2] = x[0] + t * (x[1] - x[0]);
  y[2] = y[0] + t * (y[1] - y[0]);

  return sqrt((p - x[2]) * (p - x[2]) + (q - y[2]) * (q - y[2]));
}

int CFemmviewDoc::ClosestSegment(double x, double y)
{
  double d0, d1;
  int i, j;

  if (linelist.GetSize() == 0)
    return -1;

  j = 0;
  d0 = ShortestDistance(x, y, 0);
  for (i = 0; i < linelist.GetSize(); i++) {
    d1 = ShortestDistance(x, y, i);
    if (d1 < d0) {
      d0 = d1;
      j = i;
    }
  }

  return j;
}

void CFemmviewDoc::GetGapValues(CXYPlot& p, int PlotType, int NumPlotPoints, int j)
{
  double z, dz, tta, R;
  CComplex br, bt, ac, hr, ht;
  int i, k, n;

  if (Frequency == 0) {
    switch (PlotType) {
    case 0:
      p.Create(NumPlotPoints, 2);
      if (ProblemType == 0)
        strcpy(p.lbls[1], "Potential, Wb/m");
      else
        strcpy(p.lbls[1], "Flux, Wb");
      break;
    case 1:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|B|, Tesla");
      break;
    case 2:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "Br, Tesla");
      break;
    case 3:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "Bt, Tesla");
      break;
    case 4:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|H|, Amp/m");
      break;
    case 5:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "Hr, Amp/m");
      break;
    case 6:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "Ht, Amp/m");
      break;
    default:
      p.Create(NumPlotPoints, 2);
      break;
    }
  } else {
    switch (PlotType) {
    case 0:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|A|, Wb/m");
      strcpy(p.lbls[2], "Re[A], Wb/m");
      strcpy(p.lbls[3], "Im[A], Wb/m");
      break;
    case 1:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|B|, Tesla");
      break;
    case 2:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|Br|, Tesla");
      strcpy(p.lbls[2], "Re[Br], Tesla");
      strcpy(p.lbls[3], "Im[Br], Tesla");
      break;
    case 3:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|Bt|, Tesla");
      strcpy(p.lbls[2], "Re[Bt], Tesla");
      strcpy(p.lbls[3], "Im[Bt], Tesla");
      break;
    case 4:
      p.Create(NumPlotPoints, 2);
      strcpy(p.lbls[1], "|H|, Amp/m");
      break;
    case 5:
      p.Create(NumPlotPoints, 4);
      if (ProblemType == 0) {
        strcpy(p.lbls[1], "|Hr|, Amp/m");
        strcpy(p.lbls[2], "Re[Hr], Amp/m");
        strcpy(p.lbls[3], "Im[Hr], Amp/m");
      }
      break;
    case 6:
      p.Create(NumPlotPoints, 4);
      strcpy(p.lbls[1], "|Ht|, Amp/m");
      strcpy(p.lbls[2], "Re[Ht], Amp/m");
      strcpy(p.lbls[3], "Im[Ht], Amp/m");
      break;
    default:
      p.Create(NumPlotPoints, 2);
      break;
    }
  }

  strcpy(p.lbls[0], "Angle, Degrees");
  dz = 360. / ((double)(NumPlotPoints - 1));
  R = (agelist[j].ri + agelist[j].ro) / 2.;

  for (i = 0, z = 0; i < NumPlotPoints; i++, z += dz) {
    p.M[i][0] = z;

    tta = z * PI / 180;
    ac = 0;
    br = 0;
    bt = 0;
    for (k = 0; k < agelist[j].nn; k++) {
      n = agelist[j].nh[k];
      if (n > 0) {
        ac += R * (-agelist[j].brs[k] * cos(n * tta) + agelist[j].brc[k] * sin(n * tta)) / ((double)n);
      } else {
        ac += agelist[j].aco;
      }
      br += agelist[j].brc[k] * cos(n * tta) + agelist[j].brs[k] * sin(n * tta);
      bt += agelist[j].btc[k] * cos(n * tta) + agelist[j].bts[k] * sin(n * tta);
    }
    hr = br / muo;
    ht = bt / muo;

    if (Frequency == 0) {
      switch (PlotType) {
      case 0:
        p.M[i][1] = Re(ac);
        break;
      case 1:
        p.M[i][1] = Re(sqrt(br * conj(br) + bt * conj(bt)));
        break;
      case 2:
        p.M[i][1] = Re(br);
        break;
      case 3:
        p.M[i][1] = Re(bt);
        break;
      case 4:
        p.M[i][1] = Re(sqrt(hr * conj(hr) + ht * conj(ht)));
        break;
      case 5:
        p.M[i][1] = Re(hr);
        break;
      case 6:
        p.M[i][1] = Re(ht);
        break;
      default:
        p.M[i][1] = 0;
        break;
      }
    } else {
      switch (PlotType) {
      case 0:
        p.M[i][1] = abs(ac);
        p.M[i][2] = Re(ac);
        p.M[i][3] = Im(ac);
        break;
      case 1:
        p.M[i][1] = Re(sqrt(br * conj(br) + bt * conj(bt)));
        break;
      case 2:
        p.M[i][2] = Re(br);
        p.M[i][3] = Im(br);
        p.M[i][1] = abs(br);
        break;
      case 3:
        p.M[i][2] = Re(bt);
        p.M[i][3] = Im(bt);
        p.M[i][1] = abs(bt);
        break;
      case 4:
        p.M[i][1] = Re(sqrt(hr * conj(hr) + ht * conj(ht)));
        break;
      case 5:
        p.M[i][2] = Re(hr);
        p.M[i][3] = Im(hr);
        p.M[i][1] = abs(hr);
        break;
      case 6:
        p.M[i][2] = Re(ht);
        p.M[i][3] = Im(ht);
        p.M[i][1] = abs(ht);
        break;
      default:
        p.M[i][1] = 0;
        break;
      }
    }
  }
}