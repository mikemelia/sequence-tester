#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readgenome.h"
#include "dbops.h"

char *parseLine(char *line, chromosome *chromosome) {
   if (*line == '>') 
   {
      return strtok(line + 1, " ");
   }
   int lineLength = strlen(line) - 1;
   int i = 0;
   for (i = 0; i < lineLength; ++i)
   {
      setNucleotide(chromosome->sequence + i + chromosome->sequenceLength, line + i, 0);
   }
   chromosome->sequenceLength += lineLength;
   return chromosome->identifier;
}

chromosome *createChromosome() {
      chromosome *currentChromosome = malloc(sizeof(chromosome));
      currentChromosome->sequenceLength = 0;
      currentChromosome->identifier = malloc(10);
      currentChromosome->currentMaxLength = 31250000;
      currentChromosome->sequence = malloc(sizeof(char) * currentChromosome->currentMaxLength);
      return currentChromosome;
}

void writeChromosome(chromosome *chromosome) {
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

int parseGenomeFile(FILE *file, chromosome **chromosomes) {
   size_t size = 1000;
   char *line = malloc(sizeof(char) * size);
   int count = 0;
   chromosome *currentChromosome = createChromosome();
   chromosomes[count] = currentChromosome;
   while(getline(&line, &size, file) > 0)
   {
      char *chromosome = parseLine(line, currentChromosome);
      if (strcmp(currentChromosome->identifier, chromosome) != 0) {
         printf(" %d\n", currentChromosome->sequenceLength);
         count += 1;
         currentChromosome = createChromosome();
         chromosomes [count] = currentChromosome;
         strcpy(currentChromosome->identifier, chromosome);
         printf("%s", currentChromosome->identifier);
      } else {
         if (currentChromosome->currentMaxLength < currentChromosome->sequenceLength + 1000) {
            currentChromosome->currentMaxLength *= 2;
            currentChromosome->sequence = realloc(currentChromosome->sequence, currentChromosome->currentMaxLength);
            printf(".");
         }
      }
   }
   return count;
}

chromosomeMap *createChromosomeMapFrom(char *filename) {
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

