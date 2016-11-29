#ifndef DISPLAY_H_
#define DISPLAY_H_

#define NOTE_PIC "/srv/trebletrouble/note.pnm"
#define L_PIC "/srv/trebletrouble/note_l.pnm"
#define SONG "/srv/trebletrouble/testnotes" 

int find_freq(double freq);
int find_ind(char note, int oct);

int bitblit(char* filename, char* fbp, int x, int y);
int bitblit_colour(char* filename, char* fbp, int x, int y, short colour);
void colour_screen(char* fbp, short colour);

void draw_staff(char* fbp);
void compare_notes(int expected, int actual, int i, char* fbp);
void load_song(FILE *file, char *fbp);

char *init_display(int *fbfd);
void cleanup_display(char* fbp, int *fbfd);

#endif
