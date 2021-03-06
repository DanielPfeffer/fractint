#ifndef WINPROT_H
#define WINPROT_H

/* This file contains prototypes for win specific functions. */

/*  calmanp5 -- assembler file prototypes */

extern long cdecl calcmandfpasm_p5(void);
extern void cdecl calcmandfpasmstart_p5(void);

/*  wgeneral -- assembler file prototypes */

extern long cdecl multiply(long, long, int);
extern long cdecl divide(long, long, int);

/*  dialog -- C file prototypes */

extern void PrintFile(void);
extern int Win_OpenFile(unsigned char *);
extern Win_SaveFile(unsigned char *);

/*  mainfrac -- C file prototypes */

extern int fractint_main(void);

/*  prompts1 -- C file prototypes */

extern void set_default_parms(void);

/*  windos -- C file prototypes */

extern void debugmessage(char *, char *);
extern void texttempmsg(char far *);
extern int stopmsg(int , char far *);
extern int keypressed(void);
extern int getakey(void);
extern int  farread(int, VOIDFARPTR, unsigned);
extern int  farwrite(int, VOIDFARPTR, unsigned);
extern void far_memcpy(void far *, void far *, int);
extern void far_memset(void far *, int , int);
extern int getcolor(int, int);
extern int out_line(BYTE *, int);
extern void putcolor_a (int, int, int);
extern int clear_screen(int);
extern void spindac(int, int);
extern void buzzer (int);
extern int thinking(int, char far *);
extern void CalibrateDelay(void);
extern void start_wait(void);
extern void end_wait(void);

extern int get_video_mode(struct fractal_info *,struct ext_blk_3 *);
extern int check_vidmode_keyname(char *);
extern void vidmode_keyname(int, char *);
extern int check_vidmode_key(int, int);
extern int put_line(int, int, int, BYTE *);
extern int get_line(int, int, int, BYTE *);
extern void restoredac(void);
extern void reset_zoom_corners(void);
extern void flush_screen(void);

extern int win_load(void);
extern void win_save(void);
extern void win_cycle(void);

extern void far * cdecl farmemalloc(long);
extern void farmemfree(void far *);
extern int far_memcmp(void far *, void far *, int);
extern int far_strlen (char far *);
extern void far_strcpy (char far *, char far *);
extern int far_strcmp (char far *, char far *);
extern int far_strnicmp (char far *, char far *, int);
extern void far_strcat (char far *, char far *);

extern void winfract_help(void);

/*  windos2 -- C file prototypes */

extern void movecursor(int, int);
extern void setattr(int, int, int, int);
extern int  putstringcenter(int, int, int, int, char far *);
extern void putstring(int, int, int, unsigned char far *);
extern int  fullscreen_choice(
             int, char far *, char far *, char far *, int,
             char far * far *, int far *, int, int,
             int, int, void (*)(),
             char *, int (*)(), int (*)());
extern int  strncasecmp(char far *,char far *,int);
extern int  input_field(int, int, char *, int, int, int, int (*)(int) );
extern int  field_prompt(int, char far *, char far *, char *, int, int (*)(int) );
extern void helptitle(void);
extern void stackscreen(void);
extern void unstackscreen(void);
extern void discardscreen(void);
extern void discardgraphics(void);
extern int load_palette(void);
extern void save_palette(void);
extern void fractint_help(void);
extern int getakeynohelp(void);
extern int win_make_batch_file(void);
extern int fractint_getkeypress(int);
extern int keycursor(int, int);
extern char *despace(char *str);
extern int savegraphics(void);
extern int restoregraphics(void);

extern int main_menu(int);

/*  winfract -- C file prototypes */

extern void win_set_title_text(void);
extern void win_savedac(void);

/*  winstubs -- C file prototypes */

extern void rotate(int);
extern void find_special_colors(void);
extern int spawnl(int, char *, char *);
extern int showtempmsg(char far *);
extern void cleartempmsg(void);
extern void freetempmsg(void);
extern int FromMemDisk(long, int, void far *);
extern int ToMemDisk(long, int, void far *);
extern int _fastcall common_startdisk(long, long, int);
extern long cdecl normalize(char far *);
extern void drawbox(int);

extern void farmessage(unsigned char far *);
extern void setvideomode(int, int, int, int);
extern int fromvideotable(void);
extern void home(void);

extern int intro_overlay(void);
extern int rotate_overlay(void);
extern int printer_overlay(void);
extern int pot_startdisk(void);
extern void SetTgaColors(void);
extern int startdisk(void);
extern void enddisk(void);
extern int readdisk(unsigned int,unsigned int);
extern void writedisk(unsigned int,unsigned int,unsigned int);
extern int targa_startdisk(FILE *,int);
extern void targa_writedisk(unsigned int,unsigned int,BYTE,BYTE,BYTE);
extern void targa_readdisk(unsigned int,unsigned int,BYTE *,BYTE *,BYTE *);
extern int SetColorPaletteName(char *);
extern BYTE far *findfont(int);
extern long cdecl readticker(void);
extern void EndTGA(void);

extern int key_count(int);

extern void dispbox(void);
extern void clearbox(void);
extern void _fastcall addbox(struct coords);
extern void _fastcall drawlines(struct coords, struct coords, int, int);
extern int showvidlength(void);

extern int get_sound_params(void);
extern int soundon(int);
extern void soundoff(void);
extern int initfm(void);
extern void mute(void);

extern void dvid_status(int, char far *);
extern int tovideotable(void);
extern void TranspPerPixel(void);
extern void stopslideshow(void);
extern void aspectratio_crop(float, float);
extern void setvideotext(void);
extern int load_fractint_cfg(int);

/*  wintext -- C file prototypes */

extern void wintext_destroy(void);
extern int wintext_texton(void);
extern int wintext_textoff(void);
extern void wintext_putstring(int, int, int, char far *);
extern void wintext_paintscreen(int, int, int, int);
extern void wintext_cursor(int, int, int);
extern int wintext_look_for_activity(int);
extern void wintext_addkeypress(unsigned int);
extern unsigned int wintext_getkeypress(int);

#endif

