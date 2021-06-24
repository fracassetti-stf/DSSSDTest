To fit the peak just type as such

.L FitPeaks.C
FitStripInteractively("DSSD_3178-2_Front_80V.root", "241Am")


The output file will be as such : "####_241Am.root"
The tree file will be named as Tree_241Am

Changing the second strip simply changes the file name and the root tree name.


////////////////////////////////////////////////////////////////////////////////

To produce pdf images:

.L Resolution.C
AllResolution("241Am")

The 241Am will select which file to consider,
and also produce proper labels for the images.

Notice that all the file must be present for the macro to produce them.
I don't know what happens if a file is missing.


ResolutionHistogram("DSSD_3178-2_Front_80V_241Am.root",
                         "DSSD_3178-2_Back_80V_241Am.root",
                         "241Am",
                         1
                         MaxRes) // det number

This will produce only the relative pics.
241Am and detno only used for proper labels.
241Am also to open correct tree.


##################
Le macro piu aggiornate sono qui!!!!
ora sono functionanti.
pero devi fare qualcosa con le strip broken e impossibili da fittare,
come vuoi distinguerle sul grafico?

per il resto tutto ok,
 continua a produrre fit di 241Am (tutti tranne file 1 che ho fatto venerdi
 sera).
 e ti conviene fare singole risoluzioni, con funzione una alla volta.
