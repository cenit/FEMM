#include "femm.h"
#define _CRT_SECURE_NO_WARNINGS

extern "C" _declspec(dllexport) void openfemm(char* outbuff);
extern "C" _declspec(dllexport) void call2femm(char* inbuff, char* outbuff);
extern "C" _declspec(dllexport) void closefemm();

IActiveFEMM FemmInstance;

void openfemm(char* outbuff)
{
  OleInitialize(NULL);
  if (FemmInstance.CreateDispatch("femm.ActiveFEMM") == 0)
    strcpy(outbuff, "Couldn't open FEMM");
  else
    strcpy(outbuff, "FEMM opened");
}

void call2femm(char* inbuff, char* outbuff)
{
  CString rslt;
  CString luacmd = inbuff;
  rslt = FemmInstance.mlab2femm(luacmd);
  strcpy(outbuff, rslt);
}

void closefemm()
{
  FemmInstance.ReleaseDispatch();
  OleUninitialize();
}
