// hviewDoc.cpp : implementation of the ChviewDoc class
//

#include "stdafx.h"
#include <afx.h>
#include <afxtempl.h>
#include "hv_problem.h"
#include "femm.h"
#include "xyplot.h"
#include "hviewDoc.h"
#include "hviewView.h"
#include "MainFrm.h"

#include "lua.h"

#define AXISYMMETRIC 1

extern lua_State* lua;
extern void* phviewDoc;
extern BOOL bLinehook;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ChviewDoc

IMPLEMENT_DYNCREATE(ChviewDoc, CDocument)

BEGIN_MESSAGE_MAP(ChviewDoc, CDocument)
//{{AFX_MSG_MAP(ChviewDoc)
//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChviewDoc construction/destruction

double sqr(double x);

ChviewDoc::ChviewDoc()
{
  // set some default values for problem definition
  d_LineIntegralPoints = 400;
  Depth = 1 / 0.0254;
  LengthUnits = 0;
  ProblemType = FALSE;
  ProblemNote = "Add comments here.";
  FirstDraw = -1;
  A_High = 0.;
  A_Low = 0.;
  A_lb = 0.;
  A_ub = 0.;
  extRo = extRi = extZo = 0;
  Smooth = TRUE;
  NumList = NULL;
  ConList = NULL;
  bHasMask = FALSE;
  LengthConv = (double*)calloc(6, sizeof(double));
  LengthConv[0] = 0.0254; // inches
  LengthConv[1] = 0.001; // millimeters
  LengthConv[2] = 0.01; // centimeters
  LengthConv[3] = 1.; // meters
  LengthConv[4] = 2.54e-05; // mils
  LengthConv[5] = 1.e-06; // micrometers
  Coords = FALSE;

  for (int i = 0; i < 4; i++)
    d_PlotBounds[i][0] = d_PlotBounds[i][1] = PlotBounds[i][0] = PlotBounds[i][1] = 0;

  // determine path to bin directory
  BinDir = ((CFemmApp*)AfxGetApp())->GetExecutablePath();

  ScanPreferences();

  // lua initialization stuff
  initalise_lua();
}

ChviewDoc::~ChviewDoc()
{
  int i;
  free(LengthConv);
  for (i = 0; i < meshnode.GetSize(); i++)
    if (ConList[i] != NULL)
      free(ConList[i]);
  free(ConList);
  free(NumList);

  if (phviewDoc == this)
    phviewDoc = NULL;
}

BOOL ChviewDoc::OnNewDocument()
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

  // set problem attributes to generic ones;
  LengthUnits = 0;
  ProblemType = FALSE;
  ProblemNote = "Add comments here.";
  bHasMask = FALSE;
  extRo = extRi = extZo = 0;

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// ChviewDoc serialization

void ChviewDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring()) {
    // TODO: add storing code here
  } else {
    // TODO: add loading code here
  }
}

/////////////////////////////////////////////////////////////////////////////
// ChviewDoc diagnostics

#ifdef _DEBUG
void ChviewDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void ChviewDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ChviewDoc commands

char* StripKey(char* c);

BOOL ChviewDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;

  FILE* fp;
  int i, j, k, t;
  char s[1024], q[1024];
  char* v;
  double b;
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
      if (((int)vers) != 1)
        MsgBox("File is from a different version of Bela\nTrying to read anyhow...");
      q[0] = NULL;
    }

    // Depth in the into-the-page direction
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

    // Point Properties
    if (_strnicmp(q, "<beginpoint>", 11) == 0) {
      PProp.PointName = "New Point Property";
      PProp.V = PProp.qp = 0;
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

    if (_strnicmp(q, "<tp>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.V);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qp>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &PProp.qp);
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
      BProp.Tset = BProp.Tinf = BProp.qs = 0;
      BProp.beta = BProp.h = 0;
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

    if (_strnicmp(q, "<Tset>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.Tset);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qs>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.qs);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<beta>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.beta);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<h>", 3) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.h);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<Tinf>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.Tinf);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endbdry>", 9) == 0) {
      lineproplist.Add(BProp);
      q[0] = NULL;
    }

    // Block Properties;
    if (_strnicmp(q, "<beginblock>", 12) == 0) {
      MProp.BlockName = "New Material";
      MProp.Kx = 1.;
      MProp.Ky = 1.;
      MProp.Kt = 0;
      MProp.qv = 0.;
      MProp.npts = 0;
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

    if (_strnicmp(q, "<Kx>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Kx);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<Ky>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Ky);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<Kt>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.Kt);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qv>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.qv);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<TKPoints>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &MProp.npts);
      if (MProp.npts > 0) {
        for (j = 0; j < MProp.npts; j++) {
          fgets(s, 1024, fp);
          sscanf(s, "%lf	%lf", &MProp.Kn[j].re, &MProp.Kn[j].im);
        }
      }
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endblock>", 9) == 0) {
      blockproplist.Add(MProp);
      q[0] = NULL;
    }

    // Circuit Properties
    if (_strnicmp(q, "<beginconductor>", 16) == 0) {
      CProp.CircName = "New Circuit";
      CProp.V = CProp.q = 0;
      CProp.CircType = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<conductorname>", 15) == 0) {
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

    if (_strnicmp(q, "<vc>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &CProp.V);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qc>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &CProp.q);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<conductortype>", 15) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &CProp.CircType);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endconductor>", 14) == 0) {
      circproplist.Add(CProp);
      q[0] = NULL;
    }

    // Points list;
    if (_strnicmp(q, "[numpoints]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);
        sscanf(s, "%lf	%lf	%i	%i	%i\n", &node.x, &node.y, &t,
            &node.InGroup, &node.InConductor);
        node.BoundaryMarker = t - 1;
        node.InConductor--;
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
        sscanf(s, "%i	%i	%lf %i	%i	%i	%i\n",
            &segm.n0, &segm.n1, &segm.MaxSideLength, &t, &segm.Hidden,
            &segm.InGroup, &segm.InConductor);
        segm.BoundaryMarker = t - 1;
        segm.InConductor--;
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
        sscanf(s, "%i	%i	%lf	%lf %i	%i %i %i	%lf\n", &asegm.n0, &asegm.n1,
            &asegm.ArcLength, &asegm.MaxSideLength, &t, &asegm.Hidden,
            &asegm.InGroup, &asegm.InConductor, &b);
        asegm.BoundaryMarker = t - 1;
        if (b > 0)
          asegm.MaxSideLength = b;
        asegm.InConductor--;
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
        sscanf(s, "%lf	%lf	%i	%lf	%i	%i\n", &blk.x, &blk.y,
            &blk.BlockType, &blk.MaxArea, &blk.InGroup, &blk.IsExternal);
        blk.IsDefault = blk.IsExternal & 2;
        blk.IsExternal = blk.IsExternal & 1;
        if (blk.MaxArea < 0)
          blk.MaxArea = 0;
        else
          blk.MaxArea = PI * blk.MaxArea * blk.MaxArea / 4.;
        blk.BlockType -= 1;
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
    sscanf(s, "%lf	%lf	%lf	%i", &mnode.x, &mnode.y, &mnode.T, &mnode.Q);
    meshnode.SetAt(i, mnode);
  }

  // read in elements;
  fscanf(fp, "%i\n", &k);
  meshelem.SetSize(k);
  for (i = 0; i < k; i++) {
    fgets(s, 1024, fp);
    sscanf(s, "%i	%i	%i	%i", &elm.p[0], &elm.p[1], &elm.p[2], &elm.lbl);
    elm.blk = blocklist[elm.lbl].BlockType;
    meshelem.SetAt(i, elm);
  }

  // read in circuit data;
  fscanf(fp, "%i\n", &k);
  for (i = 0; i < k; i++) {
    fgets(s, 1024, fp);
    sscanf(s, "%lf	%lf", &circproplist[i].V, &circproplist[i].q);
  }

  fclose(fp);

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

  // Find flux density in each element;
  for (i = 0; i < meshelem.GetSize(); i++)
    GetElementD(i);

  // Find extreme values of A;
  A_Low = meshnode[0].T;
  A_High = meshnode[0].T;
  for (i = 1; i < meshnode.GetSize(); i++) {
    if (meshnode[i].T > A_High)
      A_High = meshnode[i].T;
    if (meshnode[i].T < A_Low)
      A_Low = meshnode[i].T;
  }
  // save default values for extremes of A
  A_lb = A_Low;
  A_ub = A_High;

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

  // sort each connection list so that the elements are
  // arranged in a counter-clockwise order
  CComplex u0, u1;
  int swa;
  BOOL flg;
  for (i = 0; i < meshnode.GetSize(); i++) {
    for (j = 0; j < NumList[i]; j++) {
      for (k = 0, flg = FALSE; k < NumList[i] - j - 1; k++) {
        u0 = meshelem[ConList[i][k]].ctr - meshnode[i].CC();
        u1 = meshelem[ConList[i][k + 1]].ctr - meshnode[i].CC();
        if (arg(u0) > arg(u1)) {
          swa = ConList[i][k];
          ConList[i][k] = ConList[i][k + 1];
          ConList[i][k + 1] = swa;
          flg = TRUE;
        }
      }
      if (!flg)
        j = NumList[i];
    }
  }

  // Find extreme values of potential
  d_PlotBounds[0][0] = A_Low;
  d_PlotBounds[0][1] = A_High;
  PlotBounds[0][0] = d_PlotBounds[0][0];
  PlotBounds[0][1] = d_PlotBounds[0][1];

  for (i = 0; i < meshelem.GetSize(); i++)
    GetNodalD(meshelem[i].d, i);

  // Find extreme values of D and E;
  i = 0;
  while (blocklist[meshelem[i].lbl].IsExternal)
    i++;
  d_PlotBounds[1][0] = abs(D(i));
  d_PlotBounds[1][1] = d_PlotBounds[1][0];
  d_PlotBounds[2][0] = abs(E(0));
  d_PlotBounds[2][1] = d_PlotBounds[2][0];
  for (i = 0; i < meshelem.GetSize(); i++)
    if (!blocklist[meshelem[i].lbl].IsExternal) {
      b = abs(D(i));
      if (b > d_PlotBounds[1][1])
        d_PlotBounds[1][1] = b;
      if (b < d_PlotBounds[1][0])
        d_PlotBounds[1][0] = b;

      b = abs(E(i));
      if (b > d_PlotBounds[2][1])
        d_PlotBounds[2][1] = b;
      if (b < d_PlotBounds[2][0])
        d_PlotBounds[2][0] = b;
    }
  PlotBounds[1][0] = d_PlotBounds[1][0];
  PlotBounds[1][1] = d_PlotBounds[1][1];
  PlotBounds[2][0] = d_PlotBounds[2][0];
  PlotBounds[2][1] = d_PlotBounds[2][1];

  // Choose bounds based on the type of contour plot
  // currently in play
  POSITION pos = GetFirstViewPosition();
  ChviewView* theView = (ChviewView*)GetNextView(pos);

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

  FirstDraw = TRUE;

  return TRUE;
}

int ChviewDoc::InTriangle(double x, double y)
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

BOOL ChviewDoc::GetPointValues(double x, double y, CPointVals& u)
{
  int k;
  k = InTriangle(x, y);
  if (k < 0)
    return FALSE;
  GetPointValues(x, y, k, u);
  return TRUE;
}

BOOL ChviewDoc::GetPointValues(double x, double y, int k, CPointVals& u)
{
  int i, n[3];
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

  GetPointD(x, y, u.F, meshelem[k]);

  u.T = 0;
  for (i = 0; i < 3; i++)
    u.T += meshnode[n[i]].T * (a[i] + b[i] * x + c[i] * y) / (da);

  u.K = blockproplist[meshelem[k].blk].GetK(u.T);
  u.K /= AECF(k, x + I * y);

  u.G.re = u.F.re / (u.K.re);
  u.G.im = u.F.im / (u.K.im);

  return TRUE;
}

void ChviewDoc::GetPointD(double x, double y, CComplex& D, CElement& elm)
{
  // elm is a reference to the element that contains the point of interest.
  int i, n[3];
  double da, a[3], b[3], c[3];

  if (Smooth == FALSE) {
    D = elm.D;
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

  for (i = 0, D = 0; i < 3; i++)
    D += (elm.d[i] * (a[i] + b[i] * x + c[i] * y) / da);
}

BOOL ChviewDoc::IsSameMaterial(int e1, int e2)
{
  int b1, b2;

  b1 = meshelem[e1].blk;
  b2 = meshelem[e2].blk;

  // Are the same material trivially if they are the same block type
  if (b1 == b2)
    return TRUE;

  // If the materials are linear and have the same Kx and Ky, we
  // can say that they are the same material;
  if ((blockproplist[b1].Kx == blockproplist[b2].Kx) && (blockproplist[b1].Ky == blockproplist[b2].Ky) && (blockproplist[b1].npts == 0) && (blockproplist[b2].npts == 0))
    return TRUE;

  // If the materials are nonlinear and have all of the same T-k points,
  // they are the same material;
  if (blockproplist[b1].npts > 0) {
    if (blockproplist[b1].npts == blockproplist[b2].npts) {
      for (int k = 0; k < blockproplist[b1].npts; k++) {
        if ((blockproplist[b1].Kn[k].re != blockproplist[b2].Kn[k].re) || (blockproplist[b1].Kn[k].im != blockproplist[b2].Kn[k].im))
          return FALSE;
      }
      return TRUE;
    }
  }

  return FALSE;
}

void ChviewDoc::GetNodalD(CComplex* d, int N)
{
  int i, j, k, n, m, p, eos, nos, qn;
  int lf, rt;
  double xi, yi, ii, xx, xy, yy, iv, xv, yv, dx, dy, dv, Ex, Ey, det;
  static int q[21];
  CComplex kn;
  BOOL flag;

  for (i = 0; i < 3; i++) {
    j = meshelem[N].p[i];
    lf = rt = -1;
    flag = FALSE;
    for (eos = 0; eos < NumList[j]; eos++)
      if (ConList[j][eos] == N)
        break;

    // scan ccw
    for (k = 0, m = eos, qn = 0; k < NumList[j]; k++) {
      n = ConList[j][m];
      if (!IsSameMaterial(n, N))
        break;

      // figure out which node is the next one in the ccw direction
      // Note that the ConList has been sorted in ccw order,
      // and the nodes in each element are sorted in ccw order,
      // making this task a bit easier.  The next node
      // ends up in the variable p
      for (nos = 0; nos < 3; nos++)
        if (meshelem[n].p[nos] == j)
          break;
      if (nos == 3)
        break;
      nos--;
      if (nos < 0)
        nos = 2;
      p = meshelem[n].p[nos];

      // add this node to the list.  We can have a max of 20 nodes,
      // which should never actually occur (usually about 6 to 8)
      if (qn < 20)
        q[qn++] = p;

      // if this is a fixed boundary, get out of the loop;
      if ((meshnode[j].Q != -2) && (meshnode[p].Q != -2)) {
        rt = p;
        break;
      }

      m++;
      if (m == NumList[j])
        m = 0;
    }

    // scan cw
    for (k = 0, m = eos; k < NumList[j]; k++) {
      n = ConList[j][m];
      if (!IsSameMaterial(n, N))
        break;

      // figure out which node is the next one in the cw direction
      // The next node ends up in the variable p
      for (nos = 0; nos < 3; nos++)
        if (meshelem[n].p[nos] == j)
          break;
      if (nos == 3)
        break;
      nos++;
      if (nos > 2)
        nos = 0;
      p = meshelem[n].p[nos];

      // add this node to the list.  We can have a max of 20 nodes,
      // which should never actually occur (usually about 6 to 8)
      if (qn < 20)
        q[qn++] = p;

      // if this node has a fixed definition, get out of the loop;
      if ((meshnode[j].Q != -2) && (meshnode[p].Q != -2)) {
        lf = p;
        break;
      }

      m--;
      if (m < 0)
        m = NumList[j] - 1;
    }

    // catch some annoying special cases;
    if ((lf == rt) && (rt != -1) && (meshnode[j].Q != -2)) {
      // The node of interest is at the end of a conductor; not much to
      // do but punt;
      d[i] = meshelem[N].D;
      flag = TRUE;
    } else if ((rt != -1) && (meshnode[j].Q != -2) && (lf == -1)) {
      // Another instance of a node at the
      // end of a conductor; punt!
      d[i] = meshelem[N].D;
      flag = TRUE;
    } else if ((lf != -1) && (meshnode[j].Q != -2) && (rt == -1)) {
      // Another instance of a node at the
      // end of a conductor; punt!
      d[i] = meshelem[N].D;
      flag = TRUE;
    } else if ((lf == -1) && (rt == -1) && (meshnode[j].Q != -2)) {
      // The node of interest is an isolated charge. Again, not much to
      // do but punt;
      d[i] = meshelem[N].D;
      flag = TRUE;
    } else if ((lf != -1) && (rt != -1) && (meshnode[j].Q != -2)) {

      // The node of interest is on some boundary where the charge is fixed.
      // if the angle is shallow enough, we can just do the regular thing;
      // Otherwise, we punt.
      CComplex x, y;
      x = meshnode[lf].CC() - meshnode[j].CC();
      x /= abs(x);
      y = meshnode[j].CC() - meshnode[rt].CC();
      y /= abs(y);
      if (fabs(arg(x / y)) > 10.0001 * PI / 180.) {
        // if the angle is greater than 10 degrees, punt;
        d[i] = meshelem[N].D;
        flag = TRUE;
      }
    }

    if (flag == FALSE) {
      // The nominal case.
      // Fit a plane through the nodes in the list to solve for E.
      // Then, multiply by permittivity to get D.
      xi = yi = ii = xx = xy = yy = iv = xv = yv = 0;

      q[qn++] = j;

      for (k = 0; k < qn; k++) {
        dx = meshnode[q[k]].x - meshnode[j].x;
        dy = meshnode[q[k]].y - meshnode[j].y;
        dv = meshnode[j].T - meshnode[q[k]].T;

        ii += 1.;
        xi += dx;
        yi += dy;
        xx += dx * dx;
        xy += dx * dy;
        yy += dy * dy;
        iv += dv;
        xv += dx * dv;
        yv += dy * dv;
      }
      det = (-(ii * xy * xy) + 2 * xi * xy * yi - xx * yi * yi - xi * xi * yy + ii * xx * yy) * LengthConv[LengthUnits];

      if (det == 0)
        d[i] = meshelem[N].D;
      else {
        Ex = (iv * xy * yi - xv * yi * yi - ii * xy * yv + xi * yi * yv - iv * xi * yy + ii * xv * yy) / det;
        Ey = (iv * xi * xy - ii * xv * xy + xi * xv * yi - iv * xx * yi - xi * xi * yv + ii * xx * yv) / det;
        kn = blockproplist[meshelem[N].blk].GetK(meshnode[j].T);
        d[i] = Re(kn) * Ex + I * Im(kn) * Ey;
        d[i] /= AECF(N, meshnode[j].CC());
      }
    }
  }
}

void ChviewDoc::GetElementD(int k)
{
  int i, n[3];
  double b[3], c[3], da;
  CComplex E, kn;

  for (i = 0; i < 3; i++)
    n[i] = meshelem[k].p[i];

  b[0] = meshnode[n[1]].y - meshnode[n[2]].y;
  b[1] = meshnode[n[2]].y - meshnode[n[0]].y;
  b[2] = meshnode[n[0]].y - meshnode[n[1]].y;
  c[0] = meshnode[n[2]].x - meshnode[n[1]].x;
  c[1] = meshnode[n[0]].x - meshnode[n[2]].x;
  c[2] = meshnode[n[1]].x - meshnode[n[0]].x;
  da = (b[0] * c[1] - b[1] * c[0]);

  for (i = 0, E = 0, kn = 0; i < 3; i++) {
    E -= meshnode[n[i]].T * (b[i] + I * c[i]) / (da * LengthConv[LengthUnits]);
    kn += blockproplist[meshelem[k].blk].GetK(meshnode[meshelem[k].p[i]].T) / 3.;
  }
  meshelem[k].D = (E.re * kn.re + I * E.im * kn.im) / AECF(k);

  return;
}

void ChviewDoc::OnReload()
{
  // TODO: Add your command handler code here
  CString pname = GetPathName();
  if (pname.GetLength() > 0) {
    OnNewDocument();
    SetPathName(pname, FALSE);
    OnOpenDocument(pname);
  }
}

int ChviewDoc::ClosestNode(double x, double y)
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

void ChviewDoc::GetLineValues(CXYPlot& p, int PlotType, int NumPlotPoints)
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

  switch (PlotType) {
  case 0:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "Temperature, K");
    break;
  case 1:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "|F|, W/m^2");
    break;
  case 2:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "F.n, W/m^2");
    break;
  case 3:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "F.t, W/m^2");
    break;
  case 4:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "|G|, K/m");
    break;
  case 5:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "G.n, K/m");
    break;
  case 6:
    p.Create(NumPlotPoints, 2);
    strcpy(p.lbls[1], "G.t, K/m");
    break;
  default:
    p.Create(NumPlotPoints, 2);
    break;
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
    if (flag != FALSE) {
      switch (PlotType) {
      case 0:
        p.M[i][1] = v.T;
        break;
      case 1:
        p.M[i][1] = abs(v.F);
        break;
      case 2:
        p.M[i][1] = Re(v.F / n);
        break;
      case 3:
        p.M[i][1] = Re(v.F / t);
        break;
      case 4:
        p.M[i][1] = abs(v.G);
        break;
      case 5:
        p.M[i][1] = Re(v.G / n);
        break;
      case 6:
        p.M[i][1] = Re(v.G / t);
        break;
      default:
        p.M[i][1] = 0;
        break;
      }
    }
  }

  free(q);
}

BOOL ChviewDoc::InTriangleTest(double x, double y, int i)
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
  T = 0; // vector potential
  F = 0; // flux density
  K = 0; // permittivity
  G = 0; // field intensity
}

CComplex ChviewDoc::Ctr(int i)
{
  CComplex p, c;
  int j;

  for (j = 0, c = 0; j < 3; j++) {
    p.Set(meshnode[meshelem[i].p[j]].x / 3., meshnode[meshelem[i].p[j]].y / 3.);
    c += p;
  }

  return c;
}

double ChviewDoc::ElmArea(int i)
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

double ChviewDoc::ElmArea(CElement* elm)
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

CComplex ChviewDoc::HenrotteVector(int k)
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

CComplex ChviewDoc::BlockIntegral(int inttype)
{
  int i, k;
  CComplex c, z;
  double T;
  double a, R;
  double r[3];

  for (i = 0, z = 0; i < meshelem.GetSize(); i++) {
    if (blocklist[meshelem[i].lbl].IsSelected == TRUE) {
      // compute some useful quantities employed by most integrals...
      a = ElmArea(i) * pow(LengthConv[LengthUnits], 2.);
      if (ProblemType == 1) {
        for (k = 0; k < 3; k++)
          r[k] = meshnode[meshelem[i].p[k]].x * LengthConv[LengthUnits];
        R = (r[0] + r[1] + r[2]) / 3.;
      }

      // now, compute the desired integral;
      switch (inttype) {
      case 0: // T
        if (ProblemType == 1)
          a *= (2. * PI * R);
        else
          a *= Depth;
        for (T = 0, k = 0; k < 3; k++)
          T += meshnode[meshelem[i].p[k]].T / 3.;
        z += a * T;
        break;

      case 1: // cross-section area
        z += a;
        break;

      case 2: // volume
        if (ProblemType == 1)
          a *= (2. * PI * R);
        else
          a *= Depth;
        z += a;
        break;

      case 3: // F
        if (ProblemType == 1)
          a *= (2. * PI * R);
        else
          a *= Depth;
        z += a * D(i);
        break;

      case 4: // G
        if (ProblemType == 1)
          a *= (2. * PI * R);
        else
          a *= Depth;
        z += a * E(i);
        break;

      default:
        break;
      }
    }
  }

  // Integrals 0, 3 and 4 are averages over the selected volume;
  // Need to divide by the block volme to get the average.
  if ((inttype == 0) || (inttype == 3) || (inttype == 4))
    z /= BlockIntegral(2);

  return z;
}

void ChviewDoc::LineIntegral(int inttype, double* z)
{
  // inttype	Integral
  //		0	G.t
  //		1	F.n
  //		2	Contour length
  //		3   Average Temp.

  // inttype==0 => G.t
  if (inttype == 0) {
    CPointVals u;
    int k;

    k = (int)contour.GetSize();
    GetPointValues(contour[0].re, contour[0].im, u);
    z[0] = u.T;
    GetPointValues(contour[k - 1].re, contour[k - 1].im, u);
    z[0] -= u.T;
  }

  // inttype==1 => F.n
  if (inttype == 1) {
    CComplex n, t, pt;
    CPointVals v;
    double dz, u, d, Fn;
    int i, j, k, m, elm;
    int NumPlotPoints = d_LineIntegralPoints;
    BOOL flag;

    z[0] = 0;
    z[1] = 0;
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
          Fn = Re(v.F / n);

          if (ProblemType == AXISYMMETRIC)
            d = 2. * PI * pt.re * sq(LengthConv[LengthUnits]);
          else
            d = Depth * LengthConv[LengthUnits];

          z[0] += (Fn * dz * d);
          z[1] += dz * d;
        }
      }
    }
    z[1] = z[0] / z[1]; // Average F.n over the surface;
  }

  // inttype==2 => Contour Length
  if (inttype == 2) {
    int i, k;
    k = (int)contour.GetSize();
    for (i = 0, z[0] = 0; i < k - 1; i++)
      z[0] += abs(contour[i + 1] - contour[i]);
    z[0] *= LengthConv[LengthUnits];

    if (ProblemType == 1) {
      for (i = 0, z[1] = 0; i < k - 1; i++)
        z[1] += (PI * (contour[i].re + contour[i + 1].re) * abs(contour[i + 1] - contour[i]));
      z[1] *= pow(LengthConv[LengthUnits], 2.);
    } else {
      z[1] = z[0] * Depth;
    }
  }

  // inttype==3 => Average Temperature
  if (inttype == 3) {
    CComplex pt;
    CPointVals v;
    double dz, u, d;
    int i, j, k, m, elm;
    int NumPlotPoints = d_LineIntegralPoints;
    BOOL flag;

    z[0] = 0;
    z[1] = 0;
    for (k = 1; k < contour.GetSize(); k++) {
      dz = abs(contour[k] - contour[k - 1]) / ((double)NumPlotPoints);
      for (i = 0, elm = -1; i < NumPlotPoints; i++) {
        u = (((double)i) + 0.5) / ((double)NumPlotPoints);
        pt = contour[k - 1] + u * (contour[k] - contour[k - 1]);

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
          if (ProblemType == AXISYMMETRIC)
            d = 2. * PI * pt.re * sq(LengthConv[LengthUnits]);
          else
            d = Depth * LengthConv[LengthUnits];

          z[0] += (v.T * dz * d);
          z[1] += dz * d;
        }
      }
    }
    z[0] = z[0] / z[1]; // Average F.n over the surface;
  }

  return;
}

int ChviewDoc::ClosestArcSegment(double x, double y)
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

void ChviewDoc::GetCircle(CArcSegment& arc, CComplex& c, double& R)
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

double ChviewDoc::ShortestDistanceFromArc(CComplex p, CArcSegment& arc)
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

double ChviewDoc::ShortestDistanceFromSegment(double p, double q, int segm)
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

BOOL ChviewDoc::ScanPreferences()
{
  FILE* fp;
  CString fname;

  fname = BinDir + "hview.cfg";

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
    }
    fclose(fp);
  } else
    return FALSE;

  return TRUE;
}

void ChviewDoc::BendContour(double angle, double anglestep)
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

CComplex ChviewDoc::E(int k)
{
  // return average electric field intensity for the kth element
  CComplex kn;
  int i;

  for (i = 0, kn = 0; i < 3; i++)
    kn += blockproplist[meshelem[k].blk].GetK(meshnode[meshelem[k].p[i]].T) / 3.;

  return (meshelem[k].D.re / Re(kn) + I * meshelem[k].D.im / Im(kn)) * AECF(k);

  // AECF(k) part corrects permittivity for axisymmetric external region;
}

CComplex ChviewDoc::D(int k)
{
  // return average electric flux density for the kth element
  return meshelem[k].D;
}

CComplex ChviewDoc::e(int k, int i)
{
  // return nodal temperature gradient for the ith node of the kth element
  double aecf = 1;
  CComplex kn;

  if ((ProblemType) && (blocklist[meshelem[k].lbl].IsExternal)) {
    // correct for axisymmetric external region
    double x = meshnode[meshelem[k].p[i]].x;
    double y = meshnode[meshelem[k].p[i]].y - extZo;
    aecf = (x * x + y * y) / (extRi * extRo);
  }

  kn = blockproplist[meshelem[k].blk].GetK(meshnode[meshelem[k].p[i]].T);

  return (meshelem[k].d[i].re / Re(kn) + I * meshelem[k].d[i].im / Im(kn)) * aecf;
}

CComplex ChviewDoc::d(int k, int i)
{
  // return nodal flux density for the ith node of the kth element;
  return meshelem[k].d[i];
}

double ChviewDoc::ShortestDistance(double p, double q, int segm)
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

int ChviewDoc::ClosestSegment(double x, double y)
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

BOOL ChviewDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // TODO: Add your specialized code here and/or call the base class
  if (bLinehook != FALSE)
    return TRUE;
  return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

double ChviewDoc::AECF(int k, CComplex p)
{
  // Correction factor for a point within the element, rather than
  // for the center of the element.
  if (!ProblemType)
    return 1.; // no correction for planar problems
  if (!blocklist[meshelem[k].lbl].IsExternal)
    return 1; // only need to correct for external regions
  double r = abs(p - I * extZo);
  if (r == 0)
    return AECF(k);
  return (r * r) / (extRo * extRi); // permeability gets divided by this factor;
}

double ChviewDoc::AECF(int k)
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
  return (r * r) / (extRo * extRi); // permeability gets divided by this factor;
}

void ChviewDoc::FindBoundaryEdges()
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
