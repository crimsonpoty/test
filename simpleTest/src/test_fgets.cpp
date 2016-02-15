/* fgets example */
#include <stdio.h>

int main(int argc, char **argv)
{
   FILE * pFile;
   char mystring [100];
   
   if(argc != 2) {
	   printf("Input file name!!\n");
	   return 0;
   }

   pFile = fopen (argv[1], "r");
   if (pFile == NULL) perror ("Error opening file");
   else {
     if ( fgets (mystring , 100 , pFile) != NULL )
       puts (mystring);
     fclose (pFile);
   }
   return 0;
}