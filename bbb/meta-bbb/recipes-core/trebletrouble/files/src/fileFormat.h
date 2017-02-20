
/* Different lengths of Notes*/
#define SEMIBREVE       32
#define MINIM           16
#define CROTCHET        8
#define QUAVER          4
#define SEMIQUAVER      2
#define DEMISEMIQUAVER  1

#define TWINKLE_FILENAME "TwinkleTwinkle.bin"

/* Struct for Note*/
typedef struct {
	/*Will be defined by a helper function*/
	int duration;
	float frequency;
	int quantumvalue;
	/*dotted will be 1 or 0*/
	int dotted;
	}note;

/* Evaluation for how many notes per bar to be done in code*/
typedef struct{
	int numNotes;
}bar;

typedef struct {
	int bpm;
	int numBars;
	int numNotes;
	char key;
	unsigned char timeSignature;
	char name[50];
}songFormatHeader;

typedef struct {

	songFormatHeader * sfh;
	bar * fbar;
	
}song;


int millisec(int bpm, float note);

void makeBin();
