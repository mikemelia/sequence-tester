#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readsnp.h"

snp *createSNP() {
   snp *snp = malloc(sizeof(snp));
   snp->chromosome = malloc(10 * sizeof(char));
   snp->position = malloc(sizeof(int));
   snp->reference = malloc(1000 * sizeof(char));
   snp->alts = malloc(1000 * sizeof(char));
   return snp;
}

int parseLine(char *line, snp *snp) {
   if (*line == '#') return 1;
   char *space = "\t";
   char *chromosome = strtok(line, space);
   strcpy(snp->chromosome, chromosome);
   *snp->position = atoi(strtok(NULL, space));
   char *id = strtok(NULL, space);
   strcpy(snp->reference, strtok(NULL, space));
   strcpy(snp->alts, strtok(NULL, space));
   return 0;
}

FILE *open(char *filename, FILE* file) {
   if (file != NULL) {
      return file;
   }
   file = fopen ( filename, "r" );
   if ( file == NULL )
   {
      perror ( filename ); /* why didn't the file open? */
      return NULL;
   }
   return file;
}

int readSNPFromLine(char *line, size_t size, FILE *file, snp *snp) {
   if (getline(&line, &size, file) > 0)
   {
      if (parseLine(line, snp) == 1) {
         return readSNPFromLine(line, size, file, snp);
      }
      return 0;
   } else {
      return 1;
   }

}

int nextSNPFrom (char *filename, FILE **file, snp *snp, char *line, size_t size)
{
   *file = open(filename, *file);
   if (readSNPFromLine(line, size, *file, snp) != 0) {
      fclose (*file);
      return 1;
   }
   return 0;
}

int main(int argc, char const *argv[])
{
   FILE *snpFile;
   snp *snp = createSNP();
   size_t size = 2000;
   char *line = malloc(sizeof(char) * size);
   int result = nextSNPFrom("/Users/michael/dev/data/dbsnp_135.b37.vcf", &snpFile, snp, line, size);
   while (result == 0) {
 //     printf("SNP: at chromosome %s, position %d, ref %s, alts %s\n", snp->chromosome, *snp->position, snp->reference, snp->alts);
      result = nextSNPFrom("/Users/michael/dev/data/dbsnp_135.b37.vcf", &snpFile, snp, line, size);
   }
   return 0;
}