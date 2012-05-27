typedef struct snp
{
   char *chromosome;
   int position;
   char *reference;
   char *alts;
} snp;

typedef struct chromosome {
   char *identifier;
   char *sequence;
   int sequenceLength;
   int currentMaxLength;
} chromosome;

typedef struct chromosomeMap {
   chromosome **chromosomes;
   int count;
} chromosomeMap;

snp *createSNP();
int nextSNPFrom(FILE *file, snp *snp, char *line, size_t size);
chromosomeMap *createChromosomeMapFrom(char *filename);
chromosome *getChromosome(chromosomeMap *map, char *chromosomeName);
void addSNP(chromosome *chromosome, snp *snp);
