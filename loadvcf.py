#!/bin/python
import vcf
from Bio import SeqIO

vcf_reader = vcf.Reader(open('/Users/michael/dev/data/dbsnp_135.b37.vcf'))
indel = 0
count = 0
for record in vcf_reader:
	count += 1
	if record.is_indel:
		indel += 1
print count, indel
