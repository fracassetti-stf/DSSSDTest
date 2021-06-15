
#include "TF1.h"

  void plot015(){
    
   TCanvas *c1 = new TCanvas("c1", "c1",2100,1500);
   char canv[3];
   int i;
   int perm[]={6, 5, 4, 3, 2, 1, 0, 7, 8, 15, 14, 13, 12, 11, 10, 9};
   
   c1->Divide(4,4);
   gPad->Update();
   c1->cd(1);
   
   for (i=0;i<16;i++){
     c1->cd(i+1);  
     sprintf(canv,"h%d" , perm[i]);
     TH1F *h = (TH1F*) gDirectory->Get(canv);
     h->Draw();
   }

     
}
