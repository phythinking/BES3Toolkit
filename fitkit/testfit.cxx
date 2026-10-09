#include "RooConstVar.h"
#include "RooFit.h"
using namespace RooFit;
void testfit()
{

    TString inputRoot      = "/path/to/your/analysis/root/ana_data.root";
    TString outputpng      = "m1385.png";
    TString treeName       = "rec";
    TString branchName     = "mlampi";
	Double_t bins	=	50;
	Double_t low	=	1.2;
	Double_t up 	=	2.2;
	int dof = 4;

	TFile *f1 = new TFile(inputRoot);
	TTree *t1 = (TTree *)f1->Get(treeName);
    TH1D *h1= new TH1D("h1","",bins,low,up);//binned fit
    t1->Project("h1",branchName);

    RooRealVar mes("mlampi", "", low, up);	
    RooDataHist data("data","data", mes, h1); 

	TFile *f2 = new TFile("/path/to/your/analysis/root/ana_INC_MC.root");
	TTree *t2 = (TTree *)f2->Get(treeName);
    TH1D *h2= new TH1D("h2","",bins,low,up);//binned fit
    t2->Project("h2",branchName);
    RooRealVar bkg("mrho", "", low, up);	
    RooDataHist inc("inc","inc", mes, h2);
    RooHistPdf hist1_pdf("histpdf1","histpdf1",mes,inc,2);

	TFile *f3 = new TFile("/path/to/your/analysis/root/ana_Sig_MC.root");
	TTree *t3 = (TTree *)f3->Get(treeName);
    TH1D *h3= new TH1D("h3","",25,low,up);//binned fit
    t3->Project("h3",branchName);
    RooDataHist signal("sig","sig", mes, h3);
    RooHistPdf sig_pdf("histpdf2","histpdf2",mes,signal,2);

	TFile *f4 = new TFile("/path/to/your/analysis/root/ana_bkg_1385.root");
	TTree *t4 = (TTree *)f4->Get(treeName);
    TH1D *h4= new TH1D("h4","",bins,low,up);//binned fit
    t4->Project("h4",branchName);
    RooDataHist signal1("sig1","sig1", mes, h4);
    RooHistPdf sig1_pdf("histpdf3","histpdf3",mes,signal1,2);

    //Gaussian
    RooRealVar mean1("mean", "", 1.385, 1.38, 1.39);//para[7]
	RooRealVar sigma1("sigma_GS", "", 0.02, 0., 0.1);//para[8]
	RooGaussian gauss_pdf("gaussian", "", mes, mean1, sigma1);

	//Breit-Wigner
	RooRealVar mean2("mean_BW", "",1.385, 1.38, 1.39);
	RooRealVar sigma2("sigma_BW","",0.02,0.,0.1);
	RooBreitWigner BW_pdf("BW", "", mes, mean2, sigma2);

    //Crystal Ball
	RooRealVar mean3("mean_CB", "", 0.77, 0.75, 0.78);//para[3]
	RooRealVar sigma3("sigma_CB", "", 0.0629, 0., 0.5);//para[4]
	RooRealVar a2("CB2_a", "", 2.639, 1.0, 1.55);//para[1]
	RooRealVar n2("CB2_n", "", 3.5, 3.0, 9.0);//para[2]
	RooCBShape CB_pdf("cb", "", mes, mean3, sigma3, a2, n2);

    //Chebyshiv polynomials
	RooRealVar p0("p0_Cheby", "", -5, 5);//para[11]
	RooRealVar p1("p1_Cheby", "",-5, 5);//para[12]
	RooRealVar p2("p2_Cheby", "", -5, 5);//para[13]
    // RooRealVar p3("p3_Cheby", "", -5, 5);//para[14]
	RooChebychev bkg_pdf("bkg_pdf", "", mes, RooArgList(p0,p1));

	// int degree = 3;
	// RooRealVar p0("p0", "p0", -10,10); // 初始化多项式系数
	// RooRealVar p1("p1", "p1", -10,10); // 根据需要添加更多的系数
	// RooRealVar p2("p2", "p2", -10,10); // 根据需要添加更多的系数
	// RooRealVar p3("p3", "p3", 0.0); // 根据需要添加更多的系数
	// RooPolynomial poly_pdf("poly_pdf", "Polynomial PDF", mes, RooArgSet(p0, p1,p2), degree);


    //Argus
    RooRealVar argpar("argpar","argus shape parameter",-20.0,0.,100.);
    RooArgusBG argus_pdf("argus","Argus PDF",mes,RooConst(1.25),argpar);

    RooRealVar nsig("nsig","#signal events",10000,0.,100000);
    RooRealVar nbkg("nPHSP","#background events",50000,0.,100000);
    RooRealVar nbkg2("nbkg","#background events",800,500,1000);

    RooAddPdf sum("sum","g+a",RooArgList(sig1_pdf,sig_pdf,hist1_pdf),RooArgList(nsig,nbkg,nbkg2));
	RooFitResult *fitres = sum.fitTo(data, Save(kTRUE), Extended());
    fitres->Print("v");
    
	RooPlot *frame_mes = mes.frame(Name("hehe"), Bins(bins), Range(low, up));
	data.plotOn(frame_mes, MarkerColor(kBlack), MarkerSize(1.0), DataError(RooAbsData::SumW2));
	//draw total pdf of data
	sum.plotOn(frame_mes, LineColor(kRed), Name("hehe"));
	//define chi2 and print it
	double chi2_mes = frame_mes->chiSquare(dof);
	TPaveText *pt2 = new TPaveText(0.30, 0.85, 0.50, 0.90, "BRNDC");
	pt2->SetBorderSize(0);
	pt2->SetFillColor(0);
	pt2->SetTextAlign(12);
	pt2->SetTextSize(0.04);
	pt2->SetTextFont(42);
	TString Par5V2 = Form("%2.1f", frame_mes->chiSquare(dof) );
	TString Par52 = "#chi^{2}#/d.o.f = " + Par5V2;
	TText *text2 = pt2->AddText(Par52);
	cout << "chi2_mes/ndf = " << chi2_mes << endl;

	sum.plotOn(frame_mes, Components(sig_pdf), LineColor(kBlue),Name("sig_pdf"));
	//  sum.plotOn(frame_mes, Components(gauss_pdf), LineColor(kGreen));
	// sum.plotOn(frame_mes, Components(CB_pdf), LineColor(kGreen));
	sum.plotOn(frame_mes, Components(sig1_pdf), LineColor(kGreen),Name("sig1_pdf"));
	
    sum.plotOn(frame_mes, Components(hist1_pdf), LineColor(kYellow), Name("bkg_pdf"));
    // sum.plotOn(frame_mes, Components(poly_pdf), LineColor(4), LineStyle(2));
    // sum.plotOn(frame_mes, Components(bkg_pdf), LineColor(kYellow), LineStyle(2));
	// sum.plotOn(frame_mes, Components(argus_pdf), LineColor(4), LineStyle(2));
	sum.paramOn(frame_mes, Parameters(RooArgSet(mean2,nsig,nbkg,nbkg2)), RooFit::Layout(0.55, 0.98));

	RooCurve *curveall = (RooCurve *)frame_mes->findObject("hehe");
	RooCurve *curvesig1 = (RooCurve *)frame_mes->findObject("sig1_pdf");
	RooCurve *curvesig = (RooCurve *)frame_mes->findObject("sig_pdf");
	RooCurve *curveinc = (RooCurve *)frame_mes->findObject("bkg_pdf");



    // RooCurve *curvebkg2 = (RooCurve *)frame_mes->findObject("model_Norm[umiss]_Comp[bkgpdf2]");

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
	frame_mes->SetXTitle("M(#bar{#Lambda}#pi^{-}) (GeV/c^{2})");
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

	TLegend *leg2 = new TLegend(0.6, 0.5, 0.9, 0.73);
    leg2->SetTextSize(0.02);
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->SetTextFont(42);
    leg2->AddEntry(curveall, "PDF: Total", "l");
    leg2->AddEntry(curvesig1, "PDF: #Sigma(1385)", "l");
    leg2->AddEntry(curvesig, "PDF: PHSP", "l");
    leg2->AddEntry(curveinc, "PDF: Background", "l");
	leg2->Draw();

    pt2->Draw();
	TArrow* myar = new TArrow(2.98, 1.1*height, 2.98, 0.0*height, 0.04, "|>");
	myar->SetLineColor(kRed);
	myar->SetFillColor(kRed);
	myar->SetLineWidth(3);
	myar->SetAngle(40);
	// myar->Draw();    

    c_mes->SaveAs(outputpng);

	double likelihoodValue = fitres->minNll();
	std::cout << "================================" << "\n";
	std::cout << "mean: "  << mean << "\n"; 
	std::cout << "sigma: " << sigma << "\n"; 
	std::cout << "likelihood: " << likelihoodValue << "\n"; 
	std::cout << "mean - 3sigma: " << mean - 3.0 * sigma << "\n"; 
	std::cout << "mean + 3sigma: " << mean + 3.0 * sigma << "\n"; 
	std::cout << "mean - 5sigma: " << mean - 5.0 * sigma << "\n"; 
	std::cout << "mean + 5sigma: " << mean + 5.0 * sigma << "\n"; 
}
