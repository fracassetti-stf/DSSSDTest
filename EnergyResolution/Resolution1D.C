// Stefano Fracassetti
// 17 June 2021

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
#include "TGraph.h"
#include <TQObject.h>
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"

Double_t MinResAll = 0.45; // known but not used, does not make sense in a hist, only for palette
Double_t MaxResAll = 3.0; // known even 2.8 would work

TString LegendHeader239Pu  = "^{239}Pu #alpha-decay @5156.59 keV";
TString LegendHeader241Am  = "^{241}Am #alpha-decay @5485.56 keV";
TString LegendHeader244Cm  = "^{244}Cm #alpha-decay @5804.77 keV";
TString LegendHeaderPulser = "Pulser: ~4500 keV";

TString InputFile_Names[36]= {"DSSD_3178-2_Front_80V.root",   // Det1
                              "DSSD_3178-2_Back_80V.root",

                              "DSSD_3178-4_Front_80V.root",   // Det2
                              "DSSD_3178-4_Back_80V.root",

                              "DSSD_3178-8_Front_80V.root",   // Det3
                              "DSSD_3178-8_Back_80V.root",

                              "DSSD_3178-10_Front_80V.root",  // Det4
                              "DSSD_3178-10_Back_80V.root",

                              "DSSD_3178-11_Front_80V.root",  // Det5
                              "DSSD_3178-11_Back_80V.root",

                              "DSSD_3178-13_Front_100V.root", // Det6
                              "DSSD_3178-13_Back_100V.root",

                              "DSSD_3178-14_Front_80V.root",  // Det7
                              "DSSD_3178-14_Back_80V.root",

                              "DSSD_3178-17_Front_80V.root",  // Det8
                              "DSSD_3178-17_Back_80V.root",

                              "DSSD_3178-20_Front_80V.root",  // Det9
                              "DSSD_3178-20_Back_80V.root",

                              "DSSD_3178-16_Front_80V.root",  // Det10
                              "DSSD_3178-16_Back_80V.root",

                              "DSSD_3337-15_Front_130V.root", // Det11
                              "DSSD_3337-15_Back_130V.root",

                              "DSSD_3337-16_Front_120V.root", // Det12
                              "DSSD_3337-16_Back_120V.root",

                              "DSSD_3337-17_Front_120V.root", // Det 13
                              "DSSD_3337-17_Back_120V.root",

                              "DSSD_3337-18_Front_120V.root", // Det 14
                              "DSSD_3337-18_Back_120V.root",

                              "DSSD_3337-19_Front_120V.root", // Det 15
                              "DSSD_3337-19_Back_120V.root",

                              "DSSD_3337-20_Front_110V.root", // Det 16
                              "DSSD_3337-20_Back_110V.root",

                              "DSSD_3363-14_Front_80V.root",  // Det 17
                              "DSSD_3363-14_Back_80V.root",

                              "DSSD_3363-16_Front_80V.root",  // Det 18
                              "DSSD_3363-16_Back_80V.root"
};

void CleanTheCanvas();
void ResolutionHistogram(TString FrontFile_Names,
                         TString BackFile_Names,
                         TString PeakToFit,
                         Int_t DetectorNo,
                         Double_t MaxRes);

//_____________________________________________________
//_____________________________________________________


void AllResolution(TString PeakToFit, Double_t MaxRes)
{

  if(PeakToFit=="Pulser") {
    ResolutionHistogram("DSSD_3178-4_Front_80V_Pulser50micro_Pulser.root",
                        "DSSD_3178-4_Back_80V_Pulser50micro_Pulser.root",
                        PeakToFit,
                        2,
                        MaxRes);
  } // end if

  else {
    for(Int_t det=0; det<18; det++) {

      TString SelectedFrontFile_Name = InputFile_Names[2*det];
      SelectedFrontFile_Name.ReplaceAll(".root", "_");
      SelectedFrontFile_Name.Append(PeakToFit);
      SelectedFrontFile_Name.Append(".root");

      TString SelectedBackFile_Name = InputFile_Names[2*det+1];
      SelectedBackFile_Name.ReplaceAll(".root", "_");
      SelectedBackFile_Name.Append(PeakToFit);
      SelectedBackFile_Name.Append(".root");

      ResolutionHistogram(SelectedFrontFile_Name,
                          SelectedBackFile_Name,
                          PeakToFit,
                          det+1,
                          MaxRes);

      //printf("MaxResAll = %f, MinResAll = %f\n", MaxResAll, MinResAll);

    } // end for

  } // end else

} // end AllResolution()


//_____________________________________________________
//_____________________________________________________


void ResolutionHistogram(TString FrontFile_Name,
                         TString BackFile_Name,
                         TString PeakToFit,
                         Int_t DetectorNo,
                         Double_t MaxRes)
{

  Double_t MinResDet = 100;
  Double_t MaxResDet = 0;

  ////////// Front Strips

  // Input Front File
  TFile *InputFrontFile = new TFile(FrontFile_Name, "READ");
  // Front Tree
  TString FrontResolutionTree_Name = "Tree_";
  FrontResolutionTree_Name.Append(PeakToFit);
  TTree *FrontResolutionTree = (TTree*)InputFrontFile->Get(FrontResolutionTree_Name);
  // Setting Branches
  Int_t FStripNo;
  Double_t FMean, FResolution;
  FrontResolutionTree->SetBranchAddress("Strip",      &FStripNo);
  FrontResolutionTree->SetBranchAddress("Mean",       &FMean);
  FrontResolutionTree->SetBranchAddress("Resolution", &FResolution);

  ////////// Back Strips

  // Input Back File
  TFile* InputBackFile = new TFile(BackFile_Name, "READ");
  // Back Tree
  TString BackResolutionTree_Name = "Tree_";
  BackResolutionTree_Name .Append(PeakToFit);
  TTree *BackResolutionTree = (TTree*)InputBackFile->Get(BackResolutionTree_Name);
  // Setting Branches
  Int_t BStripNo;
  Double_t BMean, BResolution;
  BackResolutionTree->SetBranchAddress("Strip",      &BStripNo);
  BackResolutionTree->SetBranchAddress("Mean",       &BMean);
  BackResolutionTree->SetBranchAddress("Resolution", &BResolution);


  //////////////////////////////////////////////////////////////////////////////
  ////////// Canvas Setting
  //////////////////////////////////////////////////////////////////////////////


  TCanvas* Canvas = new TCanvas("Canvas","241Am Peak Resolution",1920, 1080);

  
  //Canvas->Range(-1,-1,1,1);
  //Canvas->SetFillColor(10);

  //Canvas->SetBorderMode(0);
  //Canvas->SetBorderSize(0);

  //Canvas->SetTickx();
  //Canvas->SetTicky();

  //Canvas->SetLeftMargin(0.10);
  //Canvas->SetRightMargin(0.10);
  //Canvas->SetTopMargin(0.10);
  //Canvas->SetBottomMargin(0.10);

  //Canvas->SetFrameFillColor(0);
  //Canvas->SetFrameBorderMode(0);
  CleanTheCanvas();

  //////////////////////////////////////////////////////////////////////////////
  ////////// Prepare the Bins
  //////////////////////////////////////////////////////////////////////////////

  // I assume the detector to be rotated as in the datasheet,
  // Front strip vertical (from right to left)
  // Back Strip horizontal (from top to bottom)
  Double_t dBin = 0.5;
  Double_t BinExtremes[65];
  Int_t StripTotalNo = 64;
  Int_t StripBin;

  for (StripBin = 0; StripBin<StripTotalNo+1; StripBin++) {
    BinExtremes[StripBin] = StripBin*dBin+0.5;
    //printf("Bin extremes: %f\n", BinExtremes[StripBin]);
  }


  //////////////////////////////////////////////////////////////////////////////
  ////////// TH1F Customisation
  //////////////////////////////////////////////////////////////////////////////
  
  THStack *HStack = new THStack("","");
  TH1F* FResHist1D = new TH1F("","", StripTotalNo, BinExtremes);
  TH1F* BResHist1D = new TH1F("","", StripTotalNo, BinExtremes);
  TH1F* BrokenHist1D = new TH1F("","", StripTotalNo, BinExtremes);
  HStack->SetName("HStack");

  ////////// gStyle

  //gStyle->SetCanvasPreferGL(true);
  //gStyle->SetTitleFontSize(0.06);

  //gStyle->SetPalette(kRainBow);
  //gStyle->SetPalette(kDarkBodyRadiator);
  //gStyle->SetPalette(kDeepSea);
  //gStyle->SetPalette(kBlueYellow);
  //gStyle->SetPalette(kBird);
  gStyle->SetHistTopMargin(0);
  gStyle->SetOptStat(0);

  ////////// Select Title

  TString HStack_Title = "Energy Resolution: DSSSD ";
  HStack_Title.Append(Form("%d",DetectorNo));
  //HStack_Title.Append(" - ^{241}Am #alpha-decay @5485.56 keV");
  HStack_Title.Append(";Strip Number ;Resolution x1000 [%]");
  HStack->SetTitle(HStack_Title);
  //gStyle->SetTitleOffset(0.05);



  //////////////////////////////////////////////////////////////////////////////
  ////////// Resolution Histogram 1D
  //////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////
  ////////// Fill Front Graph
  ////////////////////////////////////////

  for(Int_t FStrip_idx=0; FStrip_idx<32; FStrip_idx++) {

    FrontResolutionTree->GetEntry(FStrip_idx);

    Double_t Bin = FStrip_idx*2+2;
    //printf("Front Bin: %f\n", Bin);

    ////////// Flag Value Found

    if(FResolution<0) {
      if(FResolution<-1.5) {  // Strip is broken
        // do not fill the histogram
      }
      else if (FResolution<-0.5) { // Strip has high BG or peak has a very strange shape
        BrokenHist1D->SetBinContent(Bin, Int_t(1000*MaxRes));
      }
    } // end "flag" if

    ////////// Read the Tree

    if(FResolution>0) { // Value to read
      FResHist1D->SetBinContent(Bin, Int_t(1000*FResolution));
      printf("Bin = %f , FRes = %f \n",Bin, FResolution);

      if(FResolution<MinResAll) {MinResAll=FResolution;}
      if(FResolution<MinResDet) {MinResDet=FResolution;}
      if(FResolution>MaxResAll) {MaxResAll=FResolution;}
      if(FResolution>MaxResDet) {MaxResDet=FResolution;}
    } // end "read value" if

  }

  for(Int_t BStrip_idx=0; BStrip_idx<32; BStrip_idx++) {

    BackResolutionTree->GetEntry(BStrip_idx);

    Double_t Bin = BStrip_idx*2+1;
    //printf("Back Bin: %f\n", Bin);

    ////////// Flag Value Found

    if(BResolution<0) {
      if(BResolution<-1.5) {  // Strip is broken
        // do not fill the histogram
      }
      else if (BResolution<-0.5) { // Strip has high BG or peak has a very strange shape
        BrokenHist1D->SetBinContent(Bin, Int_t(1000*MaxRes));
      }
    } // end "flag" if

    ////////// Read the Tree

    if(BResolution>0) { // Value to read
      BResHist1D->SetBinContent(Bin, Int_t(1000*BResolution));
       printf("Bin = %f , BRes = %f \n",Bin, BResolution);

      if(BResolution<MinResAll) {MinResAll=BResolution;}
      if(BResolution<MinResDet) {MinResDet=BResolution;}
      if(BResolution>MaxResAll) {MaxResAll=BResolution;}
      if(BResolution>MaxResDet) {MaxResDet=BResolution;}
    } // end "read value" if

  }


  //printf("MaxResDet = %f, MinResDet = %f\n", MaxResDet, MinResDet);
      //printf("Front: %i -  %f - %i \n",FStripNo, det+1.25, Int_t(1000*FResolution));
      //printf("Back : %i -  %i - %f \n",i , BStripNo, BResolution);

  //printf("Bin: i= %f , i-1 = %f  \n",DSSSDX-dFStrip/2.0,DSSSDX-(1*dFStrip) );

  FResHist1D->SetFillColor(38);
  FResHist1D->SetMarkerColor(38);
  FResHist1D->SetMarkerStyle(21);
  FResHist1D->SetLineColor(1);
  
  BResHist1D->SetFillColor(30);
  BResHist1D->SetMarkerColor(30);
  BResHist1D->SetMarkerStyle(21);
  BResHist1D->SetLineColor(1);

  BrokenHist1D->SetFillColor(46);
  BrokenHist1D->SetMarkerColor(46);
  BrokenHist1D->SetMarkerStyle(21);
  BrokenHist1D->SetLineColor(1);
  
  //gPad->SetLogz();
  HStack->Add(FResHist1D);
  HStack->Add(BResHist1D);
  HStack->Add(BrokenHist1D);
  HStack->Draw();
  gPad->Update();
  gPad->Modified();

  
 ////////// Axes Options

  // X Axis Options
  TAxis *Xaxis = HStack->GetXaxis();
  Xaxis->SetLimits(0.0, 32.5);
  //Xaxis->CenterTitle();
  //Xaxis->SetLabelSize(0.04);
  //Xaxis->SetTitleSize(0.04);
  //Xaxis->SetTitleOffset(1.1);

  // Y Axis Options
  TAxis *Yaxis = HStack->GetYaxis();
  HStack->SetMinimum(0);//Int_t(1000*MinResAll)-1);
  HStack->SetMaximum(Int_t(1000*MaxResDet+100.5));
  
gPad->Update();
  gPad->Modified();


  Yaxis->SetTickLength(0.03);
  Yaxis->SetLabelSize(0.026);;
  //Yaxis->SetMoreLogLabels();
  Yaxis->SetTitleOffset(1.5);

  //Double_t x1,x2,y1,y2;
  //gPad->GetRangeAxis(x1,y1,x2,y2);
  //printf("Axis Range: x = %f - %f y = %f - %f", x1,x2,y1,y2);
  


  
  Canvas->Update();
  Canvas->Modified();
      
  TLegend *ResLegend = new TLegend(0.69+0.08,0.76+0.09,0.89+0.08,0.89+0.09);
  if(PeakToFit=="239Pu") {
    ResLegend->SetHeader(LegendHeader239Pu,"C"); // option "C" allows to center the header
  } else {
    if(PeakToFit=="241Am") {
      ResLegend->SetHeader(LegendHeader241Am,"C"); // option "C" allows to center the header
    } else {
      if(PeakToFit=="244Cm") {
        ResLegend->SetHeader(LegendHeader244Cm,"C"); // option "C" allows to center the header
      } else {
        if(PeakToFit=="Pulser") {
          ResLegend->SetHeader(LegendHeaderPulser,"C"); // option "C" allows to center the header
        } // pulser
      } // 244Cm
    } // 241Am
  } // 239Pu

  //ResLegend->SetTextFont(42);
  ResLegend->AddEntry(FResHist1D,  "Front Strips", "f");
  ResLegend->AddEntry(BResHist1D,  "Back Strips ", "f");
  ResLegend->AddEntry(BrokenHist1D,"High BG", "f");
  //TLegendEntry *ResHeader = (TLegendEntry*)ResLegend->GetListOfPrimitives()->First();
  //ResHeader->SetTextAlign(22);
  //ResHeader->SetTextColor(1);
  //ResHeader->SetTextSize(.035);
  ResLegend->Draw();

   //////////////////////////////////////////////////////////////////////////////
  ////////// Save the Canvas
  //////////////////////////////////////////////////////////////////////////////


  // Save in pdf format
  TString Canvas_SaveNamePDF = "Resolution";
  Canvas_SaveNamePDF.Append(Form("%d",DetectorNo));
  if(PeakToFit=="Pulser") {
    Canvas_SaveNamePDF.Append("_Pulser");
  }
  Canvas_SaveNamePDF.Append("_MaxRes");
  Canvas_SaveNamePDF.Append(Form("%g",MaxRes));
  Canvas_SaveNamePDF.Append("_DifferentForEachOne");
  //Canvas_SaveNamePDF.Append("_FixedForAll");
  Canvas_SaveNamePDF.Append(".pdf");
  Canvas->SaveAs(Canvas_SaveNamePDF);


  // Save in png format
  TString Canvas_SaveNamePNG = "Resolution";
  Canvas_SaveNamePNG.Append(Form("%d",DetectorNo));
  if(PeakToFit=="Pulser") {
    Canvas_SaveNamePNG.Append("_Pulser");
  }
  Canvas_SaveNamePNG.Append("_MaxRes");
  Canvas_SaveNamePNG.Append(Form("%g",MaxRes));
  Canvas_SaveNamePNG.Append("_DifferentForEachOne");
  //Canvas_SaveNamePNG.Append("_FixedForAll");
  Canvas_SaveNamePNG.Append(".png");
  Canvas->SaveAs(Canvas_SaveNamePNG);



} // end ResolutionHistogram()


//_____________________________________________________
//_____________________________________________________


// ClearTheCanvas()
//
// This function is used to clear the canvas with a the histogram and the relative fit.
// This is useful not only when fit is done, and another histogram needs to be drawn,
// but also whenever the fit must be re-done on the same histogram.
// In that case, line, marker and fit function are cleared,
// and the user can then proceed to a new fit.


void CleanTheCanvas()
{
  //TF1 *fitting_function = hist->GetFunction("gaus_linbg");
  //hist->GetListOfFunctions()->Remove(fitting_function);
  gPad->Clear();
  gPad->Modified();
  gPad->Update();
}
