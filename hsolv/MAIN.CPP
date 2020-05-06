#include <stdafx.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <afxtempl.h>
#include "hsolv.h"
#include "hsolvDlg.h"
#include "spars.h"
#include "mesh.h"
#include "hsolvdoc.h"

void old_main(void* inptr)
{
  ChsolvDlg* TheView;
  Chsolvdoc Doc;
  char PathName[256];
  CFileDialog* fname_dia;
  char outstr[1024];
  int i;

  TheView = (ChsolvDlg*)inptr;

  // get the name of the file to be processed,
  // either from argv or from the user
  if (__argc < 2) {

    fname_dia = new CFileDialog(
        TRUE,
        "fem | * ",
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "heat flow datafile (*.feh) | *.feh; *.FEH | All Files (*.*) | *.*||",
        NULL);

    if (fname_dia->DoModal() == IDCANCEL) {
      delete[] fname_dia;
      MsgBox("No file name!");
      exit(0);
    }

    CString fname = fname_dia->GetPathName();
    fname = fname.Left(fname.GetLength() - 4);
    strcpy(PathName, fname);
    delete[] fname_dia;
  } else
    strcpy(PathName, __argv[1]);

  Doc.PathName = PathName;
  Doc.TheView = TheView;

  // load problem geometry
  if (Doc.OnOpenDocument() != TRUE) {
    MsgBox("problem loading .feh file");
    exit(7);
  }

  // load mesh
  if (Doc.LoadMesh() != TRUE) {
    MsgBox("problem loading mesh");
    exit(2);
  }

  // load previous solution, if applicable
  if (Doc.LoadPrev() != TRUE) {
    exit(3);
  }

  // label the dialog to report which problem is being solved
  char PaneText[256];
  char* ProbName;
  ProbName = PathName;
  for (i = 0; i < (int)strlen(PathName); i++)
    if (PathName[i] == '\\')
      ProbName = PathName + i;
  if (strlen(PathName) > 0) {
    ProbName++;
    sprintf(PaneText, "%s - hsolve", ProbName);
  }

  while (!IsWindow(TheView->m_hWnd))
    Sleep(1);
  TheView->SetWindowText(PaneText);

  // renumber using Cuthill-McKee
  TheView->SetDlgItemText(IDC_STATUSWINDOW, "renumbering nodes");
  if (Doc.Cuthill() != TRUE) {
    MsgBox("problem renumbering nodes");
    exit(4);
  }

  // display details about the problem to be solved
  TheView->SetDlgItemText(IDC_STATUSWINDOW, "solving...");
  sprintf(outstr, "Problem Statistics:\n%i nodes\n%i elements\nPrecision: %3.2e\n",
      Doc.NumNodes, Doc.NumEls, Doc.Precision);
  TheView->SetDlgItemText(IDC_PROBSTATS, outstr);

  // initialize the problem, allocating the space required to solve it.

  CBigLinProb L;
  L.TheView = TheView;
  L.Precision = Doc.Precision;

  if (L.Create(Doc.NumNodes + Doc.NumCircProps, Doc.BandWidth) == FALSE) {
    MsgBox("couldn't allocate enough space for matrices");
    Doc.CleanUp();
    L.~CBigLinProb();
    exit(5);
  }

  // Create element matrices and solve the problem;
  if (Doc.AnalyzeProblem(L) == FALSE) {
    MsgBox("Couldn't solve the problem");
    Doc.CleanUp();
    L.~CBigLinProb();
    exit(6);
  }

  TheView->SetDlgItemText(IDC_STATUSWINDOW, "Problem solved");

  // now that we have results, write 'em to dist
  if (Doc.WriteResults(L) == FALSE) {
    MsgBox("couldn't write results to disk");
    Doc.CleanUp();
    L.~CBigLinProb();
    exit(7);
  }
  TheView->SetDlgItemText(IDC_STATUSWINDOW, "results written to disk");

  // make sure that everything is freed up to avoid memory leaks
  Doc.CleanUp();
  L.~CBigLinProb();
  exit(0);
}
