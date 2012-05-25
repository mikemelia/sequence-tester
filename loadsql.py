#!/bin/python
import vcf, sqlite3

connection = sqlite3.connect('/Users/michael/dev/data/snp.db')
cursor = connection.cursor()
cursor.execute('create table snp (position integer, before text)')

vcf_reader = vcf.Reader(open('/Users/michael/dev/data/dbsnp_135.b37.vcf'))
for record in vcf_reader:
	if record.is_snp:
		cursor.execute('insert into snp values(?, ?)', (record.POS, record.REF))

cursor.close()
connection.close()
