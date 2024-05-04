// Nakarin Phoorahong 6588070

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h>

char wordls[100000][100]; // wordlist  
int wsl=0; // wordlist size
int charcount=0; // character count
int wordcount=0; // word count
int misspelledc =0; // misspelled count

pthread_mutex_t mumutexx = PTHREAD_MUTEX_INITIALIZER;


// output file pointer
FILE *textout_file ;
FILE *statistic_file;

// make all text and word to be lower case first
void to_lower(char *word){
    // for(int i=0;i<strlen(word);i++){
    //     if(word[i]>='A' && word[i]<='Z'){
    //         word[i] = word[i] + 32;
    //     }
    // }
    for(int i=0;word[i];i++){
        word[i] = tolower(word[i]);
    }
}

//spell checking function based on the text file called "wordlist" and store in memory
void *spell_CK(void *arg){
    char *word = (char *)arg;
    to_lower(word);

    printf("word: %s\n",word);
    for(int i=0;i<wsl;i++){
        if(strcmp(word,wordls[i])==0) //the case of spelled correctly
        {
            return NULL;
        }
    }
    printf("[MS]%s[/MS]\n",word);
    pthread_mutex_lock(&mumutexx);
    // // normal way not file

    fprintf(textout_file,"[MS]%s[/MS]\n",word);
    misspelledc++; //increment misspelled count

    pthread_mutex_unlock(&mumutexx);
    return NULL;
}

///read wordlist file and store in memory
void read_wordlist(void *filename)
{
    FILE *file = fopen((char *)filename, "r");
    if (file == NULL)
    {
        printf("Error: File not found\n");
        exit(1);
    }
    while (fscanf(file, "%s", wordls[wsl]) != EOF)
    {
        wsl++;
    }
    fclose(file);
}

//counter function to count everything 
//1. no. of characters 
//2. no. of words
//3. no. of misspelled words
void *countering(void *arg){
    //implemeneting pthread mutex
    pthread_mutex_lock(&mumutexx);
    char *word = (char *)arg;
    if (arg == NULL) {
        // handle null argument appropriately
        return NULL;
    }

    charcount += strlen(arg);
    wordcount++;
    pthread_mutex_unlock(&mumutexx);

    return NULL;
}

int main(){
    printf("Hello World\n");
    printf("Welcome to My Project\n");
    printf("To exit the program you can type 'q'\n");
    printf("-------------------\n");
    read_wordlist("wordlist.txt");
    textout_file = fopen("output.txt","a");
    if(textout_file == NULL){
        printf("Error: File not found\n");
        exit(1);
    }
    statistic_file = fopen("statistic.txt","w");
    if(statistic_file == NULL){
        printf("Error: File not found\n");
        exit(1);
    }



    char word[300];
    pthread_t spell_CKtid, counteringtid;

    while(1) {
        if(scanf("%s",word) != EOF){
            if(strcmp(word,"q")==0){
                printf("-------------------\n");
                printf("Exiting the program\n");
                printf("-------------------\n");
                break;
            }
            // printf("word: %s\n",word);

            //normal calling
            // spell_CK(word);
            // countering_(word);
        

            //with pthread implementation
            pthread_create(&spell_CKtid,NULL,spell_CK, (void *)word);
            pthread_create(&counteringtid, NULL, countering, (void *)word);
// (void *(*)(void *))countering, (void *)word);


            //wait for both thread to finish 
            pthread_join(spell_CKtid,NULL);
            pthread_join(counteringtid,NULL);

            fprintf(textout_file,"%s\n",word);
        }
    
        else{
            break;
        }
    }
    // //normalway to print
    // printf("Character count: %d\n",charcount);
    // printf("Word count: %d\n",wordcount);
    // printf("Misspelled count: %d\n",misspelledc);

    fprintf(statistic_file,"Character count: %d\n",charcount);
    fprintf(statistic_file,"Word count: %d\n",wordcount);
    fprintf(statistic_file,"Misspelled count: %d\n",misspelledc);


    fclose(textout_file);
    fclose(statistic_file);

    
    return 0;
}