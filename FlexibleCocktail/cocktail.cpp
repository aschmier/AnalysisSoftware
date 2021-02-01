// read in decay histograms
// decay histograms store the decay photon pT distribution for a given 
// pT of the mother particle. They are determined use a MC tool like the PYTHIA decayer.

void cocktail::read_decay_histograms(const std::string filename, const std::vector<Int_t> particles, const int sec_id=22) {

	TFile f(filename.c_str());

	std::string h2d_name = "";
	Bool_t onlyPhotons = kFALSE;

	// check if there are decay histograms for the pure photon cocktail
	if ((TH1*) f.Get(("decay_hist_dndpt_photon_from_id" + std::to_string(particles[0])).c_str()))
		onlyPhotons = kTRUE;

	// loop over the list of particle
	for (auto const& id: particles) {

		if (onlyPhotons && sec_id == 22)
			h2d_name = "decay_hist_dndpt_photon_from_id" + std::to_string(id);
		else
			h2d_name = "decay_hist_dndpt_secondary_idprim" + std::to_string(id) + "_idsec"  + std::to_string(sec_id);

		TH2D* h2d = (TH2D*) f.Get(h2d_name.c_str());
		h2d->SetDirectory(0); // decouple histo from the file director

		std::string h_norm_name = "decay_hist_dndpt_primary_id" + std::to_string(id);
		TH1D* h_norm = (TH1D*) f.Get(h_norm_name.c_str());

		Int_t nb = h2d->GetNbinsX();

		// properly normalize 2d decay histograms
		for (Int_t ibx=1; ibx<=nb; ++ibx) {
			for (Int_t iby=1; iby<=nb; ++iby) {
				Double_t n_photons = h2d->GetBinContent(ibx, iby);
				Double_t n_mother = h_norm->GetBinContent(ibx);
				h2d->SetBinContent(ibx, iby, n_photons / n_mother);

				// uncertainties from error propagation
				Double_t dn_photons = h2d->GetBinError(ibx, iby);
				Double_t dn_mother = h_norm->GetBinError(ibx);
				h2d->SetBinError(ibx, iby, std::sqrt(std::pow(dn_photons / n_mother, 2) + std::pow(dn_mother * n_photons / std::pow(n_mother, 2), 2)));
			}
		}

		decay_histos[id] = h2d;
	}

	f.Close();
}

// Calculate decay photon spectra. Use parameterizations where available. Otherwise use mt scaling.
void cocktail::decay_photons_from_func_or_mt_scaling(const std::vector<Int_t> particles, const std::map<Int_t, TF1> dndpt_hadron_func, std::map<Int_t, TH1D> &dndpt_hadron, std::map<Int_t, TH1D> &dndpt_photon) {

	// mt scaling factors for hadron X / pi0
	std::map<Int_t, Double_t> C_mt = {
		{idEta, 0.45},
		{idOmegaMeson, 0.85},
		{idEtaPrime, 0.4}, 
		{idPhi, 0.35},
		{idSigma0, 0.49},
		{idAntiSigma0, 0.49}
	};

	// get histograms parameters
	// try to find eta decay histogram there is none for the pi0
	Int_t idFirstParticle = 0;
	if (decay_histos.count(idPi0) > 0) idFirstParticle = idPi0;
	else idFirstParticle = idEta;
	const Int_t nb = decay_histos.at(idFirstParticle)->GetNbinsX();
    const Int_t ptmin = decay_histos.at(idFirstParticle)->GetXaxis()->GetXmin();
    const Int_t ptmax = decay_histos.at(idFirstParticle)->GetXaxis()->GetXmax();
    TH1D h_hadron("h_hadron", "h_hadron", nb, ptmin, ptmax);

    dndpt_hadron.clear();
    dndpt_photon.clear();

    // prepare histograms containing the dn/dpT of the hadrons
	for (auto const& id: particles) {

        h_hadron.Reset();

        if (dndpt_hadron_func.count(id) > 0) {
        	// parameterization of the hadron dn/dpT spectrum is provided
            for (Int_t ib = 1; ib <= nb; ++ib) {
            	Double_t pt = h_hadron.GetBinCenter(ib);
            	h_hadron.SetBinContent(ib, dndpt_hadron_func.at(id).Eval(pt));
				// assume negligible uncertainties when using parametrizations
            	h_hadron.SetBinError(ib, EPSILON * dndpt_hadron_func.at(id).Eval(pt));
            }
        }
        else {
        	// no dn/dpT parameterization availabe, use mT scaling based on the pi0 parameterization

        	// select reference for mT scaling, pi0 spectrum for mesons, proton spectrum for baryons        	
        	Int_t id_ref = idPi0;
        	if (id % 10000 / 1000 != 0) id_ref = idProton;  // proton spectrum as reference for baryons
       
        	Double_t m_ref = pdg.GetParticle(id_ref)->Mass();

        	for (Int_t ib = 1; ib <= nb; ++ib) {
        		Double_t pt = h_hadron.GetBinCenter(ib);
            	Double_t m = pdg.GetParticle(id)->Mass();
            	Double_t pt_ref = TMath::Sqrt(pt*pt + m*m - m_ref * m_ref);
            	Double_t C = 1.;
            	if (C_mt.count(id) > 0) C = C_mt.at(id);  // mT scaling factor (if different from unity)
            	Double_t dndpt = C * pt / pt_ref * dndpt_hadron_func.at(id_ref).Eval(pt_ref);
            	h_hadron.SetBinContent(ib, dndpt);
            }
        }

		dndpt_hadron[id] = h_hadron;      
	}

	// calculate decay photon spectra based on hadron dn/dpT spectra
	calc_decay_photons_from_histograms(dndpt_hadron, dndpt_photon);

}

// calculate decay photon spectra based on hadron dn/dpT spectra
void cocktail::calc_decay_photons_from_histograms(const std::map<Int_t, TH1D> &dndpt_hadron, std::map<Int_t, TH1D> &dndpt_photon){

	// try to find eta decay histogram there is none for the pi0
	Int_t idFirstParticle = 0;
	if (decay_histos.count(idPi0) > 0) idFirstParticle = idPi0;
	else idFirstParticle = idEta; 
	const Int_t nb = decay_histos.at(idFirstParticle)->GetNbinsX();
    const Int_t ptmin = decay_histos.at(idFirstParticle)->GetXaxis()->GetXmin();
    const Int_t ptmax = decay_histos.at(idFirstParticle)->GetXaxis()->GetXmax();
    TH1D h_decay_photon("h_decay_photon", "h_decay_photon", nb, ptmin, ptmax);

    // here we take the particles of interest from the histograms containing the hadron dn/dpT spectra
	for (const auto &dndpt_hadron_pair : dndpt_hadron) {

		const Int_t id = dndpt_hadron_pair.first;
		const TH1D *dh = &dndpt_hadron_pair.second; // hadron dn/dpT histogram (input)

		h_decay_photon.Reset();

	  	Double_t n_decay_photons[nb+1]; // index 0 not used
		Double_t dn_decay_photons[nb+1];
 		for (Int_t ib=0; ib<=nb; ++ib){
			n_decay_photons[ib] = 0.;
			dn_decay_photons[ib] = 0.;
		}

		if (decay_histos.at(id)) {
			// multiply normalized 2d decay photon spectrum and hadron dn/dpT histogram
			for (Int_t ibx=1; ibx<=nb; ++ibx) {
				for (Int_t iby=1; iby<=nb; ++iby) {
					Double_t n_photons_norm = decay_histos.at(id)->GetBinContent(ibx, iby);
					Double_t n_mother = dndpt_hadron.at(id).GetBinContent(ibx);
					n_decay_photons[iby] += n_photons_norm * n_mother;

					// uncertainties from error propagation
					Double_t dn_photons_norm = decay_histos.at(id)->GetBinError(ibx, iby);
					Double_t dn_mother = dndpt_hadron.at(id).GetBinError(ibx);
					dn_decay_photons[iby] += std::pow(dn_photons_norm * n_mother, 2) + std::pow(dn_mother * n_photons_norm, 2);
				}
			}
		}

		for (Int_t ib=1; ib<=nb; ++ib) h_decay_photon.SetBinContent(ib, n_decay_photons[ib]);
		for (Int_t ib=1; ib<=nb; ++ib) h_decay_photon.SetBinError(ib, std::sqrt(dn_decay_photons[ib]));

		dndpt_photon[id] = h_decay_photon;

	}

}
