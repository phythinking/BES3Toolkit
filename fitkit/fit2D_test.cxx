#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooHist.h"
#include "TCut.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "TChain.h"
#include "RooArgSet.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLine.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooHistPdf.h"
#include "RooFFTConvPdf.h"
#include "TFile.h"
#include "TTree.h"
#include "RooFormulaVar.h"
#include "RooArgusBG.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "RooProdPdf.h"
#include "TRandom3.h"
#include "TH2D.h"
#include "stdio.h"
#include "TLatex.h"
#include "RooNDKeysPdf.h"
#include "RooKeysPdf.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace RooFit;
void fit2D_test()
{

    TString inputRoot      = "/path/to/your/analysis/root/ana_data.root";
    TString outputpng      = "mkeys.png";
    TString treeName       = "rec";
    TString branchName     = "mlampi";
	Double_t bins	=	100;
	Double_t low1	=	1.2;
	Double_t up1 	=	2.2;
	Double_t low2	=	0.54;
	Double_t up2 	=	1.54;
	int dof = 4;

    RooRealVar mlampi("mlampi", "", low1, up1);	
    RooRealVar mkpi("mkpi", "", low2, up2);	

	TFile *f1 = new TFile(inputRoot);
	TTree *t1 = (TTree *)f1->Get(treeName);
    RooDataSet data("data","data", t1, RooArgSet(mlampi,mkpi)); 

	//inclusive MC
	TFile *f2 = new TFile("/path/to/your/analysis/root/ana_INC_MC.root");
	TTree *t2 = (TTree *)f2->Get(treeName);
    RooDataSet bkg("histpdf1","histpdf1",t2,RooArgSet(mlampi,mkpi));
	bkg.Print();
	RooKeysPdf bkglpi("bkglpi","bkglpipdf",mlampi,bkg,RooKeysPdf::MirrorBoth);
	RooKeysPdf bkgkst("bkgkst","bkgkstpdf",mkpi,bkg,RooKeysPdf::MirrorBoth);
	RooProdPdf bkgpdf_2D("bkgpdf","bkgpdf",bkglpi,bkgkst);

    //Chebyshiv polynomials
	// RooRealVar p0_1("p0_1_poly", "", -5, 5);//para[11]
	// RooRealVar p1_1("p1_1_poly", "", -5, 5);//para[12]
	// RooRealVar p2_1("p2_1_poly", "", -5, 5);//para[13]
    // RooRealVar p3_1("p3_1_poly", "", -5, 5);//para[14]
	// RooRealVar p0_2("p0_2_poly", "", -5, 5);//para[11]
	// RooRealVar p1_2("p1_2_poly", "", -5, 5);//para[12]
	// RooRealVar p2_2("p2_2_poly", "", -5, 5);//para[13]
    // RooRealVar p3_2("p3_2_poly", "", -5, 5);//para[14]
	// RooChebychev poly1_pdf("poly1_pdf", "Polynomial1 PDF", mlampi, RooArgSet(p0_1, p1_1,p2_1));
	// RooChebychev poly2_pdf("poly2_pdf", "Polynomial2 PDF", mkpi, RooArgSet(p0_2, p1_2,p2_2));
	// RooProdPdf bkgpdf_2D("bkgpdf","bkgpdf",poly1_pdf,poly2_pdf);


	//PHSP MC
	TFile *f3 = new TFile("/path/to/your/analysis/root/ana_Sig_MC.root");
	TTree *t3 = (TTree *)f3->Get(treeName);
    RooDataSet phsp("phsppdf","phsppdf",t3,RooArgSet(mlampi,mkpi));
	RooKeysPdf phsplpipdf("phsplpi","pshplpipdf",mlampi,phsp,RooKeysPdf::MirrorBoth);
	RooKeysPdf phspkstpdf("phspkst","pshpkstpdf",mkpi,phsp,RooKeysPdf::MirrorBoth);
	RooProdPdf phsppdf_2D("phsppdf","phsppdf",phsplpipdf,phspkstpdf);

	//1385 MC
	// TFile *f4 = new TFile("/path/to/your/analysis/root/ana_bkg_1385.root");
	// TTree *t4 = (TTree *)f4->Get(treeName);
    // RooDataSet sig1("sig1pdf","sig1pdf",t4,RooArgSet(mlampi,mkpi));
	// RooKeysPdf sig1lpipdf("sig1lpi","sig1lpipdf",mlampi,sig1,RooKeysPdf::MirrorBoth);
	// RooKeysPdf sig1kstpdf("sig1kst","sig1kstpdf",mkpi,sig1,RooKeysPdf::MirrorBoth);
	RooRealVar mean3("mean_BW", "",1.385, 1.38, 1.39);
	RooRealVar sigma3("sigma_BW","",0.001,0.,0.1);
	RooBreitWigner sig1lpipdf("sig1lpi","sig1lpipdf", mlampi, mean3, sigma3);
    RooRealVar argpar_1("argpar_1","argus shape parameter",-10.,10.);
    RooRealVar argpar_2("argpar_2","argus shape parameter",-10.,10.);
    RooRealVar argpar_3("argpar_3","argus shape parameter",-10.,10.);
	// RooGenericPdf sig1kstpdf("sig1kst", "pow((mkpi>argpar_3)*(1-(2-mkpi/argpar_3)**2),argpar_2)*(2*argpar_3-mkpi)*exp(argpar_1*(1-(2-mkpi/argpar_3)**2))", RooArgSet(mkpi, argpar_1, argpar_2, argpar_3));
	RooPolynomial sig1kstpdf("sig1kst", "Polynomial PDF", mkpi, RooArgSet(argpar_1, argpar_2,argpar_3));

	//892 MC
	// TFile *f5 = new TFile("/path/to/your/analysis/root/ana_bkg_kst.root");
	// TTree *t5 = (TTree *)f5->Get(treeName);
    // RooDataSet sig2("sig2pdf","sig2pdf",t5,RooArgSet(mlampi,mkpi));
	// RooKeysPdf sig2lpipdf("sig2lpi","sig2lpipdf",mlampi,sig2,RooKeysPdf::MirrorBoth);
	// RooKeysPdf sig2kstpdf("sig2kst","sig2kstpdf",mkpi,sig2,RooKeysPdf::MirrorBoth);
	RooRealVar mean4("mean_kstBW", "",0.892, 0.891, 0.893);
	RooRealVar sigma4("sigma_kstBW","",0.001,0.,0.1);
	RooBreitWigner sig2kstpdf("sig2kst","sig2kstpdf", mkpi, mean4, sigma4);
    RooRealVar argpar1("argpar1","argus shape parameter",-10.,10.);
    RooRealVar argpar2("argpar2","argus shape parameter",-10.,10.);
    RooRealVar argpar3("argpar3","argus shape parameter",-10.,10.);
	// RooGenericPdf sig2lpipdf("sig2lpi", "pow((mlampi>argpar3)*(1-(2-mlampi/argpar3)**2),argpar2)*(2*argpar3-mlampi)*exp(argpar1*(1-(2-mlampi/argpar3)**2))", RooArgSet(mlampi, argpar1, argpar2, argpar3));
	RooPolynomial sig2lpipdf("sig2lpi", "Polynomial PDF", mlampi, RooArgSet(argpar1, argpar2,argpar3));

    //Gaussian
    RooRealVar mean1("mean", "", 0., -1, 1);//para[7]
	RooRealVar sigma1("sigma_GS", "", 0.01, 0., 1);//para[8]
	RooGaussian gauss1("gaussian", "", mlampi, mean1, sigma1);
	RooFFTConvPdf sig1lpi_conv("sig1lpi_conv","sig1lpi_conv",mlampi,sig1lpipdf,gauss1);

    RooRealVar mean2("mean2", "", 0., -1, 1);//para[7]
	RooRealVar sigma2("sigma_GS2", "", 0.01, 0., 1);//para[8]
	RooGaussian gauss2("gaussian2", "", mkpi, mean2, sigma2);
	RooFFTConvPdf sig2kst_conv("sig2kst_conv","sig2kst_conv",mkpi,sig2kstpdf,gauss2);

	RooProdPdf signal1_2D("signal1_2D","signal2D",sig1lpi_conv,sig1kstpdf);
	RooProdPdf signal2_2D("signal2_2D","signal2D",sig2lpipdf,sig2kst_conv);

    RooRealVar nsig1("nsig1","#signal1 events",3000,0.,5000);
    RooRealVar nsig2("nsig2","#signal2 events",3000,0.,5000);
    RooRealVar nphsp("nphsp","#background events",3000,0.,10000);
    RooRealVar nbkg("nbkg","#background events",800,500.,1000);

    RooAddPdf sum("sum","2D combined pdf",RooArgList(signal1_2D,signal2_2D,phsppdf_2D,bkgpdf_2D),RooArgList(nsig1,nsig2,nphsp,nbkg));

	sum.Print();

	RooFitResult *fitres = sum.fitTo(data, Save(kTRUE), Extended());
	const RooArgList &floatingParams = fitres->floatParsFinal();
	int numFloatingParams = floatingParams.getSize();

	RooPlot* frame_mlampi = mlampi.frame(Bins(bins), Range(low1, up1));
	RooPlot* frame_mkpi = mkpi.frame(Bins(bins), Range(low2, up2));

	data.plotOn(frame_mlampi, MarkerColor(kBlack), MarkerSize(1.0), Name("h_data"));
	sum.plotOn(frame_mlampi, LineColor(kRed),Name("hehe"));
	sum.plotOn(frame_mlampi, Components(signal1_2D), LineColor(kBlue),Name("sig1"));
	sum.plotOn(frame_mlampi, Components(signal2_2D), LineColor(kOrange),Name("sig2"));
	sum.plotOn(frame_mlampi, Components(phsppdf_2D), LineColor(kGreen),Name("phsp"));
	sum.plotOn(frame_mlampi, Components(bkgpdf_2D), LineColor(kBlack),LineStyle(2),Name("bkg"));
	sum.paramOn(frame_mlampi, Parameters(RooArgSet(nsig1,nsig2,nphsp,nbkg)), RooFit::Layout(0.6, 0.99));
	RooCurve *cur1all = (RooCurve *)frame_mlampi->findObject("hehe");
	RooCurve *cur1sig1 = (RooCurve *)frame_mlampi->findObject("sig1");
	RooCurve *cur1sig2 = (RooCurve *)frame_mlampi->findObject("sig2");
	RooCurve *cur1phsp = (RooCurve *)frame_mlampi->findObject("phsp");
	RooCurve *cur1inc = (RooCurve *)frame_mlampi->findObject("bkg");

	data.plotOn(frame_mkpi, MarkerColor(kBlack), MarkerSize(1.0),Name("h_data"));
	sum.plotOn(frame_mkpi, LineColor(kRed),Name("hehe"));
	sum.plotOn(frame_mkpi, Components(signal1_2D), LineColor(kBlue),Name("sig1"));
	sum.plotOn(frame_mkpi, Components(signal2_2D), LineColor(kOrange),Name("sig2"));
	sum.plotOn(frame_mkpi, Components(phsppdf_2D), LineColor(kGreen),Name("phsp"));
	sum.plotOn(frame_mkpi, Components(bkgpdf_2D), LineColor(kBlack),LineStyle(2),Name("bkg"));

	RooCurve *cur2all = (RooCurve *)frame_mkpi->findObject("hehe");
	RooCurve *cur2sig1 = (RooCurve *)frame_mkpi->findObject("sig1");
	RooCurve *cur2sig2 = (RooCurve *)frame_mkpi->findObject("sig2");
	RooCurve *cur2phsp = (RooCurve *)frame_mkpi->findObject("phsp");
	RooCurve *cur2inc = (RooCurve *)frame_mkpi->findObject("bkg");

	RooHist* pullHist_mlampi = frame_mlampi->pullHist("h_data","hehe");
	RooHist* pullHist_mkpi = frame_mkpi->pullHist("h_data","hehe");
	RooPlot* frame_pull_mlampi = mlampi.frame(Bins(bins), Range(low1, up1), Title("Pull Distribution"));
	RooPlot* frame_pull_mkpi = mkpi.frame(Bins(bins), Range(low2, up2), Title("Pull Distribution"));

	frame_pull_mlampi->addPlotable(pullHist_mlampi, "P");
	frame_pull_mlampi->GetYaxis()->SetRangeUser(-5,5);
	frame_pull_mlampi->GetYaxis()->SetLabelSize(0.10);
	frame_pull_mlampi->GetYaxis()->SetTickLength(0);
	frame_pull_mkpi->addPlotable(pullHist_mkpi, "P");
	frame_pull_mkpi->GetYaxis()->SetRangeUser(-5,5);
	frame_pull_mkpi->GetYaxis()->SetLabelSize(0.10);
	frame_pull_mkpi->GetYaxis()->SetTickLength(0);

	// 创建画布
	TCanvas* c1 = new TCanvas("c", "Fit Result", 1800, 980);

    c1->Divide(2, 2, 0.01, 0.01);

    c1->GetPad(1)->SetPad(0.01, 0.3, 0.5, 1);
    c1->GetPad(2)->SetPad(0.5, 0.3, 1, 1);
    c1->GetPad(3)->SetPad(0.01, 0.01, 0.5, 0.3);
    c1->GetPad(4)->SetPad(0.5, 0.01, 1, 0.3);

	c1->cd(1);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.06);
	gPad->SetTopMargin(0.03);
	gPad->SetBottomMargin(0.12);
	frame_mlampi->SetTitle("");
	frame_mlampi->SetYTitle("Events/0.01");
	frame_mlampi->SetXTitle("M(#bar{#Lambda}#pi^{-}) (GeV/c^{2})");
	frame_mlampi->GetXaxis()->SetLabelFont(52);
	frame_mlampi->GetXaxis()->SetLabelSize(0.04);
	frame_mlampi->GetXaxis()->SetLabelOffset(0.01);
	frame_mlampi->GetXaxis()->SetNdivisions(510);
	frame_mlampi->GetXaxis()->SetTitleFont(52);
	frame_mlampi->GetXaxis()->SetTitleColor(1);
	frame_mlampi->GetXaxis()->SetTitleSize(0.04);
	frame_mlampi->GetXaxis()->SetTitleOffset(1.1);
	frame_mlampi->GetXaxis()->CenterTitle();
	frame_mlampi->GetYaxis()->CenterTitle();    
	frame_mlampi->Draw();

	TLegend *leg1 = new TLegend(0.64, 0.45, 0.9, 0.68);
    leg1->SetTextSize(0.02);
    leg1->SetFillColor(0);
    leg1->SetBorderSize(0);
    leg1->SetTextFont(42);
    leg1->AddEntry(cur1all , "PDF: Total", "l");
    leg1->AddEntry(cur1sig1, "PDF: sig1 #Sigma(1385)", "l");
    leg1->AddEntry(cur1sig2, "PDF: sig2 K^{*}(892)", "l");
    leg1->AddEntry(cur1phsp, "PDF: PHSP", "l");
    leg1->AddEntry(cur1inc , "PDF: Background", "l");
	leg1->Draw();

	double chi2_mlampi = frame_mlampi->chiSquare("hehe","h_data",numFloatingParams);
	TPaveText *pt1 = new TPaveText(0.6, 0.3, 0.9, 0.45, "BRNDC");
	pt1->SetBorderSize(0);
	pt1->SetFillColor(0);
	pt1->SetTextAlign(12);
	pt1->SetTextSize(0.04);
	pt1->SetTextFont(42);
	TString P1ar5V2 = Form("%2.1f", chi2_mlampi);
	TString P1ar52 = "#chi^{2}#/d.o.f = " + P1ar5V2;
	TText *text1 = pt1->AddText(P1ar52);
	pt1->Draw();


	c1->cd(2);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.06);
	gPad->SetTopMargin(0.03);
	gPad->SetBottomMargin(0.12);
	frame_mkpi->SetTitle("");
	frame_mkpi->SetYTitle("Events/0.01");
	frame_mkpi->SetXTitle("M(K^{0}_{s}#pi^{-}) (GeV/c^{2})");
	frame_mkpi->GetXaxis()->SetLabelFont(52);
	frame_mkpi->GetXaxis()->SetLabelSize(0.04);
	frame_mkpi->GetXaxis()->SetLabelOffset(0.01);
	frame_mkpi->GetXaxis()->SetNdivisions(510);
	frame_mkpi->GetXaxis()->SetTitleFont(52);
	frame_mkpi->GetXaxis()->SetTitleColor(1);
	frame_mkpi->GetXaxis()->SetTitleSize(0.04);
	frame_mkpi->GetXaxis()->SetTitleOffset(1.1);
	frame_mkpi->GetXaxis()->CenterTitle();
	frame_mkpi->GetYaxis()->CenterTitle();    
	frame_mkpi->Draw();
	TLegend *leg2 = new TLegend(0.6, 0.5, 0.9, 0.73);
    leg2->SetTextSize(0.02);
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->SetTextFont(42);
    leg2->AddEntry(cur1all , "PDF: Total", "l");
    leg2->AddEntry(cur1sig1, "PDF: #Sigma(1385)", "l");
    leg2->AddEntry(cur1sig2, "PDF: K^{*}(892)", "l");
    leg2->AddEntry(cur1phsp, "PDF: PHSP", "l");
    leg2->AddEntry(cur1inc , "PDF: Background", "l");
	leg2->Draw();

	double chi2_mkpi = frame_mkpi->chiSquare("hehe","h_data",numFloatingParams);
	TPaveText *pt2 = new TPaveText(0.6, 0.3, 0.9, 0.5, "BRNDC");
	pt2->SetBorderSize(0);
	pt2->SetFillColor(0);
	pt2->SetTextAlign(12);
	pt2->SetTextSize(0.04);
	pt2->SetTextFont(42);
	TString Par5V2 = Form("%2.1f", chi2_mkpi );
	TString Par52 = "#chi^{2}#/d.o.f = " + Par5V2;
	TText *text2 = pt2->AddText(Par52);
	pt2->Draw();


	c1->cd(3);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.06);
	gPad->SetTopMargin(0.03);
	gPad->SetBottomMargin(0.12);
	frame_pull_mlampi->SetTitle("");
	frame_pull_mlampi->SetYTitle("Pull");
	// frame_pull_mlampi->SetXTitle("mlampi");
	frame_pull_mlampi->GetXaxis()->SetLabelFont(52);
	frame_pull_mlampi->GetXaxis()->SetLabelSize(0.08);
	frame_pull_mlampi->GetXaxis()->SetLabelOffset(0.01);
	frame_pull_mlampi->GetXaxis()->SetNdivisions(510);
	frame_pull_mlampi->GetXaxis()->SetTitleFont(52);
	frame_pull_mlampi->GetXaxis()->SetTitleColor(1);
	frame_pull_mlampi->GetYaxis()->SetTitleSize(0.08);
	frame_pull_mlampi->GetXaxis()->SetTitleOffset(1.1);
	frame_pull_mlampi->GetXaxis()->CenterTitle();
	frame_pull_mlampi->GetYaxis()->CenterTitle();    
	frame_pull_mlampi->Draw();
	TLine *lineUP = new TLine(low1, 3, up1, 3);
	lineUP->SetLineColor(kRed);
	lineUP->SetLineStyle(2);
	lineUP->Draw();
	TLine *lineDOWN = new TLine(low1, -3, up1, -3);
	lineDOWN->SetLineColor(kRed);
	lineDOWN->SetLineStyle(2);
	lineDOWN->Draw();
	TLine *lineCen = new TLine(low1, 0, up1, 0);
	lineCen->SetLineColor(kBlack);
	lineCen->SetLineStyle(2);
	lineCen->Draw();
	c1->cd(4);
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.06);
	gPad->SetTopMargin(0.03);
	gPad->SetBottomMargin(0.12);
	frame_pull_mkpi->SetTitle("");
	frame_pull_mkpi->SetYTitle("Pull");
	// frame_pull_mkpi->SetXTitle("mkpi");
	frame_pull_mkpi->GetXaxis()->SetLabelFont(52);
	frame_pull_mkpi->GetXaxis()->SetLabelSize(0.08);
	frame_pull_mkpi->GetXaxis()->SetLabelOffset(0.01);
	frame_pull_mkpi->GetXaxis()->SetNdivisions(510);
	frame_pull_mkpi->GetXaxis()->SetTitleFont(52);
	frame_pull_mkpi->GetXaxis()->SetTitleColor(1);
	frame_pull_mkpi->GetYaxis()->SetTitleSize(0.08);
	frame_pull_mkpi->GetXaxis()->SetTitleOffset(1.1);
	frame_pull_mkpi->GetXaxis()->CenterTitle();
	frame_pull_mkpi->GetYaxis()->CenterTitle();    
	frame_pull_mkpi->Draw();
	TLine *lineUP1 = new TLine(low2, 3, up2, 3);
	lineUP1->SetLineColor(kRed);
	lineUP1->SetLineStyle(2);
	lineUP1->Draw();
	TLine *lineDOWN1 = new TLine(low2, -3, up2, -3);
	lineDOWN1->SetLineColor(kRed);
	lineDOWN1->SetLineStyle(2);
	lineDOWN1->Draw();
	TLine *lineCen1 = new TLine(low2, 0, up2, 0);
	lineCen1->SetLineColor(kBlack);
	lineCen1->SetLineStyle(2);
	lineCen1->Draw();

	// 保存图像
	c1->SaveAs(outputpng);

	// 打印拟合结果
	fitres->Print("v");
	cout << "Number of floating parameters: " << numFloatingParams << endl;
}
