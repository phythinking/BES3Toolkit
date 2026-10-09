// chi2 analysis 
// Create by Dr. Xiang Zhou
// Date: before 2014, maybe 2012
// Modified: 2019-05-27
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TArrow.h>
#include <TApplication.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <chrono>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>
#include "bes3plotstyle.h"
#include "bes3plotstyle.C"
#include "userDefined.h"
#include "config.hh"
#include "config.cc"

Config configSettings;
const Int_t       WRONGNUM = -9999;
const std::string WRONGSTR = "-9999"; 
const std::string EMPTYSTR = "";

void chi2FoM(int argc, char** argv)
{
	bool     SpBswitch;
	std::string cfgFile;
	cfgFile = std::string(argv[1]);
	configSettings.ReadFile(cfgFile);
	TString S               =         configSettings.Read("S",           WRONGSTR);
	TString B               =         configSettings.Read("B",           WRONGSTR);
	TString SpB             =         configSettings.Read("SpB",         WRONGSTR);
	TString chisquare       =         configSettings.Read("chisquare",   WRONGSTR);
	Double_t ratio          =         configSettings.Read("ScaleRatio",  WRONGNUM);
	TCut basicCut           = TString(configSettings.Read("basicCut",    EMPTYSTR)).Data();
//	std::cout << "basicCut = " << basicCut << std::endl;
	TCut itopoCut           = TString(configSettings.Read("itopoCut",    EMPTYSTR)).Data();
	Int_t chi2Step          =         configSettings.Read("chi2Step",    WRONGNUM);
	Int_t chi2Arrow         =         configSettings.Read("chi2Arrow",   WRONGNUM);
	const Char_t* tree      =         configSettings.Read("tree",        WRONGSTR).data();
	const Char_t* outputDir =         configSettings.Read("outputDir",   WRONGSTR).data();
	const Char_t* outputExt =         configSettings.Read("outputExt",   WRONGSTR).data();
	
	try
	{
		if ( S == EMPTYSTR ) 
			throw "Root file for signal does not exist!\n";
	}
	catch(const char* s)
	{
		std::cerr << s;
		exit(-1);
	}
	try
	{
		if ( B == EMPTYSTR && SpB == EMPTYSTR )
			throw "Root files for background AND background plus signal do not exist!\n";
	}
	catch(const char* s)
	{
		std::cerr << s;
		exit(-1);
	}
	try
	{
		if ( B != EMPTYSTR && SpB != EMPTYSTR )
			throw "Root files for background AND background plus signal exist simultaneously!\n";
	}
	catch(const char* s)
	{
		std::cerr << s;
		exit(-1);
	}
	if ( B != EMPTYSTR )
	{
		SpBswitch = 0;
		try
		{
			if ( ratio < 0. )
				throw "Ratio for signal is wrong!\n";
		}
		catch(const char* s)
		{
			std::cerr << s;
			exit(-1);
		}
	}
	else
		SpBswitch = 1;


	TFile* SFile = new TFile(S);
	TFile* BFile;
	if ( SpBswitch )
		BFile = new TFile(SpB);
	else
		BFile = new TFile(B);

	TTree* STree = (TTree *)SFile->Get(tree);
	TTree* BTree = (TTree *)BFile->Get(tree);

	std::vector<Double_t> xfom, yfom;

	for (int i = 10; i < 201; i += chi2Step)
	{
		xfom.push_back(i);
		TCut chi2Selection;
		chi2Selection = "";
		chi2Selection = (chisquare + " < " + std::to_string(i)).Data();
//		std::cout << "chi2Selection: " << chi2Selection << "\n";
		Double_t SNum, SpBNum;
		SNum   = 0;
		SpBNum = 0;
		SNum = STree->GetEntries(basicCut + chi2Selection); 
		if ( SpBswitch )
			SpBNum = BTree->GetEntries(basicCut + chi2Selection);
		else
			SpBNum = BTree->GetEntries(basicCut + !itopoCut + chi2Selection) + SNum * ratio;
//		std::cout << "cut1 = " << basicCut + chi2Selection << std::endl;
//		std::cout << "cut2 = " << basicCut + !itopoCut +  chi2Selection << std::endl;
		yfom.push_back(SNum / sqrt(SpBNum));
//		std::cout <<   SNum / sqrt(SpBNum) << "\n";
	}

	Double_t yFront = yfom.front();
	Double_t yBack  = yfom.back();
	
	Double_t* x = xfom.data();
	Double_t* y = yfom.data();

// 	Set general style options
	SetStyle();
//      Create meeting style plots with stat and fitbox
	SetMeetingStyle();
//	Get trees from root files

//	Set a canvas and save figures into a pdf file with the tree name
	TCanvas * c1 = new TCanvas("c1","chi2 FoM Plot", 800,600);

	TGraph *g1=new TGraph(xfom.size(), x, y);
	FormatData(g1);
	FormatAxis(g1->GetXaxis());
	FormatAxis(g1->GetYaxis());
	SetStyle();
	g1->GetXaxis()->SetTitle("#chi^{2}");
	g1->GetYaxis()->SetTitle("#frac{s}{#sqrt{s+b}}");
	g1->SetTitle("#chi^{2} optimize");
//	g1->SetMarkerStyle(21);
//	g1->SetMarkerSize(1);
	g1->Draw("ALP");
	TArrow* myar = new TArrow(chi2Arrow, 0.98*yBack, chi2Arrow, 1.0*yFront, 0.04, "|>");
	myar->SetLineColor(kRed);
	myar->SetFillColor(kRed);
	myar->SetLineWidth(3);
	myar->SetAngle(40);
	myar->Draw();
	c1->Modified();

	TCanvas * c2 = new TCanvas("c2","chi2 stack Plot", 800,600);

	TH1F* hS = new TH1F("hS", "", 100, 0, 200);
	TH1F* hB = new TH1F("hB", "", 100, 0, 200);
	STree->Draw("chi2>>hS", basicCut);
//	hS->Scale(ratio);
	BTree->Draw("chi2>>hB", basicCut); 
	Double_t maxY = 0.;
	maxY = std::max(maxY, hS->GetMaximum());
	maxY = std::max(maxY, hB->GetMaximum());
	FormatMC2(hS);
	if ( SpBswitch )
		FormatData(hB);
	else
		FormatMC1(hB);
	FormatAxis(hS->GetXaxis());
	FormatAxis(hS->GetYaxis());
	hS->GetYaxis()->SetRangeUser(0, maxY*1.2);
	NameAxes(hS, "#chi^{2}", "Events / 100");
//	hS->Draw("axis");
	hS->Draw();
//	hS->Draw("hsame");
	hB->Draw("hsame");
//	hS->Draw("axissame");
	TLegend * leg = new TLegend(0.80, 0.62, 0.94, 0.72);
	leg->AddEntry(hS, "Sig", "LEP");
	if ( SpBswitch )
		leg->AddEntry(hB, "Sig+Bkg", "LEP");
	else
		leg->AddEntry(hB, "Bkg", "LEP");
	leg->Draw();
	c2->SetLeftMargin(0.17);
	c2->SetRightMargin(0.07);
	c2->SetTopMargin(0.08);
	c2->SetBottomMargin(0.18);
	c2->Update();

	Char_t command[200];
	sprintf(command, "ls %s", outputDir);
//	printf("%d\n", __LINE__);
	if (system(command) != 0)
	{
		sprintf(command, "mkdir -p %s", outputDir);
		system(command);
	}
	sprintf(command, "ls %s/chi2FoM%s", outputDir, outputExt);
	if (system(command) == 0)
	{
		sprintf(command, "rm -rf %s/chi2FoM%s", outputDir, outputExt); 
		system(command);
	}
	sprintf(command, "ls %s/chi2Stack%s", outputDir, outputExt);
	if (system(command) == 0)
	{
		sprintf(command, "rm -rf %s/chi2Stack%s", outputDir, outputExt); 
		system(command);
	}

	sprintf(command, "%s/chi2FoM%s", outputDir, outputExt);
	c1->SaveAs(command);
	sprintf(command, "%s/chi2Stack%s", outputDir, outputExt);
	c2->SaveAs(command);

//	
}

#ifndef __CINT__
int main(int argc, char** argv) {
	chi2FoM(argc, argv);
	return 0;
}
#endif
