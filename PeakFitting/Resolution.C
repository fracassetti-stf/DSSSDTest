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

Double_t BarWidth = 1.0/3.0;

/*TString InputFrontFile_Names[18]= {"DSSD_3178-2_Front_80V.root",
                                 "DSSD_3178-4_Front_80V.root",
                                   "DSSD_3178-8_Front_80V.root",
                                   "DSSD_3178-10_Front_80V.root",
                                   "DSSD_3178-11_Front_80V.root",
                                   "DSSD_3178-13_Front_80V.root",
                                   "DSSD_3178-14_Front_80V.root",
                                   "DSSD_3178-15_Front_80V.root",
                                   "DSSD_3178-16_Front_80V.root",
                                   "DSSD_3178-17_Front_80V.root",
                                   "DSSD_3178-18_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",
                                   "DSSD_3178-2_Front_80V.root",};
TString InputBackFile_Names[18];
TString OutputFile_Name[18];



void AllResolution()
{

  for(Int_t det=0; det<18;det++) {

    ResolutionHistogram(InputFrontFile_Name[det],
                        InputBackFile_Name[det],
                        OutputFile_Name[det],
                        Int_t det);

  }

}
*/

void ResolutionHistogram(TString FrontFile_Name,
                         TString BackFile_Name,
                         TString ResolutionFile_Name,
                         Int_t DetectorNo)
{

  // Output File and Tree
  //TFile* OutputFile = new TFile(ResolutionFile_Name, "RECREATE");


  // Input Front File and Tree
  TFile* InputFrontFile = new TFile(FrontFile_Name);
  TTree *FrontResolutionTree = (TTree*)InputFrontFile->Get("Am241Tree");

  Int_t FStripNo;
  Double_t FMean, FResolution;
  FrontResolutionTree->SetBranchAddress("Strip",      &FStripNo);
  FrontResolutionTree->SetBranchAddress("Mean",       &FMean);
  FrontResolutionTree->SetBranchAddress("Resolution", &FResolution);


  // Input Back File and Tree
  TFile* InputBackFile = new TFile(BackFile_Name);
  TTree *BackResolutionTree = (TTree*)InputBackFile->Get("Am241Tree");

  Int_t BStripNo;
  Double_t BMean, BResolution;
  BackResolutionTree->SetBranchAddress("Strip",      &BStripNo);
  BackResolutionTree->SetBranchAddress("Mean",       &BMean);
  BackResolutionTree->SetBranchAddress("Resolution", &BResolution);


  // Use this canvas to draw the  histogram
  TCanvas* Canvas = new TCanvas("Canvas","241Am Peak Resolution",1500,900);



  //////////////////////////////////////////////////////////////////////////////
  ////////// Resolution Histogram
  //////////////////////////////////////////////////////////////////////////////

  TMultiGraph* ResGraph = new TMultiGraph();
  TGraph *FResGraph  = new TGraph(32);
  TGraph *BResGraph  = new TGraph(32);

  Int_t nentries = 32; //(Int_t)FrontResolutionTree->GetEntries();
  for(Int_t i=0; i<nentries; i++) {

    // Fill Front Graph
    FrontResolutionTree->GetEntry(i);
    FResGraph->SetPoint(i,FStripNo+(BarWidth/2),FResolution);

    // Fill Back Graph
    BackResolutionTree->GetEntry(i);
    BResGraph->SetPoint(i,BStripNo-(BarWidth/2),BResolution);

    printf("Front: %i -  %i - %f \n",i , FStripNo, FResolution);
    printf("Back : %i -  %i - %f \n",i , BStripNo, BResolution);
  };

  FResGraph->SetFillColor(46);
  //FResGraph->SetMarkerColor(4);
  //FResGraph->SetMarkerStyle(21);

  BResGraph->SetFillColor(38);
  //BResGraph->SetMarkerColor(4);
  //BResGraph->SetMarkerStyle(21);

  // Select Title
  TString ResGraph_Title = "Energy Resolution: DSSSD ";
  ResGraph_Title.Append(Form("%d",DetectorNo));
  //ResGraph_Title.Append("241Am (@5485.56 keV)");
  ResGraph_Title.Append(";Strip Number;Resolution [\%]");
  ResGraph->SetTitle(ResGraph_Title);
  gStyle->SetTitleFontSize(0.07);

  // Bar Style
  gStyle->SetBarWidth(BarWidth);

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
  ResGraph->GetHistogram()->SetMaximum(1.4);
  ResGraph->GetHistogram()->SetMinimum(0);


  ResGraph->Add(BResGraph);
  ResGraph->Add(FResGraph);
  ResGraph->Draw("AB");


  TLegend *ResLegend = new TLegend(0.40,0.70,0.62,0.87);
  ResLegend->SetHeader("^{241}Am #alpha-decay @5485.56 keV","C"); // option "C" allows to center the header
  //ResLegend->SetTextFont(42);
  ResLegend->AddEntry(FResGraph,  "Front Strips", "f");
  ResLegend->AddEntry(BResGraph,  "Back Strips ", "f");
  //TLegendEntry *ResHeader = (TLegendEntry*)ResLegend->GetListOfPrimitives()->First();
  //ResHeader->SetTextAlign(22);
  //ResHeader->SetTextColor(1);
  //ResHeader->SetTextSize(.035);
  ResLegend->Draw();

} // end ResolutionHistogram()
