{
	TLorentzVector particle[7];
	TLorentzVector particle_raw[7];

	particle[0].SetPxPyPzE(vft4_1[0], vft4_1[1], vft4_1[2], vft4_1[3]);
	particle[1].SetPxPyPzE(vft4_2[0], vft4_2[1], vft4_2[2], vft4_2[3]);
	particle[2].SetPxPyPzE(vft4_3[0], vft4_3[1], vft4_3[2], vft4_3[3]);
	particle[3].SetPxPyPzE(vft4_4[0], vft4_4[1], vft4_4[2], vft4_4[3]);
	particle_raw[0].SetPxPyPzE(v4_1[0], v4_1[1], v4_1[2], v4_1[3]);
	particle_raw[1].SetPxPyPzE(v4_2[0], v4_2[1], v4_2[2], v4_2[3]);
	particle_raw[2].SetPxPyPzE(v4_3[0], v4_3[1], v4_3[2], v4_3[3]);
	particle_raw[3].SetPxPyPzE(v4_4[0], v4_4[1], v4_4[2], v4_4[3]);

	TLorentzVector pip     = particle[0];
	TLorentzVector pim     = particle[1];
	TLorentzVector gam1    = particle[2];
	TLorentzVector gam2    = particle[3];

	const double mpi0  = 0.1349766;
	const double metap = 0.95778;
	const double momega = 0.78265;

	double mpi0_tmp;
	double metap_tmp;
	double delta = 9999.;

	TLorentzVector rho   = pip  + pim;
	TLorentzVector pi0    = gam1 + gam2;

	// once define elements in var array, don't modify their order!	
	var[0] = rho.M();				// mJpsi
	var[1] = chisq_4c;				// chi2
	var[2] = pi0.M();				// mpi0
	var[3] = chisq_4c_less;				// chi2
	var[4] = chisq_4c_more;				// chi2

	//define cut conditions	
	std::vector<bool> cuts;
	cuts.push_back(fabs(var[0] - 0.770) < 0.5);	// 0 |mother.M() - mother_raw.M()| < 0.5 GeV

	bool condition = true;
	for (Int_t i = 0; i < cuts.size(); i++)
	{
		condition = condition && cuts[i];
	}
	if ( !condition )
		continue;

	Double_t var1D[30];
	var1D[0] = var[1];  /* mother.M()     */
	var1D[1] = var[0];  /* mother.M()     */
	// var1D[2] = var[1];  /* mother_raw.M() */ 
	fillTH1D (var1D, cutTH1Ds);

	Double_t var2D[30];
	var2D[0] = var[1];  
	var2D[1] = var[3]; /* mother.M() : mother_raw.M() -> x:y */
	var2D[2] = var[1]; 
	var2D[3] = var[4]; /* mother.M() : mother_raw.M() -> x:y */
	fillTH2D (var2D, cutTH2Ds);

}
