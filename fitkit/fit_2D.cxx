#include <iostream>
#include <fstream>
using namespace std;
using namespace RooFit;

void fit_2D()
{
    const double xoff = 1.35;
    const double yoff = 1.5;

	RooRealVar metap("metap","",0.92,1.02);
	RooRealVar mm2mft("mm2mft","",-0.25,0.50);

  TFile *F1 = TFile::Open("root/data.root");
  TTree *T1 =(TTree*) F1->Get("signal");
  RooDataSet D1("D1","",T1,RooArgSet(metap,mm2mft));
  double nevt=D1.sumEntries();

// hist model
//h_1 =  new TH2F("h_1","",100,0.92,1.02,100,-0.25,0.5);
//TChain F2("signal");
//F2.Add("root/shape_signal.root");
//F2.Draw("mm2mft:metap>>h_1");
//RooDataHist* D2 = new RooDataHist("D2", "", RooArgSet(metap,mm2mft), h_1);
//RooHistPdf sig("sig","sig",RooArgSet(metap,mm2mft),*D2,2);

//RooPlot *f=metap.frame();
//sig.plotOn(f,Project(RooArgSet(mm2mft)));
//f->Draw();
//break;

// this is for data line model
  TFile *F2 = TFile::Open("root/shape_signal.root");
  TTree* T2 =(TTree*) F2->Get("signal");
  RooDataSet D2("D2","",T2,RooArgSet(metap,mm2mft));
  RooNDKeysPdf sig("sig","",RooArgSet(metap,mm2mft),D2,"am");
     
  TFile *F3 = TFile::Open("root/shape_etappi.root");
  TTree* T3 =(TTree*) F3->Get("signal");
  RooDataSet D3("D3","",T3,RooArgSet(metap,mm2mft));
  RooNDKeysPdf bkg1("bkg1","",RooArgSet(metap,mm2mft),D3,"am");

//h_2 =  new TH2F("h_2","",100,0.92,1.02,100,-0.25,0.5);
//TChain F4("signal");
//F4.Add("root/shape_peak.root");
//F4.Draw("mm2mft:metap>>h_2");
//RooDataHist* D4 = new RooDataHist("D4", "", RooArgSet(metap,mm2mft), h_2);
//RooHistPdf bkg2("bkg2","bkg2",RooArgSet(metap,mm2mft),*D4,2);
  
  TFile *F4 = TFile::Open("root/shape_peak.root");
  TTree* T4 =(TTree*) F4->Get("signal");
  RooDataSet D4("D4","",T4,RooArgSet(metap,mm2mft));
  RooNDKeysPdf bkg2("bkg2","",RooArgSet(metap,mm2mft),D4,"am");

  TFile *F5 = TFile::Open("root/shape_bkg.root");
  TTree* T5 =(TTree*) F5->Get("signal");
  RooDataSet D5("D5","",T5,RooArgSet(metap,mm2mft));
  RooNDKeysPdf bkg3("bkg3","",RooArgSet(metap,mm2mft),D5,"am");

double p_1=0,p_2=0;
ifstream in1("fix_peak/fixnum0.dat");
in1>>p_1>>p_2;
double p_3=0,p_4=0;
ifstream in2("fix_peak/fixnum1.dat");
in2>>p_3>>p_4;

	RooRealVar nsig("nsig","",0.94*nevt,0.,nevt);
	RooRealVar B_1("B_1","",0.94*nevt,0.,nevt);
	RooRealVar B_2("B_2","",0.94*nevt,0.,nevt);
	RooRealVar npeak1("npeak1","",p_1);
	RooRealVar npeak2("npeak2","",p_3);
	RooAddPdf model("model","",RooArgList(sig,bkg2,bkg1,bkg3),RooArgList(nsig,npeak1,npeak2,B_1));


Double_t ra1,rb1,rc1;
        Double_t edm = 1000.;
        Int_t status = 1;
        Int_t covQual = 0;
        TRandom3 r1(19);
int time=0;
    while (time<=10000&&(edm>200||status!=0||covQual!=3)) {

	RooFitResult *fitresult = model.fitTo(D1,Save(),Extended(kTRUE),Warnings(kFALSE),PrintEvalErrors(-1));

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

        RooPlot* frame2 = metap.frame(30);

        D1.plotOn(frame2,MarkerSize(0.7));
        model.plotOn(frame2,LineWidth(2.0));
double chi2_1 = frame2->chiSquare(2);

  RooHist* hpull1 = frame2->pullHist() ;
  RooPlot* framePull1 = metap.frame(Title("Pull distribution")) ;
  framePull1->addPlotable(hpull1,"P") ;
  framePull1->GetYaxis()->SetRangeUser(-5,5);
  framePull1->GetYaxis()->SetLabelSize(0.10);
framePull1->GetYaxis()->SetTickLength(0);
framePull1->Draw();

        model.plotOn(frame2,Components(sig),LineWidth(2.0),LineColor(1),LineStyle(2));
        model.plotOn(frame2,Components(bkg3),LineWidth(2.0),LineColor(2),LineStyle(10));
        model.plotOn(frame2,Components(RooArgSet(bkg1,bkg2)),LineWidth(2.0),LineColor(3),LineStyle(7));

        RooPlot* frame3 = mm2mft.frame(30);
        D1.plotOn(frame3,MarkerSize(0.7));
        model.plotOn(frame3,LineWidth(2.0));
  RooHist* hpull2 = frame3->pullHist() ;
double chi2_2 = frame3->chiSquare(2);

cout<<__LINE__<<"\t"<<chi2_1<<"\t"<<chi2_2<<endl;

  RooPlot* framePull2 = mm2mft.frame(Title("Pull distribution")) ;
  framePull2->addPlotable(hpull2,"P") ;
  framePull2->GetYaxis()->SetRangeUser(-5,5);
  framePull2->GetYaxis()->SetLabelSize(0.10);
framePull2->GetYaxis()->SetTickLength(0);

        model.plotOn(frame3,Components(sig),LineWidth(2.0),LineColor(1),LineStyle(2));
        model.plotOn(frame3,Components(bkg3),LineWidth(2.0),LineColor(2),LineStyle(10));
        model.plotOn(frame3,Components(RooArgSet(bkg1,bkg2)),LineWidth(2.0),LineColor(3),LineStyle(7));

	TH2D *h2D = (TH2D*)D1.createHistogram("mm2mft:metap",100,100);
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

  double mFitMin(0.92), mFitMax(1.02);
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


  mFitMin=-0.25, mFitMax=0.5;
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
        c1->Print("jpg/signal.eps");
        c1->Print("jpg/signal.jpg");

	metap.setRange("signal1",0.946,0.97) ;
	mm2mft.setRange("signal1",-0.25,0.5) ;

	RooAbsReal* igxy_sig = sig.createIntegral(RooArgSet(metap,mm2mft),NormSet(RooArgSet(metap,mm2mft)),Range("signal1")) ;
	double fr = igxy_sig->getVal();

	double Nsigw = (nsig.getVal())*fr;
	cout<<"aaa "<<Nsigw<<endl;


ofstream  out;
out.open("sig_num.dat");

double   n1_p1= nsig.getVal();
double   n1_p2= nsig.getError();
out<<n1_p1<<"\t"<<n1_p2<<endl;
out.close();



    TFile *f1 = new TFile("signal.root","UPDATE");
    frame2->Write("tag",TObject::kOverwrite);
    framePull1->Write("chitag",TObject::kOverwrite);
    framePull2->Write("chisig",TObject::kOverwrite);
    frame3->Write("sig",TObject::kOverwrite);
    f1->Close();

}

	
