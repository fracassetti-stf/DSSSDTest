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

TString LegendHeader239Pu  = "^{239}Pu #alpha-decay @5156.59 keV";
TString LegendHeader241Am  = "^{241}Am #alpha-decay @5485.56 keV";
TString LegendHeader244Cm  = "^{244}Cm #alpha-decay @5804.77 keV";
TString LegendHeaderPulser = "Pulser: ~4500 keV";

Double_t BarWidth = 1.0/4.0;

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

  // Input Front File and Tree
  TFile *InputFrontFile = new TFile(FrontFile_Name, "READ");

  TString FrontResolutionTree_Name = "Tree_";
  FrontResolutionTree_Name .Append(PeakToFit);
  TTree *FrontResolutionTree = (TTree*)InputFrontFile->Get(FrontResolutionTree_Name);

  Int_t FStripNo;
  Double_t FMean, FResolution;
  FrontResolutionTree->SetBranchAddress("Strip",      &FStripNo);
  FrontResolutionTree->SetBranchAddress("Mean",       &FMean);
  FrontResolutionTree->SetBranchAddress("Resolution", &FResolution);


  // Input Back File and Tree
  TFile* InputBackFile = new TFile(BackFile_Name, "READ");

  TString BackResolutionTree_Name = "Tree_";
  BackResolutionTree_Name .Append(PeakToFit);
  TTree *BackResolutionTree = (TTree*)InputBackFile->Get(BackResolutionTree_Name);

  Int_t BStripNo;
  Double_t BMean, BResolution;
  BackResolutionTree->SetBranchAddress("Strip",      &BStripNo);
  BackResolutionTree->SetBranchAddress("Mean",       &BMean);
  BackResolutionTree->SetBranchAddress("Resolution", &BResolution);


  // Use this canvas to draw the  histogram
  TCanvas* Canvas = new TCanvas("Canvas","241Am Peak Resolution",1500,900);
  CleanTheCanvas();


  //////////////////////////////////////////////////////////////////////////////
  ////////// Resolution Histogram
  //////////////////////////////////////////////////////////////////////////////

  TMultiGraph* ResGraph = new TMultiGraph();
  TGraph *FResGraph  = new TGraph(32);
  TGraph *BResGraph  = new TGraph(32);
  TGraph *FBrokenGraph = new TGraph(32);
  TGraph *BBrokenGraph = new TGraph(32);

  Int_t nentries = 32; //(Int_t)FrontResolutionTree->GetEntries();
  for(Int_t i=0; i<nentries; i++) {

    // Fill Front Graph
    FrontResolutionTree->GetEntry(i);

    switch(int(FResolution)) {

      case -2:  // Strip is broken
        FResGraph   ->SetPoint(i, FStripNo+(BarWidth/2.0), -1    );
        FBrokenGraph->SetPoint(i, FStripNo+(BarWidth/2.0), MaxRes);
        break;

      case -1:  // Strip has high BG or peak has a very strange shape
        FResGraph   ->SetPoint(i, FStripNo+(BarWidth/2.0), -1    );
        FBrokenGraph->SetPoint(i, FStripNo+(BarWidth/2.0), MaxRes);
        break;
    }

    if(FResolution>0) {
      FResGraph   ->SetPoint(i, FStripNo+(BarWidth/2.0), FResolution);
      FBrokenGraph->SetPoint(i, FStripNo+(BarWidth/2.0), -1          );
    }

    // Fill Back Graph
    BackResolutionTree->GetEntry(i);

    switch(int(BResolution)) {

      case -2:  // Strip is broken
        BResGraph   ->SetPoint(i, BStripNo-(BarWidth/2.0), -1    );
        BBrokenGraph->SetPoint(i, BStripNo-(BarWidth/2.0), MaxRes);
        break;

      case -1:  // Strip has high BG or peak has a very strange shape
        BResGraph   ->SetPoint(i, BStripNo-(BarWidth/2.0), -1    );
        BBrokenGraph->SetPoint(i, BStripNo-(BarWidth/2.0), MaxRes);
        break;
    }

    if(BResolution>0) {
      BResGraph   ->SetPoint(i, BStripNo-(BarWidth/2.0), BResolution);
      BBrokenGraph->SetPoint(i, BStripNo-(BarWidth/2.0), -1          );
    }

    // printf("Front: %i -  %i - %f \n",i , FStripNo, FResolution);
    // printf("Back : %i -  %i - %f \n",i , BStripNo, BResolution);
  };


  FResGraph->SetFillColor(38); //46
  //FResGraph->SetMarkerColor(4);
  //FResGraph->SetMarkerStyle(21
  //FResGraph->GetXaxis()->SetLimits(0,33);

  BResGraph->SetFillColor(30); //38
  //BResGraph->SetMarkerColor(4);
  //BResGraph->SetMarkerStyle(21);
  //BResGraph->GetXaxis()->SetLimits(0,33);

  FBrokenGraph->SetFillColor(46);
  //BResGraph->SetMarkerColor(4);
  //BResGraph->SetMarkerStyle(21);
  //FBrokenGraph->GetXaxis()->SetLimits(0,33);

  BBrokenGraph->SetFillColor(46);
  //BResGraph->SetMarkerColor(4);
  //BResGraph->SetMarkerStyle(21);
  //BBrokenGraph->GetXaxis()->SetLimits(0,33);

  // Bar Style
  gStyle->SetBarWidth(BarWidth);


  // Select Title
  TString ResGraph_Title = "Energy Resolution: DSSSD ";
  ResGraph_Title.Append(Form("%d",DetectorNo));
  //ResGraph_Title.Append("241Am (@5485.56 keV)");
  ResGraph_Title.Append(";Strip Number;Resolution [\%]");
  ResGraph->SetTitle(ResGraph_Title);
  gStyle->SetTitleFontSize(0.07);

  // X Axis Options
  TAxis *Xaxis = ResGraph->GetXaxis();
  Xaxis->SetLimits(0,33);
  //Xaxis->CenterTitle();
  //Xaxis->SetLabelSize(0.04);
  //Xaxis->SetTitleSize(0.04);
  //Xaxis->SetTitleOffset(1.1);

  // Y Axis Options
  TAxis *Yaxis = ResGraph->GetYaxis();
  //Yaxis->CenterTitle();
  //Yaxis->SetLabelSize(0.04);
  //Yaxis->SetTitleSize(0.04);
  //Yaxis->SetTitleOffset(0.6);
  ResGraph->GetHistogram()->SetMaximum(MaxRes);
  ResGraph->GetHistogram()->SetMinimum(0);


  ResGraph->Add(BResGraph);
  ResGraph->Add(FResGraph);
  ResGraph->Add(FBrokenGraph);
  ResGraph->Add(BBrokenGraph);
  ResGraph->Draw("AB");


  TLegend *ResLegend = new TLegend(0.69,0.76,0.89,0.89);
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
  ResLegend->AddEntry(FResGraph,  "Front Strips", "f");
  ResLegend->AddEntry(BResGraph,  "Back Strips ", "f");
  ResLegend->AddEntry(FBrokenGraph,"Broken Srtip / High BG", "f");
  //TLegendEntry *ResHeader = (TLegendEntry*)ResLegend->GetListOfPrimitives()->First();
  //ResHeader->SetTextAlign(22);
  //ResHeader->SetTextColor(1);
  //ResHeader->SetTextSize(.035);
  ResLegend->Draw();

  TString Canvas_SaveName = "ResGraph";
  Canvas_SaveName.Append(Form("%d",DetectorNo));
  if(PeakToFit=="Pulser") {
    Canvas_SaveName.Append("_Pulser");
  }
  Canvas_SaveName.Append("_MaxRes");
  Canvas_SaveName.Append(Form("%g",MaxRes));
  Canvas_SaveName.Append(".png");
  Canvas->SaveAs(Canvas_SaveName);


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
