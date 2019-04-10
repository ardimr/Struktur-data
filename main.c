//***********************************************************// 
// 						[ Source Code ] 
// 
// Institution 	: Institut Teknologi Bandung 
// Name 		: <Nama Praktikan> 
// File Name 	: main.c 
// Dependency 	: stdio.h, load.h
// 
// Description: 
// 	Membuat sebuah program yang dapat membuat jadwal praktikum
//	dan mengatur jadwal asisten di setiap shift praktikum yang
//	ada.
// 
// Status: 
// 1. Rizky Ardi Maulana    - 13217054 : akses file external
// 2. Devananda             - 13217061 : Assign jadwal dan menampilkannya
// 3. Hans Jeremia          - 13217066 : membuat main program dan rule checker
// 4. Muhammad Nadhif Farizi- 13217074 : Assign asisten dan menampilkannya
//***********************************************************//


//Library
#include "Load.c"
#include "Load.h"


//Variabel
int input_menu = -98;
int input_mode = -99;
int *pInput_menu;
int *pInput_mode;


//Fungsi & Prosedur
void menu(int *input);
void mode(int *input);

//Main Program
int main(){
    //variabel
    char namafile [100];
    char namafile_jadwal[100];
    char namafile_asisten[100];
    char junk;

    Schedule jadwal;
    //assign pointer
    pInput_mode = &input_mode;
    pInput_menu = &input_menu;
    //menampilkan menu utama
    while (1) {
        menu(pInput_menu);
        if (input_menu == 1) {
            printf("====== Membuat Proyek Baru ======\n");
            jadwal = createJadwal();
            printf("Masukkan nama proyek: ");
            //baca nama file
            scanf("%s", &namafile);
        }
        else if(input_menu == 2){
            printf("====== Memuat Proyek dari Berkas =====\n");
            printf("Masukkan nama proyek: ");
            //baca nama file
            scanf("%s", &namafile);
            //buat nama file untuk dipanggil
            strcpy(namafile_jadwal,namafile);
            strcpy(namafile_asisten, namafile);
            strcat(namafile_jadwal,"_rombongan.csv");
            strcat(namafile_asisten,"_asisten.csv");
            //load
            loadJadwal(namafile_jadwal, &jadwal);
            loadStatus(namafile_asisten, &jadwal);
            printf("\n%s dan ",namafile_jadwal);
            printf("%s Telah dimuat ...\n\n",namafile_asisten);
        }
        else {
            return 0;
        }

        do{
        mode(pInput_mode);
            if (input_mode==1){
               printSchedule(jadwal);
            }else if(input_mode==2){
                ScheduleManual(&jadwal);
            }else if(input_mode==3){
                printStatus(jadwal);
            }else if(input_mode==4){
                assignStatus(&jadwal);
            }else if(input_mode==5){
                scanf("%c", &junk);
                autoSchedule(&jadwal);
            }else if(input_mode==6){
                
            }else if(input_mode==7){

            }else if(input_mode==8){
                //save file bergantung input dari menu
                 if (input_menu == 1){
                
                    strcpy(namafile_jadwal,namafile);
                    strcpy(namafile_asisten, namafile);
                    strcat(namafile_jadwal,"_rombongan.csv");
                    strcat(namafile_asisten,"_asisten.csv");
                    saveJadwal(namafile_jadwal, jadwal);
                    saveStatus(namafile_asisten, jadwal);
                    //cetak pemberitahuan
                
                    printf("Schedule disimpan dalam berkas %s", namafile_jadwal);
                    printf("\nStatus Asisten disimpan dalam berkas %s", namafile_asisten);
                } else { //input menu load data
                    saveJadwal(namafile_jadwal, jadwal);
                    saveStatus(namafile_asisten, jadwal);
                }
             }
            printf("\n");
        } while (input_mode != 8);
    }
      
    return 0;
}

void mode (int *input){
    //menampilkan Menu Utama
    printf("[Menu Utama]");
    do {
        printf("\nPilih Mode:");
        printf("\n%4d. Tampilkan Schedule",1);
        printf("\n%4d. Schedule Manual",2);
        printf("\n%4d. Tampilkan Status Assignment Asisten",3);
        printf("\n%4d. Assign Asisten Manual",4);
        printf("\n%4d. Schedule Otomatis",5);
        printf("\n%4d. Assign Otomatis",6);
        printf("\n%4d. Rule Checker",7);
        printf("\n%4d. Simpan Proyek dan Keluar",8);
        printf("\nPilih Mode: ");
        scanf("%d",input);
        
        if(((*input) <1)||((*input) >8)){
            printf("Masukan Mode Salah!\n");
        }
        printf("\n");
    } while (((*input) <1)|| ((*input) >8));
}

void menu(int *input){
    //menampilkan menu
   do {
        printf("Menu :");
        printf("\n%4d. Muat Proyek Baru",1);
        printf("\n%4d. Muat Proyek dari Berkas",2);
        printf("\n%4d. Keluar",3);
        printf("\nMasukan: ");
        scanf("%d",input);
        if(((*input) <1)||((*input) >3)){
            printf("Masukan Salah!\n");
        }
    } while (((*input) <1)|| ((*input) >3));
}