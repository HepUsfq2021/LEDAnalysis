import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("led")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#Data files
#files = FileUtils.loadListFromFile("data/CMS_Run2011A_DoubleElectron_AOD_12Oct2013-v1_20000_file_index.txt")
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2011A_DoubleElectron_AOD_12Oct2013-v1_20001_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2011B_DoubleElectron_AOD_12Oct2013-v1_00000_file_index.txt"))
#files.extend(FileUtils.loadListFromFile("data/CMS_Run2011B_DoubleElectron_AOD_12Oct2013-v1_20000_file_index.txt"))

#Background files example
#files = FileUtils.loadListFromFile("data/CMS_MonteCarlo2011_Summer11LegDR_DYJetsToLL_M-50_7TeV-madgraph-pythia6-tauola_AODSIM_PU_S13_START53_LV6-v1_00000_file_index.txt")

#process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
#    fileNames = cms.untracked.vstring('root://eospublic.cern.ch//eos/opendata/cms/Run2011A/DoubleElectron/AOD/12Oct2013-v1/20000/0014CE62-9C3E-E311-8FCC-00261894389F.root')
#)

#Signal
files = FileUtils.loadListFromFile("data/fileNamesSim.txt")
process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring(*files
#        'file:data/simData/recoDY_3-3.root'
    )
)

# Apply JSON file with lumi mask (needs to be done after the process.source definition)
#goodJSON = "data/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt"
#myLumis = LumiList.LumiList(filename=goodJSON).getCMSSWString().split(",")
#process.source.lumisToProcess = CfgTypes.untracked(
#    CfgTypes.VLuminosityBlockRange())
#process.source.lumisToProcess.extend(myLumis)

#False == Simulation
#True == RealDataCMS
process.led = cms.EDAnalyzer('LEDSkimmer', isData = cms.bool(False)
)


process.p = cms.Path(process.led)
