# DSSSD Test
Analysed Data of the Double-Sided Silicon Strip Detector @Ku Leuven.  
Tesk performed in 17 May - 10 June 2021 with Febex acquisition system.  
Partecipants: *Stefano Fracassetti*, *Jozef Klimo*, *Hilde De Witte*, *Alberto Camaiani*, *Mouftahou Latif*.  


The tests were taken for 18 DSSSD using triple-alpha source: 
- The first 10 detectors (from number 1 to number 10) were bought in November 2017 (and are usaully refered as "old detectors"), they were also already tested last time in March/April 2019, by *Jonas Refsgaard* and *Alice Mentana*, using *SpecMat* GET acquisition system. They were also used in previous experiment, such as GMR in <sup>68</sup>Ni @GANIL(Caen), and in <sup>20</sup>Ne @LNS(Catania).
- The last 8 detectors (from number 11 to number 18) were (recently) bought in February 2021 (for this reason refereed as "new detectors"), they were never used before and not even tested.
- All detectors are of the same model, and from the same company: "Micron Semiconductors LTD".
- The detector configuarion was with floating front/back GR(Guard Ring), and floating front/back FP(Field Plate). Floating = Not Connected. So far, the detectors have always been used in this configuration: The GR can be biased - tipically done for detectors of thickness >1500um - to ensure that it is at the same potential of the active area, and to sweep out any charge collected by the GR itself. The FP can be biased - typically done on single-area single-sided devices - for cases were the device stability and breakdown voltage need to be extended. There is not a compelling need to make connections for these addition pins in our case, but the possiblily to actually use the addition pins could be further investigated - this would require different feedthrough PCB.
 
## Note: Folder Content and Credits still in progress
I am updating these sections. In particular, if further credits are needed please contact me.  
`README.md` of the specific folder mentioned below still have to be produced/updated.
 
## Folder Content:
Each subfoler may have a specific `README.md`, with further description of the content.  
- `Characterization/`: Characterization of the detectors in terms of Leakage Current and Depletion Voltage.
- `ConversionLMDtoROOT/`: Program to convert the LMD files into ROOT (Root Files containing 32-histograms: h0,...,h31).
- `Data/`: Data acquired with the DSSSD.
     - `Data/Root/`: Root files containing 32 histogram, which correspond to the spectra acquired with a triple-alpha spectra.
- `EnergyResolution/`: RootMacro to extract energy resolution, strip by strip of all the 18 detectors.
- `PeakFitting/`: RootMarco for fitting the peak from the histograms. In particular an intercatively (and fast) macro has been written, which allows to fit all-strips peaks in reasonable time.
- `Pictures/`: All the output-images produced in the different steps of the analysis.
- `PlotStrips/`: RootMarco to plot 16 strips at once, in one Canvas. It offers a fast-check of the strip spectra.  
- `Pulser/`: Configuration file for the pulser. The pulser was used on one detector, in order to have a feedback on the electronic chain.
- `README.md`: this file.



## Credits:
To be implemented.

## License:
I give permission to use/modify/improve the root macro I have written (Stefano Fracassetti) in this folder, as you like and for your purpose,
but always giving credits. For the other program in this folder please refer to section Credits, and ask the corresponding person for usage/modification/sharing of the program itself. Data relative to the DSSSD test should not be used in any way.
