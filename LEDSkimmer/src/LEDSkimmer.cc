// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ref.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "math.h"

#include "TFile.h"
#include "TTree.h"

#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "FWCore/ParameterSet/interface/Registry.h"

#include <iostream>

const static std::vector<std::string> interestingTriggers = {
      "HLT_DoubleEle33_CaloIdL",
      "HLT_DoublePhoton33",
};

template <typename T>
void subtractInvisible(T g, reco::Candidate::LorentzVector& p4) {
  auto daughters = (*g).daughterRefVector();
  for (auto d = daughters.begin(); d != daughters.end(); d++) {
    const auto pdgId = (*d)->pdgId();
    if (std::abs(pdgId) == 12 || std::abs(pdgId) == 14 ||
        std::abs(pdgId) == 16 || std::abs(pdgId) == 18) {
      p4 = p4 - (*d)->p4();
    }
    subtractInvisible(*d, p4);
  }
}

template <typename T>
int findBestVisibleMatch(T& gens, reco::Candidate::LorentzVector& p4) {
  float minDeltaR = 999.0;
  int idx = -1;
  for (auto g = gens.begin(); g != gens.end(); g++) {
    auto tmp_p4 = g->p4();
    subtractInvisible(g, tmp_p4);
    const auto tmp = deltaR(tmp_p4, p4);
    if (tmp < minDeltaR) {
      minDeltaR = tmp;
      idx = g - gens.begin();
    }
  }
  return idx;
}

template <typename T>
int findBestMatch(T& gens, reco::Candidate::LorentzVector& p4) {
  float minDeltaR = 999.0;
  int idx = -1;
  for (auto g = gens.begin(); g != gens.end(); g++) {
    const auto tmp = deltaR(g->p4(), p4);
    if (tmp < minDeltaR) {
      minDeltaR = tmp;
      idx = g - gens.begin();
    }
  }
  return idx;
}


//
// class declaration
//

class LEDSkimmer : public edm::EDAnalyzer {
   public:
      explicit LEDSkimmer(const edm::ParameterSet&);
      ~LEDSkimmer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      bool providesGoodLumisection(const edm::Event &iEvent);
      bool isData;

      TTree *tree;
      TFile *fs;
// ----------member data ---------------------------
  // Event information
  Int_t value_run;
  UInt_t value_lumi_block;
  ULong64_t value_event;

  // Trigger
  const static int max_trig = 1000;
  bool value_trig[max_trig];

  // Vertices
  int value_ve_n;
  float value_ve_x;
  float value_ve_y;
  float value_ve_z;


  // Electrons
  const static int max_el = 1000;
  UInt_t value_el_n;
  float value_el_pt[max_el];
  float value_el_eta[max_el];
  float value_el_phi[max_el];
  float value_el_mass[max_el];
  int value_el_charge[max_el];
  float value_el_pfreliso03all[max_el];
  float value_el_dxy[max_el];
  float value_el_dxyErr[max_el];
  float value_el_dz[max_el];
  float value_el_dzErr[max_el];
  bool value_el_cutbasedid[max_el];
  bool value_el_pfid[max_el];
  int value_el_genpartidx[max_el];
  int value_el_jetidx[max_el];

// Generator particles
  const static int max_gen = 1000;
  UInt_t value_gen_n;
  float value_gen_pt[max_gen];
  float value_gen_eta[max_gen];
  float value_gen_phi[max_gen];
  float value_gen_mass[max_gen];
  int value_gen_pdgid[max_gen];
  int value_gen_motherid[max_gen];
  int value_gen_status[max_gen];
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
LEDSkimmer::LEDSkimmer(const edm::ParameterSet& iConfig)

        : isData(iConfig.getParameter<bool>("isData")) {
   //now do what ever initialization is needed
  fs = new TFile("EleInfo.root","RECREATE");
  tree = new TTree("tree","EleInfo");


  // Event information
  tree->Branch("run", &value_run);
  tree->Branch("luminosityBlock", &value_lumi_block);
  tree->Branch("event", &value_event);

  // Trigger
  for(size_t i = 0; i < interestingTriggers.size(); i++) {
    tree->Branch(interestingTriggers[i].c_str(), value_trig + i, (interestingTriggers[i] + "/O").c_str());
  }

  // Vertices
  tree->Branch("PV_npvs", &value_ve_n, "PV_npvs/I");
  tree->Branch("PV_x", &value_ve_x, "PV_x/F");
  tree->Branch("PV_y", &value_ve_y, "PV_y/F");
  tree->Branch("PV_z", &value_ve_z, "PV_z/F");

 // Electrons
  tree->Branch("nElectron", &value_el_n, "nElectron/i");
  tree->Branch("Electron_pt", value_el_pt, "Electron_pt[nElectron]/F");
  tree->Branch("Electron_eta", value_el_eta, "Electron_eta[nElectron]/F");
  tree->Branch("Electron_phi", value_el_phi, "Electron_phi[nElectron]/F");
  tree->Branch("Electron_mass", value_el_mass, "Electron_mass[nElectron]/F");
  tree->Branch("Electron_charge", value_el_charge, "Electron_charge[nElectron]/I");
  tree->Branch("Electron_pfRelIso03_all", value_el_pfreliso03all, "Electron_pfRelIso03_all[nElectron]/F");
  tree->Branch("Electron_dxy", value_el_dxy, "Electron_dxy[nElectron]/F");
  tree->Branch("Electron_dxyErr", value_el_dxyErr, "Electron_dxyErr[nElectron]/F");
  tree->Branch("Electron_dz", value_el_dz, "Electron_dz[nElectron]/F");
  tree->Branch("Electron_dzErr", value_el_dzErr, "Electron_dzErr[nElectron]/F");
  tree->Branch("Electron_cutBasedId", value_el_cutbasedid, "Electron_cutBasedId[nElectron]/O");
  tree->Branch("Electron_pfId", value_el_pfid, "Electron_pfId[nElectron]/O");
  tree->Branch("Electron_jetIdx", value_el_jetidx, "Electron_jetIdx[nElectron]/I");
  tree->Branch("Electron_genPartIdx", value_el_genpartidx, "Electron_genPartIdx[nElectron]/I");

  // Generator particles
  if (!isData) {
    tree->Branch("nGenPart", &value_gen_n, "nGenPart/i");
    tree->Branch("GenPart_pt", value_gen_pt, "GenPart_pt[nGenPart]/F");
    tree->Branch("GenPart_eta", value_gen_eta, "GenPart_eta[nGenPart]/F");
    tree->Branch("GenPart_phi", value_gen_phi, "GenPart_phi[nGenPart]/F");
    tree->Branch("GenPart_mass", value_gen_mass, "GenPart_mass[nGenPart]/F");
    tree->Branch("GenPart_pdgId", value_gen_pdgid, "GenPart_pdgId[nGenPart]/I");
    tree->Branch("GenPart_motherId", value_gen_motherid, "GenPart_motherId[nGenPart]/I");
    tree->Branch("GenPart_status", value_gen_status, "GenPart_status[nGenPart]/I");
  }
}


LEDSkimmer::~LEDSkimmer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
LEDSkimmer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

  // Event information
  value_run = iEvent.run();
  value_lumi_block = iEvent.luminosityBlock();
  value_event = iEvent.id().event();

  // Trigger results
  Handle<TriggerResults> trigger;
  iEvent.getByLabel(InputTag("TriggerResults", "", "HLT"), trigger);
  auto psetRegistry = edm::pset::Registry::instance();
  auto triggerParams = psetRegistry->getMapped(trigger->parameterSetID());
  TriggerNames triggerNames(*triggerParams);
  TriggerResultsByName triggerByName(&(*trigger), &triggerNames);
  for (size_t i = 0; i < interestingTriggers.size(); i++) {
    value_trig[i] = false;
  }
  const auto names = triggerByName.triggerNames();
  for (size_t i = 0; i < names.size(); i++) {
    const auto name = names[i];
    for (size_t j = 0; j < interestingTriggers.size(); j++) {
      const auto interest = interestingTriggers[j];
      if (name.find(interest) == 0) {
        const auto substr = name.substr(interest.length(), 2);
        if (substr.compare("_v") == 0) {
          const auto status = triggerByName.state(name);
          if (status == 1) {
            value_trig[j] = true;
            break;
          }
        }
      }
    }
  }
  // Vertex
  Handle<VertexCollection> vertices;
  iEvent.getByLabel(InputTag("offlinePrimaryVertices"), vertices);
  value_ve_n = vertices->size();
  value_ve_x = vertices->begin()->x();
  value_ve_y = vertices->begin()->y();
  value_ve_z = vertices->begin()->z();
  math::XYZPoint pv(vertices->begin()->position());

  // Electrons
  Handle<GsfElectronCollection> electrons;
  iEvent.getByLabel(InputTag("gsfElectrons"), electrons);

  value_el_n = 0;
  const float el_min_pt = 5;
  std::vector<GsfElectron> selectedElectrons;
  for (auto it = electrons->begin(); it != electrons->end(); it++) {
    if (it->pt() > el_min_pt) {
      //std::cout<<it->pt()<<"================================AQUI===========================\n ";
      selectedElectrons.emplace_back(*it);
      value_el_pt[value_el_n] = it->pt();
      value_el_eta[value_el_n] = it->eta();
      value_el_phi[value_el_n] = it->phi();
      value_el_charge[value_el_n] = it->charge();
      value_el_mass[value_el_n] = it->mass();
      value_el_cutbasedid[value_el_n] = it->passingCutBasedPreselection();
      value_el_pfid[value_el_n] = it->passingPflowPreselection();
      if (it->passingPflowPreselection()) {
        auto iso03 = it->pfIsolationVariables();
        value_el_pfreliso03all[value_el_n] =
            (iso03.chargedHadronIso + iso03.neutralHadronIso + iso03.photonIso)/it->pt();
      } else {
        value_el_pfreliso03all[value_el_n] = -999;
      }
      auto trk = it->gsfTrack();
      value_el_dxy[value_el_n] = trk->dxy(pv);
      value_el_dz[value_el_n] = trk->dz(pv);
      value_el_dxyErr[value_el_n] = trk->d0Error();
      value_el_dzErr[value_el_n] = trk->dzError();
      value_el_jetidx[value_el_n] = -1;
      value_el_genpartidx[value_el_n] = -1;
      value_el_n++;
    }
  }

  if (!isData) {
    Handle<GenParticleCollection> gens;
    iEvent.getByLabel(InputTag("genParticles"), gens);

    value_gen_n = 0;
    std::vector<GenParticle> interestingGenParticles;
    for (auto it = gens->begin(); it != gens->end(); it++) {
      const auto status = it->status();
      const auto pdgId = std::abs(it->pdgId());
      if (status == 1 && pdgId == 13) { // muon
        interestingGenParticles.emplace_back(*it);
      }
      if (status == 1 && pdgId == 11) { // electron
        interestingGenParticles.emplace_back(*it);
      }
      /*
      if (status == 1 && pdgId == 22) { // photon
        interestingGenParticles.emplace_back(*it);
      }
      */
      if (status == 2 && pdgId == 15) { // tau
        interestingGenParticles.emplace_back(*it);
      }
    }


    // Match electrons with gen particles
    for (auto p = selectedElectrons.begin(); p != selectedElectrons.end(); p++) {
      // Gen particle matching
      auto p4 = p->p4();
      auto idx = findBestVisibleMatch(interestingGenParticles, p4);
      if (idx != -1) {
        auto g = interestingGenParticles.begin() + idx;
        value_gen_pt[value_gen_n] = g->pt();
        value_gen_eta[value_gen_n] = g->eta();
        value_gen_phi[value_gen_n] = g->phi();
        value_gen_mass[value_gen_n] = g->mass();
        value_gen_pdgid[value_gen_n] = g->pdgId();
        value_gen_motherid[value_gen_n] = g->mother()->pdgId();
	//Mother
	auto motherRefs = g->motherRefVector();
        for(reco::GenParticleRefVector::const_iterator imr = motherRefs.begin(); imr!= motherRefs.end(); ++imr) {
	 cout<<"   - Mother "<<(*imr).key()<<" "<<(*imr)->pdgId()<<endl;
       }
        value_gen_status[value_gen_n] = g->status();
        value_el_genpartidx[p - selectedElectrons.begin()] = value_gen_n;
        value_gen_n++;
      }
    }
  } // !isData

  // Fill event
  tree->Fill();
  return;
}


// ------------ method called once each job just before starting event loop  ------------
void
LEDSkimmer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
LEDSkimmer::endJob()
{
   fs->Write();
}

// ------------ method called when starting to processes a run  ------------
void
LEDSkimmer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
LEDSkimmer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
LEDSkimmer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
LEDSkimmer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LEDSkimmer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(LEDSkimmer);
