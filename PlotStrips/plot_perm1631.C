
#include "TF1.h"

  void plot1631(){
    
   TCanvas *c1 = new TCanvas("c1", "c1",2100,1500);
   char canv[3];
   int i;
   int perm[]={6, 5, 4, 3, 2, 1, 0, 7, 8, 15, 14, 13, 12, 11, 10, 9};

   c1->Divide(4,4);
   gPad->Update();
   c1->cd(1);
   
   for (i=16;i<32;i++){
     c1->cd(i-15);  
     sprintf(canv,"h%d" ,perm[i-16]+16);
     TH1F *h = (TH1F*) gDirectory->Get(canv);
     //c1->cd(i);  
     h->Draw();
     //gPad->Update();
   }

     
}
