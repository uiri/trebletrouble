#include <stdio.h>
#include "fileFormat.h"
void makeBin(char * filename) {
	
	int i;
	int j;
	SongFormatHeader sfh = {100,4,14,C_MAJ,51,"twinkle twinkle little star"};
	//4 bars of twinkle twinkle 
	Bar allBars[4];
	Note allNotes [14];
	
	i=0;
	j=0;
	//Fist Bar
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =261.626; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        allNotes[i].dotted = 0; 
	i++;
        
        allNotes[i].quantumvalue = CROTCHET; 
        allNotes[i].frequency =261.626; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        allNotes[i].dotted = 0; 
	i++;
                	
        allNotes[i].quantumvalue = CROTCHET; 
        allNotes[i].frequency =392.628; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
                	
        allNotes[i].quantumvalue = CROTCHET; 
        allNotes[i].frequency =392.628; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        allNotes[i].dotted = 0; 
	i++;
        	
	//Second Bar
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =440.0; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
        
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =440.0; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
        
	allNotes[i].quantumvalue = MINIM; 
	allNotes[i].frequency =415.305; 
	allNotes[i].duration = millisec(sfh.bpm,MINIM);
	allNotes[i].dotted = 0; 
	i++;
	
	//Third Bar

	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =349.228; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
        
       	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =349.228; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
        
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =329.628; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;

	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =329.628; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
	
	//Fourth Bar
	
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =293.665; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
		
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =293.665; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;

	allNotes[i].quantumvalue = MINIM; 
	allNotes[i].frequency =261.626; 
	allNotes[i].duration = millisec(sfh.bpm,MINIM);
	allNotes[i].dotted = 0; 
	i++;
	
	//Keep the starting point of each note stored in a bar	
	
	

	allBars[j].notes =4;
	j++;
	

	allBars[j].notes =3;
	j++;


	allBars[j].notes =4;
	j++;


	allBars[j].notes =3;
	
	
	/* Create the file */
	FILE *fh = fopen (filename, "wb+");
	fwrite(&sfh,sizeof(SongFormatHeader),1,fh);
	fwrite (allBars, sizeof (Bar), 4, fh);
        fwrite(allNotes,sizeof(Note),14,fh);
	fclose(fh);
}

float millisec(int bpm, float note) {
	return (float)(
     60      /* seconds */
     * 1000  /* milliseconds per second */
     * note
     / (bpm *32)
		     );  
}    



