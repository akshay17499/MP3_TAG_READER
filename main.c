#include"header.h"

int main(int argc,char*argv[])
{
    F_block mp3_file;
    if (argc < 2) {
        display_help();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0) {
        display_help();
        return 0;
    }
    else if(strcmp(argv[1], "-e")==0)
    {
        if(argc<6)
        {
            display_help();
            return 0;
        }
      check_verify_file(argv,&mp3_file);
    }
    else if (strcmp(argv[1], "-v")==0)
    {
         if(argc<3)
          {
               printf("  ERROR : ./a.out : invalid arguments    \n" );
          }
         else{
           if(check_verify_file(argv,&mp3_file)==e_failure)
           {
              printf("--------------------------SONG DETAILS--------------------------------\n\n\n\n");
              printf("------------------------------------------------------------------------\n\n");
              printf("                      SOMETHING WENT WRONG PLEASE TRY AGAIN                     \n\n\n");
           }
          }
    }
   
 
}
