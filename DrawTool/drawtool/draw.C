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
typedef std::vector<int> Vint;
void fillTH1D(Double_t xvar[], std::vector<TH1D*> &VTH1D);
void fillTH2D(Double_t xvar[], std::vector<TH2D*> &VTH2D);
std::vector<TH1D*>  cutTH1Ds; 
std::vector<TH2D*>  cutTH2Ds; 
Double_t var[99];
#include "ana.C"

//------------------------------------------------------------------------
//To draw the 2D graphs, describe the distribution of events in two variables.

using namespace std;
const Int_t nsample   = 3 ;
Char_t* type[nsample] = {
	(Char_t*)"Sig_MC",
	(Char_t*)"INC_MC",
	(Char_t*)"data"
};
Int_t sampleNumber = 0;
void fillTH1D(Double_t xvar[], std::vector<TH1D*> &VTH1D){
	Int_t numOfTH1D = VTH1D.size();
	for (Int_t i = 0; i < numOfTH1D; i++)
	{
		VTH1D.at(i) ->Fill(xvar[i]);  
	}
}
void fillTH2D(Double_t xvar[], std::vector<TH2D*> &VTH2D){
	Int_t numOfTH2D = VTH2D.size();
	for (Int_t i = 0; i < numOfTH2D; i++)
	{
		VTH2D.at(i)->Fill(xvar[2*i], xvar[2*i+1]);
	}
}
void initHistoAxis(std::map<std::string, std::vector<std::string> > &histoAxis)
{
	histoAxis["chisq_4c"]                  = {"#chi^{2}"       , "Events"                 };
	histoAxis["Mphi"]                      = {"M_{#phi} MeV/c^{2}"    , "Events"                 };
	histoAxis["chisq_4c_vs_chisq_4c_less"] = {"#chi^{2}_{2#gamma}" , "#chi^{2}_{1#gamma}"        };
	histoAxis["chisq_4c_vs_chisq_4c_more"] = {"#chi^{2}_{2#gamma}" , "#chi^{2}_{3#gamma}"}; // order is x:y
}
void draw(){
   Char_t h1name[100], h2name[100],command[100];
   
   //	Make figs directory
	sprintf(command, "mkdir -p ./figs/");
	system(command);
	// remove old files, be careful!
	sprintf(command, "rm -rf ./figs/*");
	system(command);

   TFile *f = new TFile("./ana_sig.root");
   TTree *t1 = (TTree*)f->Get("rec");
   ana* t;
   t = new ana(t1);
   cutTH1Ds = 
	{
		new TH1D("chisq_4c"    , "", 50, 0.0 , 150.0 ),  // 0 mother.M()
		new TH1D("Mphi"    , "", 50, 0.0 , 2.0 ),  // 0 mother.M()
	};
	cutTH2Ds = 
	{
		new TH2D("chisq_4c_vs_chisq_4c_less", "", 50, 0.0, 150.0, 50, 0.0, 150.0), // 0  x:y
		new TH2D("chisq_4c_vs_chisq_4c_more", "", 50, 0.0, 150.0, 50, 0.0, 150.0), // 0  x:y
	};
   std::map<std::string, std::vector<std::string> > histoAxis;
   initHistoAxis(histoAxis);
   t->Loop();



   TCanvas *c1 = new TCanvas("c1","",1000,600);
   TPad *pad1  = new TPad("pad1","pad1",0,0,1,1.0);
   pad1->SetLeftMargin(0.2);
   pad1->SetBottomMargin(0.15);
   pad1->Draw(); // Draw the upper pad: pad1
   pad1->cd();   // pad1 becomes the current pad

   Int_t numOfTH1D = cutTH1Ds.size();
	for(Int_t i = 0; i < numOfTH1D; i++)
	{

      cutTH1Ds.at(i)->Draw("hist");
      cutTH1Ds.at(i)->SetTitle("");
      cutTH1Ds.at(i)->SetStats(0);
      cutTH1Ds.at(i)->GetYaxis()->SetLabelSize(0.05);
      cutTH1Ds.at(i)->GetXaxis()->SetLabelSize(0.05);
      std::string histoname = std::string(cutTH1Ds.at(i)->GetName());
      if (histoAxis.find(histoname) != histoAxis.end())
		{
      cutTH1Ds.at(i)->GetYaxis()->SetTitle(TString(histoAxis[histoname].at(1)));
      cutTH1Ds.at(i)->GetXaxis()->SetTitle(TString(histoAxis[histoname].at(0)));
		}
      cutTH1Ds.at(i)->GetYaxis()->CenterTitle();
      cutTH1Ds.at(i)->GetYaxis()->SetTitleSize(0.06);
      cutTH1Ds.at(i)->GetYaxis()->SetTitleOffset(1);
      cutTH1Ds.at(i)->GetXaxis()->CenterTitle();
      cutTH1Ds.at(i)->GetXaxis()->SetTitleSize(0.06);
      cutTH1Ds.at(i)->GetXaxis()->SetTitleOffset(1);
      cutTH1Ds.at(i)->SetLineStyle(1);
      cutTH1Ds.at(i)->SetLineWidth(3);
      cutTH1Ds.at(i)->SetLineColor(kBlue);

	   c1->Update();
      sprintf(h2name, "./figs/%s.png",  cutTH1Ds.at(i)->GetName());
	   c1->SaveAs(h2name);
   }
   TCanvas *c2 = new TCanvas("c2","",1000,600);
   TPad *pad2  = new TPad("pad2","pad2",0,0,1,1.0);
   pad2->SetLeftMargin(0.2);
   pad2->SetBottomMargin(0.15);
   pad2->Draw(); // Draw the upper pad: pad1
   pad2->cd();   // pad1 becomes the current pad

   Int_t numOfTH2D = cutTH2Ds.size();
	for(Int_t i = 0; i < numOfTH1D; i++)
	{

      cutTH2Ds.at(i)->Draw("hist");
      cutTH2Ds.at(i)->SetTitle("");
      cutTH2Ds.at(i)->SetStats(0);
      cutTH2Ds.at(i)->GetYaxis()->SetLabelSize(0.05);
      cutTH2Ds.at(i)->GetXaxis()->SetLabelSize(0.05);
      std::string histoname = std::string(cutTH2Ds.at(i)->GetName());
      if (histoAxis.find(histoname) != histoAxis.end())
		{
      cutTH2Ds.at(i)->GetYaxis()->SetTitle(TString(histoAxis[histoname].at(1)));
      cutTH2Ds.at(i)->GetXaxis()->SetTitle(TString(histoAxis[histoname].at(0)));
		}
      cutTH2Ds.at(i)->GetYaxis()->CenterTitle();
      cutTH2Ds.at(i)->GetYaxis()->SetTitleSize(0.06);
      cutTH2Ds.at(i)->GetYaxis()->SetTitleOffset(1);
      cutTH2Ds.at(i)->GetXaxis()->CenterTitle();
      cutTH2Ds.at(i)->GetXaxis()->SetTitleSize(0.06);
      cutTH2Ds.at(i)->GetXaxis()->SetTitleOffset(1);
      cutTH2Ds.at(i)->SetLineStyle(1);
      cutTH2Ds.at(i)->SetLineWidth(3);
      cutTH2Ds.at(i)->SetMarkerColor(kBlue);

	   c2->Update();
      sprintf(h2name, "./figs/%s.png",  cutTH2Ds.at(i)->GetName());
	   c2->SaveAs(h2name);
   }
}
