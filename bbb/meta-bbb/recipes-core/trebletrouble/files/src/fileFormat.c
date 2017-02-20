#include <stdio.h>
#include "fileFormat.h"
void makeBin() {
	
	int i;
	int j;
	songFormatHeader sfh = {100,4,14,'C',51,"twinkle twinkle little star"};
	//12 bars of twinkle twinkle 
	
	bar allBars[4];
	note allNotes [14];
	
	i=0;
	j=0;
	//Fist Bar
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =261.6; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        allNotes[i].dotted = 0; 
	i++;
        
        allNotes[i].quantumvalue = CROTCHET; 
        allNotes[i].frequency =261.6; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        allNotes[i].dotted = 0; 
	i++;
                	
        allNotes[i].quantumvalue = CROTCHET; 
        allNotes[i].frequency =392.0; 
        allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
                	
        allNotes[i].quantumvalue = CROTCHET; 
        allNotes[i].frequency =392.0; 
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
	allNotes[i].frequency =415.3; 
	allNotes[i].duration = millisec(sfh.bpm,MINIM);
	allNotes[i].dotted = 0; 
	i++;
	
	//Third Bar

	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =349.2; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
        
       	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =349.2; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
        
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =329.63; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;

	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =329.63; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
	
	//Fourth Bar
	
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =293.66; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;
		
	allNotes[i].quantumvalue = CROTCHET; 
	allNotes[i].frequency =293.66; 
	allNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	allNotes[i].dotted = 0; 
	i++;

	allNotes[i].quantumvalue = MINIM; 
	allNotes[i].frequency =261.6; 
	allNotes[i].duration = millisec(sfh.bpm,MINIM);
	allNotes[i].dotted = 0; 
	i++;
	
	//Keep the starting point of each note stored in a bar	
	
	

	allBars[j]. numNotes =4;
	j++;
	

	allBars[j]. numNotes =3;
	j++;


	allBars[j]. numNotes =4;
	j++;


	allBars[j]. numNotes =3;
	
	
	/* Create the file */
	FILE *fh = fopen (TWINKLE_FILENAME, "wb+");
	fwrite(&sfh,sizeof(songFormatHeader),1,fh);
	fwrite (allBars, sizeof (bar), 4, fh);
        fwrite(allNotes,sizeof(note),14,fh);
	fclose(fh);
}

int millisec(int bpm, float note) {
	return (int)(
     60      /* seconds */
     * 1000  /* milliseconds per second */
     * note
     / (bpm *32)
		     );  
}    



