#ifndef FILEREAD_H_
#define FILEREAD_H_
/*Helper Methods*/

/*use makeBin() to create TwinkleTwinkle.bin*/

/* Reads the TwinkleTwinkle.bin pupulates songheader, references the first bar in ttls
   returns the first note of the Song 
   THIS METHOD MUST BE CALLED IN ORDER TO PARSE THE SONG TWINKLE TWINKLE
*/
Note * readTwinkle(Song *ttls, char * filename);

/*Will print out all of the bars as well as notes*/
void parseBar(Song *ttls,Note * notes);
/*will free allocated memory call this function, once you are done playing the Song*/
void freeSong(Song * ttls);

/*Increments to the following bar in memory*/
void incBar(Song *ttls);

/*Increments to the following Note in memory*/
void incNote(Note * notes);

//Note API
/* Basically your getters for the Note members*/
float getDuration(Note *notes);
float getFrequency(Note *notes);
int getNoteValue(Note *notes);

/*This method does a print statement of what values your Note is holding*/
void printNote(Note *notes);

//Header API
/* Getters for your Song members, mainly information about the Song*/
char * getName(Song * ttls);
int getBpm(Song * ttls);
int getNumBars(Song * ttls);
int getNumNotes(Song * ttls);
char getKey(Song * ttls);
unsigned char getTimeSig(Song * ttls);

/*This method does a print statement of what your Song info is*/ 
void printHeader(Song * ttls);

#endif
