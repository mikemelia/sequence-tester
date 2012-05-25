#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readgenome.h"

int main ( void )
{
   chromosomeMap *map = createChromosomeMapFrom("/Users/michael/dev/data/human_g1k_v37.fasta");
   int i;
   for (i = 0; i < map->count; i++) {
      writeChromosome(map->chromosomes[i]);
   }
   return 0;
}