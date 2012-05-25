#!/bin/python
from Bio import SeqIO

genome = open('/Users/michael/dev/data/human_g1k_v37.fasta')
seqlist = SeqIO.parse(genome, "fasta")

for record in seqlist:
	print record.id, 
	print len(record.seq)