#include"header.h"

F_block dupe_mp3_file;

void initialize(F_block* mp3_file)
{
    char arr[6][5]={"TIT2","TPE1","TALB","TYER","TCON","TCOM"};  
    char **arr1 = malloc(6*sizeof(char*));
    for(int i=0;i<6;i++)
    {
        *(arr1+i)=malloc(30*sizeof(char));
    }
    *(arr1) = "Song Name              ";
    *(arr1+1) = "Lead performer(s)      ";
    *(arr1+2) = "Album/Movie/Show title ";
    *(arr1+3) = "Year                   ";
    *(arr1+4) = "Content type           ";
    *(arr1+5) = "Composer              ";

    for(int i=0;i<6;i++)
    {
        strcpy(mp3_file->data[i].frame_id ,arr[i]);
        mp3_file->data[i].frame_content=NULL;
        mp3_file->data[i].discription = *(arr1+i);
    }
    
}

void display_help() {
     printf("\n--------------------------------------------------------------\n");
    printf("---------------------MP3 TAG READER----------------------------");
    printf("\n---------------------------------------------------------------");
    printf("\nUsage: mp3tagreader [options] filename\n");
    printf("----------------------------------------------------------------\n");
    printf("Options:\n");
    printf("  -h                               Display help\n");
    printf("  -v   <file.mp3>                     View tags\n");
    printf("  -e -t/-T/-a/-A/-y/-c/-g   <Tag contents> <orginal.mp3> <copy.mp3>  \n");
    printf("----------------------------------------------------------------\n\n");
}

Status open_file(char*argv[],F_block* mp3_file)
{
   mp3_file->fptr_mp3_file = fopen(mp3_file->get_file_name,"r");

   if(mp3_file->fptr_mp3_file==NULL)
   {
    printf("failed to open file");
      return e_failure;
   }
   return e_success;
}

Status check_version(FILE* fptr)
{
    char arr[4];
    fread(arr,1,3,fptr);
    arr[3]='\0';
    if(strcmp(arr,"ID3")==0)
      return e_success;
      return e_failure;
}

void get_data(F_block* mp3_file,int size,int index)
{
    char temp[1000]={0};
     char *temp_store = malloc(1000*sizeof(char));
     int k=0;
    fseek(mp3_file->fptr_mp3_file,2,SEEK_CUR);
     fread(temp,1,size,mp3_file->fptr_mp3_file);
    for(int i=0;i<size;i++)
    {
        if(temp[i]>='a'&&temp[i]<='z'||temp[i]>='A'&&temp[i]<='Z'||temp[i]>='0'&&temp[i]<='9'||temp[i]==' '||temp[i]=='/')
        {
            temp_store[k++]=temp[i];
        }
    }           
            temp_store[k]='\0';
            mp3_file->data[index].frame_content = temp_store;  
}

int getsize(FILE* fptr)
{
    unsigned int value = 0;
    fread(&value, 1, 4, fptr);
     value=((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
    return value;
}

void get_display_data(F_block* mp3_file)
{    
     fseek(mp3_file->fptr_mp3_file,10,SEEK_SET);
     int count=0;
     char arr[5];
     
     int size;
     int n=50;
     int flag=0;
     while(n)
     {  
        fread(arr,1,4,mp3_file->fptr_mp3_file);
         n--;
         flag=0;
        for(int i=0;i<6;i++)
        {
            if(strcmp(arr,mp3_file->data[i].frame_id)==0)
            {
                count++;
                size = getsize(mp3_file->fptr_mp3_file);
                get_data(mp3_file,size,i);
                flag=1;
            }    
        }
        if(count==6)
            break;
            if(flag==0)
            {
              size = getsize(mp3_file->fptr_mp3_file);
              fseek(mp3_file->fptr_mp3_file,2+size,SEEK_CUR);    
            }
     }
    printf("--------------------------SONG DETAILS--------------------------------\n\n\n\n");
    printf("------------------------------------------------------------------------\n\n");
    printf("DESCRIPTION                             DETAILS\n");
    printf("-------------------------------------------------------------------------\n\n");
   for(int i=0;i<6;i++)
   {
    if((mp3_file->data[i].frame_content)!=NULL)
    printf("%s        %-10s\n\n",mp3_file->data[i].discription,mp3_file->data[i].frame_content);
    else
    printf("%s         Data not found\n",mp3_file->data[i].discription);

   }
  printf("-----------------------------------------------------------------------------\n\n");

}
Status open_file_to_edit(F_block* mp3_file)
{
    mp3_file->fptr_mp3_file = fopen(mp3_file->get_file_name,"rb"); 
    mp3_file->fptr_mp3_copy = fopen(mp3_file->mp3_copy_file,"wb");
    if(mp3_file->fptr_mp3_file==NULL||mp3_file->fptr_mp3_copy==NULL)
    {
        printf("failed to open file");
        return e_failure;
    }
    else
    return e_success;
    
}

void copy_rest_of_file(F_block* mp3_file)
{
    char temp[1024];
    int size;
    while((size = fread(temp,1,1024,mp3_file->fptr_mp3_file))>0)
    {
        fwrite(temp,1,size,mp3_file->fptr_mp3_copy);
    }

}

int convert_little_to_big(int value)
{
     return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
}

Status search(char*temp,F_block* mp3_file)
{   
    char find_frame_id[5]={0};
    char arr_to_cpy[200]={0};
    find_frame_id[4]='\0';
    fread(arr_to_cpy,1,10,mp3_file->fptr_mp3_file);
    fwrite(arr_to_cpy,1,10,mp3_file->fptr_mp3_copy);
    int n=30;
    int size;
    int flag=0;
    int check_flag=1;
    while(n){

      n--;
       fread(find_frame_id,1,4,mp3_file->fptr_mp3_file);
        int count=0;
       for(int i=0;i<4;i++)
       {
       
         if(!((find_frame_id[i]>='A'&&find_frame_id[i]<='Z')||(find_frame_id[i]>='0'&&find_frame_id[i]<='9')))
         {
            check_flag=0;
            
         }
         if(check_flag==0)
         {
            printf("\n\nCOUNT-> %d\n\n",count);
             printf("\nYour request was not found in the orginal file \n\n\n");
               copy_rest_of_file(mp3_file);
               return e_failure;
         }
       }
       fwrite(find_frame_id,1,4,mp3_file->fptr_mp3_copy);
      if(strcmp(temp,find_frame_id)==0)
      {
        size = getsize(mp3_file->fptr_mp3_file);
        flag=1;
        break;
      }
      size = getsize(mp3_file->fptr_mp3_file);
     if(size>500)
         {
             printf("\n\nYour request was not found in the orginal file \n\n");
               copy_rest_of_file(mp3_file);
               return e_failure;
         }
      fseek(mp3_file->fptr_mp3_file,-4,SEEK_CUR);
      fread(arr_to_cpy,1,4+2+size,mp3_file->fptr_mp3_file);
      fwrite(arr_to_cpy,1,4+2+size,mp3_file->fptr_mp3_copy);
    }
    if(flag==1){
    int value = mp3_file->add_data_size;
    value=((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
     fwrite(&value,4,1,mp3_file->fptr_mp3_copy);
     fread(arr_to_cpy,1,2,mp3_file->fptr_mp3_file);
     fwrite(arr_to_cpy,1,2,mp3_file->fptr_mp3_copy);
     fwrite(mp3_file->ptr_to_add,1,mp3_file->add_data_size,mp3_file->fptr_mp3_copy);
     fseek(mp3_file->fptr_mp3_file,size,SEEK_CUR);
     copy_rest_of_file(mp3_file);
     return e_success;
    }
    
    if(flag==0)
    {
        printf("\n\nYour request was not found in the orginal file \n\n");
        copy_rest_of_file(mp3_file);
        return e_failure;
    }
   // printf("1");
     copy_rest_of_file(mp3_file);
}

void close_heap_ptrs()
{
    
}



Status check_verify_file(char*argv[],F_block* mp3_file)
{
     initialize(mp3_file);
     void (*fun)(void)=close_heap_ptrs;
    atexit(fun);
    if(strcmp(argv[1],"-v")==0)
    {
        if(strstr(argv[2],".mp3")!=NULL)
        {
            mp3_file->get_file_name = argv[2];
           // printf("%s\n",mp3_file->get_file_name);
           if(open_file(argv,mp3_file)==e_success)
           {
              if(check_version(mp3_file->fptr_mp3_file)==e_success)
              {
                 get_display_data(mp3_file);
                   dupe_mp3_file = *mp3_file;
              }
              else{
                return e_failure;
              }
           }
           else{
             printf("failed to open file \n");
            return e_failure;
           }
        }
        else{
            printf("file is not .mp3\n");
            return e_failure;
        }       
    }
    else{
       char arr[6][3]={"-t","-a","-A","-y","-m","-c"};
       int option;
        if(strstr(argv[4],".mp3"))
        {
            int flag=0;
            for(int i=0;i<6;i++)
            {
                if(strcmp(arr[i],argv[2])==0)
                {
                  mp3_file->option=i;
                  flag =1;
                }
                
            }
            if(flag==0)
            {
                display_help();
                return e_failure;
            }
           
            mp3_file->get_file_name=argv[4];
            
           
            mp3_file->mp3_copy_file = argv[5];
            
           if(open_file_to_edit(mp3_file)==e_success)
           {
              char temp[5];
              int i = mp3_file->option;
              for(int j=0;j<4;j++)
              {
                temp[j]=mp3_file->data[i].frame_id[j];
              }
              temp[4]='\0';
              
              mp3_file->ptr_to_add=argv[3];
              mp3_file->add_data_size=strlen(argv[3]);
              
              search(temp,mp3_file);
              dupe_mp3_file = *mp3_file;
              printf("\n\n               Editing was Successfully Completed\n\n");
           }
           else{
            return e_failure;
           }             
        }
        else{
            e_failure;
        }
        return e_failure;
    }
}
