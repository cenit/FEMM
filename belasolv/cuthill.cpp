// does Cuthill-McKee algorithm as described in Hoole;

#include <stdafx.h>
#include <afxtempl.h>
#include <stdio.h>
#include <math.h>
#include "belasolv.h"
#include "belasolvDlg.h"
#include "mesh.h"
#include "spars.h"
#include "FemmeDocCore.h"

#define muo 1.2566370614359173e-6

BOOL CFemmeDocCore::SortElements()
{
  // Comb Sort -- see http://en.wikipedia.org/wiki/Comb_sort
  int* Score;
  int i, j, k, gap;
  CElement e;

  Score = (int*)calloc(NumEls, sizeof(int));

  for (k = 0; k < NumEls; k++) {
    Score[k] = meshele[k].p[0] + meshele[k].p[1] + meshele[k].p[2];
  }

  gap = NumEls;

  do {
    // update the gap value for a next comb
    if (gap > 1) {
      gap = (gap * 10) / 13;
      if ((gap == 10) || (gap == 9))
        gap = 11;
    }

    // a single "comb" over the input list
    for (j = 0, i = 0; (j + gap) < NumEls; j++) {
      if (Score[j] > Score[j + gap]) {
        k = j + gap;
        i = Score[k];
        Score[k] = Score[j];
        Score[j] = i;
        e = meshele[k];
        meshele[k] = meshele[j];
        meshele[j] = e;
        i = 1;
      }
    }
  } while ((gap > 1) && (i > 0));

  free(Score);
  return TRUE;
}

BOOL CFemmeDocCore::Cuthill()
{

  FILE* fp;
  int i, j, k, n0, n1, n;
  int newwide, *newnum, **ocon;
  int *numcon, *nxtnum;
  CNode swap;
  char infile[256];

  // allocate storage
  nxtnum = (int*)calloc(NumNodes, sizeof(int));
  newnum = (int*)calloc(NumNodes, sizeof(int));
  numcon = (int*)calloc(NumNodes, sizeof(int));
  ocon = (int**)calloc(NumNodes, sizeof(int*));
  // initialize node array;
  for (i = 0; i < NumNodes; i++)
    newnum[i] = -1;

  // read in connectivity from nodefile
  sprintf(infile, "%s.edge", PathName);
  if ((fp = fopen(infile, "rt")) == NULL) {
    fprintf(stderr, "Couldn't open %s", infile);
    return FALSE;
  }
  fscanf(fp, "%i", &k); // read in number of lines
  fscanf(fp, "%i", &j); // read in boundarymarker flag;

  // allocate space for connections;
  ocon[0] = (int*)calloc(2 * k, sizeof(int));

  // with first pass, figure out how many connections
  // there are for each node;
  for (i = 0; i < k; i++) {
    fscanf(fp, "%i", &j);
    fscanf(fp, "%i", &n0);
    fscanf(fp, "%i", &n1);
    fscanf(fp, "%i", &j);

    numcon[n0]++;
    numcon[n1]++;
  }

  // mete out connection storage space;
  for (i = 1, n = 0; i < NumNodes; i++) {
    n += numcon[i - 1];
    ocon[i] = ocon[0] + n;
  }

  // on second pass through file, store connections;
  rewind(fp);
  fscanf(fp, "%li", &k); // read in number of lines
  fscanf(fp, "%li", &j); // read in boundarymarker flag;
  for (i = 0; i < k; i++) {
    fscanf(fp, "%i", &j);
    fscanf(fp, "%i", &n0);
    fscanf(fp, "%i", &n1);
    fscanf(fp, "%i", &j);

    ocon[n0][nxtnum[n0]] = n1;
    nxtnum[n0]++;
    ocon[n1][nxtnum[n1]] = n0;
    nxtnum[n1]++;
  }
  fclose(fp);
  DeleteFile(infile);

  // sort connections in order of increasing connectivity;
  // I'm lazy, so I'm doing a bubble sort;
  for (n0 = 0; n0 < NumNodes; n0++) {
    for (i = 1; i < numcon[n0]; i++)
      for (j = 1; j < numcon[n0]; j++)
        if (numcon[ocon[n0][j]] < numcon[ocon[n0][j - 1]]) {
          n1 = ocon[n0][j];
          ocon[n0][j] = ocon[n0][j - 1];
          ocon[n0][j - 1] = n1;
        }
  }

  // search for a node to start with;
  j = numcon[0];
  n0 = 0;
  for (i = 1; i < NumNodes; i++) {
    if (numcon[i] < j) {
      j = numcon[i];
      n0 = i;
    }
    if (j == 2)
      i = k; // break out if j==2,
    // because this is the best we can do
  }

  // do renumbering algorithm;
  for (i = 0; i < NumNodes; i++)
    nxtnum[i] = -1;
  newnum[n0] = 0;
  n = 1;
  nxtnum[0] = n0;

  do {
    // renumber in order of increasing number of connections;

    for (i = 0; i < numcon[n0]; i++) {
      if (newnum[ocon[n0][i]] < 0) {
        newnum[ocon[n0][i]] = n;
        nxtnum[n] = ocon[n0][i];
        n++;
      }
    }

    // need to catch case in which problem is multiply
    // connected and still renumber right.
    if (nxtnum[newnum[n0] + 1] < 0) {
      //	MsgBox("Multiply Connected!");
      //	exit(0);

      // first, get a node that hasn't been visited yet;
      for (i = 0; i < NumNodes; i++)
        if (newnum[i] < 0) {
          j = numcon[i];
          n0 = i;
          i = NumNodes;
        }

      // now, get a new starting node;
      for (i = 0; i < NumNodes; i++) {
        if ((newnum[i] < 0) && (numcon[i] < j)) {
          j = numcon[i];
          n0 = i;
        }
        if (j == 2)
          i = NumNodes; // break out if j==2,
                        // because this is the
                        // best we can do
      }

      // now, set things to restart;
      newnum[n0] = n;
      nxtnum[n] = n0;
      n++;
    } else
      n0 = nxtnum[newnum[n0] + 1];

  } while (n < NumNodes);

  // remap connectivities;
  for (i = 0; i < NumNodes; i++)
    for (j = 0; j < numcon[i]; j++)
      ocon[i][j] = newnum[ocon[i][j]];

  // remap (anti)periodic boundary points
  for (i = 0; i < NumPBCs; i++) {
    pbclist[i].x = newnum[pbclist[i].x];
    pbclist[i].y = newnum[pbclist[i].y];
  }

  // find new bandwidth;

  // PBCs fuck up the banding, som could have to do
  // something like:
  // if(NumPBCs!=0) BandWidth=0;
  // else{
  // but if we apply the PCBs the last thing before the
  // solver is called, we can take advantage of banding
  // speed optimizations without messing things up.
  for (n0 = 0, newwide = 0; n0 < NumNodes; n0++) {
    for (i = 0; i < numcon[n0]; i++)
      if (abs(newnum[n0] - ocon[n0][i]) > newwide) {
        newwide = abs(newnum[n0] - ocon[n0][i]);
      }
  }

  BandWidth = newwide + 1;
  // }

  // free up the variables that we needed during the routine....
  free(numcon);
  free(nxtnum);
  free(ocon[0]);
  free(ocon);

  // new mapping remains in newnum;
  // apply this mapping to elements first.
  for (i = 0; i < NumEls; i++)
    for (j = 0; j < 3; j++)
      meshele[i].p[j] = newnum[meshele[i].p[j]];

  // now, sort nodes based on newnum;
  for (i = 0; i < NumNodes; i++)
    while (newnum[i] != i) {
      j = newnum[i];
      n = newnum[j];
      newnum[j] = newnum[i];
      newnum[i] = n;
      swap = meshnode[j];
      meshnode[j] = meshnode[i];
      meshnode[i] = swap;
    }

  free(newnum);

  SortElements();

  return TRUE;
}
