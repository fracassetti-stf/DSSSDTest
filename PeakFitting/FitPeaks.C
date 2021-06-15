//   This fitting program was written by Oleg Ivanov
//   Modified by Stefano Fracassetti

//      How to use: (out of date)
//   1. Load the program in root by typing .L FitPeaks.C - the code will be compiled and all the functions loaded to memory
//   2. Call GetFitPeaks() to see the list of fitting functions available
//   3. Call a function of choice with appropriate variables
//      In case of questions or comments please call me at 016 32 72 73


#include "TH1F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGClient.h"
#include "TF1.h"
#include <iostream>
#include "TMarker.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <TQObject.h>

TFile* InputFile = new TFile("DSSD_3178-11_Front.root");

Double_t gaus_lbg(Double_t *x, Double_t *par);
void FitSinglePeakInteractively(TH1F *hist);


int ica;
string in;
void terminal_input(Int_t event, Int_t x, Int_t y, TObject *selected);

Double_t gBgConstant, gBgSlope, gContent, gMean, gSigma, gBinW, gChi2pNDF;




////////////////////////////////////////////////////////////////////////////////
///// Ouput: File and Tree
////////////////////////////////////////////////////////////////////////////////

Double_t LowerX, UpperX, Mean, FWHM, Resolution;

//TTree* OutputTree = new TTree("OutputTree", "Peak Fitting");
//OutputTree->Branch("LowX", &LowerX,     "LowX/D");
//OutputTree->Branch("UpX" , &UpperX,     "UpX/D" );
//OutputTree->Branch("Mean", &Mean,       "Mean/D");
//OutputTree->Branch("FWHM", &FWHM,       "FWHM/D");
//OutputTree->Branch("Res" , &Resolution, "Resolution/D");

//TFile* OutputFile = new TFile();

//_____________________________________________________
//_____________________________________________________


// FitStripsInteractively()
//
// This function allowes to fit interactively each strip of a DSSSD side.
// You should call this function after opening the root converted file
//
// The function takes no parameters, but loop on every strip of a certain DSSSD side.
//
// The Fit will be done interactively, and taken care by FitPeakInteractively(hist)

void FitStripsInteractively() {//TString OutputFile_Name) {
  Int_t strip;
  for(strip=0;strip<32;strip++) {
    
    TString HistStrip_Name = "h";
    HistStrip_Name.Append(char(strip));
    TH1F* HistStrip = (TH1F*)InputFile->Get(HistStrip_Name);
    
    FitSinglePeakInteractively(HistStrip);
  }
}


//_____________________________________________________
//_____________________________________________________

 
// FitSinglePeakInteractively()
//
// This function will fit a peak, and output the peak parameters.
//
// The function takes only the histogram name, and let the user select the extremes of the fitting region.
// Selecting the x-region to fit is done interactivel clicking with the mouse on the x-axis.
//
// The output will be visible on terminal, and some of the parameters recorde on file.

void FitSinglePeakInteractively(TH1F *hist) {


      
  ////////// Creating the function of the form 'gaus_lbg' defined in gaus_lbg
  TF1 fitfunc("gauss_linbg",gaus_lbg, 0, 1, 5);
  fitfunc.SetParName(0,"BgConstant");
  fitfunc.SetParName(1,"BgSlope   ");
  fitfunc.SetParName(2,"Sigma     ");
  fitfunc.SetParName(3,"Content   ");
  fitfunc.SetParName(4,"Mean      ");

  ////////// Select extremes interactively from Canvas
  TCanvas* Canvas = new TCanvas("Canvas","Fit Canvas",1200,900); 
  Double_t gLowX= 19100, gUpX = 19260;
  
  hist->Draw();
  
  
  ////////// Suspend the program
  Canvas->Modified();
  Canvas->Update();
      
  ica=0;
  cout << "e for another event, q for exit" << endl;

  //connette il canvas a un evento grafico (non chiarissimo come funziona)
  Canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
		  "terminal_input(Int_t,Int_t,Int_t,TObject*)");
  
  while(ica!=1){
    usleep(100);
    gClient->HandleInput();//fondamentale, se no non funziona
  }
  
  Canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");


  

  


  ////////////////////////////////////////////////////////////////////////////////
  ///// Fitting the Peak
  ////////////////////////////////////////////////////////////////////////////////
  
  if(gLowX < gUpX)
    {
      
      ///// Obtaining and specifying the start values for the fit
      
      gContent = hist->Integral(hist->FindBin(gLowX),hist->FindBin(gUpX)); 
      gMean    = 0.5 * ( gLowX + gUpX);  
      gSigma   = 0.3 * ( gUpX  - gLowX); 
      gBinW = hist->GetBinWidth(1);
      
      //   printf("__________________");
      //   printf("_The Start Values_");
      //   printf("Bin Width: %d\n Mean Value: %d\n    Content: %d\n      Sigma: %d",gBinW,gMean,gContent,gSigma);
      //   printf("__________________");
      
      fitfunc.SetParameters(0, 0, gSigma, gContent, gMean); 
      fitfunc.SetRange(gLowX, gUpX);

      ///// Fitting: 'R' means within the range specified above
      
      hist->Fit("gauss_linbg", "R", "SAME");

      gBgConstant = fitfunc.GetParameter(0);
      gBgSlope    = fitfunc.GetParameter(1);
      gSigma      = fitfunc.GetParameter(2);
      gContent    = fitfunc.GetParameter(3);
      gMean       = fitfunc.GetParameter(4);
      gChi2pNDF   = fitfunc.GetChisquare() / fitfunc.GetNDF();

      printf("      Chi Square: %f\n",fitfunc.GetChisquare());
      printf("            FWHM: %f +- %f\n",2*gSigma*sqrt(2*log(2)),2*sqrt(2*log(2))*fitfunc.GetParError(2));


      ////////////////////////////////////////////////////////////////////////////////
      ///// Draw the fitting function, and ask for satisfaction
      ////////////////////////////////////////////////////////////////////////////////

      
      

      ////////////////////////////////////////////////////////////////////////////////
      ///// Store the value in a TTree
      ////////////////////////////////////////////////////////////////////////////////
     
      
    } // end if
  
  ///// Inconsistent limit
  else std::cout << "Couldn't fit! Error: The Lower Limit is larger than the Upper Limit!" << std::endl;
  
}


//_____________________________________________________
//_____________________________________________________

// gaus_lbg()
//
// This is a Guassian function with a linear background
//
// The function takes the x-value, and the gaussian parameters.
//   par[0] background constant
//   par[1] background slope
//   par[2] gauss width
//   par[3] gauss0 constant
//   par[4] gauss0 mean
//
// The function evaluates the gaussian in the x-value given, and return the corresponding y-value.

Double_t gaus_lbg(Double_t *x, Double_t *par)
{
  
  ////////////////////////////////////////////////////////////////////////////////
  ///// Define some constants, and initial check
  ////////////////////////////////////////////////////////////////////////////////
  
  static Float_t
    sqrt2pi = TMath::Sqrt(2*TMath::Pi()),
    sqrt2 = TMath::Sqrt(2.);
   

  // Force width to be greater then zero
  if (par[2] <= 0) {
    par[2]=1;
  }
  ////////////////////////////////////////////////////////////////////////////////
  ///// Evaluate the Gaussian
  ////////////////////////////////////////////////////////////////////////////////
  
  // Argument of the exponential
  Double_t arg = (x[0] - par[4])/(sqrt2*par[2]);
  // Calculate linear background: (bg + lbg)
  Double_t lbg = par[0] + x[0]*par[1];
  // Calculate gaussian:
  Double_t gausval = gBinW/(sqrt2pi*par[2]) * par[3] * exp(-arg*arg);
   
   return lbg + gausval;
}




//_____________________________________________________
//_____________________________________________________


void terminal_input(Int_t event, Int_t x, Int_t y, TObject *selected) {


  if(event==24){ //evento da tastiera su TPad

    //gPad->GetEventX()==99 // c

    if(gPad->GetEventX() == 101) { // e
      in = "e";
      ica=1;
         TCanvas *c = (TCanvas *) gTQSender;
   printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
          event, x, y, selected->IsA()->GetName());
      return;
    }
    if(gPad->GetEventX() == 113) { // q
      in = "q";
      ica=1;
      return;
    }
    if(gPad->GetEventX() == 110) { // n
      in = "n";
      ica=1;
      return;
    }
  }
   
}
/*
    // Insert Point when in Terminal Input

   if(event==2) { //input da mouse ??? controllare
     float px=gPad->AbsPixeltoX(x);
     float py=gPad->AbsPixeltoY(y);
     py=gPad->PadtoY(py);
     float uymin=gPad->GetUymin();
     float uymax=gPad->GetUymax();
     
     if(px>=gPad->GetUxmin() && px<=gPad->GetUxmax() && py>=gPad->PadtoY(uymin) && py<=gPad->PadtoY(uymax)) {
       TMarker* m = new TMarker(px,py,3);
       m->SetMarkerColor(2);
       m->SetMarkerSize(3);
       m->Draw();
       
	 //for(int j=0;j<np;j++) {
	 //m[j]->Draw();
	 //}
       
       gPad->Modified();
       gPad->Update();
	 //np++;
        }
     }
  
}
*/
