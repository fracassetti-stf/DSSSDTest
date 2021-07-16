# EnergyResolution
These macro allows the visualize the energy resolution, for each detectors, as well as to compare all detectors at once.  
Histograms (1D and 2D) have been produced in order to have different (visual) output:
- `Resolution1D.C`: Energy Resolution 1D-Histograms of all strip for each detectors - one figure for each detector, 
- `Resolution2D.C`: Energy Resolution 2D-Histogram of all strip for all detectors - one figure for all detectors, 
- `SpatialResolution2D.C`: Spatial Energy Resolution 2D-histogram for each detectors - one figure for each detector.  

These maros aventaully have not been used since other marcos have been written, which are better for the purpose:
- `GraphResolution1D.C` (replaced by `Resolution1D.C`): Energy Resolution 1D-Graph of all strip for each detectors - one figure for each detector.
- `GraphResolution2D.C` (replaced by `Resolution2D.C`): Energy Resolution 2D-Graph of all strip for all detectors  - one figure for all detectors.


Note: "FixedForAll" and "DifferentForEachOne" in the filenames refers to the scale used: 
the first one allows a fair comparison of different detectors, the second allows a clear picture of the resolution of a specific detector.



## Root Macro: How to use? (to be updated)

To produce pdf images:
```
.L Resolution.C  
AllResolution("241Am")
```

The 241Am will select which file to consider, and also produce proper labels for the images.  
Notice that all the file must be present for the macro to produce them. I don't know what happens if a file is missing.  

```
ResolutionHistogram("DSSD_3178-2_Front_80V_241Am.root",  // Front File Name
                    "DSSD_3178-2_Back_80V_241Am.root",   // Back File Name
                    "241Am",  // Peak to Fit
                    1  // Detector Number
                    MaxRes) // Maz Resolution to be used in plot scale
```

This will produce only the relative pics.  
241Am and detno only used for proper labels.  
241Am also to open correct tree.  
