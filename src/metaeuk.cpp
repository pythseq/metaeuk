#include "Command.h"
#include "CommandDeclarations.h"
#include "LocalCommandDeclarations.h"
#include "LocalParameters.h"

const char* binary_name = "metaeuk";
const char* tool_name = "metaeuk";
const char* tool_introduction = "Metagenomic tool for Eukaryotic data.";
const char* main_author = "Eli Levy Karin, eli.levy.karin@gmail.com";
const char* show_extended_help = "1";
const char* show_bash_info = "1";

LocalParameters& par = LocalParameters::getLocalInstance();

std::vector<struct Command> commands = {
        // Main tools  (for non-experts)
        {"predictexons",             predictexons,            &par.predictexonsworkflow,    COMMAND_MAIN,
                "Predict eukaryotic exons based on protein similarity.",
                "An analog of 6-frame translation to produce putative protein fragments. Search against protein DB. Compatible exon set identified with respect to each target.",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:contigsDB> <i:proteinsDB> <o:outDB> <tmpDir>",
                CITATION_MMSEQS2},
        {"collectoptimalset",             collectoptimalset,            &par.predictexonsworkflow,    COMMAND_EXPERT,
                "Collect the optimal set of exons for a target protein",
                "A dynamic programming procedure on all candidates of each contig and strand combination",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:proteinToPotentialExonsWithContigInfoDB> <o:optimalResultsBaseName>",
                CITATION_MMSEQS2},
        {"unitesetstosequencedb",             unitesetstosequencedb,            &par.predictexonsworkflow,    COMMAND_EXPERT,
                "Create a sequence DB from collected optimal exon sets",
                "Each optimal set is joined to a single sequence",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:contigsDB> <i:proteinsDB> <i:optimalResultsBaseName> <o:outDB>",
                CITATION_MMSEQS2},
        {"reduceredundancy",             reduceredundancy,            &par.reduceredundancyworkflow,    COMMAND_MAIN,
                "A greedy approach to cluster metaeuk predictions which share an exon",
                "A protein coding gene can be predicted more than once due to target DB homologies. A cluster representative is selected. Predictions in a cluster share an exon with the representative.",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:optimalResultsBaseName> <o:reducedResultsBaseName> <tmpDir>",
                CITATION_MMSEQS2},
        {"grouppredictions",             grouppredictions,            &par.reduceredundancyworkflow,    COMMAND_EXPERT,
                "Assignment of predictions to cluster",
                "A greedy examination of predictions accoridng to their contig order, subordered by the number of exons. Predictions in a cluster share an exon with the representative.",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:contigStrandSortedMap> <o:groupedPredictionsDB>",
                CITATION_MMSEQS2},
        {"createdb",             createdb,             &par.createdb,             COMMAND_MAIN,
                "Convert protein sequence set in a FASTA file to MMseqs sequence DB format",
                "converts a protein sequence flat/gzipped FASTA or FASTQ file to the MMseqs sequence DB format. This format is needed as input to mmseqs search, cluster and many other tools.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:fastaFile1[.gz]> ... <i:fastaFileN[.gz]> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"extractorfs",          extractorfs,          &par.extractorfs,          COMMAND_DB,
                "Extract open reading frames from all six frames from nucleotide sequence DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"orftocontig",             orftocontig,            &par.orftocontig,    COMMAND_DB,
                "Obtain location information of extractd orfs with respect to their contigs in alignment format",
                "Parses extracted orfs headers to computate their coordinates on the contig and writes the results in alignment format",
                "Eli Levy Karin <eli.levy.karin@gmail.com> ",
                "<i:contigsSequenceDB> <i:extractedOrfsHeadersDB> <o:orfsAlignedToContigDB>",
                CITATION_MMSEQS2},
        {"translatenucs",        translatenucs,        &par.translatenucs,        COMMAND_DB,
                "Translate nucleotide sequence DB into protein sequence DB",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:sequenceDB>",
                CITATION_MMSEQS2},
        {"search",               search,               &par.searchworkflow,       COMMAND_EXPERT,
                "Search with query sequence or profile DB (iteratively) through target sequence DB",
                "Searches with the sequences or profiles query DB through the target sequence DB by running the prefilter tool and the align tool for Smith-Waterman alignment. For each query a results file with sequence matches is written as entry into a database of search results (alignmentDB).\nIn iterative profile search mode, the detected sequences satisfying user-specified criteria are aligned to the query MSA, and the resulting query profile is used for the next search iteration. Iterative profile searches are usually much more sensitive than (and at least as sensitive as) searches with single query sequences.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <o:alignmentDB> <tmpDir>",
                CITATION_MMSEQS2},
        // required for search:
        {"prefilter",            prefilter,            &par.prefilter,            COMMAND_EXPERT,
                "Search with query sequence / profile DB through target DB (k-mer matching + ungapped alignment)",
                "Searches with the sequences or profiles in query DB through the target sequence DB in two consecutive stages: a very fast k-mer matching stage (double matches on same diagonal) and a subsequent ungapped alignment stage. For each query a results file with sequence matches is written as entry into the prefilter DB.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> & Maria Hauser",
                "<i:queryDB> <i:targetDB> <o:prefilterDB>",
                CITATION_MMSEQS2},
        {"align",                align,                &par.align,                COMMAND_EXPERT,
                "Compute Smith-Waterman alignments for previous results (e.g. prefilter DB, cluster DB)",
                "Calculates Smith-Waterman alignment scores between all sequences in the query database and the sequences of the target database which passed the prefiltering.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de> & Maria Hauser",
                "<i:queryDB> <i:targetDB> <i:prefilterDB> <o:alignmentDB>",
                CITATION_MMSEQS2},
        {"swapresults",          swapresults,          &par.swapresult,          COMMAND_DB,
                "Reformat prefilter/alignment/cluster DB as if target DB had been searched through query DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>, Clovis Galiez & Eli Levy Karin",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        {"swapdb",          swapdb,          &par.swapdb,          COMMAND_DB,
                "Create a DB where the key is from the first column of the input result DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>, Clovis Galiez & Eli Levy Karin",
                "<i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},   
        {"filterdb",             filterdb,             &par.filterDb,             COMMAND_DB,
                "Filter a DB by conditioning (regex, numerical, ...) on one of its whitespace-separated columns",
                NULL,
                "Clovis Galiez & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:resultDB> <o:resultDB>",
                CITATION_MMSEQS2},
        // Special-purpose utilities
        {"convert2fasta",        convert2fasta,        &par.convert2fasta,        COMMAND_FORMAT_CONVERSION,
                "Convert sequence DB to FASTA format",
                NULL,
                "Milot Mirdita <milot@mirdita.de>",
                "<i:sequenceDB> <o:fastaFile>",
                CITATION_MMSEQS2},
        {"convertalis",          convertalignments,    &par.convertalignments,    COMMAND_FORMAT_CONVERSION,
                "Convert alignment DB to BLAST-tab format, SAM flat file, or to raw pairwise alignments",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDb> <i:targetDb> <i:alignmentDB> <o:alignmentFile>",
                CITATION_MMSEQS2},
        {"result2repseq",       result2repseq,      &par.onlythreads,          COMMAND_DB,
                "Get representative sequences for a result database",
                NULL,
                "Milot Mirdita <milot@mirdita.de> & Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:sequenceDB> <i:resultDB> <o:reprSeqDB>",
                CITATION_MMSEQS2},
        {"result2flat",          result2flat,          &par.result2flat,          COMMAND_FORMAT_CONVERSION,
                "Create a FASTA-like flat file from prefilter DB, alignment DB, or cluster DB",
                NULL,
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>",
                "<i:queryDB> <i:targetDB> <i:resultDB> <o:fastaDB>",
                CITATION_MMSEQS2},
        {"createtsv",            createtsv,            &par.createtsv,        COMMAND_FORMAT_CONVERSION,
                "Create tab-separated flat file from prefilter DB, alignment DB, cluster DB, or taxa DB",
                "Create tab-separated flat file from prefilter DB, alignment DB, cluster DB, or taxa DB. The target database is optional. This is useful for taxa DB, since it does not have a target key.",
                "Martin Steinegger <martin.steinegger@mpibpc.mpg.de>r",
                "<i:queryDB> [<i:targetDB>] <i:resultDB> <o:tsvFile>",
                CITATION_MMSEQS2},
        {"shellcompletion",      shellcompletion,      &par.empty,                COMMAND_HIDDEN,
                "",
                NULL,
                "",
                "",
                CITATION_MMSEQS2},
};
