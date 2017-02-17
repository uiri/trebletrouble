
/* Different lengths of Notes*/
#define SEMIBREVE       (1.0)
#define MINIM           (1.0/2)
#define CROTCHET        (1.0/4)
#define QUAVER          (1.0/8)
#define SEMIQUAVER      (1.0/16)
#define DEMISEMIQUAVER  (1.0/32)

/* Struct for Note*/
typedef struct {
	/*Will be defined by a helper function*/
	int duration;
	double frequency;
	int quantumvalue;
	/*dotted will be 1 or 0*/
	int dotted;
	
	
}note;

/* Evaluation for how many notes per bar to be done in code*/
typedef struct{
	
	int NumNotes;
	note * value;
}bar;




typedef struct {
	char name[50];
	int bpm;
	int NumBars;
	char Key[1];
	char timeSignature[3];
}songFormatHeader;

typedef struct {

	songFormatHeader * sfh;
	bar * fbar;
	
}song;


int millisec(int bpm, double note);

void makeBin();
