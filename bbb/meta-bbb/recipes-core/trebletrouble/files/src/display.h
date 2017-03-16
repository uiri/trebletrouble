#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include "fileFormat.h"

#define NOTE_PIC "/srv/trebletrouble/note.pnm"
#define L_PIC "/srv/trebletrouble/note_l.pnm"
#define SONG "/srv/trebletrouble/testnotes"
#define FLAT "/srv/trebletrouble/flat.pnm"
#define SHARP "/srv/trebletrouble/sharp.pnm" 
#define BAR "/srv/trebletrouble/bar.pnm"

/*Below are pictures for the notes and rests:
_F for flipped, _N for note and _R for rest
W: Whole
H: Half
Q: Quater
E: Eighth
S: Sixteenth */
#define W_N "/srv/trebletrouble/whole_n.pnm"
#define H_N "/srv/trebletrouble/half_n.pnm"
#define Q_N "/srv/trebletrouble/quarter_n.pnm"
#define E_N "/srv/trebletrouble/eighth_n.pnm"
#define S_N "/srv/trebletrouble/sixteenth_n.pnm"
#define W_N_F "/srv/trebletrouble/whole_n_f.pnm"
#define H_N_F "/srv/trebletrouble/half_n_f.pnm"
#define Q_N_F "/srv/trebletrouble/quarter_n_f.pnm"
#define E_N_F "/srv/trebletrouble/eighth_n_f.pnm"
#define S_N_F "/srv/trebletrouble/sixteenth_n_f.pnm"
#define W_R "/srv/trbletrouble/whole_r.pnm"
#define H_R "/srv/trebletrouble/half_r.pnm"
#define Q_R "/srv/trebletrouble/quarter_r.pnm"
#define E_R "/srv/trebletrouble/eighth_r.pnm"
#define S_R "/srv/trebletrouble/sixteenth_r.pnm"
/*XS is the x space between notes on the xaxis */
#define XS 42

/*Temp: key signatures*/
#define C_FLAT_MAJ  0b01111111
#define A_FLAT_MIN  0b01111111
#define G_FLAT_MAJ  0b01111110
#define E_FLAT_MIN  0b01111110
#define D_FLAT_MAJ  0b01111100
#define B_FLAT_MIN  0b01111100
#define A_FLAT_MAJ  0b01111000
#define F_MIN 	    0b01111000
#define E_FLAT_MAJ  0b01110000
#define C_MIN       0b01110000
#define B_FLAT_MAJ  0b01100000
#define G_MIN       0b01100000
#define F_MAJ       0b01000000
#define D_MIN       0b01000000
#define C_MAJ       0b00000000
#define A_MIN       0b00000000
#define G_MAJ       0b11000000
#define E_MIN	    0b11000000
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

int find_freq(double freq);
int find_ind(char note, int oct);
int normalize_freq(int index);

int bitblit(char* filename, char* fbp, int x, int y);
int bitblit_colour(char* filename, char* fbp, int x, int y, short colour);
void colour_screen(char* fbp, short colour);

void draw_staff(char* fbp);
int compare_notes(Song* song, Note* note, int* actuals, int i, int j, char* fbp, int barspace);
void clear_notes(int i, int *expected, int *actual, char* fbp, int len, int value);
void load_song(char *fbp, Note * notes, Song * song);

void draw_worst_bar(char* fbp, Bar* worstBar, Note* notes, Song* song);
Bar* find_worst_bar(Song* song, int* actuals);


#endif
