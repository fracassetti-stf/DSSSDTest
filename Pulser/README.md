# Pulser
`ConfigFiles/` collects the pulser configuration files,  
and the file `DAQ_MPR_test.txt` has been created during a test with the pulser (details below).

## No allignment of Pulser Peaks
`DAQ_MPR_test.txt` is a file created during the investigation of the fact that the pulser peaks were not alligned in all the strips. 
Knowing the position of each alpha peak, it's possible to calibrate the spectra, so that one expects to find the pulser at the same energy.  
The pulser is given in input at the MPR-64, and then divided internally to all channels.  


At first, we could not understand why the pulser peaks were not positioned at the same energy, being the pulser divided on all strips starting from the same signal.
We excluded other reasons, and we eventually try to take the same measurements with another MPR-64, 
and this time the pulser positioned were changed differently, confirming that the pulser shift are due to the MPR-64 itself.
We could also verify that in the datasheet is reported that the pulser signal is divided internally on every strip, 
but there may be up to a 10% uncertainty in the pulser channel/energy.
