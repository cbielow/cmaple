//
//  region.cpp
//  alignment
//
//  Created by NhanLT on 31/3/2022.
//

#include "seqregion.h"


SeqRegion::SeqRegion(StateType n_type, PositionType n_position, RealNumType n_plength_observation, RealNumType n_plength_from_root, LHPtrType n_likelihood)
  : Mutation(n_type, n_position)
{
    plength_observation2node = n_plength_observation;
    plength_observation2root = n_plength_from_root;
    likelihood = std::move(n_likelihood);
}

SeqRegion::SeqRegion(StateType n_type, PositionType n_position, RealNumType n_plength_observation, RealNumType n_plength_from_root, LHType n_likelihood)
: Mutation(n_type, n_position)
{
  plength_observation2node = n_plength_observation;
  plength_observation2root = n_plength_from_root;
  likelihood = std::make_unique<LHType>(n_likelihood);
}


SeqRegion::SeqRegion(StateType n_type, PositionType n_position, SeqType seq_type, int max_num_states)
  : Mutation(n_type, n_position)
{
    convertAmbiguiousState(seq_type, max_num_states);
}

SeqRegion::SeqRegion(Mutation* n_mutation, SeqType seq_type, int max_num_states)
{
    type = n_mutation->type;
    position = n_mutation->position + n_mutation->getLength() - 1;
    convertAmbiguiousState(seq_type, max_num_states);
}


void SeqRegion::convertAmbiguiousState(SeqType seq_type, int max_num_states)
{
    ASSERT(type >= 0 && type < TYPE_INVALID);
    switch (seq_type) {
        case SEQ_DNA:
            convertAmbiguiousStateDNA(max_num_states);
            break;
            
        default:
            outError("Sorry! Currently, we only support DNA data.");
            break;
    }
}

void SeqRegion::convertAmbiguiousStateDNA(int max_num_states)
{
    // do nothing if it is not an ambiguious state
    if (type < max_num_states || type == TYPE_N || type == TYPE_R)
        return;
    
    switch (type) {
        case TYPE_DEL: // convert '-' into type_N
            type = TYPE_N;
            break;
        case 1+4+3: // 'R' -> A or G, Purine
        {
            IntVector entries{1,0,1,0};
            computeLhAmbiguity(entries);
            break;
        }
        case 2+8+3: // 'Y' -> C or T, Pyrimidine
        {
            IntVector entries{0,1,0,1};
            computeLhAmbiguity(entries);
            break;
        }
        case 1+8+3: // 'W' -> A or T, Weak
        {
            IntVector entries{1,0,0,1};
            computeLhAmbiguity(entries);
            break;
        }
        case 2+4+3: // 'S' -> G or C, Strong
        {
            IntVector entries{0,1,1,0};
            computeLhAmbiguity(entries);
            break;
        }
        case 1+2+3:  // 'M' -> A or C, Amino
        {
            IntVector entries{1,1,0,0};
            computeLhAmbiguity(entries);
            break;
        }
        case 4+8+3: // 'K' -> G or T, Keto
        {
            IntVector entries{0,0,1,1};
            computeLhAmbiguity(entries);
            break;
        }
        case 2+4+8+3: // 'B' -> C or G or T
        {
            IntVector entries{0,1,1,1};
            computeLhAmbiguity(entries);
            break;
        }
        case 1+2+8+3: // 'H' -> A or C or T
        {
            IntVector entries{1,1,0,1};
            computeLhAmbiguity(entries);
            break;
        }
        case 1+4+8+3: // 'D' -> A or G or T
        {
            IntVector entries{1,0,1,1};
            computeLhAmbiguity(entries);
            break;
        }
        case 1+2+4+3: // 'V' -> A or G or C
        {
            IntVector entries{1,1,1,0};
            computeLhAmbiguity(entries);
            break;
        }
        default:
            outError("Invalid character for a genome entry. Please check and try again!");
    }
}

void SeqRegion::computeLhAmbiguity(IntVector &entries)
{
    // change type to 'O'
    type = TYPE_O;

    if (!likelihood) likelihood = std::make_unique<LHType>();
    for (StateType i = 0; i < (StateType) entries.size(); ++i)
        (*likelihood)[i] = entries[i];
    
    // normalize likelihood
    normalize_arr(likelihood->data(), entries.size());
}
