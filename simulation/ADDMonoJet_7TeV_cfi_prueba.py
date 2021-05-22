import FWCore.ParameterSet.Config as cms
generator = cms.EDFilter("Pythia8GeneratorFilter",
   comEnergy = cms.double(7000.0),
   crossSection = cms.untracked.double(0.0587),
   filterEfficiency = cms.untracked.double(1),
   maxEventsToPrint = cms.untracked.int32(0),
   pythiaHepMCVerbosity = cms.untracked.bool(False),
   pythiaPylistVerbosity = cms.untracked.int32(0),

   PythiaParameters = cms.PSet(
      processParameters = cms.vstring(
         'Random:setSeed = on',
         'Random:seed = 0',
         'Main:timesAllowErrors    = 10000',
         'ParticleDecays:limitTau0 = on',
         'ParticleDecays:tauMax = 10',
         'Tune:pp 5',
         'Tune:ee 3',
         'PDF:pSet = 5',
         'ExtraDimensionsLED:ffbar2llbar = on', 
         'ExtraDimensionsLED:gg2llbar = on', 
         'PhaseSpace:mHatMin = 300',
         'ExtraDimensionsLED:CutOffmode = 0',
         'ExtraDimensionsLED:LambdaT = 2800'
      ),
      parameterSets = cms.vstring('processParameters')
   )
)
