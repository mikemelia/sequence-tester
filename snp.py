from Bio import Entrez, SeqIO

def getGenbankEntryFor(id):
	Entrez.email = 'mikemelia@gmail.com'
	return Entrez.efetch(db="snp", term=id, rettype="fasta")

def toFasta(input_handle, callback = None):
	seqlist = SeqIO.parse(input_handle, "fasta")
	print repr(seqlist)
	for record in seqlist:	
		print record

toFasta(getGenbankEntryFor("homo sapiens"))

