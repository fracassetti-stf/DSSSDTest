# Peak Fitting
The macro present here allows to fit a peak form a spectra (histogram).  
In particular, a Root macro has been written which allows to fit a (Gaussian with constant background) peak interactively - with this I mean that the extreme of the peak can be passed to the program directly clicking on the canvas itself, and the user has different option, such as confirming the fit or re-try to fit with other extremes. I was motivated by the fact that the task was to extract energy resolution of 18 Detectors, each one having 64 strips to analyse: I needed something very fast, and reliable at the same time (visual feedback) - in this way the fit of each Root file takes just a few minutes.  
This program was written starting from another program FitPeaks.C, a program which allows to fit a peak giving on the command line the extrems of the fit. This program also offer different type of functions to use as a fit.




# Marco: how to use (to be updated)
To be updated, I am not sure these info are still valid.  

## FitPeaksInteractively.C
The program prompt the first spectra, the user can zoom on a peak of interest, and click (mouse whell) on the etreme of the peak - a black line will be drawn. Eventually, the fit curve is plotted on the canvas in the range selected. For better visualization also the fit Background, Mean and FWHM are drawn. At this point the used can decide whether the fit is acceptable - the fit must be done again - or other options. For my purpuse other two options were implemented: broken strip (no spectra available), and fit not possible (for the strange shape of the peak, or the very high background). Based on the user feedback, the program record the fit parameters, and prompt the next spectrum to fit.
To fit the peak just type as such

```
.L FitPeaksInteractively.C  
FitStripInteractively("DSSD_3178-2_Front_80V.root", // File to Fit
                      "241Am") // Peak to Fit
```

The output file will be as such : "XXX_241Am.root"  
The tree file will be named as Tree_241Am  
Changing the second argument ("241Am") simply changes the file name and the root tree name.  
