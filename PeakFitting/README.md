# PeakFitting
This folder collect the marco, and the ouptup files of the peak-fitting procedure.  
- `FitPeaks_241Am/`: Folder containing the output files regarding the fit of the <sup>241</sup>Am peaks. Each file contains a single root tree for 32 entries (one for each strip), with relative peak parameters.
- `FitPeaks.C` and `FitPeaksInteractively.C` are macros that allows to fit a peak form a spectra (histogram).  
  - `FitPeaks.C` has been written by Oleg Ivanov, it performs the fit of a peak giving through the command line the extrems of the fit. This program also offer different type of functions to use as a fit (gaussian with linear bg, gaussian with linear bg, double guassian with bg, etc).
  - `FitPeaksInteractively.C` has been written by Stefano Fracassetti, starting from the program `FitPeaks.C`, and drastically modified to make it interactive. It performs the fit of a peak (Gaussian with constant background) interactively - with this I mean that the extreme of the peak can be passed to the program directly clicking on the canvas itself, and the user has different option, such as confirming the fit or re-try to fit with other extremes. I was motivated by the fact that the task was to extract energy resolution of 18 Detectors, each one having 64 strips to analyse: I needed something very fast, and reliable at the same time (visual feedback) - in this way the fit of each Root file takes just a few minutes. 
- `FilenamesForConvenience.txt` is just a simple txt file which contains all detectors filenames. It was convenient to have them all listed indeed, so that I did not have to search for them each time. This also eplains the name of the file itself.


## Root Marco: How to use?

### FitPeaks.C
A brief description of the file can be found in the file header. After loading the macro, the function GetFitPeaks() can be called to have some a brief description on the functionalities available here.

### FitPeaksInteractively.C
The program prompt the first spectra, the user can zoom on a peak of interest, and click (mouse whell) on the etreme of the peak - a black line will be drawn. Eventually, the fit curve is plotted on the canvas in the range selected. For better visualization also the fit Background, Mean and FWHM are drawn. At this point the used can decide whether the fit is acceptable - the fit must be done again - or other options. For my purpuse other two options were implemented: broken strip (no spectra available), and fit not possible (for the non-gaussian shape of the peak, or the very high background). Based on the user feedback, the program record the fit parameters, and prompt the next spectrum to fit.
To fit the peak just type as such

```
$ .L FitPeaksInteractively.C  
$ FitStripInteractively("DSSD_3178-2_Front_80V.root", // File containing the histograms to fit
                        "241Am")                      // Name of the Peak to fit
```

- The output filename will be similar to the original one: the original extension removed, and replaced by `_241Am.root`.
- The tree file will be named as `Tree_241Am`.  

Changing the second argument ("241Am" in this case) simply determine the file name and the tree name, as described above. This second argument is useful in the case in which one wants to fit more than one peak from the same histograms, and needs to distinguish them. In this analysis only <sup>241</sup>Am has been fit.  

When the program is launched, it will immediately prompt the first histogram (h0). At this point it is convenient to first zoom on the regioin of interest, and then select the extreme of the peak to fit: lower limit, and upper limit, in this order (if not the program will ask you again). The program will show to limit selected in black. When both will be selected, the fit function will be plotted (glbg = gaussian with constant background), as well the mean value and FWHM of the gaussian (in red), and the background value (in blue).  

Having plotted this information, and the value printed at terminal, the user has different possibilities (pressing a keyboard letter - read terminal input for more info):
- Accept the fit: the fit parameters will be stored in the tree as a new entry. The program will prompt the new spectrum to fit.
- Reject the fit: the fit will be discarded, and the program will prompt the selection of new extremes.
- Strip is Broken: a flag value will be stored in the tree (-2), and move to the next spectrum.
- Fit not possible: a flag value will be stored in the tree (-1), and move to the next spectrum.

Note that, once an option is selected (all apart "Reject the fit"), is not possible to come back to the same spectrum later. The program must be run again.
This will repeat up to the last spectum (h31), after that the program will terminate, and the tree stored in the corresponding file. For every tree entry the strip number, the extremes of the fit, the mean value, the FWHM the resolution(%) will be stored.
