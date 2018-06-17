/**
* Kolegij: "Programiranje 2"
* Datum:
* Ime i prezime:
* Ocjena:
*/

/******** PREPROCESSOR DEFINITION ********/

#define _CRT_SECURE_NO_WARNINGS // used in order to disable "secure" warnings

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SONG_MAX_NAME (128)
#define MAX_NAME      (128)
#define MAX_SONGS      (10)

/******** TYPE DECLARATION ********/

typedef struct _song {
    char name[SONG_MAX_NAME];
    int durationSec;
}Song;

typedef struct _artist {
    char name[MAX_NAME];
    Song songs[MAX_SONGS];
    int numberOfSongs;
}Artist;

/******** FUNCTION DECLARATION ********/

int countArtistsFromFile(char *fileName);
Artist *readArtistFromFile(char *fileName, int count);
Artist *findArtistInArray(Artist *artists, int count, char *name);
int addSongToArtist(Artist *artist, char *songName, int durationSec);
int readArtistSongsFromFile(char *fileName, Artist *artists, int count);
void printArtists(Artist *artists, int count);

/******** FUNCTION IMPLEMENTATION ********/

/**
* Napisati program koji koristi definirane funkcije u ovoj datoteci.
*
* 1) Program u poèetku korisnika pita nazive datoteka gdje se nalaze popisani izvoðaèi (artists.txt)
*    zatim popis pjesama od izvoðaèa (songs.txt).
*
* 2) countArtistsFromFile()
*    - Proèitati broj izvoðaèa iz datoteke i spremiti ga u lokalnu varijablu.
*
* 3) readArtistFromFile()
*    - Proèitati izvoðaèe iz datoteke i spremiti u pokazivaè tipa Artist.
*
* 4) readArtistSongsFromFile()
*    - Proèitati pjesme izvoðaèa i spremiti ih u niz
*
* 5) printArtists()
*    - Ispisati izvoðaèe skupa s njihovim pjesmama
*
* 6) osloboditi svu dinamièki rezerviranu memoriju.
*
* Minimum: { 1, 2, 3, 5 i 6 } Nije potrebno ispisivati pjesme na konzolu u ovom sluèaju.
*
* Opis svake funkcije koju je potrebno implementirati nalazi se iznad deklaracije svake funkcije.
*
* Sretno!
*
* @retval 0 - Program ended successfully.
* @retval 1 - Program didn't end successfully
*/
int main(int argc, char *argv[])
{
    Artist *artists = NULL;
    int count = 0;
    int result = 0;

    count = countArtistsFromFile("artists.txt");

    if (count < 1)
    {
        return EXIT_FAILURE;
    }

    artists = readArtistFromFile("artists.txt", count);

    if (artists == NULL)
    {
        return EXIT_FAILURE;
    }

    result = readArtistSongsFromFile("songs.txt", artists, count);

    if (result != 0)
    {
        free(artists);
        return EXIT_FAILURE;
    }

    printArtists(artists, count);
    free(artists);
    return EXIT_SUCCESS;
}

/**
* Count number of artists from file.
* If file doesn't open, function prints "File <file-name> not found" on console screen.
*
* @param fileName - the filename
*
* @retval -1 - file not found (error)
* @retval 0 - file empty (no records found)
* @retval > 0 - count
*/
int countArtistsFromFile(char *fileName)
{
    FILE *fp = NULL;
    int count = 0;
    char buffer[MAX_NAME] = { 0 };

    fp = fopen(fileName, "r");

    if (!fp)
    {
        printf("File %s not found!\r\n", fileName);
        return -1;
    }

    while (!feof(fp))
    {
        fgets(buffer, MAX_NAME, fp);
        count++;
    }

    fclose(fp);

    return count;
}

/**
* Read artists from file.
*
* If dynamically allocation fails, function prints "Dynamically allocation failed!" on console screen.
* If file doesn't open, function prints "File <file-name> not found" on console screen.
*
* @param fileName - the filename
* @param count - number of artists in file
*
* @retval NULL - dynamically allocation failed or file not found.
* @retval <address> - the address of dynamically allocated array.
*/
Artist *readArtistFromFile(char *fileName, int count)
{
    Artist *artists = NULL;
    FILE *fp = NULL;
    int i = 0;

    artists = calloc(count, sizeof(Artist));

    if (!artists)
    {
        printf("Dynamically allocation failed!\r\n");
        return NULL;
    }

    fp = fopen(fileName, "r");

    if (!fp)
    {
        printf("File %s not found!\r\n", fileName);
        return NULL;
    }

    while (!feof(fp))
    {
        fscanf(fp, "%s", artists[i].name);
        i++;
    }

    fclose(fp);

    return artists;
}

/**
* Find artist by name in artists array.
*
* @param artists - the array of artists
* @param count - number of artists in array
* @param name - artist name to find
*
* @retval <address> - the artist
* @retval NULL - artist not found
*/
Artist *findArtistInArray(Artist *artists, int count, char *name)
{
    int i = 0;

    for (i = 0; i < count; i++)
    {
        if (strcmp(artists[i].name, name) == 0)
        {
            return &artists[i];
        }
    }

    return NULL;
}

/**
* Add new song into artist song array.
*
* On success:
*
*     Number of artist songs are increased by one.
*     New song is written into first non initialized array element.
*
* On failure the following output shall appear as shown in example below:
*
*     Artist "The_Beatles" reached song limit!
*     Not added "Hey_Jude" 03:14
*
* @param artist - The artist where song will be added
* @param songName - The name of song
* @param durationSec - duration in seconds
*
* @retval 0 - song added successfully
* @retval -1 - reached song limit
*/
int addSongToArtist(Artist *artist, char *songName, int durationSec)
{
    Song *song = NULL;

    if (artist->numberOfSongs >= MAX_SONGS)
    {
        printf("Artist %s reached limit!\r\nNot added %s %d:%d\r\n",
               artist->name,
               songName,
               durationSec / 60,
               durationSec % 60);

        return -1;
    }

    song = &artist->songs[artist->numberOfSongs];

    strcpy(song->name, songName);
    song->durationSec = durationSec;

    artist->numberOfSongs++;

    return 0;
}

/**
* Read artist songs from file.
*
* @prerequisite: addSongToArtist(), findArtistInArray().
*
* @param fileName - the file name of songs.
* @param artists - the artist array
* @param count - number of artists
*
* @retval 0 - songs read successfully
* @retval -1 - file not found
* @retval -2 - artist not found in array
* @retval -3 - song was not added successfully
*/
int readArtistSongsFromFile(char *fileName, Artist *artists, int count)
{
    FILE *fp = NULL;
    char artistName[SONG_MAX_NAME] = { 0 };
    char songName[SONG_MAX_NAME] = { 0 };
    int durationSec = 0;
    int result = 0;
    Artist *artist = NULL;
    Song *song = NULL;

    fp = fopen(fileName, "r");

    if (!fp)
    {
        perror("");
        return -1;
    }

    while (!feof(fp))
    {
        fscanf(fp, "%s %s %d", artistName, songName, &durationSec);

        artist = findArtistInArray(artists, count, artistName);

        if (!artist)
        {
            return -2;
        }

        result = addSongToArtist(artist, songName, durationSec);

        if (result != 0)
        {
            return -3;
        }
    }

    fclose(fp);

    return 0;
}

/**
* Print artists.
*
* Example output:
*
* Pjevac1:
*    - Pjesma1 3:17
*    - Pjesma2 2:14
*         ...
*    - PjesmaN 6:17
* Pjevac2:
*    - Pjesma1 4:05
*         ...
*    - PjesmaM 6:17
*/
void printArtists(Artist *artists, int count)
{
    int i = 0;
    int j = 0;
    Artist *artist = NULL;
    Song *song = NULL;

    for (i = 0; i < count; i++)
    {
        artist = artists + i;

        printf("Artist: %s\r\n", artist->name);

        for (j = 0; j < artist->numberOfSongs; j++)
        {
            song = artist->songs + j;

            printf("\t%d. %s %02d:%02d\r\n",
                   j + 1,
                   song->name,
                   song->durationSec / 60,
                   song->durationSec % 60);
        }
    }
}
