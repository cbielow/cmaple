#include "alignment.h"

using namespace std;
using namespace cmaple;

void cmaple::Alignment::init(std::istream& aln_stream, const std::string& ref_seq, const std::string& format, const std::string& seqtype)
{
    // Init alignment base
    aln_base = new AlignmentBase();
    
    // if users specify the format -> parse it
    if (format.length())
    {
        aln_base->aln_format = aln_base->getAlignmentFormat(format);
        
        // validate the format
        if (aln_base->aln_format == IN_UNKNOWN)
            outError("Unsupported alignment format " + format + ". Please use MAPLE, FASTA, or PHYLIP");
    }
    // Otherwise, detect the format from the alignment
    else
    {
        aln_base->aln_format = detectInputFile(aln_stream);
        
        // validate the format
        if (aln_base->aln_format == IN_UNKNOWN)
            outError("Failed to detect the format from the alignment!");
    }
    
    // If users specify the sequence type -> parse it. Otherwise, we'll dectect it later when reading the alignment
    if (seqtype.length())
    {
        // parse the sequence type
        aln_base->setSeqType(aln_base->getSeqType(seqtype));
        
        // validate the sequence type
        if (aln_base->getSeqType() == SEQ_UNKNOWN)
            outError("Unknown sequence type " + seqtype + ", please use DNA or AA");
    }
    
    // Read the input alignment
    // in FASTA or PHYLIP format
    if (aln_base->aln_format != IN_MAPLE)
        aln_base->readFastaOrPhylip(aln_stream, ref_seq);
    // in MAPLE format
    else
        aln_base->readMaple(aln_stream);
}

cmaple::Alignment::Alignment(std::istream& aln_stream, const std::string& ref_seq, const std::string& format, const std::string& seqtype):aln_base(nullptr)
{
    // Initialize an alignment instance from the input stream
    init(aln_stream, ref_seq, format, seqtype);
}

cmaple::Alignment::Alignment(const std::string& aln_filename, const std::string& ref_seq, const std::string& format, const std::string& seqtype):aln_base(nullptr)
{
    ASSERT(aln_filename.length() && "Please specify an alignnment file");
    
    // Create a stream from the input alignment
    ifstream aln_stream;
    try {
        aln_stream.exceptions(ios::failbit | ios::badbit);
        aln_stream.open(aln_filename);
    } catch (ios::failure) {
        outError(ERR_READ_INPUT, aln_filename);
    }
    
    // Initialize an alignment instance from the input stream
    init(aln_stream, ref_seq, format, seqtype);
    
    // close aln_stream
    aln_stream.close();
}

cmaple::Alignment::~Alignment()
{
    if (aln_base)
    {
        delete aln_base;
        aln_base = nullptr;
    }
}
