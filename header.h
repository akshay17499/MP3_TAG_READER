#ifndef HEADER_H
#define HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct data
{
    char frame_id[5];
    char* frame_content;
    char* discription;
}data;

typedef struct mp3_block_data
{
    char* get_file_name;
    FILE* fptr_mp3_file;
    FILE* fptr_mp3_copy;
    char* ptr_to_add;
    int add_data_size;
    int option;
    char* mp3_copy_file; 
    struct data data[6];
}F_block;

typedef enum 
{
    e_success,
    e_failure
}Status;

void display_help() ;

void initialize(F_block* mp3_file);

Status open_file(char*argv[],F_block* mp3_file);

Status check_version(FILE* fptr);

void get_data(F_block* mp3_file,int size,int index);

int getsize(FILE* fptr);

void get_display_data(F_block* mp3_file);

Status open_file_to_edit(F_block* mp3_file);

void copy_rest_of_file(F_block* mp3_file);

int convert_little_to_big(int value);

Status search(char*temp,F_block* mp3_file);

Status check_verify_file(char*argv[],F_block* mp3_file);

void close_heap_ptrs();

#endif
