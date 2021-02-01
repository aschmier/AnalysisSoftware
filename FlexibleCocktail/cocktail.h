//
// Flexible decay photon calculation based on pre-calculated
// 2d decay histograms
//
// Klaus Reygers (reygers@physi.uni-heidelberg.de), April 2020
// Ilya Fokin (ilya.fokin@cern.ch)

class cocktail {

	const Int_t idPi0 = 111;
	const Int_t idEta = 221;
	const Int_t idEtaPrime = 331;
	const Int_t idOmegaMeson = 223;
	const Int_t idRho0 = 113;
	const Int_t idRhoPlus = 213;
	const Int_t idRhoMinus = -213;
	const Int_t idPhi = 333;
	const Int_t idJpsi = 443;
	const Int_t idDeltaMinus = 1114;
	const Int_t idDelta0 = 2114;
	const Int_t idDeltaPlus = 2214;
	const Int_t idDeltaPlusPlus = 2224;
	const Int_t idProton = 2212;
	const Int_t idSigma0 = 3212;
	const Int_t idAntiSigma0 = -3212;
	const Int_t idK0S = 310;
	const Int_t idK0L = 130;
	const Int_t idLambda = 3122;

	TDatabasePDG pdg;

	std::map<Int_t, TH2D *> decay_histos;

	// tiny number to protect against division by zero
	Double_t EPSILON = 1e-20;

public:

	void read_decay_histograms(const std::string filename, const std::vector<Int_t> particles, const int sec_id);
	void calc_decay_photons_from_histograms(const std::map<Int_t, TH1D> &dndpt_hadro, std::map<Int_t, TH1D> &dndpt_photon);
	void decay_photons_from_func_or_mt_scaling(const std::vector<Int_t> particles, const std::map<Int_t, TF1> dndpt_hadron_func, std::map<Int_t, TH1D> &dndpt_hadron, std::map<Int_t, TH1D> &dndpt_photon);

};