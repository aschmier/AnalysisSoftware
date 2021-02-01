#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TPythia8.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TTree.h"
#include "TList.h"
#include "TObjString.h"
#include <iostream>

// some particle id's
const Int_t idPhoton = 22;
const Int_t idPi0 = 111;
const Int_t idPiPlus = 211;
const Int_t idPiMinus = -211;
const Int_t idKPlus = 321;
const Int_t idKMinus = -321;
const Int_t idEta = 221;
const Int_t idEtaPrime = 331;
const Int_t idPhi = 333;
const Int_t idK0long = 130;  // (K0long to be excluded as primary particles)
const Int_t idK0short = 310; // (K0short to be excluded as primary particles)
const Int_t idRho0 = 113;
const Int_t idRhoPlus = 213;
const Int_t idRhoMinus = -213;
const Int_t idOmegaMeson = 223;
const Int_t idProton = 2212;
const Int_t idAntiProton = -2212;
const Int_t idNeutron = 2112;
const Int_t idLambda0 = 3122;
const Int_t idSigma0 = 3212;
const Int_t idAntiSigma0 = -3212;
const Int_t idSigmaMinus = 3112;
const Int_t idD0 = 421;
const Int_t idAntiD0 = -421;
const Int_t idJPsi = 443;
const Int_t idOmegaBaryon = 3334;
const Int_t idAntiOmegaBaryon = -3334;
const Int_t idUpsilon1S = 553;
const Int_t idUpsilon2S = 100553;
const Int_t idUpsilon3S = 200553;
const Int_t idDeltaPlusPlus = 2224;
const Int_t idDeltaPlus = 2214;
const Int_t idDeltaNull = 2114;
const Int_t idDeltaMinus = 1114;
const Int_t idKStar892 = 313;


std::vector<Int_t> custom_primary_particle_list(TPythia8 &tp) {

    std::cout << "custom particle list (cp)" << std::endl;

    // std::vector<Int_t> ids = {idPiPlus, idRho0, idDeltaPlusPlus, idDeltaPlus, idDeltaNull, idDeltaMinus};
    // std::vector<Int_t> ids = {idPiPlus, idRho0};
    std::vector<Int_t> ids = {idPi0, idRho0};
    for (auto &id : ids)
        std::cout << ">>> cp list: added " << tp.Pythia8()->particleData.name(id) << std::endl;

    return ids;
}

std::vector<Int_t> pythia_primary_particle_list(TPythia8 &tp, const double m_min, const double m_max) {

    // Start with pi0
    Int_t id_prim = 111; // pi0
    Int_t n_particles = 0;

    std::vector<Int_t> ids_pythia8;

    do {
        // mass
        const double m_prim = tp.Pythia8()->particleData.m0(id_prim);

        if (tp.Pythia8()->particleData.isHadron(id_prim) && m_prim > m_min && m_prim < m_max && id_prim != idK0long &&
            id_prim != idK0short) {

            // add the particle
            ids_pythia8.push_back(id_prim);
            n_particles++;
            std::cout << ">>> added " << tp.Pythia8()->particleData.name(id_prim)
                      << ", may decay = " << tp.Pythia8()->particleData.mayDecay(id_prim) << std::endl;

            // same for anti-particle if it exists
            if (tp.Pythia8()->particleData.hasAnti(id_prim)) {
                ids_pythia8.push_back(-id_prim);
                n_particles++;
                std::cout << ">>> added " << tp.Pythia8()->particleData.name(-id_prim)
                          << ", may decay = " << tp.Pythia8()->particleData.mayDecay(-id_prim) << std::endl;
            }
        }

        // get the next particle from Pythia's list
        id_prim = tp.Pythia8()->particleData.nextId(id_prim);

    } while (id_prim != 0);

    return ids_pythia8;
}

std::vector<Int_t> custom_measured_particle_list(TPythia8 &tp) {

    std::cout << "measured particle list (cp)" << std::endl;

    std::vector<Int_t> ids = {idPhoton, idPi0,        idPiPlus, idPiMinus, idEta,       idKPlus,
                              idKMinus, idOmegaMeson, idRho0,   idProton,  idAntiProton};
    for (auto &id : ids)
        std::cout << ">>> measured particle list: added " << tp.Pythia8()->particleData.name(id) << std::endl;

    return ids;
}

TPythia8 get_initialized_pythia_object() {

    TPythia8 tp;

    // use a random seed
    gRandom->SetSeed(0);
    Int_t rs = gRandom->Uniform(1, 900000000);
    std::cout << "Pythia seed = " << rs << std::endl;
    TString ps = Form("Random:seed = %d", rs);
    tp.Pythia8()->readString("Random:setSeed = On");
    tp.Pythia8()->readString(ps.Data());

    // Pythia settings
    // tp.Pythia8()->readString("SoftQCD:inelastic = on");
    tp.Pythia8()->readString("ProcessLevel:all = off");
    // tp.Pythia8()->readString("ParticleDecays:limitTau0 = on");
    // tp.Pythia8()->readString("ParticleDecays:tau0Max = 1 ! mm/c");
    tp.Pythia8()->readString("Next:numberShowInfo = 0");
    tp.Pythia8()->readString("Next:numberShowProcess = 0");
    tp.Pythia8()->readString("Next:numberShowEvent = 0");
    tp.Pythia8()->readString("Next:numberCount = 0");
    // this writes the default PYTHIA8 particle data to a text file
    // tp.Pythia8()->particleData.listFF("decaytable_pythia8_default.dat");

    // read particle information 
    // the option "false" means that only the information for particles found in the file
    // is replaced in the tables used by PYTHIA
    std::string decaytable = "decaytable_pythia8.dat";

    bool read_status_ok = tp.Pythia8()->particleData.readFF(decaytable, false);
    if (!read_status_ok) {
    	std::cout << " Terminating program: Could not find " << decaytable << std::endl;
    	std::exit(-11);
	}
    // one can list the information for certain particles
    // tp.Pythia8()->particleData.list(113);

    tp.Pythia8()->readString("130:mayDecay = on");
    tp.Pythia8()->readString("310:mayDecay = on");

    tp.Pythia8()->init();

    return tp;
}

std::map<Int_t, TH1F> book_histo_dndpt_primary(std::vector<Int_t> primary_ids, TPythia8 &tp, const Int_t n_pt_bins,
                                               const Double_t pt_min, const Double_t pt_max) {

    std::map<Int_t, TH1F> histo_dndpt_primary;

    for (auto const &id : primary_ids) {

        TString name = Form("decay_hist_dndpt_primary_id%d", id);
        TString title = tp.Pythia8()->particleData.name(id);
        histo_dndpt_primary[id] = TH1F(name, title, n_pt_bins, pt_min, pt_max);
        histo_dndpt_primary[id].Sumw2();
    }

    return histo_dndpt_primary;
}

auto compare_particles=[](int id1, int id2) { return std::abs(id2) != std::abs(id1) ? std::abs(id2) < std::abs(id1) : id2 < id1; };

void add_2d_histo(std::map<Int_t, std::map<Int_t, TH2F>> &m, TString prefix, const Int_t id_prim, const Int_t id_sec,
                  TPythia8 &tp, const Int_t n_pt_bins, const Double_t pt_min, const Double_t pt_max) {

    TString name = Form("%s_idprim%d_idsec%d", prefix.Data(), id_prim, id_sec);
    TString prim_particle_name = tp.Pythia8()->particleData.name(id_prim);
    TString sec_particle_name = tp.Pythia8()->particleData.name(id_sec);
    TString title = Form("%s from %s", sec_particle_name.Data(), prim_particle_name.Data());
    m[id_prim][id_sec] = TH2F(name, title, n_pt_bins, pt_min, pt_max, n_pt_bins, pt_min, pt_max);
    m[id_prim][id_sec].Sumw2();
}

void add_1d_photon_histo(std::map<Int_t, TH2F> &m, TString prefix, const Int_t id, TPythia8 &tp, const Int_t n_pt_bins,
                         const Double_t pt_min, const Double_t pt_max) {

    TString name = Form("%s_from_id%d", prefix.Data(), id);
    TString hadron_name = tp.Pythia8()->particleData.name(id);
    TString title = Form("photon from hadron decay, direct mother: %s", hadron_name.Data());
    m[id] = TH2F(name, title, n_pt_bins, pt_min, pt_max, n_pt_bins, pt_min, pt_max);
    m[id].Sumw2();
}

//
// the main program
//
// void generate_2d_hadron_decay_histograms(TString fn_out = "hadron_decays.root", const Int_t n_evt = 100000, const Int_t process_number = 0) {

int main(int argc, char *argv[]) {

    // default values
    TString fn_out = "hadron_decays.root";
    Int_t n_evt = 100000;
    Int_t process_number = 0;

    if (argc == 3) {
        std::cout << "Taking command line arguments." << std::endl;
        fn_out = argv[1];
        n_evt = std::stoi(argv[2]);
    } else if (argc == 4) {
        std::cout << "Taking 4 command line arguments." << std::endl;
        fn_out = argv[1];
        n_evt = std::stoi(argv[2]);
        process_number = std::stoi(argv[3]);
    }

    //
    // parameters
    //

    // define wether to fill calculate feeddown histograms or decay photon histograms or both
    bool calc_feeddown = true;
    bool calc_decay_photons = true;
    
    bool do_cos2phi = false;

    // create output tree for the afterburner
    TTree* output_tree = new TTree("cocktailSettings", "cocktailSettings");
    TList* user_info = (TList*) output_tree->GetUserInfo();

    // rapidity ranges (+/- max value) for particle generation and detection
    // const Double_t rap_max_gen = 1.2;
    // values for rap max from ALICE cocktail
    const Double_t rap_max_gen = 1.0;
    const Double_t rap_max_det = 0.8;

    // pt range and number of bins
    const Double_t pt_min = 0.;
    const Double_t pt_max = 200.;
    const Int_t n_pt_bins_det = 2000;

    // record cocktail settings
    user_info->Add(new TObjString("nParticles_1"));
    user_info->Add(new TObjString(Form("ptMin_%.2f", pt_min)));
    user_info->Add(new TObjString(Form("ptMax_%.2f", pt_max)));

    // get Pythia 8 object
    // http://home.thep.lu.se/~torbjorn/pythia81html/HadronLevelStandalone.html
    TPythia8 tp = get_initialized_pythia_object();

    // define primary particle list
    std::vector<Int_t> primary_particle_id = custom_primary_particle_list(tp);
    // Double_t m_min = 0.;
    // Double_t m_max = 2.;
    // std::vector<Int_t> primary_particle_id = pythia_primary_particle_list(tp, m_min, m_max);
    


    // define particles of interest (to be measured)
    std::vector<Int_t> measured_particle_id = custom_measured_particle_list(tp);
    
    std::vector<Int_t> hadrons_with_photon_decays;
    std::vector<TString> hadron_names;
    
    // in case only the decay photon calculation is done
    if (calc_decay_photons && !calc_feeddown) {
        // only decay photon calculation

        // hadrons_with_photon_decays{idPi0,     idEta,      idOmegaMeson, idEtaPrime, idRho0,
        //                            idRhoPlus, idRhoMinus, idPhi,        idSigma0,   idAntiSigma0};
        // list of hadrons used in the ALICE cocktail
        hadrons_with_photon_decays = {idPi0, idEta, idEtaPrime, idOmegaMeson, idRho0, idRhoPlus, idRhoMinus, idPhi,
                                      idJPsi, idDeltaMinus, idDeltaNull, idDeltaPlus, idDeltaPlusPlus, idSigma0,
                                      idK0short, idK0long, idLambda0};
        hadron_names = {"Pi0", "Eta", "EtaPrim", "omega", "rho0", "rho+", "rho-", "phi", "J/psi", "Delta-",
                        "Delta0", "Delta+", "Delta++", "Sigma0", "K0s", "K0l", "Lambda"};

        primary_particle_id = hadrons_with_photon_decays;

        measured_particle_id.clear();
        measured_particle_id.push_back(idPhoton);
    } else if (calc_decay_photons && calc_feeddown) {
        // particles with decays to pi0 or photons for cocktail and secondaries
        hadrons_with_photon_decays = {idPi0, idEta, idEtaPrime, idOmegaMeson, idRho0, idRhoPlus, idRhoMinus, idPhi,
                               idJPsi, idDeltaMinus, idDeltaNull, idDeltaPlus, idDeltaPlusPlus, idSigma0,
                               idK0short, idK0long, idLambda0, idKPlus, idKMinus, idOmegaBaryon, idAntiOmegaBaryon, idKStar892};
        hadron_names = {"Pi0", "Eta", "EtaPrim", "omega", "rho0", "rho+", "rho-", "phi", "J/psi", "Delta-",
                        "Delta0", "Delta+", "Delta++", "Sigma0", "K0s", "K0l", "Lambda", "K+", "K-",
                        "Omega+", "Omega-", "K*(892)0"};
        primary_particle_id = hadrons_with_photon_decays;
        
        measured_particle_id.clear();
        measured_particle_id.push_back(idPhoton);
        measured_particle_id.push_back(idPi0);
    }
    
        // create and fill the histogram with pythia branching ratios
    for (UInt_t idx = 0; idx < hadrons_with_photon_decays.size(); idx++) {
        Int_t particle_id = hadrons_with_photon_decays[idx];
        Pythia8::ParticleDataEntry* entry = tp.Pythia8()->particleData.particleDataEntryPtr(particle_id);

        Int_t  n_channels = entry->sizeChannels();
        TString hist_name = Form("PythiaBR_%s", hadron_names[idx].Data());
        TH1F* pythia_branching_ratios = new TH1F(hist_name, hist_name, n_channels + 1, -0.5, n_channels + 0.5);

        Double_t sum_br = 0;
        for (int i_channel = 0; i_channel < n_channels; i_channel++) {
            Pythia8::DecayChannel channel = entry->channel(i_channel);

            double br = channel.bRatio();
            sum_br += br;

            int products[8] = {0, 0, 0, 0, 0, 0, 0, 0};

            for (int i_prod = 0; i_prod < 8; i_prod++) {
                int prod_id = channel.product(i_prod);
                if (particle_id < 0 && tp.Pythia8()->particleData.hasAnti(prod_id))
                    prod_id = -prod_id;
                products[i_prod] = channel.product(i_prod);
            }
            std::sort(products, products + 8, compare_particles);

            TString bin_label = "";
            for (int i_prod = 0; i_prod < 8; i_prod++) {
                int prod = products[i_prod];
                if (prod != 0) {
                bin_label = bin_label + TString(tp.Pythia8()->particleData.name(prod) + " ");
                }
            }
            bin_label = bin_label.Strip();

            int i_bin = i_channel + 2;
            pythia_branching_ratios->GetXaxis()->SetBinLabel(i_bin, bin_label.Data());
            pythia_branching_ratios->SetBinContent(i_bin, br);
        }

        pythia_branching_ratios->GetXaxis()->SetBinLabel(1, "all");
        pythia_branching_ratios->SetBinContent(1, sum_br);
        pythia_branching_ratios->Sumw2(kFALSE);

        user_info->Add(pythia_branching_ratios);
    }

    std::set<Int_t> set_of_measured_primary_particle_ids;
    for (auto const &id : measured_particle_id)
        set_of_measured_primary_particle_ids.insert(id);

    // define histograms containers
    std::map<Int_t, TH1F> decay_hist_dndpt_primary =
        book_histo_dndpt_primary(primary_particle_id, tp, n_pt_bins_det, pt_min, pt_max);
    std::map<Int_t, TH1F> hist_decay_channels = book_histo_decay_channels(primary_particle_id, tp);

    std::map<Int_t, std::map<Int_t, TH2F>>
        decay_hist_dndpt_secondary; // access: decay_hist_dndpt_secondary[id_prim][id_sec]
    std::map<Int_t, std::map<Int_t, TH2F>> decay_hist_cos2phi_secondary;

    std::map<Int_t, TH2F> decay_hist_dndpt_photon; // decay photon which have id as direct mother
    std::map<Int_t, TH2F> decay_hist_cos2phi_photon;

    // make sure that there is a histogram for photons, pi0 and eta for each primary particle

    if (calc_decay_photons && calc_feeddown) {
      for (auto const &id_prim: primary_particle_id) {
        for (auto const &id_dec: measured_particle_id) {
          add_2d_histo(decay_hist_dndpt_secondary, "decay_hist_dndpt_secondary", id_prim, id_dec, tp,
                        n_pt_bins_det, pt_min, pt_max);
          if (do_cos2phi)
            add_2d_histo(decay_hist_cos2phi_secondary, "decay_hist_cos2phi_secondary", id_prim, id_dec, tp,
                            n_pt_bins_det, pt_min, pt_max);
        }
      }
    }

    //
    // main event loop
    //
    Int_t n_print = n_evt / 100;
    if (n_print == 0)
        n_print = 1;

    for (Int_t i_evt = 0; i_evt < n_evt; ++i_evt) {

        if (i_evt % n_print == 0)
            std::cout << "Event " << i_evt << std::endl;

        // loop over primary particles
        for (auto const &id_prim : primary_particle_id) {

            // particle kinematic variables
            Double_t phi_prim = 0;
            Double_t y_prim = gRandom->Uniform(-rap_max_gen, rap_max_gen);
            Double_t pt_prim = gRandom->Uniform(pt_min, pt_max);
            Double_t px_prim = pt_prim * TMath::Cos(phi_prim);
            Double_t py_prim = pt_prim * TMath::Sin(phi_prim);
            Double_t m_prim = tp.Pythia8()->particleData.m0(id_prim);
            Double_t mt_prim = TMath::Sqrt(m_prim * m_prim + pt_prim * pt_prim);
            Double_t pz_prim = mt_prim * TMath::SinH(y_prim);
            Double_t E_prim = mt_prim * TMath::CosH(y_prim);
            Double_t p_prim = TMath::Sqrt(pt_prim * pt_prim + pz_prim * pz_prim);
            // Double_t eta_prim = TMath::ATanH(pz_prim / p_prim);

            // let primary particle decay
            // primary particle: position 1 in list
            const Int_t prim_pos = 1;
            tp.Pythia8()->event.reset();
            tp.Pythia8()->event.append(id_prim, 11, 0, 0, px_prim, py_prim, pz_prim, E_prim, m_prim);
            tp.Pythia8()->next();

            // increase counter for the primary particle if the primary particle is in the measured rapidity range
            // if (TMath::Abs(eta_prim) < rap_max_det) {
            if (TMath::Abs(y_prim) < rap_max_det) {
                decay_hist_dndpt_primary[id_prim].Fill(pt_prim);
            }

            Int_t ndp = tp.Pythia8()->event.size(); // number of decay particles

            // loop over decay particles (excluding the primary: prim_pos+1)
            for (Int_t ip = prim_pos + 1; ip < ndp; ip++) {

                if (tp.Pythia8()->event[ip].id() == 90)
                    continue;

                // check if the particle is direct daughter
                Int_t index_mother1 = tp.Pythia8()->event[ip].mother1();

                Int_t id_dec = tp.Pythia8()->event[ip].id();
                Double_t pt_dec = tp.Pythia8()->event[ip].pT();
                Double_t y_dec = tp.Pythia8()->event[ip].y();
                // Double_t eta_dec = tp.Pythia8()->event[ip].eta();
                Double_t phi_dec = tp.Pythia8()->event[ip].phi();

                // check whether we have a particle id of interest
                if (tp.Pythia8()->event[index_mother1].id() == id_prim && TMath::Abs(y_dec) < rap_max_det && set_of_measured_primary_particle_ids.count(id_dec) > 0) {

                    if (calc_feeddown && calc_decay_photons) {

                        // book secondary histogram if it does not exist
                        if (decay_hist_dndpt_secondary[id_prim].count(id_dec) == 0) {

                            add_2d_histo(decay_hist_dndpt_secondary, "decay_hist_dndpt_secondary", id_prim, id_dec, tp,
                                     n_pt_bins_det, pt_min, pt_max);
                            if (do_cos2phi)
                                add_2d_histo(decay_hist_cos2phi_secondary, "decay_hist_cos2phi_secondary", id_prim, id_dec, tp,
                                        n_pt_bins_det, pt_min, pt_max);
                        }

                        decay_hist_dndpt_secondary[id_prim][id_dec].Fill(pt_prim, pt_dec);
                        if (do_cos2phi)
                            decay_hist_cos2phi_secondary[id_prim][id_dec].Fill(pt_prim, pt_dec, TMath::Cos(2. * phi_dec));

                    }

                    else if (calc_decay_photons) {

                        // hadron decay photons which have id_prim as direct mother
                        if (id_dec == idPhoton) {

                            if (decay_hist_dndpt_photon.count(id_prim) == 0) {

                                add_1d_photon_histo(decay_hist_dndpt_photon, "decay_hist_dndpt_photon", id_prim, tp,
                                                n_pt_bins_det, pt_min, pt_max);
                                if (do_cos2phi)
                                    add_1d_photon_histo(decay_hist_cos2phi_photon, "decay_hist_cos2phi_photon", id_prim, tp,
                                                n_pt_bins_det, pt_min, pt_max);
                            }

                            decay_hist_dndpt_photon[id_prim].Fill(pt_prim, pt_dec);
                            // decay_hist_dndpt_photon[id_prim].Fill(pt_prim, pt_dec, 2 * rap_max_det);
                            if (do_cos2phi)
                                decay_hist_cos2phi_photon[id_prim].Fill(pt_prim, pt_dec, TMath::Cos(2. * phi_dec));

                        } // photon cocktail part (decay particle is a photon)

                    } // do decay photon part

                } // particle of interest? (direct daughter, in rapidity window and in list of measured particles)

            } // loop over decay particles

        } // loop over primary particles

    } // event loop

    //
    //  write to root file
    //
    TFile f_out(fn_out.Data(), "recreate");

    for (auto const &x : decay_hist_dndpt_primary)
        x.second.Write();

    // 2d decay histograms
    for (auto const &x : decay_hist_dndpt_secondary) {
        for (auto &y : x.second) {
            y.second.Write();
        }
    }

    for (auto const &x : decay_hist_cos2phi_secondary) {
        for (auto &y : x.second) {
            y.second.Write();
        }
    }

    // 2d decay photon histograms (photon is direct daughter of primary)
    for (auto const &x : decay_hist_dndpt_photon)
        x.second.Write();

    for (auto const &x : decay_hist_cos2phi_photon)
        x.second.Write();

    if (process_number == 0)
        output_tree->Write();

    std::cout << "wrote " << fn_out.Data() << std::endl;
    f_out.Close();

    return 0;
}
