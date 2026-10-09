#include "TROOT.h"
#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TPad.h"
#include "TLatex.h"
#include "iostream"
#include "TLorentzVector.h"
//------------------------------------------------------------------------
//To draw the 2D graphs, describe the distribution of events in two variables.

using namespace std;

void draw(){
   TFile *f = new TFile("./pnpi_psip_data_2009.root");
   TTree *t1 = (TTree*)f->Get("pnpi");

   TH1D *Histgram = new TH1D("h1","mass",100,0.0,2.0);

   // Declaration of leaf types
   Double_t        mass;


   // List of branches
   TBranch        *b_mass;   //!

//-----------------------------------------------------------------------------------------
//get the tree's branch from the root by change the content in "" to the branch name in root's tree and name it with the sencond parameter
   t1->SetBranchAddress("n_mass", &mass, &b_mass);

   for (int i = 0; i < t1->GetEntries(); i++)
   {
      t1->GetEntry(i);
   //  if( p_ptrk>0.0001 ){
      Histgram->Fill(mass);
   //   }else{
   //     continue;
   //   }
   
   }

   TCanvas *c1 = new TCanvas("c1","",1000,600);
   TPad *pad1  = new TPad("pad1","pad1",0,0,1,1.0);
   pad1->SetLeftMargin(0.2);
   pad1->SetBottomMargin(0.15);
   pad1->Draw(); // Draw the upper pad: pad1
   pad1->cd();   // pad1 becomes the current pad

   Histgram->Draw("hist");
   Histgram->SetTitle("");
   Histgram->SetStats(0);
   // Do not draw the Y axis label on the upper plot and redraw a small
   // axis instead, in order to avoid the first label (0) to be clipped.
   Histgram->GetYaxis()->SetLabelSize(0.05);
   Histgram->GetXaxis()->SetLabelSize(0.05);
   Histgram->GetYaxis()->SetTitle("events");
   Histgram->GetXaxis()->SetTitle("MissingMass");
   Histgram->GetYaxis()->CenterTitle();
   Histgram->GetYaxis()->SetTitleSize(0.06);
   Histgram->GetYaxis()->SetTitleOffset(1);
   Histgram->GetXaxis()->CenterTitle();
   Histgram->GetXaxis()->SetTitleSize(0.06);
   Histgram->GetXaxis()->SetTitleOffset(1);
   Histgram->SetLineStyle(1);
   Histgram->SetLineWidth(3);
   Histgram->SetLineColor(kBlue);

   c1->SaveAs("Positive_draw1D.png");

}
