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
#include "TGraph2D.h"
#include "TH2Poly.h"


////////////////////////////////////////////////////////////////////////////////
////////// Constants
////////////////////////////////////////////////////////////////////////////////


TString LegendHeader239Pu  = "^{239}Pu #alpha-decay @5156.59 keV";
TString LegendHeader241Am  = "^{241}Am #alpha-decay @5485.56 keV";
TString LegendHeader244Cm  = "^{244}Cm #alpha-decay @5804.77 keV";

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


//_____________________________________________________
//_____________________________________________________


// Resolution2D
// This function produces the TH2Poly histogram
// for All Si and All Strips.


void ResolutionHistogram(TString PeakToFit, Double_t MaxRes)
{

  Double_t MinRes = MaxRes; // searching for MinRes to set proper axis limit.


  //////////////////////////////////////////////////////////////////////////////
  ////////// Canvas Setting
  //////////////////////////////////////////////////////////////////////////////


  TCanvas* Canvas = new TCanvas("Canvas","241Am Peak Resolution",1920, 1080);
  TH1F* ResHist = new TH1F("","", 100, 0, 5);
  ResHist->SetName("ResHist");

  Canvas->Range(-1,-1,1,1);
  Canvas->SetFillColor(10);

  Canvas->SetBorderMode(0);
  Canvas->SetBorderSize(0);

  Canvas->SetTickx();
  Canvas->SetTicky();

  Canvas->SetLeftMargin(0.10);
  Canvas->SetRightMargin(0.13);
  Canvas->SetTopMargin(0.10);
  Canvas->SetBottomMargin(0.10);

  Canvas->SetFrameFillColor(0);
  Canvas->SetFrameBorderMode(0);


  //////////////////////////////////////////////////////////////////////////////
  ////////// Graph2D Customisation
  //////////////////////////////////////////////////////////////////////////////

  ////////// gStyle

  gStyle->SetCanvasPreferGL(true);
  gStyle->SetTitleFontSize(0.06);

  //gStyle->SetPalette(kRainBow);
  //gStyle->SetPalette(kDarkBodyRadiator);
  //gStyle->SetPalette(kDeepSea);
  //gStyle->SetPalette(kBlueYellow);
  gStyle->SetPalette(kBird);

  gStyle->SetOptStat(0);

  ////////// Select Title

  TString ResHist_Title = "Energy Resolution: All DSSSD   -   ^{241}Am #alpha-decay @5485.56 keV";
  //ResHist_Title.Append(Form("%d",det));
  //ResHist_Title.Append("241Am (@5485.56 keV)");
  ResHist_Title.Append(";Strip Number;Detector Number;Resolution x1000 [%]");
  ResHist->SetTitle(ResHist_Title);
  ResHist->SetTitleOffset(1);

/*//////////////////////////////////////////////////////////////////////////////
  ////////// Prepare the Bins
  //////////////////////////////////////////////////////////////////////////////

  Int_t DetectorBin,StripBin;
  Int_t DetNo = 36;
  Int_t StripNo = 32;
  Double_t DetVal1,DetVal2,StripVal1,StripVal2;
  Double_t dDet=0.5;
  Double_t dStrip=1;


  DetVal1 = 0.5;
  DetVal2 = DetVal1 + dDet;

  for (DetectorBin = 0; DetectorBin<DetNo; DetectorBin++) {

    StripVal1 = 0.5;
    StripVal2 = StripVal1 + dStrip;

    for (StripBin = 0; StripBin<StripNo; StripBin++) {

      ResHist->AddBin(StripVal1, DetVal1, StripVal2,  DetVal2);
      //printf("Bin: X= %f - %f , Y = %f - %f \n",DetVal1, DetVal2, StripVal1, StripVal2);

      StripVal1 = StripVal2;
      StripVal2 = StripVal1 + dStrip;
    }

    DetVal1 = DetVal2;
    DetVal2 = DetVal1 + dDet;
  }
*/

//////////////////////////////////////////////////////////////////////////////
////////// Fill the Histogram
/////////////////////////////////////////////////////////////////////////////


  for(Int_t det=0; det<18; det++) {

    ////////// Front Strips

    TString InputFrontFile_Name = InputFile_Names[2*det];
    InputFrontFile_Name.ReplaceAll(".root", "_");
    InputFrontFile_Name.Append(PeakToFit);
    InputFrontFile_Name.Append(".root");
    // Input Front File
    TFile *InputFrontFile = new TFile(InputFrontFile_Name, "READ");
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

    TString InputBackFile_Name = InputFile_Names[2*det+1];
    InputBackFile_Name.ReplaceAll(".root", "_");
    InputBackFile_Name.Append(PeakToFit);
    InputBackFile_Name.Append(".root");
    // Input Back File
    TFile* InputBackFile = new TFile(InputBackFile_Name, "READ");
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
    ////////// Resolution Histogram
    //////////////////////////////////////////////////////////////////////////////


    for(Int_t i=0; i<32; i++) {

      ////////////////////////////////////////
      ////////// Fill Front Graph
      ////////////////////////////////////////

      FrontResolutionTree->GetEntry(i);


      ////////// Flag Value Found

      if(FResolution<0) {
        if(FResolution<-1.5) {  // Strip is broken
          // do not fill the histogram
        }
        else if (FResolution<-0.5) { // Strip has high BG or peak has a very strange shape
          //ResHist->Fill(FStripNo, det+1.25, Int_t(1000*MaxRes)); // do nothing
        }
      } // end "flag" if


      ////////// Read the Tree

      if(FResolution>0) { // Value to read
        ResHist->Fill(FResolution);
        if(FResolution<MinRes) {MinRes=FResolution;}
      } // end "read value" if


      //printf("Front: %i -  %f - %i \n",FStripNo, det+1.25, Int_t(1000*FResolution));
      //printf("Back : %i -  %i - %f \n",i , BStripNo, BResolution);


      ////////////////////////////////////////
      ////////// Fill Back Graph
      ////////////////////////////////////////


      BackResolutionTree->GetEntry(i);


      ////////// Flag Value Found

      if(BResolution<0) {  // Flag
        if(BResolution<-1.5) {  // Strip is broken
          // do not fill the histogram
        }
        else if (BResolution<-0.5) { // Strip has high BG or peak has a very strange shape
           //ResHist->Fill(BStripNo, det+0.75, Int_t(1000*MaxRes));
        }
      } // end "flag" if

      ////////// Read the Tree

      if(BResolution>0) { // Value to read
        ResHist->Fill(BResolution);
           if(BResolution<MinRes) {MinRes=BResolution;}
       } // end "read value" if


    } // end for strips


  } // end for det


  ////////// Axes Options

  // X Axis Options
//  TAxis *Xaxis = ResHist->GetXaxis();
//  Xaxis->SetLimits(0,5);
  //Xaxis->CenterTitle();
  //Xaxis->SetLabelSize(0.04);
  //Xaxis->SetTitleSize(0.04);
  //Xaxis->SetTitleOffset(1.1);

  // Y Axis Options
//  TAxis *Yaxis = ResHist->GetYaxis();
//  Yaxis->SetLimits(0.5,18.5);

/*  // Z Axis Options
  TAxis *Zaxis = ResHist->GetZaxis();
  ResHist->SetMinimum(Int_t(1000*MinRes)-1);
  ResHist->SetMaximum(Int_t(1000*MaxRes)+1);

  Zaxis->SetTickLength(0.03);
  Zaxis->SetLabelSize(0.026);;
  Zaxis->SetMoreLogLabels();
  Zaxis->SetTitleOffset(1.2);
  //ResHist->SetFillColor(38); //46
  //ResHist->SetMarkerColor(4);
  //ResHist->SetMarkerStyle(21);
*/
  Canvas->cd();
//  gPad->SetLogz();
//  ResHist->Draw("COLZL0");


  //////////////////////////////////////////////////////////////////////////////
  ////////// Palette
  //////////////////////////////////////////////////////////////////////////////

/*
  Canvas->Update();
  TPaletteAxis* Palette;
  Palette = (TPaletteAxis*)ResHist->GetListOfFunctions()->FindObject("palette");

  Palette->SetX1NDC(0.88);
  Palette->SetX2NDC(0.91);
  Palette->SetY1NDC(0.10);
  Palette->SetY2NDC(0.90);

  Palette->SetLabelOffset(0.01);
  Palette->SetLineWidth(1);


  Canvas->Modified();
  Canvas->Update();
*/

  //////////////////////////////////////////////////////////////////////////////
  ////////// Save the Canvas
  //////////////////////////////////////////////////////////////////////////////


  // Save in pdf format
  TString Canvas_SaveNamePDF = "ResolutionHistogram";
  //Canvas_SaveNamePDF.Append(Form("%d",det));
  if(PeakToFit=="Pulser") {
    Canvas_SaveNamePDF.Append("_Pulser");
  }
  Canvas_SaveNamePDF.Append("_MaxRes");
  Canvas_SaveNamePDF.Append(Form("%g",MaxRes));
  Canvas_SaveNamePDF.Append(".pdf");
  Canvas->SaveAs(Canvas_SaveNamePDF);


  // Save in png format
  TString Canvas_SaveNamePNG = "ResolutionHistogram";
  //Canvas_SaveNamePNG.Append(Form("%d",det));
  if(PeakToFit=="Pulser") {
    Canvas_SaveNamePNG.Append("_Pulser");
  }
  Canvas_SaveNamePNG.Append("_MaxRes");
  Canvas_SaveNamePNG.Append(Form("%g",MaxRes));
  Canvas_SaveNamePNG.Append(".png");
  Canvas->SaveAs(Canvas_SaveNamePNG);

} // end AllResolution()
