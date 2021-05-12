## LEDAnalyzer

Root compilable macro to execute the final analysis for LED search.  This code produces a root file with histograms.

It can be compiled as:

`g++ -g -O3 -Wall -Wextra -Wpedantic -o LEDAnalyzer LEDAnalyzer.cxx $(root-config --cflags --libs)`

and then executed as:

`./LEDAnalyzer`