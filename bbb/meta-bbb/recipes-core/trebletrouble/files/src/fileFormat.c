#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileRead.h"
#include "fileFormat.h"


void rl(FILE * fp,char * str);
char eval(int key);


char  eval(int key){
	
	char a_key;
	switch(key) {
	case 1 :a_key=C_FLAT_MAJ;  
		break;
	case 2:	a_key=A_FLAT_MIN;  
		break;
	case 3 :a_key=G_FLAT_MAJ;
		break;
	case 4:	a_key=E_FLAT_MIN;  
		break;
	case 5:	a_key=D_FLAT_MAJ;
		break;
	case 6:	a_key=B_FLAT_MIN;
		break;
	case 7:	a_key=A_FLAT_MAJ;
		break;
	case 8:	a_key=F_MIN;
		break;
	case 9:	a_key=E_FLAT_MAJ;  
		break;
	case 10:a_key=C_MIN;
		break;
	case 11:a_key=B_FLAT_MAJ;
		break;
	case 12:a_key=G_MIN;
		break;
	case 13:a_key=F_MAJ;
		break;
	case 14:a_key=D_MIN;
		break;
	case 15:a_key=C_MAJ;
		break;
	case 16:a_key=A_MIN;
		break;
	case 17:a_key=G_MAJ;
		break;
	case 18:a_key=E_MIN;
		break;
	case 19:a_key=D_MAJ;
		break;
	case 20:a_key=B_MIN;
		break;
	case 21:a_key=A_MAJ;
		break;
	case 22:a_key=F_SHARP_MIN;
		break;
	case 23:a_key=E_MAJ;
		break;
	case 24:a_key=C_SHARP_MIN;
		break;
	case 25:a_key=B_MAJ;
		break;
	case 26:a_key=G_SHARP_MIN;
		break;
	case 27:a_key=F_SHARP_MAJ;
		break;
	case 28:a_key=D_SHARP_MIN;
		break;
	case 29:a_key=C_SHARP_MAJ;
		break;
	case 30:a_key=A_SHARP_MIN;
		break;
	default :printf("Invalid grade\n" );
		a_key = 'b';
		break;
	}

	return a_key;	
}

void makeBin(char * filename, char * binname)
{
	FILE * fp;
	char str[60];
	int bpm;
	int numBars;
	int numNotes;
	char key;
	unsigned char timeSig;
	char * name;
	int i;
	int j;

	/* opening file for reading */
	fp = fopen(filename , "r");
	rl(fp,str);
	bpm = atoi(str);
	rl(fp,str);
	numBars = atoi(str);
	rl(fp,str);
	numNotes = atoi(str);
	rl(fp,str);
	key = eval(atoi(str));
	rl(fp,str);
	timeSig =(unsigned char)atoi(str);
	rl(fp,str);
	name = str;
	SongFormatHeader sfh = {bpm,numBars,numNotes,key,timeSig,};
	//100,4,14,C_FLAT_MAJ,51,"twinkle twinkle little star"
	strcpy(sfh.name, name);
	printf("This is the int %d\n",bpm);
	printf("This is the int %d\n",numBars);
	printf("This is int numNotes %d\n", numNotes);
	printf("This is the key %c", key);
	printf("This is the timeSig %d\n",timeSig);
	printf("This is the name %s\n",name);
	Bar allBars[sfh.numBars];
	Note allNotes[sfh.numNotes];
		
	for (j = 0;j<sfh.numBars;j++){
			rl(fp,str);
			allBars[j].notes = atoi(str);

	}
	
	for (i = 0;i<sfh.numNotes;i++){
		rl(fp,str);
		allNotes[i].quantumvalue =atoi(str); 
		rl(fp,str);
		allNotes[i].frequency = atof(str);
		allNotes[i].duration = millisec(sfh.bpm,sfh.timeSignature,allNotes[i].quantumvalue);
		rl(fp,str);
		allNotes[i].dotted = atoi(str);
	}

	
	/* Create the file */
	
	strcpy(binname,sfh.name); 
	strcat(binname, ".bin");
	FILE *fh = fopen (binname, "wb+");
	fwrite(&sfh,sizeof(SongFormatHeader),1,fh);
	fwrite (allBars, sizeof (Bar),sfh.numBars, fh);
        fwrite(allNotes,sizeof(Note),sfh.numNotes,fh);
	fclose(fh);
	
}

float millisec(int bpm, unsigned char ts, float note)
{
	/* (60 seconds per minute * 1000 milliseconds per second) 
	 * / (beats per minute * adjustment factor) */
	float adj = 32.0 / ((float)CROTCHET);

	/*
	 * 6/8, 9/8 and 12/8 use dotted CROTCHETs
	 * (6-1) * 16 + (8-1) = 87
	 * (9-1) * 16 + (8-1) = 135
	 * (12-1) * 16 + (8-1) = 183
	 */
	if (ts == 87 || ts == 135 || ts == 183)
		adj /= 1.5;

	return (float)(60 * 1000 * note) / (bpm * adj);
}

void rl(FILE * fp, char *str)
{
	if(fp == NULL){
			perror("Error opening file");
			fclose(fp);
	}
	if( fgets (str, 200, fp)!=NULL ){
			strtok(str, "\n");
			/* writing content to stdout */
			puts(str);
	
	}
	


}
