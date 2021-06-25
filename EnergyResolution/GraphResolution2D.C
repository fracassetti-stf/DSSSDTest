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

TString LegendHeader239Pu  = "^{239}Pu #alpha-decay @5156.59 keV";
TString LegendHeader241Am  = "^{241}Am #alpha-decay @5485.56 keV";
TString LegendHeader244Cm  = "^{244}Cm #alpha-decay @5804.77 keV";
TString LegendHeaderPulser = "Pulser: ~4500 keV";

Double_t BarWidth = 1.0/3.0;

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


//_____________________________________________________
//_____________________________________________________


void AllResolution(TString PeakToFit, Double_t MaxRes)
{

  // Use this canvas to draw the  histogram
  TCanvas* Canvas = new TCanvas("Canvas","241Am Peak Resolution",1500,900);
  TGraph2D* FResGraph = new TGraph2D();
  TGraph2D* BResGraph = new TGraph2D();
  TGraph2D* BrokenGraph = new TGraph2D();


  //////////////////////////////////////////////////////////////////////////////
  ////////// Graph2D Customisation
  //////////////////////////////////////////////////////////////////////////////

  // Select Title
  TString FResGraph_Title = "Energy Resolution: All DSSSD ";
  //ResGraph_Title.Append(Form("%d",det));
  //ResGraph_Title.Append("241Am (@5485.56 keV)");
  FResGraph_Title.Append(";Strip Number;Detector Number;Resolution [\%]");
  FResGraph->SetTitle(FResGraph_Title);
  gStyle->SetTitleFontSize(0.07);
  gStyle->SetPalette(40);


  // Bar Style
  gStyle->SetBarWidth(BarWidth);

  //////////////////////////////////////////////////////////////////////////////
  ////////// Fill the Graph
  /////////////////////////////////////////////////////////////////////////////

  for(Int_t det=0; det<18; det++) {

    TString InputFrontFile_Name = InputFile_Names[2*det];
    InputFrontFile_Name.ReplaceAll(".root", "_");
    InputFrontFile_Name.Append(PeakToFit);
    InputFrontFile_Name.Append(".root");
    // Input Front File
    TFile *InputFrontFile = new TFile(InputFrontFile_Name, "READ");

    TString InputBackFile_Name = InputFile_Names[2*det+1];
    InputBackFile_Name.ReplaceAll(".root", "_");
    InputBackFile_Name.Append(PeakToFit);
    InputBackFile_Name.Append(".root");
    // Input Back File
    TFile* InputBackFile = new TFile(InputBackFile_Name, "READ");

    ////////// TTrees
    // Front Tree
    TString FrontResolutionTree_Name = "Tree_";
    FrontResolutionTree_Name.Append(PeakToFit);
    TTree *FrontResolutionTree = (TTree*)InputFrontFile->Get(FrontResolutionTree_Name);

    Int_t FStripNo;
    Double_t FMean, FResolution;
    FrontResolutionTree->SetBranchAddress("Strip",      &FStripNo);
    FrontResolutionTree->SetBranchAddress("Mean",       &FMean);
    FrontResolutionTree->SetBranchAddress("Resolution", &FResolution);

    // Back Tree
    TString BackResolutionTree_Name = "Tree_";
    BackResolutionTree_Name .Append(PeakToFit);
    TTree *BackResolutionTree = (TTree*)InputBackFile->Get(BackResolutionTree_Name);

    Int_t BStripNo;
    Double_t BMean, BResolution;
    BackResolutionTree->SetBranchAddress("Strip",      &BStripNo);
    BackResolutionTree->SetBranchAddress("Mean",       &BMean);
    BackResolutionTree->SetBranchAddress("Resolution", &BResolution);


    //////////////////////////////////////////////////////////////////////////////
    ////////// Resolution Histogram
    //////////////////////////////////////////////////////////////////////////////


    Int_t nentries = 32; //(Int_t)FrontResolutionTree->GetEntries();
    for(Int_t i=0; i<32; i++) {

      // Fill Front Graph
      FrontResolutionTree->GetEntry(i);

      switch(int(FResolution)) {

        case -2:  // Strip is broken
          FResGraph->SetPoint(det*32+i, FStripNo+(BarWidth/2.0), det, MaxRes);
          break;

        case -1:  // Strip has high BG or peak has a very strange shape
          FResGraph->SetPoint(det*32+i, FStripNo+(BarWidth/2.0), det, MaxRes);
          break;
      }

      if(FResolution>0) {
        FResGraph->SetPoint(det*32+i, FStripNo+(BarWidth/2.0), det, FResolution);
      }

      printf("Front: %i -  %f - %f \n",det*32+i , FStripNo+(BarWidth/2.0), FResolution);
      //printf("Back : %i -  %i - %f \n",i , BStripNo, BResolution);
      
      // Fill Back Graph

      BackResolutionTree->GetEntry(i);

      switch(int(BResolution)) {

        case -2:  // Strip is broken
          FResGraph->SetPoint(det*32+i+1152, BStripNo-(BarWidth/2.0), det, MaxRes);
          break;

        case -1:  // Strip has high BG or peak has a very strange shape
          FResGraph->SetPoint(det*32+i+1152, BStripNo-(BarWidth/2.0), det, MaxRes);
          break;
      }

      if(BResolution>0) {
        FResGraph->SetPoint(det*32+i+1152, BStripNo-(BarWidth/2.0), det, BResolution);
      }



    } // end for strips


  } // end for det
  /*
  ResGraph->SetPoint(0, 1, 1, 5 );
  ResGraph->SetPoint(1, 1, 2, 5 );
  ResGraph->SetPoint(2, 1, 3, 5 );
  ResGraph->SetPoint(3, 1, 4, 5 );
  ResGraph->SetPoint(4, 2, 1, 5 );
  ResGraph->SetPoint(5, 3, 2, 5 );
  ResGraph->SetPoint(6, 4, 3, 5 );
  ResGraph->SetPoint(7, 5, 4, 5 );
  */


    // X Axis Options
  TAxis *FXaxis = FResGraph->GetXaxis();
  FXaxis->SetLimits(0,33);
  //Xaxis->CenterTitle();
  //Xaxis->SetLabelSize(0.04);
  //Xaxis->SetTitleSize(0.04);
  //Xaxis->SetTitleOffset(1.1);

  // Y Axis Options
  TAxis *FYaxis = FResGraph->GetYaxis();
  FYaxis->SetLimits(0,5);
  //Yaxis->CenterTitle();
  //Yaxis->SetLabelSize(0.04);
  //Yaxis->SetTitleSize(0.04);
  //Yaxis->SetTitleOffset(0.6);
  //ResGraph->GetHistogram()->SetMaximum(MaxRes);
  //ResGraph->GetHistogram()->SetMinimum(0);

// Z Axis Options
  TAxis *FZaxis = FResGraph->GetZaxis();
  //Zaxis->SetLimits(0,3);
  //Yaxis->CenterTitle();
  //Yaxis->SetLabelSize(0.04);
  //Yaxis->SetTitleSize(0.04);
  //Yaxis->SetTitleOffset(0.6);
  FResGraph->SetMinimum(0);
  FResGraph->SetMaximum(3);

  //ResGraph->GetHistogram()->SetMaximum(MaxRes);
  //ResGraph->GetHistogram()->SetMinimum(0);


  FResGraph->SetFillColor(38); //46
  FResGraph->SetMarkerColor(4);
  FResGraph->SetMarkerStyle(21);


  /*
    // X Axis Options
  TAxis *BXaxis = BResGraph->GetXaxis();
  BXaxis->SetLimits(0,33);
  //Xaxis->CenterTitle();
  //Xaxis->SetLabelSize(0.04);
  //Xaxis->SetTitleSize(0.04);
  //Xaxis->SetTitleOffset(1.1);

  // Y Axis Options
  TAxis *BYaxis = BResGraph->GetYaxis();
  BYaxis->SetLimits(0,5);
  //Yaxis->CenterTitle();
  //Yaxis->SetLabelSize(0.04);
  //Yaxis->SetTitleSize(0.04);
  //Yaxis->SetTitleOffset(0.6);
  //ResGraph->GetHistogram()->SetMaximum(MaxRes);
  //ResGraph->GetHistogram()->SetMinimum(0);

// Z Axis Options
  TAxis *BZaxis = BResGraph->GetZaxis();
  //Zaxis->SetLimits(0,3);
  //Yaxis->CenterTitle();
  //Yaxis->SetLabelSize(0.04);
  //Yaxis->SetTitleSize(0.04);
  //Yaxis->SetTitleOffset(0.6);
  BResGraph->SetMinimum(0);
  BResGraph->SetMaximum(3);

  //ResGraph->GetHistogram()->SetMaximum(MaxRes);
  //ResGraph->GetHistogram()->SetMinimum(0);
  */

  BResGraph->SetFillColor(46); //46
  BResGraph->SetMarkerColor(2);
  BResGraph->SetMarkerStyle(2);


  FResGraph->Draw();
  //BResGraph->Draw();


  //////////////////////////////////////////////////////////////////////////////
  ////////// Graph Legend
  //////////////////////////////////////////////////////////////////////////////
  /*
  TLegend *ResLegend = new TLegend();//0.69,0.76,0.89,0.89);
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
  //ResLegend->AddEntry(FResGraph,  "Front Strips", "f");
  //ResLegend->AddEntry(BResGraph,  "Back Strips ", "f");
  //ResLegend->AddEntry(FBrokenGraph,"Broken Srtip / High BG", "f");
  //TLegendEntry *ResHeader = (TLegendEntry*)ResLegend->GetListOfPrimitives()->First();
  //ResHeader->SetTextAlign(22);
  //ResHeader->SetTextColor(1);
  //ResHeader->SetTextSize(.035);
  ResLegend->Draw("surf1");
  */

  //////////////////////////////////////////////////////////////////////////////
  ////////// Save the Canvas
  //////////////////////////////////////////////////////////////////////////////

  TString Canvas_SaveName = "ResGraphAll";
  //Canvas_SaveName.Append(Form("%d",det));
  if(PeakToFit=="Pulser") {
    Canvas_SaveName.Append("_Pulser");
  }
  Canvas_SaveName.Append("_MaxRes");
  Canvas_SaveName.Append(Form("%g",MaxRes));
  Canvas_SaveName.Append(".pdf");
  Canvas->SaveAs(Canvas_SaveName);
} // end AllResolution()


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
