## LEDSkimmer

This is an EDAnalyzer (CMSSW) to extract information from CMS open data in order to search for LED.

Need a CMSSW_5_3_32 environment to run.

To compile and run:

`git clone the repository`

`cd LEDAnalysis/LEDSkimmer`

`cmsenv`

Test in the background:

`cmsRun ledskimmer_cfg.py > ledskimmer.log 2>&1 &`
