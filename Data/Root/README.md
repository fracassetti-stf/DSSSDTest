# Data/Root
These files are the file acquired during the test, using a triple-alpha source: <sup>239</sup>Pu, <sup>241</sup>Am and <sup>244</sup>Cm. 
The Root files have been produced converting the lmd files (not in this folder) using the program `Conversion_Front.cc` and `Conversion_Back.cc` respectively. 
Each run lasted for 1 hour, the distance source-DSSSD was of 13.7 cm, reduced to ~10 cm for last measurements: the statistics is enough to clearly define the Gaussian peaks. 
The spectra acquired should be rebinned by a factor or 4 for a better visual output.  
For one detector (Det. No. 2) data were collected in two configuration: with and without pulser, in addition to the triple-alpha source, which was present in both cases.



## Filenames 
- **Front and Back:** The filenames contains Front/Back since only 32 strips were collected at once - so Front and Back were collected at different moment in time. In general one straight after the other.

- **Voltage:** The filenames contain the depletion voltage used for the test, which is usually around 80V for the old detectors, and generally higher for the new ones.  

- **Pulser:** The filename contains pulser for the case the pulser signal was used in input to the pre-amplifier MPR-64 (in addition to alpha source).


