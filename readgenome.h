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

chromosomeMap *createChromosomeMapFrom(char *filename);
