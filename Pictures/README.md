# Pictures
This folder contains the results of this analysis. Note that each plot is saved both in `.pdf` and `.png`.

## Characterization

### DepletionVoltage
Plots representing the channel corresponing of the <sup>239</sup>Pu peak, in relation to the bias voltage applied.  
- `AllDSSSD_PeakChannel.pdf/.png`: All detectors on the same plot, this gives a clear overview of all detector. 
A grey scale has been choosen for the old detectors, adn a red one for the new ones.
- `OldDSSSD_PeakChannel.pdf/.png`: Only the old detectors are plotted. Colors allows to easily distinguish the detectors.
- `NewDSSSD_PeakChannel.pdf/.png`: Only the new detectors are plotted. Colors allows to easily distinguish the detectors.

### LeakageCurrent
Leakage current as a function of the bias voltage applied.  
- `AllDSSSD_Current.pdf/.png`: All detectors on the same plot, this gives a clear overview of all detector. 
A grey scale has been choosen for the old detectors, and a red one for the new ones.
- `OldDSSSD_CurrentLog.pdf/.png`: Only the old detectors are plotted. 
Colors allows to easily distinguish the detectors. 
For a better comparison with the data sheet, the y-axis is drawn in log10 scale.
- `NewDSSSD_CurrentLog.pdf/.png`: Only the new detectors are plotted. 
Colors allows to easily distinguish the detectors. 
For a better comparison with the data sheet, the y-axis is drawn in log10 scale.

## EnergyResolution_241
This are the plots for the energy resolution strip-by-strip for all detectors. The resolution was extracted fitting the <sup>241</sup>Am peak.  

### How to read the data? 
- `Broken Strrip`: when a strip is broken (basically no spectum available), the corresponding bar has been laft empty.
- `High Background`: in case it was not possible to fit a peak for high bg, distorted peak (not gaussian, high low-energy tail) 
then the value of the resolution has been put on end-scale for 2D histogram, and also indicated in red in case of 1D.  

The plots have been drawn using two y-scale for the resolution: 
- `FixForAll`: the same y-scale has been used for all detectors. 
This allow a fair comprison of all detectors.
- `DifferentForEachOne`: a different y-scale has been chosen to represent the data of each detector. 
This allow to better visualize difference between strips of a single detector.

### Resolution1D
### Resolution2D
### 

## Spectra
Some example of the spectra acquired, for some of the detectors.
