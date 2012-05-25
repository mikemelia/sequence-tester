

void setNucleotide(char *position, char *nucleotide, int shift) {
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
