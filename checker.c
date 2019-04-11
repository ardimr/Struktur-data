#include "Load.h"

void loadChecker(char * filename, Checker*checker){
    FILE *file;
    char buff[50];
    char *token;
    file = fopen("checker.txt", "r" );
    int i = 0; //index 
    while (fgets(buff, 50, file) != NULL) {
         printf("%d :%s", i,buff);
         token = strtok(buff,";");
         //printf("%s ", token);
        //alokasi memori untuk minggu
        checker->minggu[i]= malloc(strlen(token)+1);
        checker->minggu[i] = atoi(token);
        //ambil token lagi
        token = strtok(NULL,";");
        //printf("%s", token);
        //alokasi memori untuk hari
        checker->hari[i]= malloc(strlen(token)+1);
        checker->hari[i] = token;

        printf("%s-%s", checker->minggu[i], checker->hari[i]);
     i++;
    }  
}

void buildChecker(Checker *checker){
    char const *hari;
    //assign buat contoh saja
    checker ->minggu[0] = 3;
    hari = "Rabu";
    checker->hari[0] = malloc(strlen(hari)+1);
    strcpy(checker->hari[0],hari);


    checker ->minggu[1] = 4;
    hari = "Senin";
    checker->hari[1] = malloc(strlen(hari)+1);
    strcpy(checker->hari[1],hari);

}
void RuleChecker(Checker checker, Schedule jadwal){

    int i_minggu;
    int i_hari;
    for (int i = 0; i<size;i++){
        printf("minggu : %d Hari : %s\n", checker.minggu[i], checker.hari[i]);
         i_minggu = 0;
         i_hari = 0;

        //untuk database index pertama
        while ((checker.minggu[i] != jadwal.week[i_minggu].element)&&(i_minggu <2)){
            i_minggu++;
        }

        //cek hari
        while ((strcmp(checker.hari[i],jadwal.week[i_minggu].day[i_hari].element)!=0)&&(i_hari <5)){


//        printf("%s : %s\n", checker.hari[i],jadwal.week[i_minggu].day[i_hari].element);
//        printf("%d : %d\n", strlen(checker.hari[i]),strlen(jadwal.week[i_minggu].day[i_hari].element));

        i_hari++;

        }

        printf("i_minggu: %d i_hari : %d\n", i_minggu,i_hari);
    }


}
