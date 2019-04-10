//Implementasi Load.H
#include "Load.h"

void loadJadwal(char * filename, Schedule *jadwal){
    // Schedule jadwal;
    FILE *file;
    char buff[71];

    file = fopen(filename, "r" );
    int count=0;
    int i_minggu = 0, i_hari= 0, i_lab;
    while (fgets(buff, 71, file) != NULL){
        count++;
        int cursor_str = 0;
        int cursor_end = 0;
        
        //bagian ambil data
        if ((count >2)&&(buff!= "\n")){
        //parsing minggu
            shift(buff,&cursor_str, &cursor_end, ';',';','\n');
        //salin minggu ke variabel minggu
            assignWeek(&jadwal, i_minggu, cursor_str, cursor_end, buff);
           
        //parsing hari      
            shift(buff,&cursor_str,&cursor_end, ';',';','\n');
         //salin hari ke variabel hari
            assignDay(&jadwal, i_minggu, i_hari, cursor_str, cursor_end, buff);

        //parsing lab
            for (int z = 0; z<4; z++){
                //cursor geser dulu sampe ketemu separator |
                bool isEmpty = false;
                isPraktikumEmpty(&isEmpty,buff, &cursor_str, &cursor_end);
                // printf("isEmpty :%d\n", isEmpty);
                if (isEmpty== true) {
                    //assign praktikum dan rombongan dengan NULL
                    jadwal->week[i_minggu].day[i_hari].lab[z].group.praktikum = NULL;
                    jadwal->week[i_minggu].day[i_hari].lab[z].group.rombongan = NULL;
                }
                else {
                    shift(buff,&cursor_str, &cursor_end, ';','-','-');
                    assignPraktikum(&jadwal, i_minggu, i_hari,z, cursor_str, cursor_end, buff);
                
                    ///geser cursor buat dapetin rombongan
                    shift(buff,&cursor_str,&cursor_end, '-',';','\n');
                    assignRombongan(&jadwal, i_minggu, i_hari,z, cursor_str, cursor_end, buff);
                }
                
            }
            i_hari ++;
            if (i_hari >4){
                i_hari = 0;
                i_minggu ++;
            }

        }
    }
    fclose(file);
}

void loadStatus( char * filename, Schedule * jadwal){
    // Schedule jadwal;
    FILE *file;
    char buff[71];

    file = fopen(filename, "r" );
    int count=0;
    int i_minggu = 0, i_hari= 0, i_lab;
    while (fgets(buff, 71, file) != NULL){
        count++;
        int cursor_str = 0;
        int cursor_end = 0;
    //bagian ambil data
        if ((count >2)&&(buff!= "\n")){
        //parsing minggu
            shift(buff,&cursor_str, &cursor_end, ';',';','\n');
        //salin minggu ke variabel minggu
            assignWeek(&jadwal, i_minggu, cursor_str, cursor_end, buff);
            // printf("\nweek : %d ",jadwal->week[i_minggu].element);
           
        //parsing hari
            shift(buff,&cursor_str,&cursor_end, ';',';','\n');

        //salin hari ke variabel hari
            assignDay(&jadwal, i_minggu, i_hari, cursor_str, cursor_end, buff);

        //parsing lab
        for (int z = 0; z<4; z++){
                //cursor geser dulu sampe ketemu separator |
                bool isEmpty = false;
                isPraktikumEmpty(&isEmpty,buff, &cursor_str, &cursor_end);
                // printf("isEmpty :%d ", isEmpty);
                if (isEmpty== true) {
                    //assign praktikum dan rombongan dengan NULL
                    jadwal->week[i_minggu].day[i_hari].lab[z].asprak[0].code = '\0';
                    jadwal->week[i_minggu].day[i_hari].lab[z].asprak[1].code = '\0';
                }
                else {
                    shift(buff,&cursor_str, &cursor_end, ';',',',',');
                    jadwal->week[i_minggu].day[i_hari].lab[z].asprak[0].code = buff[cursor_str];
                    ///geser cursor buat dapetin rombongan
                    shift(buff,&cursor_str,&cursor_end, ',',';','\n');
                    jadwal->week[i_minggu].day[i_hari].lab[z].asprak[1].code = buff[cursor_str];
                    
                }
                
            }
            i_hari ++;
            if (i_hari >4){
                i_hari = 0;
                i_minggu ++;
            }

        }
    }
    fclose(file);
}


void isPraktikumEmpty(bool *isEmpty,char buff[], int *cursor_str, int * cursor_end){
    *cursor_str = *cursor_end;
    if ((buff[*cursor_str+1] == ';')||(buff[*cursor_str+1]=='\n')){
        *isEmpty = true;
        *cursor_str = *cursor_str +1;
        *cursor_end =  *cursor_str;
        
    }
    else {
        *isEmpty = false;
        *cursor_str = *cursor_end;
    }
    
}

char* copyStr(int a, int b,char const * string){
    const char* from = string;
    char *to = (char*) malloc(b-a);
    strncpy(to, from+a, b-a);
    return to;
}

void saveJadwal(char const * filename, Schedule jadwal){
    FILE *file;
    file = fopen(filename, "w");
    FprintSchedule(jadwal, file);
    fclose(file);
}

void saveStatus( char const *filename, Schedule jadwal){
    FILE *file;
    file = fopen(filename, "w");
    FprintStatus(jadwal, file);
    fclose(file);
}
void printSchedule(Schedule jadwal){
    //mencetak header
    printf("|----------------|------------|------------|------------|------------|");
    //|Ruang Praktikum | LAB1 | LAB2 | LAB3 | LSS |
    printf("\n|Ruang Praktikum |    LAB1    |    LAB2    |    LAB3    |    LSS     |");
    //mencetak header
    printf("\n|----------------|------------|------------|------------|------------|");
    //|Minggu | Hari |------------|------------|------------|-----------|
    printf("\n|Minggu |  Hari  |------------|------------|------------|------------|\n");
    printf("|----------------|------------|------------|------------|------------|");
    for (int i = 0; i< 12; i++){
        printf("\n");
        //separator
        for (int j = 0; j < 5; j++){
            //print minggu
            printf("|%4d%4c",jadwal.week[i].element,'|');
            //print hari
            int space = 8 - strlen(jadwal.week[i].day[j].element);
            printf(" %s%*c",jadwal.week[i].day[j].element,space,'|');
            //print rombongan
            for (int k = 0; k <4; k++){
                int space_p = 6;
                int space_r = 5;
                //cek apa string null
                if (isStringNull(jadwal.week[i].day[j].lab[k].group.praktikum)){

                }
                else {
                     space_p = 6 - strlen(jadwal.week[i].day[j].lab[k].group.praktikum);
                }
                if (isStringNull(jadwal.week[i].day[j].lab[k].group.rombongan)){

                }
                else {
                     space_p =  - strlen(jadwal.week[i].day[j].lab[k].group.rombongan);
                }
        
                if ((space_p + space_r) >=11){
                    printf(" %*c",space_r+space_p+1,'|');
                }
                else {
                    printf(" %s-%s%*c",jadwal.week[i].day[j].lab[k].group.praktikum, jadwal.week[i].day[j].lab[k].group.rombongan,space_r+space_p,'|');
                }
            }
            printf("\n");
        }
        printf("|----------------|------------|------------|------------|------------|");
    }
}

void printStatus(Schedule jadwal){
    //mencetak header
    printf("|----------------|------------|------------|------------|------------|");
    //|Ruang Praktikum | LAB1 | LAB2 | LAB3 | LSS |
    printf("\n|Ruang Praktikum |    LAB1    |    LAB2    |    LAB3    |    LSS     |");
    //mencetak header
    printf("\n|----------------|------------|------------|------------|------------|");
    //|Minggu | Hari |------------|------------|------------|-----------|
    //                            |------------|
    printf("\n|Minggu |  Hari  |------------|------------|------------|------------|\n");
    printf("|----------------|------------|------------|------------|------------|");
    for (int i = 0; i< 12; i++){
        printf("\n");
        //separator
        for (int j = 0; j < 5; j++){
            //print minggu
            printf("|%4d%4c",jadwal.week[i].element,'|');
            //print hari
            int space = 8 - strlen(jadwal.week[i].day[j].element);
            printf(" %s%*c",jadwal.week[i].day[j].element,space,'|');
            //print asisten di lab
            for (int k = 0; k <4; k++){
                if (jadwal.week[i].day[j].lab[k].asprak[0].code == '\0'){
                    //cetak separator saja
                    printf("%13c", '|');
                }
                else{
                    printf("%5c,%c%6c",jadwal.week[i].day[j].lab[k].asprak[0].code,jadwal.week[i].day[j].lab[k].asprak[1].code,'|');

                }
            }
            printf("\n");
        }
        printf("|----------------|------------|------------|------------|------------|");
    }
}

bool isStringNull(const char * string){
    if (string == NULL){
        return true;
    }
    else
    {
        return false;
    }
    
}

void FprintStatus(Schedule jadwal, FILE * file){
    //|Ruang Praktikum | LAB1 | LAB2 | LAB3 | LSS |
    fprintf(file,"Ruang Praktikum;;LAB1;LAB2;LAB3;LSS");
    //|Minggu | Hari |------------|------------|------------|-----------|
    fprintf(file,"\nMinggu;Hari;;;;\n");
    for (int i = 0; i< 12; i++){
        //separator
        for (int j = 0; j < 5; j++){
            //print minggu
            fprintf(file,"%d%c",jadwal.week[i].element,';');
            //print hari
            fprintf(file,"%s",jadwal.week[i].day[j].element);
            //print rombongan
            for (int k = 0; k <4; k++){
                if (jadwal.week[i].day[j].lab[k].asprak[0].code == '\0'){
                    //cetak separator saja
                    fprintf(file,";");
                }
                else{
                    fprintf(file,";%c,%c",jadwal.week[i].day[j].lab[k].asprak[0].code,jadwal.week[i].day[j].lab[k].asprak[1].code,'|');
                }
            }
            fprintf(file,"\n");
        }
    }
}

void FprintSchedule(Schedule jadwal, FILE *file){
    
    //|Ruang Praktikum | LAB1 | LAB2 | LAB3 | LSS |
    fprintf(file,"Ruang Praktikum;;LAB1;LAB2;LAB3;LSS");
    //|Minggu | Hari |------------|------------|------------|-----------|
    fprintf(file,"\nMinggu;Hari;;;;\n");
    for (int i = 0; i< 12; i++){
        //separator
        for (int j = 0; j < 5; j++){
            //print minggu
            fprintf(file,"%d;",jadwal.week[i].element);
            //print hari
            
            fprintf(file,"%s",jadwal.week[i].day[j].element);
            //print rombongan
            for (int k = 0; k <4; k++){
                //cek apa isinya kosong
                if ((isStringNull(jadwal.week[i].day[j].lab[k].group.praktikum))&&(isStringNull(jadwal.week[i].day[j].lab[k].group.rombongan))){
                    //cetak kosong
                    fprintf(file,";");
                }    
                else {
                    fprintf(file,";");
                    fprintf(file,"%s-%s",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                }
            }
            fprintf(file,"\n");
        }
    }
}

void shift(char buff[],int *cursor_str, int *cursor_end, char sep_1, char sep_2, char sep_3){
    //jadikan cursor str sama dengan end
    *cursor_str = *cursor_end;
    while (buff[*cursor_str] == sep_1 ){
        (*cursor_str) ++;
    }
    //samakan cursor end dan str
    *cursor_end = *cursor_str;
    while((buff[*cursor_end] != sep_2) && (buff[*cursor_end] != sep_3 )){
         (*cursor_end) ++;
    }
}

void assignWeek(Schedule ** jadwal,int i_minggu, int cursor_str, int cursor_end, char buff[]){
   char *temp = copyStr(cursor_str, cursor_end, buff);
   (*jadwal)->week[i_minggu].element = atoi(temp);
}

void assignDay(Schedule ** jadwal,int i_minggu,int i_hari, int cursor_str, int cursor_end, char buff[]){
    (*jadwal)->week[i_minggu].day[i_hari].element = malloc (strlen(copyStr(cursor_str,cursor_end,buff)));
    (*jadwal)->week[i_minggu].day[i_hari].element = copyStr(cursor_str, cursor_end, buff);
}

void assignPraktikum(Schedule ** jadwal,int i_minggu,int i_hari,int z, int cursor_str, int cursor_end, char buff[]){
    (*jadwal)->week[i_minggu].day[i_hari].lab[z].group.praktikum = malloc (strlen(copyStr(cursor_str,cursor_end,buff)));
    (*jadwal)->week[i_minggu].day[i_hari].lab[z].group.praktikum = copyStr(cursor_str,cursor_end,buff);
}
void assignRombongan(Schedule ** jadwal,int i_minggu,int i_hari,int z, int cursor_str, int cursor_end, char buff[]){
    (*jadwal)->week[i_minggu].day[i_hari].lab[z].group.rombongan = malloc (strlen(copyStr(cursor_str,cursor_end,buff))-1);
    (*jadwal)->week[i_minggu].day[i_hari].lab[z].group.rombongan = copyStr(cursor_str,cursor_end,buff);
}

void assignDataAsprak (dataAsprak *Asprak){
    char const * nama;
    char const *praktikum;
    char const * berhalangan;
    int index;
// isi nama Amir
    index = 0;
    nama = "Amir";
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'A';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Rabu";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
        
    }
// isi nama Budi
    nama = "Budi";
    index = 1;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'B';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Senin";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }
// isi nama Cici
    nama = "Cici";
    index = 2;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'C';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Selasa";
        }
        else if (i == 1 ){
            berhalangan = "Rabu";
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
        
    }
// isi nama Doni
    nama = "Doni";
    index = 3;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'D';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Senin";
        }
        else if (i == 1 ){
            berhalangan = "Rabu";
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }
// isi nama Endang
    nama = "Endang";
    index = 4;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'E';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = "EL2208";
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = NULL;
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Fadel
    nama = "Fadel";
    index = 5;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'F';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Selasa";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }    

// isi nama Gilang
    nama = "Gilang";
    index = 6;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'G';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = "EB2200";
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Kamis";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Hero
    nama = "Hero";
    index = 7;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'H';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2208";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Kamis";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Intan
    nama = "Intan";
    index = 8;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'I';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2205";
        }
        else if (i == 1 ){
            praktikum = "EL2208";
        }
        else {
            praktikum = "EB2200";
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Rabu";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Joko
    nama = "Joko";
    index = 9;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'J';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2208";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Jumat";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Kiki
    nama = "Kiki";
    index = 10;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'K';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2208";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Jumat";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Luis
    nama = "Luis";
    index = 11;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'L';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2208";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = NULL;
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

// isi nama Mini
    nama = "Mini";
    index = 12;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'M';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2208";
        }
        else if (i == 1 ){
            praktikum = NULL;
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Rabu";
        }
        else if (i == 1 ){
            berhalangan = "Kamis";
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }   
    
// isi nama Nina
    nama = "Nina";
    index = 13;
    Asprak->asprak[index].nama = malloc(strlen(nama)+1);
    strcpy(Asprak->asprak[index].nama,nama);
    //isi kode
    Asprak->asprak[index].code = 'N';
    //isi praktikum
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            praktikum = "EL2208";
        }
        else if (i == 1 ){
            praktikum = "EB2200";
        }
        else {
            praktikum = NULL;
        }

        if(praktikum == NULL){
            Asprak->asprak[index].praktikum[i] = NULL;
        }
        else{
            Asprak->asprak[index].praktikum[i] = malloc(strlen(praktikum)+1);
            strcpy(Asprak->asprak[index].praktikum[i],praktikum);
        }
        
    }

    // isi berhalangan
    for (int i = 0; i<3 ; i++){
        if (i == 0) {
            berhalangan = "Selasa";
        }
        else if (i == 1 ){
            berhalangan = NULL;
        }
        else {
            berhalangan = NULL;
        }

        if(berhalangan == NULL){
            Asprak->asprak[index].berhalangan[i] = NULL;
        }
        else{
            Asprak->asprak[index].berhalangan[i] = malloc(strlen(berhalangan)+1);
            strcpy(Asprak->asprak[index].berhalangan[i],berhalangan);
        }
    }

}

Rombongan buildRombongan(){
    //buat databse rombongan praktikum
    Rombongan rombongan;
    rombongan.elka.modul = 7;
    rombongan.ppmc.modul = 9;
    rombongan.ptb.modul = 5;
    rombongan.elka.size = 6;
    rombongan.ppmc.size = 9;
    rombongan.ptb.size = 2;
    char const *rombElka;
    char const *rombPpmc;
    char const *rombPtb;
    int i;
    for (i = 0 ;i<7; i ++){
        if (i == 0){
            rombElka = "A1";
            rombPpmc = "A1";
            rombPtb = "A";
        }
        else if(i==1){
            rombElka = "A2";
            rombPpmc = "A2";
            rombPtb = "B";        
        }
        else if (i==2){
            rombElka = "B1";
            rombPpmc = "A3";
        }
        else if (i==3){
            rombElka = "B2";
            rombPpmc = "B1";
        }
        else if (i==4){
            rombElka = "C1";
            rombPpmc = "B2";
        }
        else if ( i==5){
            rombElka = "C2";
            rombPpmc = "B3";
        }
        else {
            rombPpmc = "C";
        }
        
        if (i < 2){
            //assign untuk PTB
            rombongan.ptb.nama[i] = malloc(strlen(rombPtb)+1);
            strcpy(rombongan.ptb.nama[i], rombPtb);
        }

        if (i<6){
            //assign untuk ELKA
            rombongan.elka.nama[i] = malloc (strlen(rombElka)+1);
            strcpy(rombongan.elka.nama[i],rombElka);
        }
        //assign nama untuk PPMC
        rombongan.ppmc.nama[i] = malloc (strlen(rombPpmc)+1);
        strcpy(rombongan.ppmc.nama[i],rombPpmc); 
    }
    return rombongan;
}

void printDataAsprak ( dataAsprak Asprak){
    for ( int index = 0; index <14; index++){
        printf("%s\n", Asprak.asprak[index].nama);
        printf("%c\n", Asprak.asprak[index].code);
        for (int i = 0; i < 3 ; i++){
            if (Asprak.asprak[index].praktikum[i] != NULL){
               printf("%s ", Asprak.asprak[index].praktikum[i]);
           }
       } 
       printf("\n");
        for (int i = 0; i < 3 ; i++){
            if (Asprak.asprak[index].berhalangan[i] != NULL){
               printf("%s ", Asprak.asprak[index].berhalangan[i]);
           }
       }
       printf("\n\n");
   }

}

void autoSchedule (Schedule * jadwal){
    //build data rombongan
    Rombongan rombongan = buildRombongan();
    //mulai mengisi
    char const * praktikum;

      for (int i = 0; i <12;i++){
        rombongan.elka.size = 6;
        rombongan.ppmc.size = 7; 
        rombongan.ptb.size = 2;
    
        for (int j = 0 ; j < 5; j++){ //hari
              for (int k = 0 ; k < 4 ; k++){ //lab
                    //isi rombongan elka
                    if ((rombongan.elka.modul>=0)&&(j<2)&&(k!=3)&&(rombongan.elka.size >=0)){
                        //rombongan sisa berkurang
                        rombongan.elka.size -= 1;
                        praktikum = "EL2205";
                        //isi nama praktikum
                        jadwal->week[i].day[j].lab[k].group.praktikum = malloc(strlen(praktikum)+1);
                        strcpy(jadwal->week[i].day[j].lab[k].group.praktikum, praktikum);
                        jadwal->week[i].day[j].lab[k].group.rombongan = malloc(strlen(rombongan.elka.nama[rombongan.elka.size])+1);
                        strcpy(jadwal->week[i].day[j].lab[k].group.rombongan, rombongan.elka.nama[rombongan.elka.size]);
                        
                    }
                    
                    // isi rombongan pmc
                    if((rombongan.ppmc.modul>=0)&&(j>1)&&(rombongan.ppmc.size>0)){
                        //rombongan sisa berkurang
                        rombongan.ppmc.size --;
                        praktikum = "EL2208";
            
                        
                        if (strcmp(*(rombongan.ppmc.nama),"C")==0){ //rabu
                            
                            jadwal->week[i].day[2].lab[k].group.praktikum = malloc(strlen(praktikum)+1);
                            strcpy(jadwal->week[i].day[2].lab[k].group.praktikum, praktikum);
                            jadwal->week[i].day[2].lab[k].group.rombongan = malloc(strlen(rombongan.ppmc.nama[rombongan.ppmc.size])+1);
                            strcpy(jadwal->week[i].day[2].lab[k].group.rombongan, rombongan.ppmc.nama[rombongan.ppmc.size]);
                            rombongan.ppmc.size -= 1;

                        } 
                        
                        if (jadwal->week[i].day[j].lab[k].group.rombongan == NULL){
                            // printf("%d ", rombongan.ppmc.size);
                    
                            jadwal->week[i].day[j].lab[k].group.praktikum = malloc(strlen(praktikum)+1);
                            strcpy(jadwal->week[i].day[j].lab[k].group.praktikum, praktikum);
                            jadwal->week[i].day[j].lab[k].group.rombongan = malloc(strlen(rombongan.ppmc.nama[rombongan.ppmc.size])+1);
                            strcpy(jadwal->week[i].day[j].lab[k].group.rombongan, rombongan.ppmc.nama[rombongan.ppmc.size]);
                        }
                    }
                    
                    // isi rombongan ptb
                    if((i >9)&&(rombongan.ptb.modul>=0)&&((j==1)||(j==3))&&(k<4)&&(rombongan.ptb.size>0)&&(jadwal->week[i].day[j].lab[k].group.rombongan == NULL)){
                        rombongan.ptb.size -= 1;
                        praktikum = "EL2200";
                        jadwal->week[i].day[j].lab[k].group.praktikum = malloc(strlen(praktikum)+1);
                            strcpy(jadwal->week[i].day[j].lab[k].group.praktikum, praktikum);
                            jadwal->week[i].day[j].lab[k].group.rombongan = malloc(strlen(rombongan.ppmc.nama[rombongan.ptb.size])+1);
                            strcpy(jadwal->week[i].day[j].lab[k].group.rombongan, rombongan.ppmc.nama[rombongan.ptb.size]);
                    }
                    
                }
                        
       }
       
        rombongan.elka.modul --;
        rombongan.ppmc.modul --;
        if(i>9){
            rombongan.ptb.modul --;
        }
    }
    printf("\nAuto Schedule Berhasil!\n");
}

Schedule createJadwal(){
    //bikin jadwal dengan isi kosong
    Schedule jadwal;
    //isi minggu
    for (int i =0; i<12; i++){
        jadwal.week[i].element = i+3;
        for (int j = 0; j<5; j++){
            char const *hari;
            if (j ==0){
                hari = "Senin";
                }
            else if (j ==1){
                hari = "Selasa";
                }
            else if (j ==2){
                hari = "Rabu";
                }
            else if (j ==3){
                hari = "Kamis";
                }
            else if (j ==4){
                hari = "Jumat";
                }
            jadwal.week[i].day[j].element = malloc (strlen(hari)+1);
            strcpy(jadwal.week[i].day[j].element, hari);

            //isi lab
            for (int z = 0; z<4;z++) {
                jadwal.week[i].day[j].lab[z].group.praktikum = NULL;
                jadwal.week[i].day[j].lab[z].group.rombongan = NULL;
                jadwal.week[i].day[j].lab[z].asprak[0].code = '\0';
                jadwal.week[i].day[j].lab[z].asprak[1].code = '\0';
                }
            }
        }
    
    return jadwal;
}

void printstrip(){
    //Mencetak strip setiap dipanggil
    int i;
    printf("|");
    for(i=1;i<=67;i++){
        printf("-");
    }
    printf("|\n");
}
void TampilkanJadwal(Schedule jadwal){
    //Header atas
    int i,j,k,y,z,L;
    int x=100;
    char h[5][10]={"Senin ","Selasa","Rabu  ","Kamis ","Jumat "};
    printstrip();
    printf("|Ruang Praktikum |    LAB1    |    LAB2    |    LAB3    |    LSS    |\n");
    printf("|----------------|------------|------------|------------|-----------|\n");
    printf("|Minggu |  Hari  |------------|------------|------------|-----------|\n");
    printf("|-------|--------|------------|------------|------------|-----------|\n");
    //Mendisplaykan kan jadwal yang tidak NULL serta menyesuaikan agar tabel tidak berubah
    for(i=0;i<=6;i++){
            for(j=0;j<5;j++){
                printf("|   %d   | %s |",i+3,h+j);
                for(k=0;k<=2;k++){
                    if(checkarr(jadwal,i,j,k) == 1){
                        printf(" %s-%s  |",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                    }else if(checkarr(jadwal,i,j,k) == 2){
                        printf(" %s-%s   |",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                    }else{
                        printf("            |");
                    }
                }
            k=3;
            if(checkarr(jadwal,i,j,k) == 1){
                printf(" %s-%s |\n",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
            }else if(checkarr(jadwal,i,j,k) == 2){
                printf(" %s-%s  |\n",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
            }else{
                printf("           |\n");
            }}
        printstrip();
    }

    for(i=7;i<=9;i++){
            for(j=0;j<5;j++){
                printf("|   %d  | %s |",i+3,h+j);
                for(k=0;k<=2;k++){
                    if(checkarr(jadwal,i,j,k) == 1){
                        printf(" %s-%s  |",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                    }else if(checkarr(jadwal,i,j,k) == 2){
                        printf(" %s-%s   |",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                    }else{
                        printf("            |");
                    }
                }
                k=3;
                if(checkarr(jadwal,i,j,k) == 1){
                    printf(" %s-%s |\n",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                }else if(checkarr(jadwal,i,j,k) == 2){
                    printf(" %s-%s  |\n",jadwal.week[i].day[j].lab[k].group.praktikum,jadwal.week[i].day[j].lab[k].group.rombongan);
                }else{
                    printf("           |\n");
                }
            }
            printstrip();
    }
}

int checkarr(Schedule jadwal,int a,int b,int d){
    //fungsi ini akan mengeluarkan int 0 jika jadwal hari itu NULL, int 1 jika jadwal hari itu jika hari tersebut memiliki jadwal dengan rombongan 2 char, int 2 jika rombongan hari itu 1 char
    char *c;
    c=jadwal.week[a].day[b].lab[d].group.rombongan;
    if(jadwal.week[a].day[b].lab[d].group.rombongan == NULL){
        return 0;
    }
    if(c[0] == 'B' && c[1]== '1'||
       c[0] == 'B' && c[1]== '2'||
       c[0] == 'B' && c[1]== '3'||
       c[0] == 'A' && c[1]== '1'||
       c[0] == 'A' && c[1]== '2'||
       c[0] == 'A' && c[1]== '3'||
       c[0] == 'C' && c[1]== '1'||
       c[0] == 'C' && c[1]== '2'){
        return 1;
    }else{
        return 2;
    }
}

void ScheduleManual(Schedule *jadwal){
    //ALGORITMA UNTUK MENERIMA DAN MEMVALIDASI INPUT
    char prak[8];
    char romb[3],in[2];
    int m,c,i,bol,valm,j,k,vald,vald1,vald2,vald3,valdc;
    char h[7];
    char r[5];
    char *ch,*cx1,*cx2,*cx3,*cx4;
    do{
        for(i=0;i<=3;i++){
            romb[i]='\0';
        }
        printf("Pilih Kode Praktikum (EL2205, EL2208, EB2200): ");
        gets(prak);
        if(strcmp (prak, "EL2208") == 0){
            do{
                printf("Rombongan (A1,A2,A3,B1,B2,B3 atau C):");
                gets(romb);
            }while(strcmp (romb, "A1") != 0&&
                   strcmp (romb, "A2") != 0&&
                   strcmp (romb, "A3") != 0&&
                   strcmp (romb, "B1") != 0&&
                   strcmp (romb, "B2") != 0&&
                   strcmp (romb, "B3") != 0&&
                   strcmp (romb, "C") != 0&&
                   strcmp (romb, "A") != 0&&
                   strcmp (romb, "B") != 0);
            vald=0;
            vald1=0;
            vald2=0;
            vald3=0;
            valdc=0;
            for(k=0;k<=9;k++){
                for(j=0;j<=4;j++){
                    for(i=0;i<=3;i++){
                        cx3=jadwal->week[k].day[j].lab[i].group.praktikum;
                        cx4=jadwal->week[k].day[j].lab[i].group.rombongan;
                        if(jadwal->week[k].day[j].lab[i].group.praktikum==NULL){
                        }else if(cx3[5]=='8' && cx4[0]==romb[0] && cx4[1]==romb[1]){
                            vald++;
                        }else if(cx3[5]=='8' && cx4[0]==romb[0] && romb[1]=='\0'){
                            if(cx4[1]=='1'){
                                vald1++;
                            }else if(cx4[1]=='2'){
                                vald2++;
                            }else if(cx4[1]=='3'){
                                vald3++;
                            }else{
                                valdc++;
                            }
                        }
                    }
                }
            }
            if(vald <= 8 && vald1<=8 && vald2<=8 && vald3<=8 && valdc<=8){
                do{
                    printf("Minggu ke: ");
                    scanf("%d",&m);
                }while(m<3 || m>12);
                while((c=getchar())!= '\n' && c!=EOF);
                valm=0;
                for(j=0;j<=4;j++){
                    for(i=0;i<=3;i++){
                        cx1=jadwal->week[m-3].day[j].lab[i].group.praktikum;
                        cx2=jadwal->week[m-3].day[j].lab[i].group.rombongan;
                        if(jadwal->week[m-3].day[j].lab[i].group.praktikum==NULL){
                        }else if(cx1[5]=='8' && cx2[0]==romb[0] && cx2[1]==romb[1]){
                            valm++;
                        }else if(romb[1]=='\0' && cx1[5]=='8' && cx2[0]==romb[0]){
                            valm++;
                        }
                    }
                }
                if(valm==0){
                    do{
                        printf("Hari :");
                        gets(h);
                    }while(cekhari(h)== 99);
                    bol=0;
                    for(i=0;i<=2;i++){
                        ch=jadwal->week[m-3].day[cekhari(h)].lab[i].group.praktikum;
                        if(jadwal->week[m-3].day[cekhari(h)].lab[i].group.praktikum==NULL){
                        }else if(ch[5]=='5'){
                            bol=1;
                        }
                    }
                    if(strcmp (romb,"A")!= 0 && strcmp (romb, "B") != 0 && strcmp (romb, "C") != 0 ){
                        do{
                            printf("Ruangan :");
                            gets(r);
                        }while(ceklab(r)>4);
                        if(bol == 0){
                            jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum = malloc(strlen(prak)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum, prak);
                            jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan = malloc(strlen(romb)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan, romb);
                            jadwal->week[m-3].element=m;
                            jadwal->week[m-3].day[cekhari(h)].element =malloc(strlen(h)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].element, h);
                            jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element =malloc(strlen(r)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element, r);
                        }else{
                            printf("Terdapat Praktikum EL2205 pada hari ini\n");
                        }
                    }else if(strcmp (romb,"C")== 0){
                    do{
                        printf("Ruangan :");
                        gets(r);
                    }while(ceklab(r)!=3);
                        jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum = malloc(strlen(prak)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum, prak);
                        jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan = malloc(strlen(romb)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan, romb);
                        jadwal->week[m-3].element=m;
                        jadwal->week[m-3].day[cekhari(h)].element =malloc(strlen(h)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].element, h);
                        jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element =malloc(strlen(r)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element, r);
                    }else{
                        if(bol == 0){
                            for(i=1;i<=3;i++){
                                do{
                                    printf("Ruangan %d :",i);
                                    gets(r);
                                }while(ceklab(r)>=4);
                                if(i == 1){
                                    romb[1]='1';
                                }else if (i==2){
                                    romb[1]='2';
                                }else {
                                    romb[1]='3';
                                }
                                jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum = malloc(strlen(prak)+1);
                                strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum, prak);
                                jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan = malloc(strlen(romb)+1);
                                strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan, romb);
                                jadwal->week[m-3].element=m;
                                jadwal->week[m-3].day[cekhari(h)].element =malloc(strlen(h)+1);
                                strcpy(jadwal->week[m-3].day[cekhari(h)].element, h);
                                jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element =malloc(strlen(r)+1);
                                strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element, r);

                            }
                        }else{
                            printf("Terdapat Praktikum EL2205 pada hari ini\n");
                        }
                    }
                }else{
                    printf("Sudah terdapat praktikum rombongan ini pada minggu ini\n");
                }
            }else{
                printf("Terdapat rombongan yang telah menyelesaikan parktikum\n");
            }
        }else if(strcmp (prak, "EL2205") == 0){
            do{
                printf("Rombongan (A1,A2,B1,B2,C1, atau C2):");
                gets(romb);
            }while(strcmp (romb, "A1") != 0&&
                   strcmp (romb, "A2") != 0&&
                   strcmp (romb, "A3") != 0&&
                   strcmp (romb, "B1") != 0&&
                   strcmp (romb, "B2") != 0&&
                   strcmp (romb, "C1") != 0&&
                   strcmp (romb, "C2") != 0&&
                   strcmp (romb, "A") != 0&&
                   strcmp (romb, "B") != 0&&
                   strcmp (romb, "C") != 0);
            vald=0;
            vald1=0;
            vald2=0;
            valdc=0;
            for(k=0;k<=9;k++){
                for(j=0;j<=4;j++){
                    for(i=0;i<=3;i++){
                        cx3=jadwal->week[k].day[j].lab[i].group.praktikum;
                        cx4=jadwal->week[k].day[j].lab[i].group.rombongan;
                        if(jadwal->week[k].day[j].lab[i].group.praktikum==NULL){
                        }else if(cx3[5]=='5' && cx4[0]==romb[0] && cx4[1]==romb[1]){
                            vald++;
                        }else if(cx3[5]=='5' && cx4[0]==romb[0] && romb[1]=='\0'){
                            if(cx4[1]=='1'){
                                vald1++;
                            }else if(cx4[1]=='2'){
                                vald2++;
                            }else{
                                valdc++;
                            }
                        }
                    }
                }
            }
            if(vald <= 6 && vald1<=6 && vald2<=6 && valdc<=6){
                do{
                    printf("Minggu ke: ");
                    scanf("%d",&m);
                }while(m<3 || m>12);
                while((c=getchar())!= '\n' && c!=EOF);
                valm=0;
                for(j=0;j<=4;j++){
                    for(i=0;i<=3;i++){
                        cx1=jadwal->week[m-3].day[j].lab[i].group.praktikum;
                        cx2=jadwal->week[m-3].day[j].lab[i].group.rombongan;
                        if(jadwal->week[m-3].day[j].lab[i].group.praktikum==NULL){
                        }else if(cx1[5]=='5' && cx2[0]==romb[0] && cx2[1]==romb[1]){
                            valm++;
                        }else if(romb[1]=='\0' && cx1[5]=='5' && cx2[0]==romb[0]){
                            valm++;
                        }
                    }
                }
                if(valm==0){
                do{
                    printf("Hari :");
                    gets(h);
                }while(cekhari(h)==99);
                bol=0;
                for(i=0;i<=2;i++){
                    ch=jadwal->week[m-3].day[cekhari(h)].lab[i].group.praktikum;
                    if(jadwal->week[m-3].day[cekhari(h)].lab[i].group.praktikum==NULL){
                    }else if(ch[5]=='8'){
                        bol=1;
                    }
                }
                if(strcmp (romb,"A")!= 0 && strcmp (romb, "B") != 0 && strcmp (romb, "C") != 0 ){
                    do{
                        printf("Ruangan :");
                        gets(r);
                    }while(ceklab(r)>=3);
                    if(bol == 0){
                        jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum = malloc(strlen(prak)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum, prak);
                        jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan = malloc(strlen(romb)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan, romb);
                        jadwal->week[m-3].element=m;
                        jadwal->week[m-3].day[cekhari(h)].element =malloc(strlen(h)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].element, h);
                        jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element =malloc(strlen(r)+1);
                        strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element, r);
                    }else{
                        printf("Terdapat Praktikum EL2208 pada Hari ini\n");
                    }
                }else{
                    if(bol==0){
                        for(i=1;i<=2;i++){
                            do{
                                printf("Ruangan %d :",i);
                                gets(r);
                            }while(ceklab(r)>=3);
                            if(i == 1){
                                romb[1]='1';
                            }else{
                                romb[1]='2';
                            }
                            jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum = malloc(strlen(prak)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum, prak);
                            jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan = malloc(strlen(romb)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan, romb);
                            jadwal->week[m-3].element=m;
                            jadwal->week[m-3].day[cekhari(h)].element =malloc(strlen(h)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].element, h);
                            jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element =malloc(strlen(r)+1);
                            strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element, r);
                        }
                    }else{
                        printf("Terdapat Praktikum EL2208 pada Hari ini");
                    }
                }
            }else{
                printf("Sudah terdapat praktikum rombongan ini pada minggu ini\n");
            }
        }else{
            printf("Terdapat rombongan yang telah menyelesaikan parktikum\n");
        }
    }else if(strcmp (prak, "EL2200") == 0){
        do{
            printf("Rombongan (A atau B):");
            gets(romb);
        }while(strcmp (romb, "A") != 0 && strcmp (romb, "B") != 0);
        vald=0;
        for(k=0;k<=9;k++){
            for(j=0;j<=4;j++){
                for(i=0;i<=3;i++){
                    cx3=jadwal->week[k].day[j].lab[i].group.praktikum;
                    cx4=jadwal->week[k].day[j].lab[i].group.rombongan;
                    if(jadwal->week[k].day[j].lab[i].group.praktikum==NULL){
                    }else if(cx3[5]=='0' && cx4[0]==romb[0]){
                        vald++;
                    }
                }
            }
        }
        if(vald<=4){
            do{
                printf("Minggu ke: ");
                scanf("%d",&m);
            }while(m<3 || m>12);
            while((c=getchar())!= '\n' && c!=EOF);
            valm=0;
            for(j=0;j<=4;j++){
                for(i=0;i<=3;i++){
                    cx1=jadwal->week[m-3].day[j].lab[i].group.praktikum;
                    cx2=jadwal->week[m-3].day[j].lab[i].group.rombongan;
                    if(jadwal->week[m-3].day[j].lab[i].group.praktikum==NULL){
                    }else if(cx1[5]=='0' && cx2[0]==romb[0] && cx2[1]==romb[1]){
                        valm++;
                    }else if(romb[1]=='\0' && cx1[5]=='0' && cx2[0]==romb[0]){
                        valm++;
                    }
                }
            }
            if(valm==0){
                do{
                    printf("Hari:");
                    gets(h);
                }while(cekhari(h)!= 1 && cekhari(h)!=3);
                do{
                    printf("Ruangan :");
                    gets(r);
                }while(ceklab(r)>=3);
                    jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum = malloc(strlen(prak)+1);
                    strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.praktikum, prak);
                    jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan = malloc(strlen(romb)+1);
                    strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].group.rombongan, romb);
                    jadwal->week[m-3].element=m;
                    jadwal->week[m-3].day[cekhari(h)].element =malloc(strlen(h)+1);
                    strcpy(jadwal->week[m-3].day[cekhari(h)].element, h);
                    jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element =malloc(strlen(r)+1);
                    strcpy(jadwal->week[m-3].day[cekhari(h)].lab[ceklab(r)].element, r);
            }else{
                printf("Sudah terdapat praktikum rombongan ini pada minggu ini\n");
            }
        }else{
            printf("Terdapat rombongan yang telah menyelesaikan parktikum\n");
        }
    }else if((strcmp (prak, "Q") == 0)||(strcmp (prak, "q") == 0)){
}else{
    printf("Input Tidak Sesuai");
}
    printf("\n");
}while((strcmp (prak, "Q") != 0)&&(strcmp (prak, "q") != 0));

}

int cekhari(char c[]){
    //MENGUBAH HARI KE INTEGER
    if(strcmp(c,"Senin") == 0){
        return 0;
    }else if(strcmp(c,"Selasa") == 0){
        return 1;
    }else if(strcmp(c,"Rabu") == 0){
        return 2;
    }else if(strcmp(c,"Kamis") == 0){
        return 3;
    }else if(strcmp(c,"Jumat") == 0){
        return 4;
    }else{
        return 99;
    }
}

int ceklab(char r[]){
    //MENGUBAH RUANGAN KE INT
    if(strcmp(r,"LAB1") == 0){
        return 0;
    }else if(strcmp(r,"LAB2") == 0){
        return 1;
    }else if(strcmp(r,"LAB3") == 0){
        return 2;
    }else if(strcmp(r,"LSS") == 0){
        return 3;
    }else{
        return 99;
    }
}

void print_asisten(Schedule jadwal)
{
     int i; int j;  int k; int x; int p; //p pencacah lab
      char karakter;
     

    printf("|-------------------------------------------------------------------------------|\n");
    printf("|   Ruang Praktikum          | LAB1       | LAB2       | LAB3       | LSS       |\n");
    printf("|   Minggu   |     Hari      |------------|------------|------------|-----------|\n");
    printf("|------------|-------------- |------------|------------|------------|-----------|\n");

    for(i =0; i<12; i++)
    {
        jadwal.week[i].element =i+3;
        for(j=0; j<5; j++)
        {
            printf("|------%d-----|", jadwal.week[i].element);
            
            if(j==0)
            {
                jadwal.week[i].day[j].element = calloc(6, sizeof(char));
                jadwal.week[i].day[j].element = "senin";
            }
            else if(j==1)
            {
                jadwal.week[i].day[j].element = calloc(7, sizeof(char));
                jadwal.week[i].day[j].element = "selasa";
            }
            else if(j==2)
            {
                jadwal.week[i].day[j].element = calloc(5, sizeof(char));
                jadwal.week[i].day[j].element = "Rabu";
            }
            else if(j==3)
            {
                jadwal.week[i].day[j].element = calloc(6, sizeof(char));
                jadwal.week[i].day[j].element = "kamis";
            }
            else {
                jadwal.week[i].day[j].element = calloc(6, sizeof(char));
                jadwal.week[i].day[j].element = "jumat";
            }

            printf("-------%s", jadwal.week[i].day[j].element);
            int q;
            q=strlen(jadwal.week[i].day[j].element);
            print_strip(q);
            int k=1;
            //jadwal.week[i].day[i].lab[k].asprak = calloc(2, sizeof(Asprak));        // untuk buat memori asprak yang tentatif(sesuai input asprak berapa)
            //jadwal.week[i].day[i].lab[k].asprak->code = calloc(2, sizeof(char));    // udh ada pas assign di print_asisten
            

            
            for(k=0; k<4; k++)                                       //untuk pencacah ada 4 lab
                {
                    for(p=0; p<2; p++)                              // untuk pencacah berapa banyak asisten di satu lab
                    {
                        printf("       ");  int karakter=0;         //karakter adalah karakter yang di print di kolom lab(kode asisten)
                        if(jadwal.week[i].day[j].lab[k].group.praktikum=NULL)
                        {
                            if(karakter>=1)
                            {
                                printf(",");
                            }
                            printf("%c", jadwal.week[i].day[j].lab[k].asprak[p].code);
                            karakter =karakter+1;
                        }
                    }  
                    print_strip(karakter);                                   
                }
            
            printf("\n");

        }
    }
   
}

void print_strip(int x)                                              //x adalah panjang string
{
    int i;                                                            

    for(i=1; i<=(7-x); i++)
    {
        printf(" ");
    }
    printf(" ");
}

void assignStatus (Schedule *jadwal){
    char inputAsisten;
    char junk;
    int  inputWeek;
    char inputDay[10];
    char inputRuang [5];
    dataAsprak Asprak;
    int i_asisten;
    int i_week, i_day, i_lab;

     //build database
    assignDataAsprak(&Asprak);
    scanf("%c", &junk);

    //mencetak header
    printf("[Mode Assign Asisten]\n");
    printf("Isi `q` atau `Q` untuk kembali ke menu");
    do{
    //baca pertama
        do 
        {
            printf("\nPilih Asisten (A-N): ");
            scanf("%c", &inputAsisten);
            //baca junk
            // printf("%c input\n",inputAsisten);
            if(((inputAsisten<65)||(inputAsisten>78))&&((inputAsisten != 'q')&&(inputAsisten!= 'Q')))
            {
                printf("Tidak ada Asisten dengan Kode %c!", inputAsisten);
            
            }
        }   while (((inputAsisten<65)||(inputAsisten>78))&&((inputAsisten != 'q')&&(inputAsisten!= 'Q')));

            
        while((inputAsisten != 'q')&&(inputAsisten!= 'Q')){
            //baca Minggu
            do{
                printf("Minggu: ");
                scanf("%d", &inputWeek);
                if ((inputWeek>14)||(inputWeek<3)){
                    printf("Masukan harus antara 3-14\n");
                }
            } while((inputWeek>14)||(inputWeek<3));
            i_week = inputWeek - 3;

                
            //baca hari
            do{
                printf("Hari: ");
                scanf("%s", &inputDay);

                if (strcmp(inputDay,"Senin")==0){
                    i_day = 0;
                }
                else if (strcmp(inputDay,"Selasa")==0){
                    i_day = 1;
                }
                else if (strcmp(inputDay,"Rabu")==0){
                    i_day = 2;
                }
                else if (strcmp(inputDay,"Kamis")==0){
                    i_day = 3;
                }
                else if(strcmp(inputDay,"Jumat")==0){
                    i_day = 4;
                }
                else {
                    printf("Masukan Hari Salah!\n");
                    i_day = -999;
                }
            } while(i_day<0);
                
                    
            //baca Ruang
            do {
                printf("Ruang: ");
                scanf("%s", &inputRuang);
                //baca junk
                scanf("%c", &junk);
                if (strcmp(inputRuang,"LAB1")==0){
                    i_lab = 0;
                }
                else if (strcmp(inputRuang,"LAB2")==0){
                    i_lab = 1;
                }
                else if (strcmp(inputRuang,"LAB3")==0){
                    i_lab = 2;
                }
                else if (strcmp(inputRuang,"LSS")==0){
                    i_lab = 3;
                }
                else
                {
                    i_lab = -999;
                    printf("Masukan Ruangan Salah!\n");
                }
            } while (i_lab <0);
                
            
        //cari asisten di database
                //mendapatkan index asisten di database
                i_asisten = getIndexAsisten(inputAsisten,Asprak);
        //cek apakah ada praktikum hari itu
                if (isPraktikumExist(Asprak.asprak[i_asisten],jadwal->week[i_week].day[i_day].lab[i_lab],jadwal->week[i_week].day[i_day])){
                //assign di jadwal
                    //cari tahu dulu ada berapa asisten di praktikum rombongan itu
                    int count = countAsisten(jadwal->week[i_week].day[i_day].lab[i_lab]);
                    if(count ==2) { //masih kosong
                        printf("\n");
                    }
                    else
                    {
                        jadwal->week[i_week].day[i_day].lab[i_lab].asprak[count].code = inputAsisten;
                    }
                }
                else
                {
                    printf("Jadwal Asisten %s Tidak Sesuai!\n", Asprak.asprak[i_asisten].nama);
                }

                do {
                    printf("\nPilih Asisten (A-N): ");
                    scanf("%c", &inputAsisten);
                    
                    if(((inputAsisten<65)||(inputAsisten>78))&&((inputAsisten != 'q')&&(inputAsisten!= 'Q')))
                    {
                        printf("Tidak ada Asisten dengan Kode %c!", inputAsisten);
                    
                    }
                }   while (((inputAsisten<65)||(inputAsisten>78))&&((inputAsisten != 'q')&&(inputAsisten!= 'Q')));
            
            } 
            if(isAsistenSingle(*jadwal)){
                printf("Ada Asisten Single!\nIsi sampai tidak ada asisten single!\n");
                
            }
            scanf("%c", &junk);
    }while ((isAsistenSingle(*jadwal))==true);

}

int getIndexAsisten (char asisten, dataAsprak Asprak){
    int index =0;
    while ((asisten != Asprak.asprak[index].code)&&(index <14)){
        index ++;
    }
    return index;
}

bool isPraktikumExist ( Asprak asprak, Lab lab, Day day){
    bool isExist = false;
    int i = 0;

    //kalo jadawal kosong return false lsg
    if (lab.group.praktikum == NULL){
        isExist =  false;
    }
    else { //cari apa ada praktikum yang dihandle
        while (( i < 2)&&(isExist == false)){
            
            if ((asprak.praktikum[i] != NULL)&&(strcmp(asprak.praktikum[i],lab.group.praktikum)==0)){
                isExist = true;
            }            
        i++;
        }
    }
    
    if (isExist == false){
        
    }
    else {  //praktikum exist
        //apakah berhalangan
        i = 0;
        while((i<3)&& (isExist == true)){
            if ((asprak.berhalangan[i]!= NULL)&&(strcmp(asprak.berhalangan[i],day.element)==0)){
                isExist = false;
            }
            i++;
        }
        
    }
    return isExist;
}

int countAsisten(Lab lab){
    int count = 0;
    for (int i = 0; i <2;i++){
        if (lab.asprak[i].code!=NULL){
            count++;
        }
    }
    return count;
}

bool isAsistenSingle (Schedule jadwal){
    //searching
    bool isSingle = false;

    for (int i = 0; i<12; i++){ //minggu
        //hari
        for(int j = 0 ; j < 5; j++){
            //lab
            for(int k = 0; k <4; k++){
                if (countAsisten(jadwal.week[i].day[j].lab[k]) == 1){
                    // printf("index %d, %d, %d", i,j,k);
                    isSingle = true;
                    break;
                }
            }
        }
    }
    return isSingle;
}

