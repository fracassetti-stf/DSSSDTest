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
#include "TLine.h"
#include "TSystem.h"
#include <TQObject.h>


////////// Function definitions
// Gaussian function (with linear background)
Double_t gaus_lbg(Double_t *x, Double_t *par);
// Fitting a single peak interactively
void FitSinglePeakInteractively(TH1F *hist);
// Terminal input mode for choosing an extreme of the peak
void terminal_input_extremes(Int_t event, Int_t x, Int_t y, TObject *selected);
// Terminal input mode for confirming/rejecting the fit
void terminal_input_check(Int_t event, Int_t x, Int_t y, TObject *selected);
// Clear the canvas in order to attempt another fit
void ClearTheCanvas(TH1F *hist);


///////// Help variables
int ica; // use to terminate terminal input mode: 0 loop, 1 exit
string in; // string value of the pressed value in terminal input
Bool_t ExtremeIsValid=false; // true when an extreme value is valid
Bool_t FitIsGood=false; // true when the fit is good, false if fit needs to be repeated
Bool_t StripIsBroken=false; // true if the fit is broken, and there is nothing to fit


// Function parameters
Double_t gBgConstant, gBgSlope, gContent, gMean, gSigma, gFWHM, gMax, gBinW, gChi2pNDF;
// Extreme values of the current fit
Double_t gUpX, gLowX;
// Temporary value stored when clicking on gPad
Double_t px, py, uymin, uymax;
// Value of the resolution to store in TTree
Double_t Resolution;
// Strip Number
Int_t StripNo;


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

void FitStripsInteractively(TString InputFile_Name, TString OutputFile_Name)
{

  // Output Tree
  TTree* OutputTree = new TTree("Am241Tree", "Peak Fitting for 241Am");
  // Prepare the Branches
  OutputTree->Branch("Strip", &StripNo,    "Strip/I");
  OutputTree->Branch("LowX",  &gLowX,      "LowX/D");
  OutputTree->Branch("UpX" ,  &gUpX,       "UpX/D" );
  OutputTree->Branch("Mean",  &gMean,      "Mean/D");
  OutputTree->Branch("FWHM",  &gFWHM,      "FWHM/D");
  OutputTree->Branch("Resolution" ,  &Resolution, "Resolution/D");

  // Output File
  TFile* OutputFile = new TFile(OutputFile_Name, "RECREATE");

  // Input file:
  // The file should contain multiple histograms: h0,....,h31
  TFile* InputFile = new TFile(InputFile_Name);


  // Loop on every strip
  for(StripNo=1;StripNo<=32;StripNo++) {
    TString HistStrip_Name = "h";
    HistStrip_Name.Append(Form("%d",StripNo-1));
    TH1F* HistStrip = (TH1F*)InputFile->Get(HistStrip_Name);
    printf("This is the name of the histogram %s\n",HistStrip_Name.Data());

    FitSinglePeakInteractively(HistStrip);
    OutputTree->Fill();
  }

  std::cout << "Printing the Full Tree" << std::endl;
  OutputTree->Print();

  OutputFile->cd();
  OutputTree->Write();
  OutputFile->Write();
  OutputFile->Close();

}


//_____________________________________________________
//_____________________________________________________


// FitSinglePeakInteractively()
//
// This function will fit a peak, and output the peak parameters.
//
// The function takes only the histogram name,
// and let the user select the extremes of the fitting region.
// Selecting the x-region to fit is done interactively
// clicking with the mouse wheel on the x-axis.
//
// The output will be visible on terminal,
// and the parameters recorde on file.


void FitSinglePeakInteractively(TH1F *hist)
{

  // Use this canvas to draw the  histogram
  TCanvas* Canvas = new TCanvas("Canvas","Fit Canvas",2800,800);


  ////////// Creating the function of the form 'gaus_lbg' defined in gaus_lbg
  TF1 fitfunc("gaus_linbg",gaus_lbg, 0, 1, 5);
  // Assigning names to the function parameters
  fitfunc.SetParName(0,"BgConstant");
  fitfunc.SetParName(1,"BgSlope   ");
  fitfunc.SetParName(2,"Sigma     ");
  fitfunc.SetParName(3,"Content   ");
  fitfunc.SetParName(4,"Mean      ");
  hist->Draw();


  // Loop until user is satisfied with the fit
  FitIsGood=false;
  while(FitIsGood!=true) {


    ////////////////////////////////////////////////////////////////////////////////
    /////  Terminal Input: Select Lower Extreme of the Peak
    ////////////////////////////////////////////////////////////////////////////////


    ExtremeIsValid=false;
    while(ExtremeIsValid!=true) {
      std::cout << "Click on LOWER EXTREME of the peak" << std::endl;
      Canvas->Modified();
      Canvas->Update();
      ica=0;

      //Connect Canvas to Graphic Event
      Canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
                      "terminal_input_extremes(Int_t,Int_t,Int_t,TObject*)");
      while(ica!=1){
        usleep(100);
        gClient->HandleInput();
      }
      Canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
    }

    gLowX=px;


    ////////////////////////////////////////////////////////////////////////////////
    /////  Terminal Input: Select Upper Extreme of the Peak
    ////////////////////////////////////////////////////////////////////////////////


    ExtremeIsValid=false;
    while(ExtremeIsValid!=true) {
      std::cout << "Click on UPPER EXTREME of the peak" << std::endl;

      Canvas->Modified();
      Canvas->Update();
      ica=0;

      //Connect Canvas to Graphic Event
      Canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
                      "terminal_input_extremes(Int_t,Int_t,Int_t,TObject*)");
      while(ica!=1){
        usleep(100);
        gClient->HandleInput();
      }
      Canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
    }

    gUpX=px;


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

      ////////// Fitting: 'R' means within the range specified above

      hist->Fit("gaus_linbg", "R", "SAME");
      TF1 *fitting_function = hist->GetFunction("gaus_linbg");
      fitting_function->SetLineColor(2);
      fitting_function->SetLineWidth(3);

      gBgConstant = fitfunc.GetParameter(0);
      gBgSlope    = fitfunc.GetParameter(1);
      gSigma      = fitfunc.GetParameter(2);
      gContent    = fitfunc.GetParameter(3);
      gMean       = fitfunc.GetParameter(4);
      gChi2pNDF   = fitfunc.GetChisquare() / fitfunc.GetNDF();
      gMax        = gaus_lbg(&gMean,fitfunc.GetParameters());
      gFWHM       = 2*gSigma*sqrt(2*log(2));
      Resolution = (gFWHM/gMean)*100;

      printf("      Chi Square: %f\n",fitfunc.GetChisquare());
      printf("            FWHM: %f +- %f\n",gFWHM,2*sqrt(2*log(2))*fitfunc.GetParError(2));


      // TPad current_pad = (TVirtualPad)gROOT->GetSelectedPad()
      TLine *Mean_line = new TLine(gMean,0,gMean,gMax);
      Mean_line->SetVertical(true);
      Mean_line->SetLineColor(2);
      Mean_line->SetLineWidth(3);
      Mean_line->Draw();

      TLine *FWHM_line = new TLine(gMean-gFWHM/2,gMax/2,gMean+gFWHM/2,gMax/2);
      FWHM_line->SetHorizontal(true);
      FWHM_line->SetLineColor(2);
      FWHM_line->SetLineWidth(3);
      FWHM_line->Draw();



      gPad->Modified();
      gPad->Update();
    } // end if


    ////////// Inconsistent limit
    else {
      std::cout << "Couldn't fit! Error: The Lower Limit is larger than the Upper Limit!" << std::endl;
      std::cout << "Try again a to select new limits:" << std::endl;
      FitIsGood=false;
      ClearTheCanvas(hist);
      continue;
    }


    //////////////////////////////////////////////////////////////////////////////
    ////////// Terminal Input: User-check of the fit: confirm or reject
    //////////////////////////////////////////////////////////////////////////////


    std::cout << "Confirm or Reject the Fit?" << std::endl;
    std::cout << "Press \"e\" to confirm, \"q\" to reject, and \"c\" if strip is broken" << std::endl;

    Canvas->Modified();
    Canvas->Update();
    ica=0;

    //Connect Canvas to Graphic Event
    Canvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
                    "terminal_input_check(Int_t,Int_t,Int_t,TObject*)");
    while(ica!=1){
      usleep(100);
      gClient->HandleInput();
    }
    Canvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");

    // Remove previous fit
    ClearTheCanvas(hist);

    ////////// If the strip is broken the fit has no meaning, save -1 value as flag.
    if(StripIsBroken) {
      gLowX = -1;
      gUpX  = -1;
      gMean = -1;
      gFWHM = -1;
      Resolution = -1;
    }
  } // end While(FitIsGood)


  //////////////////////////////////////////////////////////////////////////////
  ///// Clear Canvas for new fit
  //////////////////////////////////////////////////////////////////////////////


  if (Canvas) { Canvas->Close(); gSystem->ProcessEvents(); }

} // end FitPeakInteractively()


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
// The function evaluates the gaussian in the x-value given,
// and return the corresponding y-value.


Double_t gaus_lbg(Double_t *x, Double_t *par)
{

  // Define some constants
  static Float_t
      sqrt2pi = TMath::Sqrt(2*TMath::Pi()),
      sqrt2 = TMath::Sqrt(2.);
  // Force width to be greater then zero
  if (par[2] <= 0) {
    par[2]=1;
  }

  ////////// Evaluate the Gaussian

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


// terminal_input_check()
//
// This function acquire a pressed key by the user,
// and accept[e], reject[q] or consider the strip as broken(i.e. no peak at all) [c].
// Note that the key must be pressed while the mouse is on the pad to work properly.


void terminal_input_check(Int_t event, Int_t x, Int_t y, TObject *selected)
{

  if(event==24){ //evento da tastiera su TPad

    if(gPad->GetEventX()==99) { // c
      std::cout << "Strip is broken: There is no peak to fit" << std::endl;
      std::cout << "The TTree will store with all value set to \"-1\"" << std::endl;
      std::cout << "Moving to next histogram to fit, if any" << std::endl;
      FitIsGood=true; // true, since you want to step to next fit
      StripIsBroken=true; // true, so values will be saved differently
      in = "c";
      ica=1;
      return;
    }

    if(gPad->GetEventX() == 101) { // e
      std::cout << "Fit accepted" << std::endl;
      std::cout << "Moving to next histogram to fit, if any" << std::endl;
      FitIsGood=true;
      StripIsBroken=false;
      in = "e";
      ica=1;
      return;
    }

    if(gPad->GetEventX() == 113) { // q
      std::cout << "Fit unacceptable: Repeating the fit" << std::endl;
      FitIsGood=false;
      StripIsBroken=false;
      in = "q";
      ica=1;
      return;
    }
  } // end TPad event

} // end terminal_input_check()


//_____________________________________________________
//_____________________________________________________


// terminal_input_extremes()
//
// This function acquire the x (and y) value pressed using the mouse wheel on the pad.
// The value is then stored and used as extreme of the peak to fit.
// Marker and Line are drawn in the click position.
// It also checks whether the click happen in a valid area (i.e. inside the pad),
// In the opposite case a flag variable is activated.


void terminal_input_extremes(Int_t event, Int_t x, Int_t y, TObject *selected)
{

  if(event==2){ // mouse wheel press
    px=gPad->AbsPixeltoX(x);
    py=gPad->AbsPixeltoY(y);
    py=gPad->PadtoY(py);
    uymin=gPad->GetUymin();
    uymax=gPad->GetUymax();

    // Check if extreme is in pad
    if(px>=gPad->GetUxmin() && px<=gPad->GetUxmax() && py>=gPad->PadtoY(uymin) && py<=gPad->PadtoY(uymax)) {
      ExtremeIsValid=true;

      // Draw Marker
      TMarker* m = new TMarker(px,py,3);
      m->SetMarkerColor(1);
      m->SetMarkerSize(3);
      m->Draw();

      //Draw Vertical Line
      TLine *l = new TLine(px,gPad->GetUymin(),px,gPad->GetUymax());
      l->SetVertical(true);
      l->SetLineColor(1);
      l->SetLineWidth(3);
      l->Draw();

      gPad->Modified();
      gPad->Update();
    }

    else {
      ExtremeIsValid=false;
      std::cout << "The value selected is not valid!" << std::endl;
      std::cout << "Please, try again" << std::endl;
    }

    ica=1;

  } // event if mouse wheel pressed

} // end terminal_input_extremes()


//_____________________________________________________
//_____________________________________________________


// ClearTheCanvas()
//
// This function is used to clear the canvas with a the histogram and the relative fit.
// This is useful not only when fit is done, and another histogram needs to be drawn,
// but also whenever the fit must be re-done on the same histogram.
// In that case, line, marker and fit function are cleared,
// and the user can then proceed to a new fit.


void ClearTheCanvas(TH1F *hist)
{
  TF1 *fitting_function = hist->GetFunction("gaus_linbg");
  hist->GetListOfFunctions()->Remove(fitting_function);
  gPad->Clear();
  gPad->Modified();
  gPad->Update();
  hist->Draw();
}
