#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>

#define NOTE_PIC "/srv/trebletrouble/note.pnm"
#define L_PIC "/srv/trebletrouble/note_l.pnm"
#define SONG "/srv/trebletrouble/testnotes" 
#define NUM_NOTES 16

/*Below are pictures for the notes:
Q_N - quater note
W_N - whole note
_F is flipped */
#define Q_N "/srv/trebletrouble/q_n.pnm"
#define Q_N_F "/srv/trebletrouble/q_n_f.pnm"
#define W_N "/srv/trebletrouble/w_n.pnm"
/*XSTART is the start of the notes on the xaxis on the staff
XS is the x space between notes on the xaxis */
#define XSTART 110
#define XS 42
int find_freq(double freq);
int find_ind(char note, int oct);

int bitblit(char* filename, char* fbp, int x, int y);
int bitblit_colour(char* filename, char* fbp, int x, int y, short colour);
void colour_screen(char* fbp, short colour);

void draw_staff(char* fbp);
void compare_notes(int expected, int actual, int i, char* fbp);
void clear_notes(int i, int expected[NUM_NOTES], int actual[NUM_NOTES], char* fbp);
void load_song(FILE *song, char *fbp, int expected[NUM_NOTES]);

char *init_display(int *fbfd);
void cleanup_display(char* fbp, int *fbfd);

#endif
