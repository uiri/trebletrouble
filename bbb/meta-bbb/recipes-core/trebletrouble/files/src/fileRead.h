/*Helper Methods*/

/*Creates TwinkleTwinkle.bin*/
void createTwinkle();

/* Reads the TwinkleTwinkle.bin pupulates songheader, rferences firs bar in ttls
   returns the first note of the song 
   THIS METHOD MUST BE CALLED IN ORDER TO PARSE THE SONG TWINKLE TWINKLE
*/
note * readTwinkle(song *ttls);

/*Will print out all of the bars as well as notes*/
void parseBar(song *ttls,note * notes);
/*will free allocated memory call this function, once you are done playing the song*/
void freeSong(song * ttls);

/*Increments to the following bar in memory*/
void incBar(song *ttls);

/*Increments to the following note in memory*/
void incNote(note * notes);



//Note API
/* Basically your getters for the note members*/
int getDuration(note *notes);
int getFrequency(note *notes);
int getNoteValue(note *notes);

/*This method does a print statement of what values your note is holding*/
void printNote(note *notes);

//Header API
/* Getters for your song members, mainly information about the song*/
char * getName(song * ttls);
int getBpm(song * ttls);
int getNumBars(song * ttls);
int getNumNotes(song * ttls);
char getKey(song * ttls);
unsigned char getTS(song * ttls);

/*This method does a print statement of what your song info is*/ 
void printHeader(song * ttls);



