#include <string>
#include "utils/tools.h"
#include "alignment/alignment.h"

using namespace std;

#ifndef MODEL_H
#define MODEL_H

class Model
{

public:
    string model_name;
    double *root_freqs, *root_log_freqs;
    double *mutation_mat;
    double* pseu_mutation_count;
    
	/**
		constructor
	*/
    Model();
    
    /**
        deconstructor
    */
    ~Model();
    
    /**
        Extract reference-related info (freqs, log_freqs)
     */
    void extractRefInfo(vector<StateType> ref_seq, StateType num_states);
    
    /**
        Init the mutation rate matrix from a model
        @param n_model_name: name of the model; num_states: the number of states
     */
    void initMutationMat(string n_model_name, StateType num_states);
    
    /**
        Update the mutation rate matrix regarding the pseu_mutation_count
        @param num_states: the number of states
     */
    void updateMutationMat(StateType num_states);
    
    /**
        compute cumulative rate of the ref genome
     */
    void computeCumulativeRate(double *&cumulative_rate, vector<vector<PositionType>> &cumulative_base, Alignment* aln);
    
    /**
        Update the mutation matrix periodically from the empirical count of mutations
     */
    void updateMutationMatEmpirical(double *&cumulative_rate, vector<vector<PositionType>> &cumulative_base, Alignment* aln);
    
    /**
        update pseudocounts from new sample to improve the estimate of the substitution rates
        @param node_regions the genome list at the node where the appending happens;
        @param sample_regions the genome list for the new sample.
     */
    void updatePesudoCount(Alignment* aln, Regions* node_regions, Regions* sample_regions);
};
#endif
