# EnergyResolution
These macro allows the visualize the energy resolution, for each detectors, as well as to compare all detectors at once.  
Histograms (1D and 2D) have been produced in order to have different (visual) output:
- `Resolution1D.C`: Energy Resolution 1D-Histograms of all strip for each detectors - one figure for each detector, 
- `Resolution2D.C`: Energy Resolution 2D-Histogram of all strip for all detectors - one figure for all detectors, 
- `SpatialResolution2D.C`: Spatial Energy Resolution 2D-histogram for each detectors - one figure for each detector.  

These maros aventaully have not been used since other marcos have been written, which are better for the purpose:
- `GraphResolution1D.C` (replaced by `Resolution1D.C`): Energy Resolution 1D-Graph of all strip for each detectors - one figure for each detector.
- `GraphResolution2D.C` (replaced by `Resolution2D.C`): Energy Resolution 2D-Graph of all strip for all detectors  - one figure for all detectors.

## Root Macros: How to use?
For more information regarding the images produced with these macro, see `Results/README.md` in section [EnergyResolution_241Am](https://github.com/fracassetti-stf/DSSSDTest/tree/main/Results#energyresolution_241).

### Resolution1D.C
First load the macro:
```
$ .L Resolution1D.C
```
You can then access the functions: 
```
$ AllResolution(TString PeakToFit, // Peak to consider
                Double_t MaxRes)   // Maximum resolution (%) to use in the plots
```
This function will produce 18 figures (one for each detector) of the energy resolution for the <sup>241</sup>Am peak.
The first argument (for example `241Am`) will select which file to consider, and will also produce proper labels for the images.  
The second argument is the maximum resolution to use in the plot (end scale value y-scale), which has been actually overwritten: it is better to find the maximum resolution for each detector, or for all the set, and choose the value accordingly. You may want to chenge the maximum resolution in the macro based on your needs.
Be sure tho have all the files present in the folder where the macro is saved.  

```
$ void ResolutionHistogram(TString FrontFile_Name, // Front file
                           TString BackFile_Name,  // Back File
                           TString PeakToFit,      // Peak to consider
                           Int_t DetectorNo,       // Detector Number
                           Double_t MaxRes)        // Maximum resolution (%) to use in the plots
```
This function is similar to the one before, but will produce just a single plot, of the desired detector: in fact the previous function loops on this one, for all detectors.

### Resolution2D.C
First load the macro:
```
$ .L Resolution2D.C
```
You can then access the functions: 
```
$ void Resolution2D(TString PeakToFit, // Peak to consider
                    Double_t MaxRes)   // Maximum resolution (%) to use in the plots
```
This function produce a 2D Poly histogram of all detectors.
The first argumnet (for example `241Am`) will select which files to open and will produce proper labels.
The second argument is the maximum resolution to use in the plot in % (for example `4`). This value actually sets the z-axis end scale value, differently then before, which was overwritten. It makes sense to use the value `MaxRes=4` in this case, since it is actually the higher resolution value in this detector set, anyway for better visual output, I used the max value of `3`, and log scale for the z-axis. In the end this graph is meant to give a quick overview of all detectors performances, and does not have to be very detailed.  

### SpatialResolution2D.C
First load the macro:
```
$ .L SpatialResolution2D.C
```
You can then access the functions: 
```
$ void AllSpatialResolution(TString PeakToFit, // Peak to consider
                            Double_t MaxRes)   // Maximum resolution (%) to use in the plots
```
This function will produce 18 figures (one for each detector) of the spatial energy resolution for the <sup>241</sup>Am peak. The spatial resolution is the resolution of the detector, as a function of the position of the impinging particle on the detector surface.
The first argument (for example `241Am`) will select which file to consider, and will also produce proper labels for the images.  
The second argument is the maximum resolution to use in the plot (end scale value z-scale), which has been actually overwritten: it is better to find the maximum resolution for each detector, or for all the set, and choose the value accordingly. You may want to chenge the maximum resolution in the macro based on your needs.
  
```
$ void SpatialResolutionHistogram(TString FrontFile_Names, // Front file
                                  TString BackFile_Names,  // Back File
                                  TString PeakToFit,       // Peak to consider
                                  Int_t DetectorNo,        // Detector Number
                                  Double_t MaxRes)         // Maximum resolution (%) to use in the plots
```
This function is similar to the one before, but will produce just a single plot, of the desired detector: in fact the previous function loops on this one, for all detectors.
