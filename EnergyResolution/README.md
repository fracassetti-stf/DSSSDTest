# Energy Resolution Macro
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
