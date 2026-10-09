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
#include "RooMispartition.h"
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
double meanx = 0;
double meany = 0;
double meanx_err = 0;
double meany_err=0;
double sigmax = 0;
double sigmay = 0;
double sigmax_err = 0;
double sigmay_err = 0;
int main()
{
for(int A=1;A<2;A++){ 

        ifstream aa1;
        aa1.open("../shape_60/sig_par.log");

        ifstream aa2;
        aa2.open("../shape_60/tag_par.log");

                aa1 >> meanx >> sigmax;
                aa2 >> meany >> sigmay;

                cout<<"the mean and sigma of sig is:"<< meanx << "---"<<sigmax<<endl;
                cout<<"the mean and sigma of tag is:"<< meany << "---"<<sigmay<<endl;

    const double xoff = 1.35;
    const double yoff = 1.5;

	RooRealVar mbc_tag("mbc_tag","",1.8365,1.8865);
	RooRealVar mbc_sig("mbc_sig","",1.8365,1.8865);

  TFile *gettag = TFile::Open("../data/data.root");

  TTree *tree =(TTree*) gettag->Get("mark_60");

  RooDataSet data("data","",tree,RooArgSet(mbc_tag,mbc_sig));

double nevt=data.sumEntries();

 TFile *hist1 = TFile::Open("../shape_60/diy.root");
  TTree* mbcshape =(TTree*) hist1->Get("mark_60");
  RooDataSet D2mbc("D2mbc","ntuple of D candidates",mbcshape,RooArgSet(mbc_tag,mbc_sig));

  RooNDKeysPdf sig("sig","a Pdf from TTree",RooArgSet(mbc_tag,mbc_sig),D2mbc,"am");//Can also be smoother with 2,3; 3 is for sigma

cout<<"now begin for smear:"<<endl;

                RooDataSet *sigsetold = sig.generate(RooArgSet(mbc_sig,mbc_tag),50000);
                TH2F *sighnew = new TH2F("sighnew","",100,1.8365,1.8865,100,1.8365,1.8865);
                TRandom G1;
                TRandom G2;
                G1.SetSeed(244);
                G2.SetSeed(255);
                for(int i=0; i<50000; i++){
if (i % 500 == 0) cout<<i<<endl;
                        double xval, yval;
                        xval = (sigsetold->get(i))->getRealValue("mbc_sig");
                        yval = (sigsetold->get(i))->getRealValue("mbc_tag");

                        for(int j=0; j<10000; j++){
                                double g1 = G1.Gaus(meanx,sigmax);
                                double g2 = G2.Gaus(meany,sigmay);

                                double xnew = xval + g1;
                                double ynew = yval + g2;
                                sighnew->Fill(xnew,ynew);
                        }
                }

                RooDataHist sighistMC("sighistMC", "sighistMC", RooArgSet(mbc_sig,mbc_tag), sighnew);
                RooHistPdf sigPdf("sigPdf","sigPdf",RooArgSet(mbc_sig,mbc_tag), sighistMC, 2);
                RooHistPdf sigPdf2("sigPdf2","sigPdf2",RooArgSet(mbc_sig,mbc_tag), sighistMC, 2);

cout<<"now end for smear"<<endl;

    TH1D *bb=sighnew->ProjectionX("bb");
    RooDataHist sigbb("sigbb", "sigbb",mbc_sig,bb);
    RooHistPdf sigpdf("sigpdf","sigpdf",mbc_sig,sigbb, 2);
    
    TH1D *aa=sighnew->ProjectionY("aa");
    RooDataHist tagaa("tagaa", "tagaa",mbc_tag,aa);
    RooHistPdf tagpdf("tagpdf","tagpdf",mbc_tag,tagaa, 2);
//                RooKeysPdf *sigpdf=new RooKeysPdf("sigpdf","",mbc_sig,D2mbc,RooKeysPdf::MirrorBoth,1);
//                RooKeysPdf *tagpdf=new RooKeysPdf("tagpdf","",mbc_tag,D2mbc,RooKeysPdf::MirrorBoth,1);


//     TFile *S_sig = TFile::Open("../shape_60/shape_sig.root");
//     RooKeysPdf *sigpdf=(RooKeysPdf*)S_sig->Get("sig_sig");
//
//     TFile *S_tag = TFile::Open("../shape_60/shape_tag.root");
//     RooKeysPdf *tagpdf=(RooKeysPdf*)S_tag->Get("sig_tag");

#if 1
     RooRealVar m0("m0","",1.8865);
     RooRealVar c_1("c_1","",-100.,-0.);
     RooRealVar c_2("c_2","",-100.,-0.);
     RooRealVar c_3("c_3","",-100.,-0.);
     RooArgusBG tagbkg("tagbkg","",mbc_tag,m0,c_1);
     RooArgusBG sigbkg("sigbkg","",mbc_sig,m0,c_2);
     //	RooArgusBG covbkg("covbkg","",mbc_sig,m0,c_3);
     RooRealVar frac("frac","",0.,0.,1.);
     RooRealVar sigma3("sigma3","",1.0250e-02 ,0.000,0.05);
     RooRealVar sigma4("sigma4","",1.9963e-03,0.000,0.05);
     RooRealVar scor("scor","",-3.2548e-01,-3,3);
     RooRealVar m02("m02","",2.6679);
     RooRealVar p("p","",0.5);
     RooMispartition cbkg("cbkg","",mbc_tag,mbc_sig,scor,sigma3,m02,c_3,p);

     RooRealVar b1("b1","",-100,0.);
     RooRealVar b2("b2","",-100,0.);
     RooArgusBG tag_bkg("tag_bkg","",mbc_tag,m0,b1);
     RooArgusBG sig_bkg("sig_bkg","",mbc_sig,m0,b2);

     RooProdPdf wtag("wtag","",RooArgSet(tag_bkg,sigpdf));
     RooProdPdf wsig("wsig","",RooArgSet(tagpdf,sig_bkg));
     RooProdPdf wbk("wbk","",RooArgSet(tag_bkg,sig_bkg));

     RooRealVar nsig("nsig","",0.94*nevt,0.,nevt);
     RooRealVar nsig2("nsig2","",119.57);
     RooRealVar nwtag("nwtag","",0.03*nevt,0.0,nevt);
     RooRealVar nwsig("nwsig","",0.02*nevt,0.0,nevt);
     RooRealVar nwbk("nwbk","",0.02*nevt,0.0,nevt);
     //	nwsig.setConstant(kTRUE);
     RooRealVar ncbkg("ncbkg","",0.05*nevt,0.0,nevt);
     RooAddPdf model("model","",RooArgList(sigPdf,sigPdf2,wtag,wsig,cbkg,wbk),RooArgList(nsig,nsig2,nwtag,nwsig,ncbkg,nwbk));
     //	RooAddPdf model("model","",RooArgList(sig,sig2,wtag,wsig,cbkg,wbk),RooArgList(nsig,nsig2,nwtag,nwsig,ncbkg,nwbk));
     //	RooAddPdf model("model","",RooArgList(wsig,cbkg,wbk),RooArgList(nwsig,ncbkg,nwbk));

     int seedn;
     ifstream seedx;
     seedx.open("../seed/seed1.dat");
     for (int i=0; i<1; i++) {
	     seedx >> seedn;
     }

     int seed1=seedn*11;
     int seed2=seedn*22;
     int seed3=seedn*33;
     int seed4=seedn*44;
     int seed5=seedn*55;
     int seed6=seedn*66;
     int seed7=seedn*77;



     Double_t ra1,rb1,rc1;
     Double_t edm = 1000.;
     Int_t status = 1;
     Int_t covQual = 0;
     TRandom3 r1(seed1);
     TRandom3 r2(seed2);
     TRandom3 r3(seed3);
     TRandom3 r4(seed4);
     TRandom3 r5(seed5);
     TRandom3 r6(seed6);
     TRandom3 r7(seed7);
     int time=0;
Lab:
     while (time<=10000&&(edm>200||status!=0||covQual!=3)) {

	     c_1.setVal(r1.Uniform(-100,0.));
	     c_2.setVal(r2.Uniform(-100,0.));
	     c_3.setVal(r3.Uniform(-100,0.));
	     b1.setVal(r3.Uniform(-100,0.));
	     b2.setVal(r3.Uniform(-100,0.));
	     sigma3.setVal(r4.Uniform(0.,0.05));
	     sigma4.setVal(r5.Uniform(0.,0.05));
	     scor.setVal(r6.Uniform(-3,3));
	     frac.setVal(r7.Uniform(0.,1));

	     ra1=r1.Uniform(0.,nevt);
	     rb1=r2.Uniform(0.,nevt-ra1);
	     rc1=r3.Uniform(0.,nevt-ra1-rb1);

	     //	nsig.setVal(ra1);
	     //	nwtag.setVal(rb1);
	     //	nwsig.setVal(rc1);
	     //	ncbkg.setVal(nevt-ra1-rb1-rc1);
	     //RooFitResult *fitresult = model.fitTo(data,Save(),Extended(kTRUE),Warnings(kFALSE),PrintEvalErrors(-1),Minos(kTRUE));
	     RooFitResult *fitresult = model.fitTo(data,Save(),Extended(kTRUE),Warnings(kFALSE),PrintEvalErrors(-1));

	     //	w->import(model);
	     //	w->import(data);

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





     ofstream  out3;
     out3.open("parameter.dat");
     out3<<c_1.getVal()<<"\t"<<c_2.getVal()<<"\t"<<c_3.getVal()<<"\t"<<frac.getVal()<<"\t"<<sigma3.getVal()<<"\t"<<sigma4.getVal()<<"\t"<<scor.getVal()<<endl;
     out3.close();
     RooPlot* frame2 = mbc_tag.frame(50);

     data.plotOn(frame2,MarkerSize(0.5));
     model.plotOn(frame2,LineWidth(1));
     double chi2_1 = frame2->chiSquare();

     RooHist* hpull1 = frame2->pullHist() ;
     RooPlot* framePull1 = mbc_tag.frame(Title("Pull distribution")) ;
     framePull1->addPlotable(hpull1,"P") ;
     framePull1->GetYaxis()->SetRangeUser(-5,5);
     framePull1->GetYaxis()->SetLabelSize(0.10);
     framePull1->GetYaxis()->SetTickLength(0);
     framePull1->Draw();

     model.plotOn(frame2,Components(sigPdf),LineWidth(1.5),LineColor(1),LineStyle(3));
     model.plotOn(frame2,Components(sigPdf2),LineWidth(1.5),LineColor(7),LineStyle(1));
     model.plotOn(frame2,Components(cbkg),LineWidth(1.5),LineColor(2),LineStyle(10));
     model.plotOn(frame2,Components(wsig),LineWidth(1.5),LineColor(4),LineStyle(4));
//     model.plotOn(frame2,Components(wtag),LineWidth(1.5),LineColor(4),LineStyle(4));
     model.plotOn(frame2,Components(wbk),LineWidth(1.5),LineColor(6),LineStyle(9));

     RooPlot* frame3 = mbc_sig.frame(50);
     data.plotOn(frame3,MarkerSize(0.5));
     model.plotOn(frame3,LineWidth(1));
     RooHist* hpull2 = frame3->pullHist() ;
     double chi2_2 = frame3->chiSquare();

     cout<<__LINE__<<"\t"<<chi2_1<<"\t"<<chi2_2<<endl;

     RooPlot* framePull2 = mbc_sig.frame(Title("Pull distribution")) ;
     framePull2->addPlotable(hpull2,"P") ;
     framePull2->GetYaxis()->SetRangeUser(-5,5);
     framePull2->GetYaxis()->SetLabelSize(0.10);
     framePull2->GetYaxis()->SetTickLength(0);


ofstream  chi;
chi.open(Form("chi/chi_%d.dat",A));
chi<<chi2_1<<"\t"<<chi2_2<<endl;
chi.close();


     model.plotOn(frame3,Components(sigPdf),LineWidth(1.5),LineColor(1),LineStyle(3));
     model.plotOn(frame3,Components(sigPdf2),LineWidth(1.5),LineColor(7),LineStyle(1));
     model.plotOn(frame3,Components(cbkg),LineWidth(1.5),LineColor(2),LineStyle(10));
     model.plotOn(frame3,Components(wsig),LineWidth(1.5),LineColor(4),LineStyle(4));
//     model.plotOn(frame3,Components(wtag),LineWidth(1.5),LineColor(4),LineStyle(4));
     model.plotOn(frame3,Components(wbk),LineWidth(1.5),LineColor(6),LineStyle(9));

     TH2D *h2D = (TH2D*)data.createHistogram("mbc_sig:mbc_tag",100,100);
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
     //    TTree *Tree1 = (TTree*) f1->Get("kpiomega");
     //    Tree1->Draw("mbc2:mbc1>>h2D",cut);


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
//     c1->Print("jpg/mark_60.eps");
//     c1->Print("jpg/mark_60.jpg");

     ofstream  out;
     out.open(Form("sig_num/sig_num_%d.dat",A));

     double   n1_p1= nsig.getVal();
     double   n1_p2= nsig.getError();
     out<<n1_p1<<"\t"<<n1_p2<<endl;
     out.close();

/*
     mbc_tag.setRange("signal1",1.865,1.875) ;
     mbc_sig.setRange("signal1",1.865,1.875) ;

     RooAbsReal* igxy_sig = sig.createIntegral(RooArgSet(mbc_tag,mbc_sig),NormSet(RooArgSet(mbc_tag,mbc_sig)),Range("signal1")) ;
     RooAbsReal* igxy_2 = wsig.createIntegral(RooArgSet(mbc_tag,mbc_sig),NormSet(RooArgSet(mbc_tag,mbc_sig)),Range("signal1")) ;
     RooAbsReal* igxy_3 = cbkg.createIntegral(RooArgSet(mbc_tag,mbc_sig),NormSet(RooArgSet(mbc_tag,mbc_sig)),Range("signal1")) ;
     RooAbsReal* igxy_4 = wbk.createIntegral(RooArgSet(mbc_tag,mbc_sig),NormSet(RooArgSet(mbc_tag,mbc_sig)),Range("signal1")) ;

     double fr = igxy_sig->getVal();
     double fr2 = igxy_2->getVal();
     double fr3 = igxy_3->getVal();
     double fr4 = igxy_4->getVal();

     double Nsigw = (nsig.getVal())*fr;

     ofstream out1;
     out1.open("datamc.dat");
     out1<<Nsigw<<"\t"<<tree->GetEntries("mbc_sig>1.865&&mbc_sig<1.875&&mbc_tag>1.865&&mbc_tag<1.875")<<endl;
     out1<<Nsigw<<"\t"<<Nsigw+nwsig.getVal()*fr2+ncbkg.getVal()*fr3+nwbk.getVal()*fr4<<endl;
     out1.close();
*/

     //TFile *f1 = new TFile("mark_60.root","UPDATE");
     TFile *f1 = new TFile(Form("root/mark_60_%d.root",A),"UPDATE");
     frame2->Write("tag",TObject::kOverwrite);
     framePull1->Write("chitag",TObject::kOverwrite);
     framePull2->Write("chisig",TObject::kOverwrite);
     frame3->Write("sig",TObject::kOverwrite);
     f1->Close();

#endif	
}


}

