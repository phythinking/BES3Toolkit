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
#include "RooSimultaneous.h"
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
#include "RooMispartition.h"
#include "RooCategory.h"
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

int main()
{
    const double xoff = 1.35;
    const double yoff = 1.5;

	RooRealVar mbc_tag("mbc_tag","",1.8365,1.8865);
	RooRealVar mbc_sig("mbc_sig","",1.8365,1.8865);

TChain F1("komega");
F1.Add("../data.root");
TTree *t1=F1.CopyTree("abs(m_234-0.782)<0.04");
TTree *t2=F1.CopyTree("(m_234>0.6&&m_234<0.7)||(m_234>0.85&&m_234<0.95)");
RooDataSet data_1("data_1","",t1,RooArgSet(mbc_tag,mbc_sig));
RooDataSet data_2("data_2","",t2,RooArgSet(mbc_tag,mbc_sig));

double nevt=data_1.sumEntries()+data_2.sumEntries();
cout<<nevt<<endl;

RooCategory sample("sample", "sample");
sample.defineType("tag1");
sample.defineType("tag2");

RooDataSet combData1("combData1","combined data",RooArgSet(mbc_tag,mbc_sig),Index(sample),Import("tag1",data_1));
RooDataSet combData2("combData2","combined data",RooArgSet(mbc_tag,mbc_sig),Index(sample),Import("tag2",data_2));
combData1.append(combData2);

TFile *hist1 = TFile::Open("../shape/diy.root");
TTree* mbcshape =(TTree*) hist1->Get("komega");
RooDataSet D2mbc("D2mbc","ntuple of D candidates",mbcshape,RooArgSet(mbc_tag,mbc_sig));
RooNDKeysPdf sig("sig","a Pdf from TTree",RooArgSet(mbc_tag,mbc_sig),D2mbc,"am");//Can also be smoother with 2,3; 3 is for sigma
RooNDKeysPdf sig2("sig2","a Pdf from TTree",RooArgSet(mbc_tag,mbc_sig),D2mbc,"am");//Can also be smoother with 2,3; 3 is for sigma


     RooKeysPdf *sigpdf=new RooKeysPdf("sigpdf","",mbc_sig,D2mbc,RooKeysPdf::MirrorBoth,1);
     RooKeysPdf *tagpdf=new RooKeysPdf("tagpdf","",mbc_tag,D2mbc,RooKeysPdf::MirrorBoth,1);

//     TFile *S_peak1 = TFile::Open("../shape/peak_sig_1.root");
//     RooKeysPdf *masswindow=(RooKeysPdf*)S_peak1->Get("peak_sig1");
//
//     TFile *S_peak2 = TFile::Open("../shape/peak_sig_2.root");
//     RooKeysPdf *sideband=(RooKeysPdf*)S_peak2->Get("peak_sig2");
//     RooProdPdf m_peak("m_peak","",RooArgSet(*tagpdf,*masswindow));
//     RooProdPdf s_peak("s_peak","",RooArgSet(*tagpdf,*sideband));

ifstream par("/path/to/your/parameter.dat");
double para[3];for(int i=0;i<3;i++)par>>para[i];
RooRealVar m0("m0","",1.8865);
RooRealVar c_1("c_1","",-100.,0.);
RooRealVar c_2("c_2","",-100.,0.);
RooRealVar c_3("c_3","",para[0]);
RooArgusBG tagbkg("tagbkg","",mbc_tag,m0,c_1);
RooArgusBG sigbkg("sigbkg","",mbc_sig,m0,c_2);
RooRealVar s_1("s_1","",para[1]);
RooRealVar scor("scor","",para[2]);
RooRealVar m02("m02","",2.6679);
RooRealVar p("p","",0.5);
	RooMispartition cbkg("cbkg","",mbc_tag,mbc_sig,scor,s_1,m02,c_3,p);

	RooProdPdf wtag_1("wtag_1","",RooArgSet(tagbkg,*sigpdf));
	RooProdPdf wsig_1("wsig_1","",RooArgSet(*tagpdf,sigbkg));
	RooProdPdf wbk_1("wbk_1","",RooArgSet(tagbkg,sigbkg));
//	double p_1=0,p_2=0;
//	ifstream in1("../../fix_peak/delE/dat/value_35.dat");
//	in1>>p_1>>p_2;


	RooRealVar nsig_2("nsig_2","",0.94*nevt,0.,nevt);
	RooRealVar nwtag_2("nwtag_2","",0.03*nevt,0.0,nevt);
	RooRealVar nwsig_2("nwsig_2","",0.02*nevt,0.0,nevt);
	RooRealVar nwbk_2("nwbk_2","",0.02*nevt,0.0,nevt);
	RooRealVar ncbkg_2("ncbkg_2","",0.05*nevt,0.0,nevt);
	//RooAddPdf model_2("model_2","",RooArgList(sig,cbkg,s_peak),RooArgList(nsig_2,ncbkg_2,n_sideband));
	RooAddPdf model_2("model_2","",RooArgList(sig,cbkg),RooArgList(nsig_2,ncbkg_2));

	RooRealVar nsig_1("nsig_1","",0.94*nevt,0.,nevt);
RooFormulaVar npeak_2("npeak_2","","@0/4.12",RooArgList(nsig_2));
//	RooRealVar n_masswindow("n_masswindow","",p_1);
	RooRealVar nwtag_1("nwtag_1","",0.03*nevt,0.0,nevt);
	RooRealVar nwsig_1("nwsig_1","",0.02*nevt,0.0,nevt);
	RooRealVar nwbk_1("nwbk_1","",0.02*nevt,0.0,nevt);
	RooRealVar ncbkg_1("ncbkg_1","",0.05*nevt,0.0,nevt);
	//RooAddPdf model_1("model_1","",RooArgList(sig,cbkg,sig2,m_peak),RooArgList(nsig_1,ncbkg_1,npeak_2,n_masswindow));
	RooAddPdf model_1("model_1","",RooArgList(sig,cbkg,sig2),RooArgList(nsig_1,ncbkg_1,npeak_2));

RooSimultaneous sum("sum", "simultaneous pdf", sample);

sum.addPdf(model_1, "tag1");
sum.addPdf(model_2, "tag2");


Double_t ra1,rb1,rc1;
        Double_t edm = 1000.;
        Int_t status = 1;
        Int_t covQual = 0;
        TRandom3 r1(112);
int time=0;
    while (time<=10000&&(edm>200||status!=0||covQual!=3)) {


	RooFitResult *fitresult = sum.fitTo(combData1,Save(),Extended(kTRUE),Warnings(kFALSE),PrintEvalErrors(-1),Minos(kTRUE));

	edm = fitresult->edm();
	status = fitresult->status();
	covQual = fitresult->covQual();

for (UInt_t j=0; j<fitresult->numStatusHistory(); j++) {
                  if (fitresult->statusCodeHistory(j) != 0) {
                  cout << "Wrong status return from " << fitresult->statusLabelHistory(j) << endl;
                  status = fitresult->statusCodeHistory(j);
                  }
                }
                fitresult->printValue(cout);
fitresult->Print();
time++;
	}
cout<<__LINE__<<"\t"<<nsig_1.getVal()<<"\t"<<nsig_1.getErrorLo()<<"\t"<<nsig_1.getErrorHi()<<endl;
for(int i=0;i<2;i++){
ofstream  out;
out.open(Form("sig_num%d.dat",i));

double   n1_p1= nsig_1.getVal();
double   n1_p2= nsig_1.getErrorLo();
double   n1_p3= nsig_1.getErrorHi();
double   n2_p1= nsig_2.getVal();
double   n2_p2= nsig_2.getErrorLo();
double   n2_p3= nsig_2.getErrorHi();
if(i==0)out<<n1_p1<<"\t"<<n1_p2<<"\t"<<n1_p3<<endl;
if(i==1)out<<n2_p1<<"\t"<<n2_p2<<"\t"<<n2_p3<<endl;
out.close();

        RooPlot* frame2 = mbc_tag.frame(20);
if(i==0){
        data_1.plotOn(frame2,MarkerSize(0.7));
        model_1.plotOn(frame2,LineWidth(2.0));
}
if(i==1){
        data_2.plotOn(frame2,MarkerSize(0.7));
        model_2.plotOn(frame2,LineWidth(2.0));
}

        RooHist* hpull1 = frame2->pullHist() ;
        RooPlot* framePull1 = mbc_tag.frame(Title("Pull distribution")) ;
        framePull1->addPlotable(hpull1,"P") ;
        framePull1->GetYaxis()->SetRangeUser(-5,5);
        framePull1->GetYaxis()->SetLabelSize(0.10);
        framePull1->GetYaxis()->SetTickLength(0);
        framePull1->Draw();
if(i==0){
        model_1.plotOn(frame2,Components(sig),LineWidth(2.0),LineColor(1),LineStyle(2));
        model_1.plotOn(frame2,Components(cbkg),LineWidth(2.0),LineColor(2),LineStyle(10));
        model_1.plotOn(frame2,Components(sig2),LineWidth(2.0),LineColor(3),LineStyle(7));
}
if(i==1){
        model_2.plotOn(frame2,Components(sig),LineWidth(2.0),LineColor(1),LineStyle(2));
        model_2.plotOn(frame2,Components(cbkg),LineWidth(2.0),LineColor(2),LineStyle(10));
}
        RooPlot* frame3 = mbc_sig.frame(20);
if(i==0){
        data_1.plotOn(frame3,MarkerSize(0.7));
        model_1.plotOn(frame3,LineWidth(2.0));
}
if(i==1){
        data_2.plotOn(frame3,MarkerSize(0.7));
        model_2.plotOn(frame3,LineWidth(2.0));
}
  RooHist* hpull2 = frame3->pullHist() ;
double chi2_2 = frame3->chiSquare();


  RooPlot* framePull2 = mbc_sig.frame(Title("Pull distribution")) ;
  framePull2->addPlotable(hpull2,"P") ;
  framePull2->GetYaxis()->SetRangeUser(-5,5);
  framePull2->GetYaxis()->SetLabelSize(0.10);
framePull2->GetYaxis()->SetTickLength(0);
if(i==0){
        model_1.plotOn(frame3,Components(sig),LineWidth(2.0),LineColor(1),LineStyle(2));
        model_1.plotOn(frame3,Components(cbkg),LineWidth(2.0),LineColor(2),LineStyle(10));
        model_1.plotOn(frame3,Components(sig2),LineWidth(2.0),LineColor(3),LineStyle(7));
}
if(i==1){
        model_2.plotOn(frame3,Components(sig),LineWidth(2.0),LineColor(1),LineStyle(2));
        model_2.plotOn(frame3,Components(cbkg),LineWidth(2.0),LineColor(2),LineStyle(10));
}
TH2D *h2D;
if(i==0)	h2D = (TH2D*)data_1.createHistogram("mbc_sig:mbc_tag",100,100);
if(i==1)	h2D = (TH2D*)data_2.createHistogram("mbc_sig:mbc_tag",100,100);
	h2D->GetXaxis()->CenterTitle( kTRUE );
	h2D->SetTitleOffset( xoff, "x" );
	h2D->SetXTitle( "M_{BC}^{sig} (GeV/#font[12]{c}^{2})" );
	h2D->GetYaxis()->CenterTitle( kTRUE );
	h2D->GetXaxis()->SetLabelSize(0.05);
	h2D->GetYaxis()->SetLabelSize(0.05);
	h2D->GetXaxis()->SetTitleSize(0.05);
	h2D->GetYaxis()->SetTitleSize(0.05);
	h2D->GetYaxis()->SetNdivisions(505);
	h2D->GetXaxis()->SetNdivisions(505);
	h2D->SetTitleOffset( yoff, "y" );
	h2D->SetYTitle( "M_{BC}^{tag} (GeV/#font[12]{c}^{2})" );

        TCanvas *c1 = new TCanvas("c1","c1",1600,600);
        c1->Divide(3,1);

        c1->cd(1);

        TPad *pad1 = new TPad("pad1","pad1",0.01,0.20,0.99,0.99);
        TPad *pad2 = new TPad("pad2","pad2",0.01,0.01,0.99,0.20);
        pad1->Draw();
        pad2->Draw();

  pad1->cd();
  pad1->SetLeftMargin(0.15);
  pad1->SetRightMargin(0.05);/////////////////

  frame2->Draw();
  frame2->SetMarkerSize(0.5);
  pad2->cd();
  pad2->SetLeftMargin(0.15);
  pad2->SetRightMargin(0.05);
  framePull1->SetMarkerSize(0.05);
  framePull1->Draw();

  double mFitMin(1.8365), mFitMax(1.8865);
  TLine *lineUP = new TLine(mFitMin, 3, mFitMax, 3);
  lineUP->SetLineColor(kRed);
  lineUP->SetLineStyle(2);
  lineUP->Draw();
  TLine *lineDOWN = new TLine(mFitMin, -3, mFitMax, -3);
  lineDOWN->SetLineColor(kRed);
  lineDOWN->SetLineStyle(2);
  lineDOWN->Draw();

        c1->cd(2);

        TPad *pad3 = new TPad("pad3","pad1",0.01,0.20,0.99,0.99);
        TPad *pad4 = new TPad("pad4","pad2",0.01,0.01,0.99,0.20);
        pad3->Draw();
        pad4->Draw();
  pad3->cd();
//gPad->SetLogy();
  pad3->SetLeftMargin(0.15);
  pad3->SetRightMargin(0.05);/////////////////
  frame3->Draw();
  frame3->SetMarkerSize(0.5);
  pad4->cd();
  pad4->SetLeftMargin(0.15);
  pad4->SetRightMargin(0.05);
  framePull2->SetMarkerSize(0.05);
  framePull2->Draw();


  TLine *lineUP1 = new TLine(mFitMin, 3, mFitMax, 3);
  lineUP1->SetLineColor(kRed);
  lineUP1->SetLineStyle(2);
  lineUP1->Draw();

  TLine *lineDOWN1 = new TLine(mFitMin, -3, mFitMax, -3);
  lineDOWN1->SetLineColor(kRed);
  lineDOWN1->SetLineStyle(2);
  lineDOWN1->Draw();

  TLine *lineDOWN3 = new TLine(mFitMin, 0, mFitMax, 0);
  lineDOWN3->SetLineColor(1);
  lineDOWN3->SetLineStyle(2);
  lineDOWN3->Draw();
    c1->cd(3);
    h2D->Draw();
        c1->Print(Form("jpg/komega%d.eps",i));

    TFile *f1 = new TFile(Form("komega%d.root",i),"UPDATE");
    frame2->Write("tag",TObject::kOverwrite);
    framePull1->Write("chitag",TObject::kOverwrite);
    framePull2->Write("chisig",TObject::kOverwrite);
    frame3->Write("sig",TObject::kOverwrite);
    f1->Close();

delete c1;
delete h2D;
delete frame2;
delete frame3;
delete framePull1;
delete framePull2;


        mbc_tag.setRange("signal1",1.863,1.875) ;
        mbc_sig.setRange("signal1",1.863,1.875) ;

        RooAbsReal* igxy_sig = sig.createIntegral(RooArgSet(mbc_tag,mbc_sig),NormSet(RooArgSet(mbc_tag,mbc_sig)),Range("signal1")) ;
        double fr = igxy_sig->getVal();

        double Nsigw = (nsig_1.getVal())*fr;



ofstream out1;
out1.open("datamc.dat");
out1<<Nsigw<<"\t"<<t1->GetEntries("mbc_sig>1.863&&mbc_sig<1.875&&mbc_tag>1.863&&mbc_tag<1.875")<<endl;
out1.close();
}

}

	
