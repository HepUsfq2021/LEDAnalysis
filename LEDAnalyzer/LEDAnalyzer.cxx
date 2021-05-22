#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TH1D.h"
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <numeric>      // std::iota
#include <algorithm>
//compiling line
//g++ -g -O3 -Wall -Wextra -Wpedantic -o LEDAnalyzer LEDAnalyzer.cxx $(root-config --cflags --libs)

double invmass(float el_pt[], float el_eta[], float el_phi[], float el_mass[], UInt_t nel){
	using namespace std;
	double m=-100;
	TLorentzVector P1,P2,P;

	//get first hardest idx
	int idx0 = -1;
	float el_pt_max=0;
	for(UInt_t j=0;j<nel;++j){
		if (el_pt[j]>el_pt_max){
			el_pt_max=el_pt[j];
			idx0=j;
		}
	}
	//get second hardest idx
        int idx1=-1;
	el_pt_max=0;
	for(UInt_t i=0;i<nel;++i){
		if (i==UInt_t(idx0)) continue;
		if (el_pt[i]>el_pt_max){
			el_pt_max = el_pt[i];
			idx1=i;
		}
	}


	P1.SetPtEtaPhiM(el_pt[idx0],el_eta[idx0],el_phi[idx0],el_mass[idx0]);
	P2.SetPtEtaPhiM(el_pt[idx1],el_eta[idx1],el_phi[idx1],el_mass[idx1]);
	P = (P1+P2);
	m = P.M();
	return m;
}


void LEDAnalyzer(){
using namespace std;
//open the root file to read branches from
TFile* infile = new TFile("data/EleInfoData1400.root", "READ");

//Read the tree where the info was stored
TTree* mytree = (TTree*)infile->Get("tree");

//declare variables to be used
UInt_t el_n;
float el_pt[1400];
float el_eta[1400];
float el_phi[1400];
float el_mass[1400];
float el_HcalOverEcal[1400];
float el_pZ[1400];
float el_dxy[1400];
int el_charge[el_n];

TH1F* himass=new TH1F("himass","Masa invariante",250,0,2500);
TH1F* hhovere=new TH1F("hhovere","HCAL/ECAL",250,0,250);

mytree->SetBranchAddress("nElectron",&el_n);
mytree->SetBranchAddress("Electron_pt",&el_pt);
mytree->SetBranchAddress("Electron_eta",&el_eta);
mytree->SetBranchAddress("Electron_phi",&el_phi);
mytree->SetBranchAddress("Electron_mass",&el_mass);
mytree->SetBranchAddress("Electron_HcalOverEcal",&el_HcalOverEcal);
mytree->SetBranchAddress("PV_z",&el_pZ);
mytree->SetBranchAddress("Electron_dxy",&el_dxy);
mytree->SetBranchAddress("Electron_charge",&el_charge);

int nEvents = mytree->GetEntries();
//loop over event
for (int j=0;j<nEvents;++j){
	int total_charge = 0;
	float invm = -100;
	cout<<"Event "<<j+1<<endl;
	mytree->GetEntry(j);
//	cout<<"Number of electrons found is "<<el_n<<"  ch: "<<el_charge[j]<<"  dxy:   "<<el_dxy[j]<<endl;
	//if more than 2 electrons found, calculate the inv mass
	//To complete more cuts
	if(el_HcalOverEcal[j]<0.05){
		if(el_n>=2){
			if(el_dxy[j]<2 && abs(el_pZ[j])<24){
				if(abs(el_eta[j])<1.5){
					for(UInt_t i=0; i<el_n; ++i){
						total_charge += el_charge[i];
					}
						if(total_charge == 0 || abs(total_charge) == 1){
							invm = invmass(el_pt,el_eta,el_phi,el_mass,el_n);
							cout<<"Number of electrons found is "<<el_n<<"  ch: "<<el_charge[j]<<"  dxy:   "<<el_dxy[j]<<endl;
							himass->Fill(invm);
							hhovere->Fill(el_HcalOverEcal[j]);
						}
					
				}
			}
		}
	}
}

TFile* outfile = new TFile("histos.root","RECREATE");
himass->Write();
hhovere->Write();
outfile->Close();
}

int main (){
	using namespace std;
	cout<<"Running the LEDAnalyzer ...."<<endl;
	LEDAnalyzer();
}
