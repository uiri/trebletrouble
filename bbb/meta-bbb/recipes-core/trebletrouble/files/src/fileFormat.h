#ifndef FILEFORMAT_H_
#define FILEFORMAT_H_
/* Different lengths of Notes*/
#define SEMIBREVE       32
#define MINIM           16
#define CROTCHET        8
#define QUAVER          4
#define SEMIQUAVER      2
#define DEMISEMIQUAVER  1

#define TWINKLE_FILENAME "TwinkleTwinkle.bin"
#define C_FLAT_MAJ  0b01111111
#define A_FLAT_MIN  0b01111111
#define G_FLAT_MAJ  0b01111110
#define E_FLAT_MIN  0b01111110
#define D_FLAT_MAJ  0b01111100
#define B_FLAT_MIN  0b01111100
#define A_FLAT_MAJ  0b01111000
#define F_MIN       0b01111000
#define E_FLAT_MAJ  0b01110000
#define C_MIN       0b01110000
#define B_FLAT_MAJ  0b01100000
#define G_MIN       0b01100000
#define F_MAJ       0b01000000
#define D_MIN       0b01000000
#define C_MAJ       0b00000000
#define A_MIN       0b00000000
#define G_MAJ       0b11000000
#define E_MIN       0b11000000
#define D_MAJ       0b11100000
#define B_MIN       0b11100000
#define A_MAJ       0b11110000
#define F_SHARP_MIN 0b11110000
#define E_MAJ       0b11111000
#define C_SHARP_MIN 0b11111000
#define B_MAJ       0b11111100
#define G_SHARP_MIN 0b11111100
#define F_SHARP_MAJ 0b11111110
#define D_SHARP_MIN 0b11111110
#define C_SHARP_MAJ 0b11111111
#define A_SHARP_MIN 0b11111111
/* Struct for Note*/
typedef struct {
	/*Will be defined by a helper function*/
	float duration;
	float frequency;
	int quantumvalue;
	/*dotted will be 1 or 0*/
	int dotted;
} Note;

/* Evaluation for how many notes per bar to be done in code*/
typedef struct{
	int notes;
	int barspace;	
} Bar;

typedef struct {
	int bpm;
	int numBars;
	int numNotes;
	char key;
	unsigned char timeSignature;
	char name[50];
} SongFormatHeader;

typedef struct {

	SongFormatHeader * sfh;
	Bar * fbar;
	int* expected;
	
} Song;


float millisec(int bpm, unsigned char ts, float note);

void makeBin(char * filename);

#endif
