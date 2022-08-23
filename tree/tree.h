#include "node.h"
#include "alignment/alignment.h"
#include "model/model.h"

#ifndef TREE_H
#define TREE_H

/** The tree structure */
class Tree {
private:
    /**
        Traverse the intial tree from root to re-calculate all lower likelihoods regarding the latest/final estimated model parameters
     */
    void refreshAllLowerLhs(RealNumType *cumulative_rate, RealNumType default_blength, RealNumType max_blength, RealNumType min_blength);
    
    /**
        Traverse the intial tree from root to re-calculate all non-lower likelihoods regarding the latest/final estimated model parameters
     */
    void refreshAllNonLowerLhs(RealNumType *cumulative_rate, RealNumType default_blength, RealNumType max_blength, RealNumType min_blength);
    
    /**
        Try to improve a subtree rooted at node with SPR moves
        @return total improvement
     */
    RealNumType improveSubTree(Node* node, RealNumType *cumulative_rate, RealNumType default_blength, RealNumType max_blength, RealNumType min_blength);
    
public:
    /**
        Program parameters
     */
    Params *params;
    
    /**
        Alignment
     */
    Alignment *aln;
    
    /**
        Evolutionary model
     */
    Model* model;
    
    /**
        Root node of the tree
     */
    Node* root;
    
    /**
        Constructor
     */
    Tree();
    
    /**
        Constructor
    */
    Tree(Params *params, Node* n_root = NULL);
    
    /**
        Deconstructor
     */
    ~Tree();
    
    /**
        Export tree string in Newick format
     */
    string exportTreeString(Node* node = NULL);
    
    /**
        Iteratively update partial_lh starting from the nodes in node_stack
        @param node_stack stack of nodes;
     */
    void updatePartialLh(stack<Node*> &node_stack, RealNumType* cumulative_rate, RealNumType default_blength, RealNumType min_blength, RealNumType max_blength);
    
    /**
        Seek a position for a sample placement starting at the start_node
     */
    void seekSamplePlacement(Node* start_node, string seq_name, SeqRegions* sample_regions, Node* &selected_node, RealNumType &best_lh_diff , bool &is_mid_branch, RealNumType &best_up_lh_diff, RealNumType &best_down_lh_diff, Node* &best_child, RealNumType* cumulative_rate, RealNumType default_blength, RealNumType min_blength_mid);
    
    /**
        Seek a position for placing a subtree/sample starting at the start_node
     */
    void seekPlacement(Node* &best_node, RealNumType &best_lh_diff, bool &is_mid_branch, RealNumType &best_up_lh_diff, RealNumType &best_down_lh_diff, Node* &best_child, Node* start_node, RealNumType &removed_blength, RealNumType* cumulative_rate, RealNumType default_blength, RealNumType min_blength_mid, bool search_subtree_placement = true, SeqRegions* sample_regions = NULL);

    /**
        Place a new sample on the tree
     */
    void placeNewSample(Node* selected_node, SeqRegions* sample, string seq_name, RealNumType best_lh_diff , bool is_mid_branch, RealNumType best_up_lh_diff, RealNumType best_down_lh_diff, Node* best_child, RealNumType* cumulative_rate, vector< vector<PositionType> > &cumulative_base, RealNumType default_blength, RealNumType max_blength, RealNumType min_blength);
    
    /**
        Traverse the intial tree from root to re-calculate all likelihoods regarding the latest/final estimated model parameters
     */
    void refreshAllLhs(RealNumType *cumulative_rate, RealNumType default_blength, RealNumType max_blength, RealNumType min_blength);
    
    /**
        Traverse the tree to set the outdated flag (which is used to prevent traversing the same part of the tree multiple times) of all nodes to TRUE
     */
    void setAllNodeOutdated();
    
    /**
        Try to improve the entire tree with SPR moves
        @return total improvement
     */
    RealNumType improveEntireTree(RealNumType *cumulative_rate, RealNumType default_blength, RealNumType max_blength, RealNumType min_blength);
};

#endif
