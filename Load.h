#ifndef Load_H
#define Load_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct group_t {
    char *praktikum;
    char *rombongan;
}Group;

typedef struct asprak_t {
    char *nama;
    char code;
    char *praktikum[3];
    char *berhalangan[3];
}Asprak;

typedef struct lab_t {
    char *element;
    Group  group;
    Asprak  asprak[2]; 
}Lab;
typedef struct day_t {
    char *element;
    Lab lab [4];
}Day;

typedef struct week_t {
    int element;
    Day day[5];
} Week;

typedef struct schedule_t {
    Week week [12];
} Schedule;

typedef struct temp_t {
    int minggu;
    char hari[6];
    char ruangan [4];
    char kode [6];
    char rombongan [2];
    char lab [4];
} Temp;

//structur data asisten
typedef struct dataAsprak_t {
   Asprak asprak [14];
} dataAsprak;

typedef struct elka_t {
    char*nama[6];
    int size;
    int modul;
} ELKA;

typedef struct ppmc_t {
    char*nama[9];
    int size;
    int modul;
}PPMC;

typedef struct ptb_t {
    char*nama[2];
    int size;
    int modul;
}PTB;

typedef struct listRombongan_t{
    ELKA elka;
    PPMC ppmc;
    PTB ptb;
}Rombongan;

//struktur data Rule Checker
typedef struct checker {
    int minggu [2];
    char *hari [2];
} Checker;

void loadJadwal(char * filename, Schedule * jadwal);
void loadStatus( char * filename, Schedule * jadwal);
void saveJadwal(char const * filename , Schedule jadwal);
void saveStatus(char const * filename , Schedule jadwal);
void printSchedule(Schedule jadwal);
void printStatus(Schedule jadwal);
void FprintSchedule(Schedule jadwal, FILE *file);
void FprintStatus(Schedule jadwal, FILE *file);
char* copyStr(int a, int b,char const * string);
void shift(char buff[],int *cursor_str, int *cursor_end, char sep_1, char sep_2, char sep_3);
void assignWeek(Schedule ** jadwal,int i_minggu, int cursor_str, int cursor_end, char buff[]);
void assignDay(Schedule ** jadwal,int i_minggu,int i_hari, int cursor_str, int cursor_end, char buff[]);
void assignPraktikum(Schedule ** jadwal,int i_minggu,int i_hari,int z, int cursor_str, int cursor_end, char buff[]);
void assignRombongan(Schedule ** jadwal,int i_minggu,int i_hari,int z, int cursor_str, int cursor_end, char buff[]);
void isPraktikumEmpty(bool *isEmpty,char buff[],int *cursor_str, int* cursor_end);
bool isStringNull( const char * string);
void assignDataAsprak (dataAsprak *Asprak);
void printDataAsprak ( dataAsprak Asprak);
void autoSchedule (Schedule * jadwal);
void printstrip();
void TampilkanJadwal(Schedule jadwal);
int checkarr(Schedule jadwal,int a,int b,int d);
int ceklab(char r[]);
int cekhari(char c[]);
void ScheduleManual(Schedule *jadwal);
void print_strip(int x);
void print_asisten(Schedule jadwal);
void loadChecker(char * filename, Checker*checker);
Rombongan buildRombongan();
Schedule createJadwal();
int IsDiaSendiri(Schedule jadwal, int minggu, int nomer_hari, int nomer_lab, char pilih_asisten[]);
int IsAvailable(Schedule jadwal, int minggu, int nomer_hari,int nomer_lab, char pilih_asisten[]);
void assign_asisten(Schedule *jadwal);
void assignStatus(Schedule *jadwal);
int getIndexAsisten(char asisten, dataAsprak Asprak);
bool isPraktikumExist ( Asprak asprak, Lab lab, Day day);
int countAsisten(Lab lab);
bool isAsistenSingle(Schedule jadwal);

#endif


