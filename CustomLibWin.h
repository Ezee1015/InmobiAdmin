# include <stdio.h>
# include <time.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <sys/stat.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void Limpiar_Pantalla () {
    system("cls");
    system("clear");
}

void Limpiar_Pantalla_Win () {
    system("cls");
}

int LimpiarBuffer(void){
    int c;
    c = getchar();
    while (c != EOF && c != '\n') {
        c = getchar();
    }

    return 1;
}

int Pot (int Base, int Exponente) {
    if (Exponente==0) {
        return 1;
    } else {
        return Base*Pot(Base,Exponente-1);
    }
}

void Insertar_Fecha_en_Esquina() {
  time_t t;
  struct tm *tm;
  char Fecha[25];
  char NombreDia[25];
  /* char Hora[3]; */
  /* char Minutos[3]; */

  t=time(NULL);
  tm=localtime(&t);

  strftime(Fecha, 100, "%d/%m/%Y", tm);

  strftime(NombreDia, 100, "%A", tm);
  if(!strcmp(NombreDia,"Sunday")){strcpy(NombreDia,"Domingo");}
  if(!strcmp(NombreDia,"Monday")){strcpy(NombreDia,"Lunes");}
  if(!strcmp(NombreDia,"Tuesday")){strcpy(NombreDia,"Martes");}
  if(!strcmp(NombreDia,"Wednesday")){strcpy(NombreDia,"Miercoles");}
  if(!strcmp(NombreDia,"Thursday")){strcpy(NombreDia,"Jueves");}
  if(!strcmp(NombreDia,"Friday")){strcpy(NombreDia,"Viernes");}
  if(!strcmp(NombreDia,"Saturday")){strcpy(NombreDia,"Sabado");}

    /* strftime(Hora, 100, "%H", tm); */
    /* strftime(Minutos, 100, "%M", tm); */

  /* printf ("|   %-8s |\n| %s |\n|   %s:%-5s |\n--------------", NombreDia, Fecha, Hora, Minutos); */
  printf ("|   %-10s |\n|  %s  |\n----------------", NombreDia, Fecha);
}

int AnioActual () {
  time_t t;
  struct tm *tm;
  char Fecha[5];
  int FechaInt;

  t=time(NULL);
  tm=localtime(&t);

  strftime(Fecha, 100, "%Y", tm);
  FechaInt=atoi(Fecha);
  return FechaInt;
}

int MesActual () {
  time_t t;
  struct tm *tm;
  char Fecha[5];
  int FechaInt;

  t=time(NULL);
  tm=localtime(&t);

  strftime(Fecha, 100, "%m", tm);
  FechaInt=atoi(Fecha);
  return FechaInt;
}

int DiaActual () {
  time_t t;
  struct tm *tm;
  char Fecha[5];
  int FechaInt;

  t=time(NULL);
  tm=localtime(&t);

  strftime(Fecha, 100, "%d", tm);
  FechaInt=atoi(Fecha);
  return FechaInt;
}

