#include <stdio.h>
#include "fileFormat.h"
void makeBin() {
	
	int i;
	int j;
	note * start;
	bar * bstart;
	songFormatHeader sfh = {"twinkle twinkle little star",100,4,'C',"4/4"};
	//12 bars of twinkle twinkle 
	
	bar AllBars[4];
	note AllNotes [14];
	start = AllNotes;
	i=0;
	j=0;
	//Fist Bar
	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =261.6; 
        AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        AllNotes[i].dotted = 0; 
	i++;
        
        AllNotes[i].quantumvalue = CROTCHET; 
        AllNotes[i].frequency =261.6; 
        AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        AllNotes[i].dotted = 0; 
	i++;
        
        	
        AllNotes[i].quantumvalue = CROTCHET; 
        AllNotes[i].frequency =415.3; 
        AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
        	

        	
        AllNotes[i].quantumvalue = CROTCHET; 
        AllNotes[i].frequency =415.3; 
        AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
        AllNotes[i].dotted = 0; 
	i++;
        
	
	
	//Second Bar
	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =440.0; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
        
	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =440.0; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
        
	AllNotes[i].quantumvalue = MINIM; 
	AllNotes[i].frequency =415.3; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
	//Third Bar

	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =349.2; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
        
       	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =349.2; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
        
	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =329.63; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;

	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =329.63; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
	
	//Fourth Bar
	
	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =293.66; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
		
	AllNotes[i].quantumvalue = CROTCHET; 
	AllNotes[i].frequency =293.66; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;

	AllNotes[i].quantumvalue = MINIM; 
	AllNotes[i].frequency =261.6; 
	AllNotes[i].duration = millisec(sfh.bpm,CROTCHET);
	AllNotes[i].dotted = 0; 
	i++;
	
	//Keep the starting point of each note stored in a bar	
	
	

	AllBars[j]. NumNotes =4;
	AllBars[j]. value = start;
	start += AllBars[j].NumNotes;
	j++;
	

	AllBars[j]. NumNotes =3;
	AllBars[j]. value = start;
	start += AllBars[j].NumNotes;
	j++;


	AllBars[j]. NumNotes =4;
	AllBars[j]. value = start;
	start += AllBars[j].NumNotes;
	j++;


	AllBars[j]. NumNotes =4;
	AllBars[j]. value = start;
	start += AllBars[j].NumNotes;
	j++;
	
	song Twinkle = {&sfh,AllBars };

	/* Create the file */
	FILE *fh = fopen ("Twinkle.bin", "wb+");
	fwrite(&Twinkle,sizeof(song),1,fh);
	fwrite(&sfh,sizeof(songFormatHeader),1,fh);
	fwrite (AllBars, sizeof (bar), 4, fh);
        fwrite(AllNotes,sizeof(note),14,fh);
	
	}

int millisec(int bpm, double note) {
	return (int)(
     60      /* seconds */
     * 1000  /* milliseconds per second */
     * note
        / bpm
		     );  
}    



