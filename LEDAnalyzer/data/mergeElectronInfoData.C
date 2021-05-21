#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include <iostream>

void mergeElectronInfoData() {
TChain ch("aod2nanoaod/Events");
for (int i = 1; i < 3; ++i)
  {
    TString filename = TString::Format("ElectronInfoData-%i.root", i);
    ch.Add(filename);
  }
ch.Merge("ElectronInfoData.root");
}