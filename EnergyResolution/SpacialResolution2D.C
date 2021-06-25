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

Double_t MinResAll = 0.57; // known
Double_t MaxResAll = 4; // known

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
void SpatialResolutionHistogram(TString FrontFile_Names,
                                TString BackFile_Names,
                                TString PeakToFit,
                                Int_t DetectorNo,
                                Double_t MaxRes);

//_____________________________________________________
//_____________________________________________________


void AllSpatialResolution(TString PeakToFit, Double_t MaxRes)
{


  if(PeakToFit=="Pulser") {
    SpatialResolutionHistogram("DSSD_3178-4_Front_80V_Pulser50micro_Pulser.root",
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

      SpatialResolutionHistogram(SelectedFrontFile_Name,
                          SelectedBackFile_Name,
                          PeakToFit,
                          det+1,
                          MaxRes);

    } // end for

  } // end else

} // end AllSpatialResolution()


//_____________________________________________________
//_____________________________________________________


void SpatialResolutionHistogram(TString FrontFile_Name,
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


  TCanvas* Canvas = new TCanvas("Canvas","241Am Peak Resolution",1140, 1050);

  Canvas->Range(-1,-1,1,1);
  Canvas->SetFillColor(10);

  Canvas->SetBorderMode(0);
  Canvas->SetBorderSize(0);

  Canvas->SetTickx();
  Canvas->SetTicky();

  Canvas->SetLeftMargin(0.10);
  Canvas->SetRightMargin(0.15);
  Canvas->SetTopMargin(0.10);
  Canvas->SetBottomMargin(0.10);

  Canvas->SetFrameFillColor(0);
  Canvas->SetFrameBorderMode(0);
  CleanTheCanvas();


  //////////////////////////////////////////////////////////////////////////////
  ////////// Graph2D Customisation
  //////////////////////////////////////////////////////////////////////////////

  TH2Poly* ResHist = new TH2Poly();
  ResHist->SetName("ResHist");

  ////////// gStyle

  gStyle->SetCanvasPreferGL(true);
  //gStyle->SetTitleFontSize(0.16);

  //gStyle->SetPalette(kRainBow);
  //gStyle->SetPalette(kDarkBodyRadiator);
  //gStyle->SetPalette(kDeepSea);
  //gStyle->SetPalette(kBlueYellow);
  gStyle->SetPalette(kBird);

  gStyle->SetOptStat(0);

  ////////// Select Title

  TString ResHist_Title = "Spatial Energy Resolution: DSSSD ";
  ResHist_Title.Append(Form("%d",DetectorNo));
  ResHist_Title.Append(" - ^{241}Am #alpha-decay @5485.56 keV");
  ResHist_Title.Append(";DSSSD Width [cm] ;DSSSD Height [cm];Resolution x1000 [%]");
  ResHist->SetTitle(ResHist_Title);
  //ResHist->SetTitleOffset(0.5);

  //////////////////////////////////////////////////////////////////////////////
  ////////// Prepare the Bins
  //////////////////////////////////////////////////////////////////////////////

  // I assume the detector to be rotated as in the datasheet,
  // Front strip vertical (from right to left)
  // Back Strip horizontal (from top to bottom)

  Double_t DSSSDX = 100.20; // mm
  Double_t DSSSDY = 100.20; // mm
  Int_t FStripBin,BStripBin;
  Int_t FStripTotalNo = 32;
  Int_t BStripTotalNo = 32;
  Double_t FStrip1,FStrip2,BStrip1,BStrip2;
  Double_t dFStrip= DSSSDX/FStripTotalNo;
  Double_t dBStrip= DSSSDY/BStripTotalNo;


  FStrip1 = 0.0;
  FStrip2 = FStrip1 + dFStrip;

  for (FStripBin = 0; FStripBin<FStripTotalNo; FStripBin++) {

    BStrip1 = 0.0;
    BStrip2 = BStrip1 + dBStrip;

    for (BStripBin = 0; BStripBin<BStripTotalNo; BStripBin++) {

      ResHist->AddBin(FStrip1, BStrip1, FStrip2,  BStrip2);
      //printf("Bin: X= %f - %f , Y = %f - %f \n",FStrip1, FStrip2, BStrip1,  BStrip2);

      BStrip1 = BStrip2;
      BStrip2 = BStrip1 + dBStrip;
    }

    FStrip1 = FStrip2;
    FStrip2 = FStrip1 + dFStrip;
  }


 //////////////////////////////////////////////////////////////////////////////
    ////////// Resolution Histogram
    //////////////////////////////////////////////////////////////////////////////

  for(Int_t FStrip_idx=0; FStrip_idx<32; FStrip_idx++) {

    for(Int_t BStrip_idx=0; BStrip_idx<32; BStrip_idx++) {

      ////////////////////////////////////////
      ////////// Fill Front Graph
      ////////////////////////////////////////

      FrontResolutionTree->GetEntry(FStrip_idx);
      BackResolutionTree->GetEntry(BStrip_idx);

      Double_t X = DSSSDX-dFStrip/2.0 -(FStrip_idx*dFStrip);
      Double_t Y = DSSSDY-dBStrip/2.0 -(BStrip_idx*dBStrip);

      ////////// Flag Value Found

      if(FResolution<0 || BResolution<0) {
        if(FResolution<-1.5 || BResolution<-1.5) {  // Strip is broken
          // do not fill the histogram
        }
        else if (FResolution<-0.5 || BResolution<-0.5 ) { // Strip has high BG or peak has a very strange shape
          ResHist->Fill(X, Y, Int_t(1000*MaxRes));
        }
      } // end "flag" if


      ////////// Read the Tree

      if(FResolution>0 && BResolution>0) { // Value to read
        Double_t FBRes = TMath::Sqrt(FResolution*FResolution+BResolution*BResolution); // Resolution of the sum
        ResHist->Fill(X, Y, Int_t(1000*FBRes));
        //printf("Bin: X= %f , Y = %f , FBRes %f \n",X, Y, FBRes);
        if(FBRes<MinResAll) {MinResAll=FBRes; MinResDet=FBRes;}
        if(FBRes<MinResDet) {MinResDet=FBRes;}
        if(FBRes>MaxResAll) {MaxResAll=FBRes;}
        if(FBRes>MaxResDet) {MaxResDet=FBRes;}
      } // end "read value" if


      //printf("Front: %i -  %f - %i \n",FStripNo, det+1.25, Int_t(1000*FResolution));
      //printf("Back : %i -  %i - %f \n",i , BStripNo, BResolution);


    } // end for Strip B
  } // end for Strip F

  //printf("Bin: i= %f , i-1 = %f  \n",DSSSDX-dFStrip/2.0,DSSSDX-(1*dFStrip) );
  ////////// Axes Options

  // X Axis Options
  TAxis *Xaxis = ResHist->GetXaxis();
  Xaxis->SetLimits(0.00,100.20);
  //Xaxis->CenterTitle();
  //Xaxis->SetLabelSize(0.04);
  //Xaxis->SetTitleSize(0.04);
  //Xaxis->SetTitleOffset(1.1);

  // Y Axis Options
  TAxis *Yaxis = ResHist->GetYaxis();
  Yaxis->SetLimits(0.00,100.20);

  // Z Axis Options
  TAxis *Zaxis = ResHist->GetZaxis();
  ResHist->SetMinimum(Int_t(1000*MinResAll)-1);
  ResHist->SetMaximum(Int_t(1000*MaxResAll)+1);

  Zaxis->SetTickLength(0.03);
  Zaxis->SetLabelSize(0.026);;
  Zaxis->SetMoreLogLabels();
  Zaxis->SetTitleOffset(1.5);
  //ResHist->SetFillColor(38); //46
  //ResHist->SetMarkerColor(4);
  //ResHist->SetMarkerStyle(21);

  Canvas->cd();
  gPad->SetLogz();
  ResHist->Draw("COLZL0");


  //////////////////////////////////////////////////////////////////////////////
  ////////// Palette
  //////////////////////////////////////////////////////////////////////////////


  Canvas->Update();
  TPaletteAxis* Palette;
  Palette = (TPaletteAxis*)ResHist->GetListOfFunctions()->FindObject("palette");

  Palette->SetX1NDC(0.86);
  Palette->SetX2NDC(0.89);
  Palette->SetY1NDC(0.10);
  Palette->SetY2NDC(0.90);

  Palette->SetLabelOffset(0.01);
  Palette->SetLineWidth(1);


  Canvas->Modified();
  Canvas->Update();

  //printf("MaxRes %f\n", TrueMaxRes);
  //printf("MinRes %f\n", MinRes);
  //////////////////////////////////////////////////////////////////////////////
  ////////// Save the Canvas
  //////////////////////////////////////////////////////////////////////////////


  // Save in pdf format
  TString Canvas_SaveNamePDF = "SpatialResolution";
  Canvas_SaveNamePDF.Append(Form("%d",DetectorNo));
  if(PeakToFit=="Pulser") {
    Canvas_SaveNamePDF.Append("_Pulser");
  }
  Canvas_SaveNamePDF.Append("_MaxRes");
  Canvas_SaveNamePDF.Append(Form("%g",MaxRes));
  //Canvas_SaveNamePDF.Append("_DifferentForEachOne");
  Canvas_SaveNamePDF.Append("_FixedForAll");
  Canvas_SaveNamePDF.Append(".pdf");
  Canvas->SaveAs(Canvas_SaveNamePDF);


  // Save in png format
  TString Canvas_SaveNamePNG = "SpatialResolution";
  Canvas_SaveNamePNG.Append(Form("%d",DetectorNo));
  if(PeakToFit=="Pulser") {
    Canvas_SaveNamePNG.Append("_Pulser");
  }
  Canvas_SaveNamePNG.Append("_MaxRes");
  Canvas_SaveNamePNG.Append(Form("%g",MaxRes));
  //Canvas_SaveNamePNG.Append("_DifferentForEachOne");
  Canvas_SaveNamePNG.Append("_FixedForAll");
  Canvas_SaveNamePNG.Append(".png");
  Canvas->SaveAs(Canvas_SaveNamePNG);

} // end SpatialResolution()


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
