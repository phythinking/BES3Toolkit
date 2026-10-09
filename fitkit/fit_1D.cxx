#include "RooConstVar.h"
#include "RooFit.h"
using namespace RooFit;
void fit_1D()
{

    TString inputRoot      = "/path/to/your/analysis/root/ana_data.root";
    TString outputpng      = "metac.png";
    TString treeName       = "rec";
    TString branchName     = "metac4c";
	Double_t bins	=	100;
	Double_t low	=	2.85;
	Double_t up 	=	3.1;
	int dof = 4;

	TFile *f1 = new TFile(inputRoot);
	TTree *t1 = (TTree *)f1->Get(treeName);
    TH1D *h1= new TH1D("h1","",bins,low,up);//binned fit
    t1->Project("h1",branchName);

    RooRealVar mes("mrho", "", low, up);	
    RooDataHist data("data","data", mes, h1); 

	TFile *f2 = new TFile("/path/to/your/analysis/root/ana_INC_MC.root");
	TTree *t2 = (TTree *)f2->Get(treeName);
    TH1D *h2= new TH1D("h2","",bins,low,up);//binned fit
    t2->Project("h2",branchName);
    RooRealVar bkg("mrho", "", low, up);	
    RooDataHist inc("inc","inc", bkg, h2);
    RooHistPdf hist1_pdf("histpdf1","histpdf1",bkg,inc,0);

	TFile *f3 = new TFile("/path/to/your/analysis/root/ana_Sig_MC.root");
	TTree *t3 = (TTree *)f3->Get(treeName);
    TH1D *h3= new TH1D("h3","",bins,low,up);//binned fit
    t3->Project("h3",branchName);
    RooRealVar metac("metac", "", low, up);	
    RooDataHist signal("sig","sig", metac, h3);
    RooHistPdf sig_pdf("histpdf2","histpdf2",metac,signal,0);

    //Gaussian
    RooRealVar mean1("mean", "", 2.984, 2.97, 3.0);//para[7]
	RooRealVar sigma1("sigma_GS", "", 0.02, 0., 0.5);//para[8]
	RooGaussian gauss_pdf("gaussian", "", mes, mean1, sigma1);

	//Breit-Wigner
	RooRealVar mean2("mean_BW", "",2.984, 2.98, 2.99);
	RooRealVar sigma2("sigma_BW","",0.02,0.,0.1);
	RooBreitWigner BW_pdf("BW", "", mes, mean2, sigma2);

    //Crystal Ball
	RooRealVar mean3("mean_CB", "", 0.77, 0.75, 0.78);//para[3]
	RooRealVar sigma3("sigma_CB", "", 0.0629, 0., 0.5);//para[4]
	RooRealVar a2("CB2_a", "", 2.639, 1.0, 1.55);//para[1]
	RooRealVar n2("CB2_n", "", 3.5, 3.0, 9.0);//para[2]
	RooCBShape CB_pdf("cb", "", mes, mean3, sigma3, a2, n2);

    //Chebyshiv polynomials
	RooRealVar p0("p0_Cheby", "", -1, 1);//para[11]
	RooRealVar p1("p1_Cheby", "",-1, 1);//para[12]
	RooRealVar p2("p2_Cheby", "", -1, 1);//para[13]
    // RooRealVar p3("p3_Cheby", "", -1, 1);//para[14]
	RooChebychev bkg_pdf("bkg_pdf", "", mes, RooArgList(p0,p1,p2));

    //Argus
    RooRealVar argpar("argpar","argus shape parameter",-20.0,-100.,0.);
    RooArgusBG argus_pdf("argus","Argus PDF",mes,RooConst(3.2),argpar);

    RooRealVar nsig("nsig","#signal events",80,0.,100);
    RooRealVar nbkg("nbkg","#background events",1500,0.,10000);

    RooAddPdf sum("sum","g+a",RooArgList(gauss_pdf,bkg_pdf),RooArgList(nsig,nbkg)) ;
	RooFitResult *fitres = sum.fitTo(data, Save(kTRUE), Extended());
    fitres->Print("v");
    
	RooPlot *frame_mes = mes.frame(Name("hehe"), Bins(bins), Range(low, up));
	data.plotOn(frame_mes, MarkerColor(kBlack), MarkerSize(1.0), DataError(RooAbsData::SumW2));
	//draw total pdf of data
	sum.plotOn(frame_mes, LineColor(kRed));

	//define chi2 and print it
	double chi2_mes = frame_mes->chiSquare(dof);
	TPaveText *pt2 = new TPaveText(0.20, 0.80, 0.40, 0.90, "BRNDC");
	pt2->SetBorderSize(0);
	pt2->SetFillColor(0);
	pt2->SetTextAlign(12);
	pt2->SetTextSize(0.04);
	pt2->SetTextFont(42);
	TString Par5V2 = Form("%2.1f", frame_mes->chiSquare(dof) );
	TString Par52 = "#chi^{2}#/d.o.f = " + Par5V2;
	TText *text2 = pt2->AddText(Par52);
	cout << "chi2_mes/ndf = " << chi2_mes << endl;

//	sum.plotOn(frame_mes, Components(sig_pdf), LineColor(kBlue));
//	 sum.plotOn(frame_mes, Components(gauss_pdf), LineColor(kBlue));
	// sum.plotOn(frame_mes, Components(CB_pdf), LineColor(kGreen));
	sum.plotOn(frame_mes, Components(BW_pdf), LineColor(kBlue));
	
    // sum.plotOn(frame_mes, Components(hist1_pdf), LineColor(4), LineStyle(2));
    sum.plotOn(frame_mes, Components(bkg_pdf), LineColor(4), LineStyle(2));
	// sum.plotOn(frame_mes, Components(argus_pdf), LineColor(4), LineStyle(2));
	sum.paramOn(frame_mes, Parameters(RooArgSet(mean1,mean2,nsig,nbkg)), RooFit::Layout(0.6, 0.98));

   	TCanvas *c_mes = new TCanvas("eta", "eta",0,0,900,700);
	c_mes->SetLeftMargin(0.15);
	c_mes->SetRightMargin(0.06);
	c_mes->SetTopMargin(0.03);
	c_mes->SetBottomMargin(0.03);
	c_mes->SetFillColor(0);
	c_mes->Divide(1,1);
	c_mes->cd(1);

	frame_mes->SetTitle("");
	frame_mes->SetYTitle("Events");
	frame_mes->SetXTitle("M(#gamma_{h}#pi^{+}#pi^{-}) (GeV/c^{2})");
	frame_mes->GetXaxis()->SetLabelFont(52);
	frame_mes->GetXaxis()->SetLabelSize(0.04);
	frame_mes->GetXaxis()->SetLabelOffset(0.01);
	frame_mes->GetXaxis()->SetNdivisions(510);
	frame_mes->GetXaxis()->SetTitleFont(42);
	frame_mes->GetXaxis()->SetTitleColor(1);
	frame_mes->GetXaxis()->SetTitleSize(0.05);
	frame_mes->GetXaxis()->SetTitleOffset(0.9);
	frame_mes->GetXaxis()->CenterTitle();
	frame_mes->GetYaxis()->CenterTitle();    
    frame_mes->Draw();
    
   	double height = h1->GetMaximum();
	double mean = mean2.getVal();
    // double mean = Nf.getVal()*mean1.getVal() + (1-Nf.getVal())*mean2.getVal();
    double sigma = sigma2.getVal();
    // double sigma = sqrt(fabs(Nf.getVal()*(TMath::Power(mean1.getVal(),2)+TMath::Power(sigma1.getVal(),2))+(1-Nf.getVal())*(TMath::Power(mean2.getVal(),2)+TMath::Power(sigma2.getVal(),2))-TMath::Power(mean2.getVal(),2))); 


    pt2->Draw();
	TArrow* myar = new TArrow(2.98, 1.1*height, 2.98, 0.0*height, 0.04, "|>");
	myar->SetLineColor(kRed);
	myar->SetFillColor(kRed);
	myar->SetLineWidth(3);
	myar->SetAngle(40);
	// myar->Draw();    

    c_mes->SaveAs(outputpng);

	std::cout << "================================" << "\n";
	std::cout << "mean: "  << mean << "\n"; 
	std::cout << "sigma: " << sigma << "\n"; 
	std::cout << "mean - 3sigma: " << mean - 3.0 * sigma << "\n"; 
	std::cout << "mean + 3sigma: " << mean + 3.0 * sigma << "\n"; 
	std::cout << "mean - 5sigma: " << mean - 5.0 * sigma << "\n"; 
	std::cout << "mean + 5sigma: " << mean + 5.0 * sigma << "\n"; 
}