# DSSSD Test: Analysis
Analysed Data of the Double-Sided Silicon Strip Detector @Ku Leuven.  
The tests were taken for all 18 DSSSD using triple-alpha source: 
- The first 10 detectors (from number 1 to number 10) were bought in November 2017 (and are usaully refered as "old detectors"), they were also already tested last time in March/April 2019, by Jonas Refsgaard and Alice Mentana.
- The last 8 detectors (from number 11 to number 18) were (recently) bought in February 2021 (for this reason refereed as "new detectors"), they were never used before and not even tested.
- All detectors are of the same model, and from the same company: "Micron Semiconductors LTD".
- The detector configuarion was with floating front/back GR(Guard Ring), and floating front/back FP(Field Plate). Up to now, the detectors have always been used in this configuration. The GR can be biased to increase performances, but it is tipically done for detectors of thickness >1500um. The FP can be biased 
Tesk performed in 17 May - 10 June 2021 with Febex acquisition system.
Partecipants: Stefano Fracassetti, jozef Klimo, Alberto Camaiani, Mouftahou Latif, Hilde De Witte.
 
 
 ## Folder Content:
 - `ConversionLMDtoROOT/`: Program to convert the LMD files into ROOT (Root Files containing 32-histograms: h0,...,h31). The details on the program are writtent in the file `ConversionLMDtoROOT/README.md`.

- `Data/`: Data acquired with the DSSSD.
     - `Data/Root/`: Root files containing 32 histogram, which correspond to the spectra acquired with a triple-alpha spectra. Those files are divided in Front, adn Back since only 32 strips were collected at once - so Front and Back were collected at different moment in time. Each run lasted for 1 hour, the distance source-DSSSD was of 13.7 cm (moved to ~10 cm) for last measurements. The spectra acquired should be rebinned by a factor or 4 for a better visual output.

- `Depletion/`: Characterization of the detectors in terms of Leakage Current and Depletion Voltage. For each detector, we varied the voltage from 60V up to 120/140V (depending on the detector) at step of 10V and register the corresponding value of Current and Peak Channel. This allow to monitor the current at first, and secondly to obtain the value of the depletion voltage, which is clearly visible looking at the shift of the channel of a certain peak.

- `EnergyResolution/`: RootMacro to extract energy resolution, strip by strip of all the 18 detectors. 1D and 2D hisograms are produced in order to have different (visual) output. Further details on the implementation, and usage are written in `EnergyResolution/README.md`
    - `Resolution1D.C`: Energy Resolution 1D-Histograms of all strip for each detectors - one figure for each detector, 
    - `Resolution2D.C`: Energy Resolution 2D-Histogram of all strip for all detectors - one figure for all detectors, 
    - `SpatialResolution.C`: Spatial Energy Resolution 2D-histogram for each detectors - one figure for each detector.
    - FixedForAll and DifferentForEachOne refers to the scale used: the first one allows a fair comparison of different detectors, the second allows a clear picture of the resolution of a specific detector.

- `PeakFitting/`: RootMarco for fitting the peak from the histograms. In particular an intercatively (and fast) macro has been written, which allows to fit all-strips peaks in reasonable time. FRefer to `PeakFitting/README.md` for further information on how to use this program, and implementation details. 

- `Pictures/`: All the output-images produced in the different steps of the analysis.

- `PlotStrips/`: RootMarco to plot 16 strips at once, in one Canvas. It offers a fast-check of the strip spectra. Further details in the file `PlotStrips/README.md`.
    -  "015" or "1631" refers at the number of strip plotted: first half (from 0 to 15) or second half (from 16 to 31).
 
Be aware that the root macro for fit and resolution may need some more comments,
and a proper explanation on how to use them. I will update them in the future.


## Credits:

License:
You have permission only to use/modify the root macro in this folder for your purpose,
giving credits. Data relative to the DSSSD test should not be used.

