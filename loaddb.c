//
//  main.c
//  test-harness
//
//  Created by Michael Melia on 26/05/12.
//  Copyright 2012 Gigantiq. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readgenome.h"

FILE *open(char *filename) 
{
   FILE *openTheFile = fopen(filename, "r" );
   if (openTheFile == NULL)
   {
      perror (filename); /* why didn't the file open? */
   }
   return openTheFile;
}

void applySNPs(char *filename, chromosomeMap *map)
{
    FILE *snpFile = open(filename);
    snp *snp = createSNP();
    size_t size = 5000;
    char *line = malloc(sizeof(char) * size);
    int i;
    chromosome *chromosome = map->chromosomes[0];
    int result = nextSNPFrom(snpFile, snp, line, size);
    while (result == 0) {
        if (strcmp(chromosome->identifier, snp->chromosome) != 0) 
        {
            chromosome = getChromosome(map, snp->chromosome);
            printf("chromosome %s\n", snp->chromosome);
        }
        addSNP(chromosome, snp);
        result = nextSNPFrom(snpFile, snp, line, size);
    }
    fclose(snpFile);
}

void printMap(chromosomeMap *map)
{
    int i;
    chromosome **chromosomes = map->chromosomes;
    for (i = 0; i < map->count; i++) 
    {
        printf("Contains chromosome : %s\n", chromosomes[i]->identifier);
    }
}
int main (int argc, const char * argv[])
{
    chromosomeMap *map = createChromosomeMapFrom("/Users/michael/dev/data/human_g1k_v37.fasta");
    printMap(map);
    applySNPs("/Users/michael/dev/data/dbsnp_135.b37.vcf", map);
    int i;
    for (i = 0; i < map->count; i++) {
        writeChromosome(map->chromosomes[i]);
    }
    return 0;
}

