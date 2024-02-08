// FemmeDocCore.cpp : implementation of the CFemmeDocCore class
//

#include <stdafx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "belasolv.h"
#include "belasolvDlg.h"
#include "mesh.h"
#include "spars.h"
#include "FemmeDocCore.h"

/////////////////////////////////////////////////////////////////////////////
// CFemmeDocCore construction/destruction

CFemmeDocCore::CFemmeDocCore()
{
  TheView = NULL;
  Precision = NULL;
  LengthUnits = NULL;
  ProblemType = NULL;
  Coords = NULL;
  BandWidth = NULL;
  NumNodes = NULL;
  NumEls = NULL;
  NumBlockProps = NULL;
  NumPBCs = NULL;
  NumLineProps = NULL;
  NumPointProps = NULL;
  NumCircProps = NULL;
  NumBlockLabels = NULL;

  meshnode = NULL;
  meshele = NULL;
  blockproplist = NULL;
  lineproplist = NULL;
  nodeproplist = NULL;
  circproplist = NULL;
  labellist = NULL;
  pbclist = NULL;
  PathName = NULL;

  extRo = extRi = extZo = NULL;
}

CFemmeDocCore::~CFemmeDocCore()
{
  // This space for rent.
}

void CFemmeDocCore::CleanUp()
{
  if (meshnode != NULL)
    free(meshnode);
  if (meshele != NULL)
    free(meshele);
  if (blockproplist != NULL)
    free(blockproplist);
  if (lineproplist != NULL)
    free(lineproplist);
  if (nodeproplist != NULL)
    free(nodeproplist);
  if (circproplist != NULL)
    free(circproplist);
  if (labellist != NULL)
    free(labellist);
  if (pbclist != NULL)
    free(pbclist);
}

/////////////////////////////////////////////////////////////////////////////
// CFemmeDocCore commands

char* StripKey(char* c)
{
  char* d;
  int i, k;

  k = (int)strlen(c);

  for (i = 0; i < k; i++) {
    if (c[i] == '=') {
      d = c + i + 1;
      return d;
    }
  }

  return c + k;
}

BOOL CFemmeDocCore::OnOpenDocument()
{
  FILE* fp;
  int k;
  char s[1024], q[1024];
  char* v;
  CPointProp PProp;
  CBoundaryProp BProp;
  CMaterialProp MProp;
  CCircuit CProp;
  CBlockLabel blk;

  sprintf(s, "%s.fee", PathName);
  if ((fp = fopen(s, "rt")) == NULL) {
    fprintf(stderr, "Couldn't read from specified .fee file\n");
    return FALSE;
  }

  // define some defaults
  Precision = 1.e-08;
  Depth = -1;
  ProblemType = 0;
  Coords = 0;
  NumPointProps = 0;
  NumLineProps = 0;
  NumBlockProps = 0;
  NumCircProps = 0;

  // parse the file

  while (fgets(s, 1024, fp) != NULL) {
    sscanf(s, "%s", q);

    // Precision
    if (_strnicmp(q, "[precision]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &Precision);
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

    // Depth for 2D planar problems;
    if (_strnicmp(q, "[depth]", 7) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &Depth);
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
    if (_strnicmp(q, "[pointprops]", 12) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0)
        nodeproplist = (CPointProp*)calloc(k, sizeof(CPointProp));
      q[0] = NULL;
    }

    if (_strnicmp(q, "<beginpoint>", 11) == 0) {
      PProp.V = 0;
      PProp.qp = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<vp>", 4) == 0) {
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
      nodeproplist[NumPointProps] = PProp;
      NumPointProps++;
      q[0] = NULL;
    }

    // Boundary Properties;
    if (_strnicmp(q, "[bdryprops]", 11) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0)
        lineproplist = (CBoundaryProp*)calloc(k, sizeof(CBoundaryProp));
      q[0] = NULL;
    }

    if (_strnicmp(q, "<beginbdry>", 11) == 0) {
      BProp.BdryFormat = 0;
      BProp.V = 0;
      BProp.qs = 0;
      BProp.c0 = 0;
      BProp.c1 = 0;
      q[0] = NULL;
    }

    if (_strnicmp(q, "<bdrytype>", 10) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &BProp.BdryFormat);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<vs>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.V);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qs>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.qs);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c0>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c0);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<c1>", 4) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &BProp.c1);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endbdry>", 9) == 0) {
      lineproplist[NumLineProps] = BProp;
      NumLineProps++;
      q[0] = NULL;
    }

    // Block Properties;
    if (_strnicmp(q, "[blockprops]", 12) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0)
        blockproplist = (CMaterialProp*)calloc(k, sizeof(CMaterialProp));
      q[0] = NULL;
    }

    if (_strnicmp(q, "<beginblock>", 12) == 0) {
      MProp.ex = 1;
      MProp.ey = 1; // permittivity, relative
      MProp.qv = 1; // charge density, C/m^3
      q[0] = NULL;
    }

    if (_strnicmp(q, "<ex>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.ex);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<ey>", 6) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.ey);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<qv>", 5) == 0) {
      v = StripKey(s);
      sscanf(v, "%lf", &MProp.qv);
      q[0] = NULL;
    }

    if (_strnicmp(q, "<endblock>", 9) == 0) {
      blockproplist[NumBlockProps] = MProp;
      NumBlockProps++;
      q[0] = NULL;
    }

    // Conductor Properties
    if (_strnicmp(q, "[conductorprops]", 16) == 0) {
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0)
        circproplist = (CCircuit*)calloc(k, sizeof(CCircuit));
      q[0] = NULL;
    }

    if (_strnicmp(q, "<beginconductor>", 16) == 0) {
      CProp.V = 0;
      CProp.q = 0;
      CProp.CircType = 0;
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
      circproplist[NumCircProps] = CProp;
      NumCircProps++;
      q[0] = NULL;
    }

    // read in regional attributes
    if (_strnicmp(q, "[numblocklabels]", 13) == 0) {
      int i;
      v = StripKey(s);
      sscanf(v, "%i", &k);
      if (k > 0)
        labellist = (CBlockLabel*)calloc(k, sizeof(CBlockLabel));
      NumBlockLabels = k;
      for (i = 0; i < k; i++) {
        fgets(s, 1024, fp);
        sscanf(s, "%lf	%lf	%i	%lf	%i	%i", &blk.x, &blk.y, &blk.BlockType,
            &blk.MaxArea, &blk.InGroup, &blk.IsExternal);
        blk.IsDefault = blk.IsExternal & 2;
        blk.IsExternal = blk.IsExternal & 1;
        blk.BlockType--;
        labellist[i] = blk;
      }
      q[0] = NULL;
    }
  }

  fclose(fp);

  return TRUE;
}

BOOL CFemmeDocCore::LoadMesh()
{
  int i, j, k, q, n0, n1, n;
  char infile[256];
  FILE* fp;
  char s[1024];
  double c[] = { 25.4, 1., 10., 1000., 0.0254, 0.001 };

  // read meshnodes;
  sprintf(infile, "%s.node", PathName);
  if ((fp = fopen(infile, "rt")) == NULL) {
    return FALSE;
  }
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);
  NumNodes = k;

  meshnode = (CNode*)calloc(k, sizeof(CNode));
  CNode node;
  for (i = 0; i < k; i++) {
    fscanf(fp, "%i", &j);
    fscanf(fp, "%lf", &node.x);
    fscanf(fp, "%lf", &node.y);
    fscanf(fp, "%i", &n);

    if (n > 1) {
      // strip off point BC number;
      j = n & 0xffff;
      j = j - 2;
      if (j < 0)
        j = -1;

      // strip off Conductor number
      n = (n - (n & 0xffff)) / 0x10000 - 1;
    } else {
      j = -1;
      n = -1;
    }
    node.bc = j;
    node.InConductor = n;

    // convert all lengths to internal working units of mm
    node.x *= c[LengthUnits];
    node.y *= c[LengthUnits];

    meshnode[i] = node;
  }
  fclose(fp);

  // read in periodic boundary conditions;
  sprintf(infile, "%s.pbc", PathName);
  if ((fp = fopen(infile, "rt")) == NULL) {
    return FALSE;
  }
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);
  NumPBCs = k;

  if (k != 0)
    pbclist = (CCommonPoint*)calloc(k, sizeof(CCommonPoint));
  CCommonPoint pbc;
  for (i = 0; i < k; i++) {
    fscanf(fp, "%i", &j);
    fscanf(fp, "%i", &pbc.x);
    fscanf(fp, "%i", &pbc.y);
    fscanf(fp, "%i", &pbc.t);
    pbclist[i] = pbc;
  }
  fclose(fp);

  // read in elements;
  sprintf(infile, "%s.ele", PathName);
  if ((fp = fopen(infile, "rt")) == NULL) {
    return FALSE;
  }
  fgets(s, 1024, fp);
  sscanf(s, "%i", &k);
  NumEls = k;

  meshele = (CElement*)calloc(k, sizeof(CElement));
  CElement elm;

  int defaultLabel;
  for (i = 0, defaultLabel = -1; i < NumBlockLabels; i++)
    if (labellist[i].IsDefault)
      defaultLabel = i;

  for (i = 0; i < k; i++) {
    fscanf(fp, "%i", &j);
    fscanf(fp, "%i", &elm.p[0]);
    fscanf(fp, "%i", &elm.p[1]);
    fscanf(fp, "%i", &elm.p[2]);
    fscanf(fp, "%i", &elm.lbl);
    elm.lbl--;
    if (elm.lbl < 0)
      elm.lbl = defaultLabel;
    if (elm.lbl < 0) {
      CString msg;
      msg = "Material properties have not been defined for\n";
      msg += "all regions. Press the \"Run Mesh Generator\"\n";
      msg += "button to highlight the problem regions.";
      MsgBox(msg);
      fclose(fp);
      sprintf(infile, "%s.ele", PathName);
      DeleteFile(infile);
      sprintf(infile, "%s.node", PathName);
      DeleteFile(infile);
      sprintf(infile, "%s.pbc", PathName);
      DeleteFile(infile);
      sprintf(infile, "%s.poly", PathName);
      DeleteFile(infile);
      sprintf(infile, "%s.edge", PathName);
      DeleteFile(infile);
      exit(1);
    }
    // look up block type out of the list of block labels
    elm.blk = labellist[elm.lbl].BlockType;

    meshele[i] = elm;
  }
  fclose(fp);

  // initialize edge bc's and element permeabilities;
  for (i = 0; i < NumEls; i++)
    for (j = 0; j < 3; j++)
      meshele[i].e[j] = -1;

  // read in edges to which boundary conditions are applied;

  // first, do a little bookkeeping so that element
  // associated with a give edge can be identified fast
  int* nmbr;
  int** mbr;

  nmbr = (int*)calloc(NumNodes, sizeof(int));

  // Make a list of how many elements that tells how
  // many elements to which each node belongs.
  for (i = 0; i < NumEls; i++)
    for (j = 0; j < 3; j++)
      nmbr[meshele[i].p[j]]++;

  // mete out some memory to build a list of the
  // connectivity...
  mbr = (int**)calloc(NumNodes, sizeof(int*));
  for (i = 0; i < NumNodes; i++) {
    mbr[i] = (int*)calloc(nmbr[i], sizeof(int));
    nmbr[i] = 0;
  }

  // fill up the connectivity information;
  for (i = 0; i < NumEls; i++)
    for (j = 0; j < 3; j++) {
      k = meshele[i].p[j];
      mbr[k][nmbr[k]] = i;
      nmbr[k]++;
    }

  sprintf(infile, "%s.edge", PathName);
  if ((fp = fopen(infile, "rt")) == NULL) {
    return FALSE;
  }
  fscanf(fp, "%i", &k); // read in number of lines

  fscanf(fp, "%i", &j); // read in boundarymarker flag;
  for (i = 0; i < k; i++) {
    fscanf(fp, "%i", &j);
    fscanf(fp, "%i", &n0);
    fscanf(fp, "%i", &n1);
    fscanf(fp, "%i", &n);

    // BC number;
    if (n < 0) {
      n = (-n);
      j = (n & 0xffff) - 2;
      if (j < 0)
        j = -1;

      // Conductor number;
      n = (n - (n & 0xffff)) / 0x10000 - 1;
      if (n >= 0) {
        meshnode[n0].InConductor = n;
        meshnode[n1].InConductor = n;
      }
    } else
      j = -1;

    if (j >= 0) {
      // search through elements to find one containing the line;
      // set corresponding edge equal to the bc number
      for (q = 0, n = FALSE; q < nmbr[n0]; q++) {
        elm = meshele[mbr[n0][q]];

        if ((elm.p[0] == n0) && (elm.p[1] == n1)) {
          elm.e[0] = j;
          n = TRUE;
        }
        if ((elm.p[0] == n1) && (elm.p[1] == n0)) {
          elm.e[0] = j;
          n = TRUE;
        }

        if ((elm.p[1] == n0) && (elm.p[2] == n1)) {
          elm.e[1] = j;
          n = TRUE;
        }
        if ((elm.p[1] == n1) && (elm.p[2] == n0)) {
          elm.e[1] = j;
          n = TRUE;
        }

        if ((elm.p[2] == n0) && (elm.p[0] == n1)) {
          elm.e[2] = j;
          n = TRUE;
        }
        if ((elm.p[2] == n1) && (elm.p[0] == n0)) {
          elm.e[2] = j;
          n = TRUE;
        }

        meshele[mbr[n0][q]] = elm;

        // this is a little hack: line charge distributions should be applied
        // to at most one element;
        if ((lineproplist[j].BdryFormat == 2) && (n))
          q = nmbr[n0];
      }
    }
  }
  fclose(fp);

  // free up the connectivity information
  free(nmbr);
  for (i = 0; i < NumNodes; i++)
    free(mbr[i]);
  free(mbr);

  // clear out temporary files
  sprintf(infile, "%s.ele", PathName);
  DeleteFile(infile);
  sprintf(infile, "%s.node", PathName);
  DeleteFile(infile);
  sprintf(infile, "%s.pbc", PathName);
  DeleteFile(infile);
  sprintf(infile, "%s.poly", PathName);
  DeleteFile(infile);

  return TRUE;
}
