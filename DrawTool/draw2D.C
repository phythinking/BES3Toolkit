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

void draw2D(){
   TFile *f = new TFile("./pnpi_psip_data_2009.root");
   TTree *t1 = (TTree*)f->Get("pnpi");

   TH2D *Pdedx = new TH2D("h1","ppi_vs_npi",100, 0.0, 3.0, 100, 0.0, 3.0);

   // Declaration of leaf types
   Double_t        x_mass;
   Double_t        y_mass;


   // List of branches
   TBranch        *b_x_mass;   //!
   TBranch        *b_y_mass;   //!

//-----------------------------------------------------------------------------------------
//get the tree's branch from the root by change the content in "old_x/y" to the branch name in root's tree and name it with the sencond parameter
   t1->SetBranchAddress("old_x", &x_mass, &b_x_mass);
   t1->SetBranchAddress("old_y", &y_mass, &b_y_mass);

   for (int i = 0; i < t1->GetEntries(); i++)
   {
      t1->GetEntry(i);
   //   if( p_ptrk>0.0001 ){
      Pdedx->Fill(x_mass,y_mass);
   //   }else{
   //      continue;
   //   }
   
   }

   TCanvas *c1 = new TCanvas("c1","",1000,600);
   TPad *pad1  = new TPad("pad1","pad1",0,0,1,1.0);
   pad1->SetLeftMargin(0.2);
   pad1->SetBottomMargin(0.15);
   pad1->Draw(); // Draw the upper pad: pad1
   pad1->cd();   // pad1 becomes the current pad

   Pdedx->Draw("hist");
   Pdedx->SetTitle("");
   Pdedx->SetStats(0);
   // Do not draw the Y axis label on the upper plot and redraw a small
   // axis instead, in order to avoid the first label (0) to be clipped.
   Pdedx->GetYaxis()->SetLabelSize(0.05);
   Pdedx->GetXaxis()->SetLabelSize(0.05);
   Pdedx->GetYaxis()->SetTitle("x_title");
   Pdedx->GetXaxis()->SetTitle("y_title");
   Pdedx->GetYaxis()->CenterTitle();
   Pdedx->GetYaxis()->SetTitleSize(0.06);
   Pdedx->GetYaxis()->SetTitleOffset(1);
   Pdedx->GetXaxis()->CenterTitle();
   Pdedx->GetXaxis()->SetTitleSize(0.06);
   Pdedx->GetXaxis()->SetTitleOffset(1);
   Pdedx->SetLineStyle(1);
   Pdedx->SetLineWidth(3);
   Pdedx->SetLineColor(kBlue);

   pad1->SaveAs("tryfordraw.png");


}
