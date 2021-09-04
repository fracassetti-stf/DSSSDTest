## Note: Credits still in progress
- Credits still need to be completed. Contacts: Stefano Fracassetti.  

 
# DSSSD Test @KU Leuven
Data Analysis of the **D**ouble-**S**ided **S**ilicon **S**trip **D**etector @Ku Leuven, Belgium.  
Those measurements have been performed in 17 May - 10 June 2021 with the Febex acquisition system.  
Characterization and performance of the DSSSD array ( 18 detectors) has been evaluated using a triple-alpha source: 
- The first 10 detectors (from number 1 to number 10) were bought in November 2017 (and are usaully refered as "old detectors"), they were also already tested last time in March/April 2019, by *Jonas Refsgaard* and *Alice Mentana*, using *SpecMat* GET acquisition system. They were also used in previous experiment, such as GMR in <sup>68</sup>Ni @GANIL(Caen), and in <sup>20</sup>Ne @LNS(Catania).
- The last 8 detectors (from number 11 to number 18) were (recently) bought in February 2021 (for this reason refereed as "new detectors"), they were never used before and not even tested.
- All detectors are of the same model, and from the same company: "Micron Semiconductors LTD".
- The detector configuarion was with floating front/back GR(Guard Ring), and floating front/back FP(Field Plate). Floating = Not Connected. 
    - So far, the detectors have always been used in this configuration: The GR can be biased - tipically done for detectors of thickness >1500um - to ensure that it is at the same potential of the active area, and to sweep out any charge collected by the GR itself. The FP can be biased - typically done on single-area single-sided devices - for cases were the device stability and breakdown voltage need to be extended. 
    - There is not a compelling need to make connections for these addition pins in our case, but the possiblily to actually use the addition pins could be further investigated. In particular, noise and lower-resolution in the first and last strips may be reduced in this way. On the other side, this new configuration would require different feedthrough PCB, so new feedthroughs must be produced.

 
## Folder Content:
Each subfoler may have a specific `README.md`, with further description of the content.  
- `Characterization/`: Characterization of the detectors in terms of Leakage Current and Depletion Voltage.
- `ConversionLMDtoROOT/`: Program to convert the LMD files into ROOT (Root Files containing 32-histograms: h0,...,h31).
- `Data/`: Data acquired with the DSSSD.
     - `Data/Root/`: Root files containing 32 histogram, which correspond to the spectra acquired with a triple-alpha spectra.
- `EnergyResolution/`: RootMacro to extract energy resolution, strip by strip of all the 18 detectors.
- `PeakFitting/`: RootMarco for fitting the peak from the histograms. In particular an intercatively (and fast) macro has been written, which allows to fit all-strips peaks in reasonable time.
- `Results/`: All the results (images) produced in the different steps of the analysis.
- `PlotStrips/`: RootMarco to plot 16 strips at once, in one Canvas. It offers a fast-check of the strip spectra.  
- `Pulser/`: Configuration file for the pulser. The pulser was used on one detector, in order to have a feedback on the electronic chain.
- `README.md`: this file.


## Credits:
- DSSD data have been collected at the **Katholieke Universiteit Leuven** (Belgium).  
Contributions: _Stefano Fracassetti_<sup>1</sup>, _Jozef Klimo_<sup>1</sup>, _Hilde De Witte_<sup>1</sup>, _Alberto Camaiani_<sup>1</sup>, _Mouftahou Latif_<sup>1</sup>.  
Supervisor: _Riccardo Raabe_<sup>1</sup>
- The [Charaterization](https://github.com/fracassetti-stf/DSSSDTest/tree/main/Characterization) analysis of the DSSSD has been carried out by _Stefano Fracassetti_<sup>1</sup> and _Jozef Klimo_<sup>1</sup>.
- The converstion macro [conversion.cc](https://github.com/fracassetti-stf/DSSSDTest/blob/main/ConversionLMDtoROOT/conversion.cc) (from LMD to ROOT) which has been used in this analysis has been written by (to be updated).
- The [Energy Resolution](https://github.com/fracassetti-stf/DSSSDTest/tree/main/EnergyResolution) macros have been written by _Stefano Fracassetti_<sup>1</sup>.
- The program to fit the peak has been written by _Oleg Ivanov_ ([FitPeaks.C](https://github.com/fracassetti-stf/DSSSDTest/blob/main/PeakFitting/FitPeaks.C)) and _Stefano Fracassetti_<sup>1</sup> ([FitPeaksInteractively.C](https://github.com/fracassetti-stf/DSSSDTest/blob/main/PeakFitting/FitPeaksInteractively.C)).
- The [Results](https://github.com/fracassetti-stf/DSSSDTest/tree/main/Results) of the analysis have been obtained by _Stefano Fracassetti_<sup>1</sup>, writing or using the different prgram present in this folder.  

<sup>1</sup> Katholieke Universiteit Leuven, Belgium

## License:
Data relative to the DSSSD test should not be used in any way.  

I give permission to use/modify/improve the root macro I have written (_Stefano Fracassetti_) as you like and for your purpose, but always giving credits.  

For the other program in this folder please refer to section Credits, and ask the corresponding person for usage/modification/sharing of the program/macro itself. 
