// 30 June 2021
// Stefano Fracassetti @KU Leuven
// stefano.fracassetti@kuleuven.be


////////////////////////////////////////////////////////////////////////////////
////////// DESCRIPTION
////////////////////////////////////////////////////////////////////////////////

The program conversion is used to convert lmd files produced with Febex
acquisition system.
The output of this program will be a Root file containing 32 histogram:
h0,....,h31 each one containing the spectra of the relative channel acquired.

////////////////////////////////////////////////////////////////////////////////
////////// COMPILATION
////////////////////////////////////////////////////////////////////////////////

The Makefile is used to create the executable:
conversion.hh and RootLinkDef.h are needed for the compilation.

The file "filenames" must be filled with:
1 // first line
<lmd file name to convert into root> // second line


////////////////////////////////////////////////////////////////////////////////
////////// MODIFICATION OF THE ORIGINAL ONE
////////////////////////////////////////////////////////////////////////////////


////////// CONVERSION FRONT AND BACK

For our purpoe (DSSSD test @Ku Leuven) we modified slightly the program:
In our setup a permutation in the channel occurred internally after the signal
reached the 2-input 32-channel card, so that the strip were not acquired in the
corret order. It was necessary to implement : Conversion_Front and
Conversion_Back in order to acquired the signals in the proper order.
The only difference with the original program is that there is a permutation
that swap how the histogram, saving them in the correct order.
At this point we can be sure that h0 correspond to first channel and so on.
The permutation differ from Front and Back because of the pin mapping of our
feedthroughs, (called Front and Back) in particular the pin-mapping of the Back
one is reversed in respect to the Front one, which is order normally instead.


////////// CONVERSION REBIN 5

This program differ only for the fact that the histogram have a lower number of
bin, exactly 5 time less than the original one: not 25000, but 5000.
This was a good choice for our case, since the bin were clearly too thin for the
statistics we had acquired.
