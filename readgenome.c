#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readgenome.h"

snp *createSNP() 
{
   snp *snp = malloc(sizeof(snp));
   snp->chromosome = malloc(100 * sizeof(char));
   snp->position = 0;
   snp->reference = malloc(1000 * sizeof(char));
   snp->alts = malloc(1000 * sizeof(char));
   return snp;
}

int parseLine(char *line, snp *snp) 
{
   if (*line == '#') return 1;
   char *space = "\t";
   snp->chromosome = strtok(line, space);
   snp->position = atoi(strtok(NULL, space)) - 1;
   strtok(NULL, space);
   snp->reference = strtok(NULL, space);
   snp->alts = strtok(NULL, space);
   return 0;
}

int readSNPFromLine(char *line, size_t size, FILE *file, snp *snp) 
{
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

int nextSNPFrom (FILE *file, snp *snp, char *line, size_t size)
{
   if (readSNPFromLine(line, size, file, snp) != 0) {
      return 1;
   }
   return 0;
}

void setNucleotide(char *position, char *nucleotide, int shift) 
{
   switch (*nucleotide) {
      case 'A':
         *position |= (char)8 << shift;
         break;
      case 'C':
         *position |= (char)4 << shift;
         break;
      case 'G':
         *position |= (char)2 << shift;
         break;
      case 'T':
         *position |= (char)1 << shift;
         break;
      case 'N':
         *position |= (char)15 << shift;
         break;
      default:
         break;
   }
}

void addSNP(chromosome *chromosome, snp *snp)
{
   if (strlen(snp->reference) == 1)
   {
      setNucleotide((chromosome->sequence + snp->position), snp->alts, 4);
   }
}

chromosome *getChromosome(chromosomeMap *map, char *chromosomeName)
{
   int i;
   chromosome **chromosomes = map->chromosomes;
   for(i = 0; i < map->count; i++)
   {
      if (strcmp(chromosomes[i]->identifier, chromosomeName) == 0)
      {
         return chromosomes[i];
      }
   }
   return NULL;
}

char *parseEntry(char *line, chromosome *chromosome) 
{
   if (*line == '>') 
   {
      return strtok(line + 1, " ");
   }
   int lineLength = (int) strlen(line) - 1;
   int i = 0;
   for (i = 0; i < lineLength; ++i)
   {
      setNucleotide(chromosome->sequence + i + chromosome->sequenceLength, line + i, 0);
   }
   chromosome->sequenceLength += lineLength;
   return chromosome->identifier;
}

chromosome *createChromosome() 
{
   chromosome *currentChromosome = malloc(sizeof(chromosome));
   currentChromosome->sequenceLength = 0;
   currentChromosome->identifier = malloc(10);
   currentChromosome->currentMaxLength = 31250000;
   currentChromosome->sequence = malloc(sizeof(char) * currentChromosome->currentMaxLength);
   return currentChromosome;
}

void writeChromosome(chromosome *chromosome) 
{
   char *fname = strcat(chromosome->identifier, ".bin");
   char directory[100];
   strcpy(directory, "/Users/michael/dev/data/");
   char *fullname = strcat(directory, fname);
   printf("Writing to %s\n", fullname);
   FILE *file = fopen(fullname, "w");
   if (file != NULL) 
   {
      fwrite(chromosome->sequence, sizeof(char), chromosome->sequenceLength, file);
      fclose(file);
   }

}

int parseGenomeFile(FILE *file, chromosome **chromosomes) 
{
   size_t size = 1000;
   char *line = malloc(sizeof(char) * size);
   int count = 0;
   chromosome *currentChromosome = createChromosome();
   chromosomes[count] = currentChromosome;
   while(getline(&line, &size, file) > 0)
   {
      char *chromosome = parseEntry(line, currentChromosome);
      if (strcmp(currentChromosome->identifier, chromosome) != 0) {
         currentChromosome = createChromosome();
         chromosomes[count] = currentChromosome;
         strcpy(currentChromosome->identifier, chromosome);
         count++;
      } else {
         if (currentChromosome->currentMaxLength < currentChromosome->sequenceLength + 1000) {
            currentChromosome->currentMaxLength *= 2;
            currentChromosome->sequence = realloc(currentChromosome->sequence, currentChromosome->currentMaxLength);
         }
      }
   }
   return count;
}

chromosomeMap *createChromosomeMapFrom(char *filename) 
{
   chromosome **chromosomes = malloc(sizeof(chromosome) * 1000);
   chromosomeMap *map = malloc(sizeof(chromosomeMap));
   map->chromosomes = chromosomes;
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      map->count = parseGenomeFile(file, chromosomes);
      fclose ( file );
   }
   else
   {
      perror ( filename ); /* why didn't the file open? */
   }
   return map;
}

