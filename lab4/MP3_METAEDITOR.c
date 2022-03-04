#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <uchar.h>


#define ID3_MAX_SIZE 128

typedef struct ID3TAG {
    char signature[3]; 
    char name[30]; 
    char artist[30]; 
    char album[30]; 
    char year[4]; 
    char description[30]; 
    unsigned short number_in_album[1];
    unsigned short null_byte[1]; 
    char genre; 
} ID3TAG;


long file_offset(FILE* fp) {
    fseek(fp, 0, SEEK_END); 
    return ftell(fp) - ID3_MAX_SIZE;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Rus");
    FILE* Filein = NULL;
    FILE* Fileout = NULL; 
    char* mp3_name;
    char* tag; 
    char* val;
    ID3TAG* TAGS = NULL;

   
    if (!strncmp(argv[1],"--filepath=",11)) {
        mp3_name = strpbrk(argv[1], "=") + 1;
        if ((Filein = fopen(mp3_name, "rb")) == NULL) {
            printf("Failed to open file %s for reading.\n", mp3_name);
            return 1;
        }
    }
    else {
        printf("Failed --filepath= .\n");
        return 1;
    }


    char* temp = (char*) malloc(sizeof(char) * ID3_MAX_SIZE);
    memset(temp, 0, ID3_MAX_SIZE);
    fseek(Filein, file_offset(Filein), SEEK_SET);
    fread(temp, 1, ID3_MAX_SIZE, Filein);
    TAGS = (ID3TAG*)(temp);


    /* --show */
    if(!strcmp("--show", argv[2])) {
        printf("\nSong Name:     %s\n",TAGS->name);
        printf("Artist:        %s\n",TAGS->artist);
        printf("Album:             %s\n",TAGS->album);
        printf("Year:                %s\n",TAGS->year);
        printf("Description:           %s\n",TAGS->description);
        printf("Number in album:           %s\n",TAGS->number_in_album);
        printf("null_byte:           %s\n",TAGS->null_byte);
        printf("Genre:               %d\n\n",TAGS->genre);
    }
    
        /* --get */
    else if (!strncmp("--get=", argv[2], 6)) {
        tag = strpbrk(argv[2], "=") + 1;
        if ( !strcmp(tag, "name") )
            printf("Song Name:     %s\n",TAGS->name);

        else if ( !strcmp(tag, "artist") )
            printf("Artist:        %s\n",TAGS->artist);

        else if ( !strcmp("album", tag) )
            printf("Album:             %s\n",TAGS->album);

        else if ( !strcmp("year", tag) )
            printf("Year:                %s\n",TAGS->year);

        else if ( !strcmp("description", tag))
            printf("Description:           %s\n",TAGS->description);

        else if ( !strcmp("number_in_album", tag) )
            printf("Number in album:               %d\n\n",TAGS->number_in_album);

        else if ( !strcmp("null_byte", tag) )
            printf("null_byte:               %d\n\n",TAGS->null_byte);

        else if ( !strcmp("genre", tag) )
            printf("Genre:               %d\n\n",TAGS->genre);

        else {
            printf("Unknown TAG.\n");
            return 1;
        }
    }
        /* --set= */
    else if (!strncmp("--set=", argv[2], 6)) {
        // Название тэга
        tag = strpbrk(argv[2], "=") + 1;
        if (!strncmp("--value=", argv[3], 8)) {
            val = strpbrk(argv[3], "=") + 1;
            if ( !strcmp(tag, "name") )
                memcpy(TAGS->name, val, 30);

            else if ( !strcmp(tag, "artist") )
                memcpy(TAGS->artist, val, 30);

            else if ( !strcmp(tag, "album") )
                memcpy(TAGS->album, val, 30);

            else if ( !strcmp(tag, "year") )
                memcpy(TAGS->year, val, 4);

            else if ( !strcmp(tag, "description") )
                memcpy(TAGS->description, val, 30);

            else if ( !strcmp("number_in_album", tag) )
                memcpy(TAGS->number_in_album, val, 1);

            else if ( !strcmp("null_byte", tag) )
                memcpy(TAGS->null_byte, val, 1);

            else if ( !strcmp("genre", tag) )
                memcpy(TAGS->genre, val, 1);

        }
        else {
            printf("Error with command --value=\n");
            return 1;
        }
    }

    else {
        printf("Unknown command: %s\n", argv[2]);
        return 1;
    }

    fclose(Filein);

    if ((Fileout = fopen(mp3_name, "wb+")) == NULL) {
        printf("Failed to open file %s  for writing.\n", mp3_name);
        return 0;
    }

    char* temp2 =(char*) malloc(sizeof(char) * ID3_MAX_SIZE);
    memset(temp2, 0, ID3_MAX_SIZE);
    temp2 = (char*)TAGS;
    fseek(Fileout, file_offset(Fileout), SEEK_SET);
    fwrite(temp2, 1, ID3_MAX_SIZE, Fileout);
    free(temp2);
    fclose(Fileout);

    return 0;
} 
