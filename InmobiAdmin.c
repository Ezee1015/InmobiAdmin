#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//////////////////////////////
#include "CustomLib.h"      // si es WINDOWS, Comentar esta linea y descomentar la de abajo
//#include "CustomLibWin.h" // si es Windows, en el comand mkdir borrar el segundo parámetro
//////////////////////////////
int Dispositivo=1;          // 1=PC | 2=CELU
         int SO=1;          // 1=LINUX/unix/Android | -1=WINDOWS
char Espacio[100];          //
//////////////////////////////

typedef struct {
    int Dia;
    int Mes;
    int Anio;
} Fecha;

typedef struct {
    char NombreGarant[40];
    long DNI;
    char DireccionGarant[35];
} Garantia;


typedef struct {
    long Legajo;
    char DireccionLocal[35];
        // TITULAR
        char NombreTitular[40];
        long DNITitular;
        char DireccionTitular[35];

        //INQUILINO
        char NombreInquilino[40];
        long DNIInquilino;
        Garantia GarantiaInquilino;

    int TipoContrato; // Semestral = 1  ;  Anual = 2 ; BiAnual = 3 ; TriAnual = 4

    float PrecioAlquiler; // Casi no Utilizado. Solamente para trasladar este valor al InteresMonto[0]. Solo se usa en InteresPorcentaje

    int TipoAjuste;
    int FormatoDeAjuste;

    float DeudaAlquiler[12];  // LO QUE ADEUDA EL INQUILINO
    float InteresMonto[12]; // EL VALOR DEL ALQUILER SEGUN MES
    int InteresPorcentaje;  //EL VALOR QUE TIENE QUE IR AUMENTANDO EL PrecioAlquiler SEGUN LOS MESES PACTADOS EN EL FORMATO AJUSTE
    float PrecioABL[12];
    float PrecioExpensas[12];
    float PrecioAysa[12];
    float InteresFormularCarrito[12];
    int AQueMesesCorrespondeLaUltimaDeuda[12];// SI HAY UNA DEUDA, A QUE MESES CORRESPONDO
        Fecha UltimoPagoAlquiler;
        int HistorialPagoAlquiler[12];
    /* float BancaDeudaTitular; // LO QUE EL TITULAR DEBE COBRAR (Deuda del Inquilino + Intereses) QUE LE FALTABA AL INQUILINO PAGAR Y QUE YA SE PUSO AL DIA */
        Fecha UltimoPagoTitular;
        int HistorialPagoTitular[12];
        float TotalDeCobroInquilino[12];  // ESTO SE USO PARA CALCULAR EL MONTO DE DEUDA E INTERESES A COBRAR PARA EL Titular, DESCONTANDO DEL TOTAL DEL INQUILINO EL AYSA,ABL,EXPENSAS Y ALQUILER, QUE DA LA DEUDA E INTERESES
        float TotalNetoAlInquilino[12];

    Fecha FechaInicio;
    Fecha FechaRecesion;

    int InteresPorPagoTarde; // Si no paga entre el 1 al 10 del mes, se aplica este interés según fórmula

    int ComisionDeLaInmobiliaria; //PORCENTAJE DE COMISION (SUELDO DE LA INMOBILIARIA)

} InquilinosSTR ;

InquilinosSTR Inquilino[10000];
int Menu_Principal ();
int CargarArchivoEnStruct (int);
int Mes_Cobrar (long);
int Menu_Cobrar ();
int Menu_Pagar ();
int BackupBaseDeDatos ();


char Nombre_Mes[11] = "    ";

void Nombre_Mes_Completo_Segun_Numero (int NumeroMes) {
    switch (NumeroMes%12) {
        case 1 :
            strcpy(Nombre_Mes,"Enero");
        break;
        case 2 :
            strcpy(Nombre_Mes,"Febrero");
        break;
        case 3 :
            strcpy(Nombre_Mes,"Marzo");
        break;
        case 4 :
            strcpy(Nombre_Mes,"Abril");
        break;
        case 5 :
            strcpy(Nombre_Mes,"Mayo");
        break;
        case 6 :
            strcpy(Nombre_Mes,"Junio");
        break;
        case 7 :
            strcpy(Nombre_Mes,"Julio");
        break;
        case 8 :
            strcpy(Nombre_Mes,"Agosto");
        break;
        case 9 :
            strcpy(Nombre_Mes,"Septiembre");
        break;
        case 10 :
            strcpy(Nombre_Mes,"Octuber");
        break;
        case 11 :
            strcpy(Nombre_Mes,"Noviembre");
        break;
        case 0 :
            strcpy(Nombre_Mes,"Diciembre");
        break;
    }
}


void Nombre_Mes_Segun_Numero (int NumeroMes) {
    switch (NumeroMes%12) {
        case 1 :
            strcpy(Nombre_Mes,"Ene");
        break;
        case 2 :
            strcpy(Nombre_Mes,"Feb");
        break;
        case 3 :
            strcpy(Nombre_Mes,"Mar");
        break;
        case 4 :
            strcpy(Nombre_Mes,"Abr");
        break;
        case 5 :
            strcpy(Nombre_Mes,"May");
        break;
        case 6 :
            strcpy(Nombre_Mes,"Jun");
        break;
        case 7 :
            strcpy(Nombre_Mes,"Jul");
        break;
        case 8 :
            strcpy(Nombre_Mes,"Ago");
        break;
        case 9 :
            strcpy(Nombre_Mes,"Sep");
        break;
        case 10 :
            strcpy(Nombre_Mes,"Oct");
        break;
        case 11 :
            strcpy(Nombre_Mes,"Nov");
        break;
        case 0 :
            strcpy(Nombre_Mes,"Dic");
        break;
    }
}

void Dibujar_Registro (int Pos, int Registro) {
            char Tit[50],Inq[50],Dir[50];
            long Leg= Inquilino[Pos].Legajo ;
            strcpy(Tit, Inquilino[Pos].NombreTitular );
            strcpy(Inq, Inquilino[Pos].NombreInquilino );
            strcpy(Dir, Inquilino[Pos].DireccionLocal );
            int i=0;

    if (Dispositivo==1) { //SI ES PC
        if (Registro==0) printf("-->|  %-14ld | ", Leg);
        else printf("   |  %-14ld | ", Leg);
            for(i=0;i<32;i++) {
                if (strlen(Tit)<i) printf(" ");
                else if (strlen(Tit)>32 && i==32-4) { printf("..."); i=i+3; }
                else if (strlen(Tit)>i) printf("%c",Tit[i]);
            }
        printf(" | ");
            for(i=0;i<34;i++) {
                if (strlen(Inq)<i) printf(" ");
                else if (strlen(Inq)>34 && i==34-4) { printf("..."); i=i+3; }
                else if (strlen(Inq)>i) printf("%c",Inq[i]);
            }
        printf(" | ");
            for(i=0;i<25;i++) {
                if (strlen(Dir)<i) printf(" ");
                else if (strlen(Dir)>25 && i==25-4) { printf("..."); i=i+3; }
                else if (strlen(Dir)>i) printf("%c",Dir[i]);
            }
        printf(" |\n");
    } else {
        if(Registro==0) {
            printf("\n+------------");
            printf("\n| Legajo N %ld", Leg);
            printf("\n|\t Titular: %s", Tit);
            printf("\n|\t Inquilino: %s", Inq);
            printf("\n|\t Direccion: %s", Dir);
            printf("\n+------------");
        } else {
            printf("\n  Legajo N %ld", Leg);
            printf("\n \t Titular: %s", Tit);
            printf("\n \t Inquilino: %s", Inq);
            printf("\n \t Direccion: %s", Dir);
        }
        fflush(stdout);
    }
}



int Comprobar_Agregado (int Pos) {

    char Caract;

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf(ANSI_COLOR_YELLOW"\n Resumen del Nuevo Alquiler...\n"ANSI_COLOR_RESET);

    printf("\n\t\t\t%-35s%ld","N  de Legajo: ",Inquilino[Pos].Legajo );
    printf("\n\t\t\t%-35s%s","Direccion del Local: ",Inquilino[Pos].DireccionLocal);
    printf("\n\t\t\t%-35s%s","Nombre del Titular: ", Inquilino[Pos].NombreTitular);
    printf("\n\t\t\t%-35s%ld","DNI del Titular: ", Inquilino[Pos].DNITitular);
    printf("\n\t\t\t%-35s%s","Direccion del Titular: ", Inquilino[Pos].DireccionTitular);
    printf("\n\t\t\t%-35s%s","Nombre del Inquilino: ", Inquilino[Pos].NombreInquilino);
    printf("\n\t\t\t%-35s%ld","DNI del Inquilino: ", Inquilino[Pos].DNIInquilino);
    printf("\n\t\t\t%-35s%s","Nombre del Garante: ", Inquilino[Pos].GarantiaInquilino.NombreGarant);
    printf("\n\t\t\t%-35s%ld","DNI del Garante: ", Inquilino[Pos].GarantiaInquilino.DNI);
    printf("\n\t\t\t%-35s%s","Direccion del Garante: ", Inquilino[Pos].GarantiaInquilino.DireccionGarant);
    printf("\n\t\t\t%-35s", "Tipo de Contrato: " );

    switch (Inquilino[Pos].TipoContrato) {
        case 1 :
            printf("6 Meses\n");
        break;
        case 2 :
            printf("1 Anio\n");
        break;
        case 3 :
            printf("2 Anios\n");
        break;
        case 4 :
            printf("3 Anios\n");
        break;
    }

    printf("\t\t\t%-35s%i/%i/%i", "Fecha de Inicio del Contrato: ", Inquilino[Pos].FechaInicio.Dia,Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio);
    printf("\n\t\t\t%-35s%i/%i/%i","Fecha de Recesion del Contrato: ", Inquilino[Pos].FechaRecesion.Dia, Inquilino[Pos].FechaRecesion.Mes,Inquilino[Pos].FechaRecesion.Anio);
    printf("\n\t\t\t%-35s$%.2f","Precio del Alquiler:",Inquilino[Pos].PrecioAlquiler);
    printf("\n\t\t\t%-35s$%.2f","Precio del ABL:",Inquilino[Pos].PrecioABL[0]);
    printf("\n\t\t\t%-35s$%.2f","Precio de las Expensas:",Inquilino[Pos].PrecioExpensas[0]);
    printf("\n\t\t\t%-35s$%.2f","Precio del AYSA:",Inquilino[Pos].PrecioAysa[0]);
    printf("\n\t\t\t%-35s", "Tipo de Ajuste: ");

    switch (Inquilino[Pos].TipoAjuste) {
        case 10 :
            printf("Sin Ajuste");
        break;
        case 1 :
            printf("Trimestral");
        break;
        case 2 :
            printf("Semestral");
        break;
        case 3 :
            printf("Anual");
        break;
    }

    printf("\n\t\t\t%-35s", "Formato de Ajuste: ");

        int i;
        float AniosContrato;
        char TextoMonto[35]=" ";

        switch(Inquilino[Pos].TipoContrato) {
            case 1 :
                AniosContrato=0.5;
            break;
            case 2 :
                AniosContrato=1;
            break;
            case 3 :
                AniosContrato=2;
            break;
            case 4 :
                AniosContrato=3;
            break;
        }

        int CantMontos = (12 * AniosContrato ) / (3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 ));
    switch (Inquilino[Pos].FormatoDeAjuste) {
        case 0 :
            printf("Sin Ajuste");
        break;
        case 1 :
            printf("Por Monto");
            for(i=0; i<CantMontos;i++) {
                sprintf(TextoMonto,"%s%-2i%s%-2i%s", "Monto del Mes ",i*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )), " al ", (i+1)*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )) , ":    $");
                printf("\n\t\t\t\t%s%.2f", TextoMonto, Inquilino[Pos].InteresMonto[i]);
            }
        break;
        case 2 :
            printf("Por Porcentaje");
            printf("\n\t\t\t\t%-27s%i%%","Porcentaje de Ajuste:",Inquilino[Pos].InteresPorcentaje);

        break;
    }

    printf("\n\t\t\t%-35s%i%%","Interes por Demora de Pago: ", Inquilino[Pos].InteresPorPagoTarde);

    printf("\n\t\t\t%-35s%i%%","Comision de la Inmobiliaria: ", Inquilino[Pos].ComisionDeLaInmobiliaria);

    printf(ANSI_COLOR_CYAN"\n\n        ¿Esta seguro que desea Agregar este Nuevo Alquiler? (S/N) > "ANSI_COLOR_RESET);
        LimpiarBuffer();
        fflush(stdin);
        Caract=getch();

        if (Caract=='s' || Caract=='S' || Caract=='1') {
            FILE *BaseDeDatos = fopen("Inquilinos.bin","a+b");
            if (BaseDeDatos!=NULL) {// SI SE ABRE CORRECTAMENTE
                fwrite(&Inquilino[Pos],sizeof(InquilinosSTR),1,BaseDeDatos);
                printf(ANSI_COLOR_GREEN"\nSe ha guardado Correctamente. Presione una Tecla para Salir al Menu..."ANSI_COLOR_RESET);
                getch();
            } else {
                printf(ANSI_COLOR_RED"\n\nERROR! No se Ha podido guardar. Presione una Tecla para Salir al Menu...\n"ANSI_COLOR_RESET);
                getch();
            }
            fclose(BaseDeDatos);
            return 1;
        } else {
            printf ("\nSe ha Elegido NO Guardar. Presione una Tecla para Salir al Menu...");
            getch();
            CargarArchivoEnStruct(1);
            return 0;
        }
}

int Exportar_Inquilino(int Pos) {

    char Archivo[200];

    sprintf(Archivo, "Contratos Vencidos/%s - %s - %i-%i.txt",Inquilino[Pos].DireccionLocal,Inquilino[Pos].NombreInquilino,Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio /*PONER ACA LOS CAMPOS*/);
    FILE *Exportar = fopen(Archivo,"w");

    fprintf(Exportar, "\tDatos del Alquiler:\n");


    fprintf(Exportar, "\n\t\t\t%-36s%ld","N  de Legajo: ",Inquilino[Pos].Legajo );
    fprintf(Exportar, "\n\t\t\t%-35s%s","Direccion del Local: ",Inquilino[Pos].DireccionLocal);
    fprintf(Exportar, "\n\t\t\t%-35s%s","Nombre del Titular: ", Inquilino[Pos].NombreTitular);
    fprintf(Exportar, "\n\t\t\t%-35s%ld","DNI del Titular: ", Inquilino[Pos].DNITitular);
    fprintf(Exportar, "\n\t\t\t%-35s%s","Direccion del Titular: ", Inquilino[Pos].DireccionTitular);
    fprintf(Exportar, "\n\t\t\t%-35s%s","Nombre del Inquilino: ", Inquilino[Pos].NombreInquilino);
    fprintf(Exportar, "\n\t\t\t%-35s%ld","DNI del Inquilino: ", Inquilino[Pos].DNIInquilino);
    fprintf(Exportar, "\n\t\t\t%-35s%s","Nombre del Garante: ", Inquilino[Pos].GarantiaInquilino.NombreGarant);
    fprintf(Exportar, "\n\t\t\t%-35s%ld","DNI del Garante: ", Inquilino[Pos].GarantiaInquilino.DNI);
    fprintf(Exportar, "\n\t\t\t%-35s%s","Direccion del Garante: ", Inquilino[Pos].GarantiaInquilino.DireccionGarant);
    fprintf(Exportar, "\n\t\t\t%-35s", "Tipo de Contrato: " );
    switch (Inquilino[Pos].TipoContrato) {
        case 1 :
            fprintf(Exportar, "6 Meses\n");
        break;
        case 2 :
            fprintf(Exportar, "1 Anio\n");
        break;
        case 3 :
            fprintf(Exportar, "2 Anios\n");
        break;
        case 4 :
            fprintf(Exportar, "3 Anios\n");
        break;
    }
    fprintf(Exportar, "\t\t\t%-35s%i/%i/%i", "Fecha de Inicio del Contrato: ", Inquilino[Pos].FechaInicio.Dia,Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio);
    fprintf(Exportar, "\n\t\t\t%-35s%i/%i/%i","Fecha de Recesion del Contrato: ", Inquilino[Pos].FechaRecesion.Dia, Inquilino[Pos].FechaRecesion.Mes,Inquilino[Pos].FechaRecesion.Anio);
    fprintf(Exportar, "\n\t\t\t%-35s$%.2f","Precio del Alquiler:  ",Inquilino[Pos].PrecioAlquiler);
    fprintf(Exportar, "\n\t\t\t%-35s$%.2f","Precio del ABL:  ",Inquilino[Pos].PrecioABL[0]);
    fprintf(Exportar, "\n\t\t\t%-35s$%.2f","Precio de las Expensas:  ",Inquilino[Pos].PrecioExpensas[0]);
    fprintf(Exportar, "\n\t\t\t%-35s$%.2f","Precio del AYSA:  ",Inquilino[Pos].PrecioAysa[0]);
    fprintf(Exportar, "\n\t\t\t%-35s", "Tipo de Ajuste: ");
    switch (Inquilino[Pos].TipoAjuste) {
        case 1 :
            fprintf(Exportar, "Trimestral");
        break;
        case 2 :
            fprintf(Exportar, "Semestral");
        break;
        case 3 :
            fprintf(Exportar, "Anual");
        break;
    }
    fprintf(Exportar, "\n\t\t\t%-35s", "Formato de Ajuste: ");
        int i;
        float AniosContrato;
        char TextoMonto[35]=" ";
        switch(Inquilino[Pos].TipoContrato) {
            case 1 :
                AniosContrato=0.5;
            break;
            case 2 :
                AniosContrato=1;
            break;
            case 3 :
                AniosContrato=2;
            break;
            case 4 :
                AniosContrato=3;
            break;
        }
    int CantMontos = (12 * AniosContrato ) / (3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 ));
    switch (Inquilino[Pos].FormatoDeAjuste) {
        case 1 :
            fprintf(Exportar, "Por Monto");
            for(i=0; i<CantMontos;i++) {
                sprintf( TextoMonto,"%s%-2i%s%-2i%s", "Monto del Mes ",i*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )), " al ", (i+1)*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )) , ":    $");
                fprintf(Exportar, "\n\t\t\t\t%s%.2f", TextoMonto, Inquilino[Pos].InteresMonto[i]);
            }
        break;
        case 2 :
            fprintf(Exportar, "Por Porcentaje");
            fprintf(Exportar, "\n\t\t\t\t%-27s%i%%","Porcentaje de Ajuste:",Inquilino[Pos].InteresPorcentaje);
        break;
    }
    fprintf(Exportar, "\n\t\t\t%-35s%i%%","Interes por Demora de Pago: ", Inquilino[Pos].InteresPorPagoTarde);


    int TiempoDesdeUltimoPago=(AnioActual()-Inquilino[Pos].UltimoPagoAlquiler.Anio)*12+(MesActual()-Inquilino[Pos].UltimoPagoAlquiler.Mes);

    fprintf(Exportar, "\n\n\n\n\tPago Alquiler (Ultimos Registros A Partir Del Ultimo Pago):\n");


    for (i=0;i<12;i++) {

        int NumeroMes=(((TiempoDesdeUltimoPago-i)%12)+MesActual())%12;
        if(NumeroMes==0) NumeroMes=12;
        fprintf(Exportar, "\n\t\t    %2i", NumeroMes);

        int NumeroAnio;
        int MesUltimoPago=(24+(MesActual()-TiempoDesdeUltimoPago))%12;
        if(MesUltimoPago==0) MesUltimoPago=12;
        if(NumeroMes>MesUltimoPago) NumeroAnio=AnioActual()-(TiempoDesdeUltimoPago/12)-1;
        else NumeroAnio=AnioActual()-(TiempoDesdeUltimoPago/12);
        fprintf(Exportar, "/%i ", NumeroAnio);

           switch(Inquilino[Pos].HistorialPagoAlquiler[i]){
                case 0 :
                        fprintf(Exportar, " [");
                        fprintf(Exportar, "  IMPAGO  ");
                        fprintf(Exportar, "]");
                break;
                case 1:
                    fprintf(Exportar, " [");
                    fprintf(Exportar, "  PAGADO  ");
                    fprintf(Exportar, "]");
                break;
                case 2 :
                    fprintf(Exportar, " [");
                    fprintf(Exportar, "  ADEUDA  ");
                    fprintf(Exportar, "] Deuda (Sin Interes): $%.2f", Inquilino[Pos].DeudaAlquiler[i]);
                break;
            }
    }

    fclose(Exportar);
    return 0;
}

int Menu_Agregar () {

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    char Caract=' ';
    int i=0, Pos=0;

    Insertar_Fecha_en_Esquina();

    // CONSIGUE LA POSICION DEL ULTIMO REGISTRO DEL ARCHIVO
    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
    if (BaseDeDatos!=NULL) {
        fseek(BaseDeDatos,0,SEEK_END);
        Pos= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  ) + 1 ; // SE TIENE QUE AGREGAR EL +1 YA QUE SINO SOBREESCRIBE EL ULTIMO
        fclose(BaseDeDatos);
    } else printf(ANSI_COLOR_RED"\n\n         (No se ha encontrado una Base de Datos. Creandola al Guardar los Cambios de este Alquiler)"ANSI_COLOR_RESET);

    printf("\n\n    Agregar un Nuevo Inquilino...\n");

    printf("    Aprete una tecla para comenzar o Escape para Salir...\n\n");
    fflush(stdin);
    Caract=getch();
    if(Caract==27) return 0;

    printf(ANSI_COLOR_RED"             Recuerde de NO Ingresar Legajo 0 NI un Legajo Repetido, ya que ese es un Legajo reservado para el sistema!"ANSI_COLOR_RESET);
    printf("\n        N  de Legajo: ");
        scanf("%ld", &Inquilino[Pos].Legajo);

    printf("        Direccion del Alquiler : ");
        LimpiarBuffer();
        fgets(Inquilino[Pos].DireccionLocal, sizeof(Inquilino[Pos].DireccionLocal),stdin);
        if( Inquilino[Pos].DireccionLocal [strlen( Inquilino[Pos].DireccionLocal ) - 1] == '\n' ) {
            Inquilino[Pos].DireccionLocal [strlen( Inquilino[Pos].DireccionLocal ) - 1] = 0;
        }

    printf("        Nombre y Apellido del Titular: ");
        fgets(Inquilino[Pos].NombreTitular, sizeof(Inquilino[Pos].NombreTitular),stdin);
        if( Inquilino[Pos].NombreTitular [strlen( Inquilino[Pos].NombreTitular ) - 1] == '\n' ) {
            Inquilino[Pos].NombreTitular [strlen( Inquilino[Pos].NombreTitular ) - 1] = 0;
        }

    printf("        DNI del Titular: ");
        do {
            scanf("%ld", &Inquilino[Pos].DNITitular);
            if((Inquilino[Pos].DNITitular/10000000)<1 || (Inquilino[Pos].DNITitular/100000000)>=1) printf(ANSI_COLOR_RED"        DNI Ingresado Incorrrecto. Intente de Vuelta: "ANSI_COLOR_RESET);
        } while((Inquilino[Pos].DNITitular/10000000)<1 || (Inquilino[Pos].DNITitular/100000000)>=1);


    printf("        Direccion del Titular: ");
        LimpiarBuffer();
        fgets(Inquilino[Pos].DireccionTitular, sizeof(Inquilino[Pos].DireccionTitular),stdin);
        if( Inquilino[Pos].DireccionTitular [strlen( Inquilino[Pos].DireccionTitular ) - 1] == '\n' ) {
            Inquilino[Pos].DireccionTitular [strlen( Inquilino[Pos].DireccionTitular ) - 1] = 0;
        }

    printf("        Nombre y Apellido del Inquilino: ");
        fgets(Inquilino[Pos].NombreInquilino, sizeof(Inquilino[Pos].NombreInquilino),stdin);
        if( Inquilino[Pos].NombreInquilino [strlen( Inquilino[Pos].NombreInquilino ) - 1] == '\n' ) {
            Inquilino[Pos].NombreInquilino [strlen( Inquilino[Pos].NombreInquilino ) - 1] = 0;
        }

    printf("        DNI del Inquilino: ");
        do {
            scanf("%ld", &Inquilino[Pos].DNIInquilino);
            if((Inquilino[Pos].DNIInquilino/10000000)<1 || (Inquilino[Pos].DNIInquilino/100000000)>=1) printf(ANSI_COLOR_RED"        DNI Ingresado Incorrrecto. Intente de Vuelta: "ANSI_COLOR_RESET);
        } while((Inquilino[Pos].DNIInquilino/10000000)<1 || (Inquilino[Pos].DNIInquilino/100000000)>=1);


    printf("        Nombre y Apellido de la Garantia: ");
        LimpiarBuffer();
        fgets(Inquilino[Pos].GarantiaInquilino.NombreGarant, sizeof(Inquilino[Pos].GarantiaInquilino.NombreGarant),stdin);
        if( Inquilino[Pos].GarantiaInquilino.NombreGarant [strlen( Inquilino[Pos].GarantiaInquilino.NombreGarant ) - 1] == '\n' ) {
            Inquilino[Pos].GarantiaInquilino.NombreGarant [strlen( Inquilino[Pos].GarantiaInquilino.NombreGarant ) - 1] = 0;
        }

    printf("        DNI de la Garantia: ");
        do {
            scanf("%ld", &Inquilino[Pos].GarantiaInquilino.DNI);
            if((Inquilino[Pos].GarantiaInquilino.DNI/10000000)<1 || (Inquilino[Pos].GarantiaInquilino.DNI/100000000)>=1) printf(ANSI_COLOR_RED"        DNI Ingresado Incorrrecto. Intente de Vuelta: "ANSI_COLOR_RESET);
        } while((Inquilino[Pos].GarantiaInquilino.DNI/10000000)<1 || (Inquilino[Pos].GarantiaInquilino.DNI/100000000)>=1);

    printf("        Direccion de la Garantia: ");
        LimpiarBuffer();
        fgets(Inquilino[Pos].GarantiaInquilino.DireccionGarant, sizeof(Inquilino[Pos].GarantiaInquilino.DireccionGarant),stdin);
        if( Inquilino[Pos].GarantiaInquilino.DireccionGarant [strlen( Inquilino[Pos].GarantiaInquilino.DireccionGarant ) - 1] == '\n' ) {
            Inquilino[Pos].GarantiaInquilino.DireccionGarant [strlen( Inquilino[Pos].GarantiaInquilino.DireccionGarant ) - 1] = 0;
        }


    printf("        ¿Que tipo de Contrato es?");
    printf(ANSI_COLOR_CYAN"\n                   1.- 0,5 Anios \\ 2.- 1 Anio \\ 3.- 2 Anios \\ 4.- 3 Anios"ANSI_COLOR_RESET);
    printf("\n        Ingrese ( 1 , 2 , 3 o 4 ) > ");
            fflush(stdin);
            Caract=getch();
            while (Caract=='\n' || Caract>'4' || Caract<'1') {
                fflush(stdin);
                Caract=getch();
            }
            Inquilino[Pos].TipoContrato=atoi(&Caract);
            printf(" %i",Inquilino[Pos].TipoContrato);

    printf(ANSI_COLOR_RED"\n           Recuerde NO usar 0 en el Principio del MES ni del DIA!"ANSI_COLOR_RESET);
    printf("\n        Ingrese la Fecha de Inicio del Contrato (Formato: d/m/yyyy): ");
        scanf("%i/%i/%i", &Inquilino[Pos].FechaInicio.Dia, &Inquilino[Pos].FechaInicio.Mes, &Inquilino[Pos].FechaInicio.Anio);
        Inquilino[Pos].UltimoPagoAlquiler.Dia=Inquilino[Pos].FechaInicio.Dia;
        Inquilino[Pos].UltimoPagoAlquiler.Mes=Inquilino[Pos].FechaInicio.Mes;
        Inquilino[Pos].UltimoPagoAlquiler.Anio=Inquilino[Pos].FechaInicio.Anio;
        Inquilino[Pos].UltimoPagoTitular.Dia=Inquilino[Pos].FechaInicio.Dia;
        Inquilino[Pos].UltimoPagoTitular.Mes=Inquilino[Pos].FechaInicio.Mes;
        Inquilino[Pos].UltimoPagoTitular.Anio=Inquilino[Pos].FechaInicio.Anio;

    printf(ANSI_COLOR_RED"\n           Recuerde NO usar 0 en el Principio del MES ni del DIA!"ANSI_COLOR_RESET);
    printf("\n        Ingrese la Fecha de Recesion del Contrato (Formato: d/m/yyyy): ");
        scanf("%i/%i/%i", &Inquilino[Pos].FechaRecesion.Dia, &Inquilino[Pos].FechaRecesion.Mes, &Inquilino[Pos].FechaRecesion.Anio);

    printf("        Ingrese el Precio del Alquiler (Ej.: $2500.25): $");
        scanf("%f", &Inquilino[Pos].PrecioAlquiler);
        Inquilino[Pos].HistorialPagoAlquiler[0]=1;

    printf("        Ingrese el Precio del ABL (Ej.: $2500.25): $");
        scanf("%f", &Inquilino[Pos].PrecioABL[0]);

    printf("        Ingrese el Precio de las Expensas (Ej.: $2500.25): $");
        scanf("%f", &Inquilino[Pos].PrecioExpensas[0]);

    printf("        Ingrese el Precio del AYSA (Ej.: $2500.25): $");
        scanf("%f", &Inquilino[Pos].PrecioAysa[0]);

    Inquilino[Pos].HistorialPagoTitular[0]=1;

        // OPCIONES DEL TIPO DE AJUSTE
        switch (Inquilino[Pos].TipoContrato) {
            case 1 : // Contrato Semestral
                    Inquilino[Pos].TipoAjuste = 10; // SIN AJUSTE
                    Inquilino[Pos].FormatoDeAjuste = 0; // SIN AJUSTE
                    Inquilino[Pos].InteresMonto[0]=Inquilino[Pos].PrecioAlquiler;
                    printf("        Ingrese el Porcentaje de la Tarifa de la Demora: ");
                        scanf("%i", &Inquilino[Pos].InteresPorPagoTarde);

                    printf("        Ingrese el Porcentaje de la Comision de la Inmobiliaria: ");
                        scanf("%i", &Inquilino[Pos].ComisionDeLaInmobiliaria);
                    return Comprobar_Agregado(Pos);
            break;

            case 2 : // Contrato Anual
                printf("        Ingrese el Tipo de Ajuste: ");
                printf(ANSI_COLOR_CYAN"\n                   1.- Trimestral \\ 2.- Semestral"ANSI_COLOR_RESET); // Solo 2 ajustes,en adelante (3 y 4), todos los ajustes disponibles
                printf("\n        Ingrese ( 1 o 2 ) > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'2' || Caract<'1') {
                        fflush(stdin);
                        Caract=getch();
                    }
                    Inquilino[Pos].TipoAjuste=atoi(&Caract);
                    printf("%i", Inquilino[Pos].TipoAjuste);
            break;

            case 3 : // Contrato BiAnual
                printf("        Ingrese el Tipo de Ajuste: ");
                printf(ANSI_COLOR_CYAN"\n                   1.- Trimestral \\ 2.- Semestral \\ 3.- Anual"ANSI_COLOR_RESET);
                printf("\n        Ingrese ( 1 , 2 o 3) > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'4' || Caract<'1') {
                        fflush(stdin);
                        Caract=getch();
                    }
                    Inquilino[Pos].TipoAjuste=atoi(&Caract);
                    printf("%i", Inquilino[Pos].TipoAjuste);
            break;

            case 4 : // Contrato TriAnual
                printf("        Ingrese el Tipo de Ajuste: ");
                printf(ANSI_COLOR_CYAN"\n                   1.- Trimestral \\ 2.- Semestral \\ 3.- Anual"ANSI_COLOR_RESET);
                printf("\n        Ingrese ( 1 , 2 o 3) > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'4' || Caract<'1') {
                        fflush(stdin);
                        Caract=getch();
                    }
                    Inquilino[Pos].TipoAjuste=atoi(&Caract);
                    printf("%i", Inquilino[Pos].TipoAjuste);
            break;
        }

    printf("\n        Ingrese el Formato de Ajuste: ");
    printf(ANSI_COLOR_CYAN"\n                   1.- Por Monto \\ 2.- Por Porcentaje"ANSI_COLOR_RESET);
    printf("\n        Ingrese ( 1 o 2 ) > ");

        fflush(stdin);
        Caract=getch();
        while (Caract=='\n' || Caract>'2' || Caract<'1') {
            fflush(stdin);
            Caract=getch();
        }
        Inquilino[Pos].FormatoDeAjuste=atoi(&Caract);
        printf("%i\n", Inquilino[Pos].FormatoDeAjuste);

        // OPCIONES DEL TIPO DE TARIFA
        float AniosContrato;

        switch(Inquilino[Pos].TipoContrato) {
            case 1 :
                AniosContrato=0.5;
            break;
            case 2 :
                AniosContrato=1;
            break;
            case 3 :
                AniosContrato=2;
            break;
            case 4 :
                AniosContrato=3;
            break;
        }

        int CantMontos = (12 * AniosContrato ) / (3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 ));
        switch (Inquilino[Pos].FormatoDeAjuste) {
            case 1 :
                Inquilino[Pos].InteresMonto[0]=Inquilino[Pos].PrecioAlquiler;
                for (i=1;i<CantMontos;i++) {
                    printf("              Ingrese el Monto Del mes %i al %i > $", i*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )), (i+1)*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )));
                        scanf("%f", &Inquilino[Pos].InteresMonto[i]);
                }
            break;
            case 2 :
                printf("\n        Ingrese el Porcentaje del Ajuste: ");
                    scanf("%i", &Inquilino[Pos].InteresPorcentaje);
                        Inquilino[Pos].InteresMonto[0]=Inquilino[Pos].PrecioAlquiler;
                        for (i=1;i<CantMontos;i++) {
                            Inquilino[Pos].InteresMonto[i]=Inquilino[Pos].InteresMonto[i-1]+Inquilino[Pos].InteresMonto[i-1]*20/100;
                        }
            break;
        }

        printf("        Ingrese el Porcentaje de la Tarifa de la Demora: ");
            scanf("%i", &Inquilino[Pos].InteresPorPagoTarde);

        printf("        Ingrese el Porcentaje de la Comision de la Inmobiliaria: ");
            scanf("%i", &Inquilino[Pos].ComisionDeLaInmobiliaria);


    return Comprobar_Agregado(Pos);
}



long SeleccionBusqueda (char PatronBusquedaShow[50], int EleccionCampo, int Resultados) {
    int d, i, CantRegs=0;
    long LegajoDelMasAltoAsciertos;
        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            fclose(BaseDeDatos);
        } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}

    int x,z, Asciertos=0;
    char LegajoNumero[50];

    LegajoDelMasAltoAsciertos=-1;

        switch (EleccionCampo) {
            case 1 :
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;

                    for (d=0;d<50;d++){
                        LegajoNumero[d]=0;
                    }

                    sprintf(LegajoNumero,"%ld",Inquilino[i].Legajo); //Transforma de Numero a String


                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(LegajoNumero);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==LegajoNumero[z] && PatronBusquedaShow[x+1]==LegajoNumero[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if (Resultados==0) return Inquilino[i].Legajo;
                        Resultados++;
                        if (LegajoDelMasAltoAsciertos<Asciertos) LegajoDelMasAltoAsciertos = Inquilino[i].Legajo;
                    }
                }

            break;
            case 2 :
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;
                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(Inquilino[i].NombreTitular);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==Inquilino[i].NombreTitular[z] && PatronBusquedaShow[x+1]==Inquilino[i].NombreTitular[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if (Resultados==0) return Inquilino[i].Legajo;
                        Resultados++;
                        if (LegajoDelMasAltoAsciertos<Asciertos) LegajoDelMasAltoAsciertos = Inquilino[i].Legajo;
                    }
                }
            break;
            case 3 :
                Resultados=0;
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;
                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(Inquilino[i].NombreInquilino);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==Inquilino[i].NombreInquilino[z] && PatronBusquedaShow[x+1]==Inquilino[i].NombreInquilino[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if (Resultados==0) return Inquilino[i].Legajo;
                        Resultados++;
                        if (LegajoDelMasAltoAsciertos<Asciertos) LegajoDelMasAltoAsciertos = Inquilino[i].Legajo;
                    }
                }
            break;
            case 4 :
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;
                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(Inquilino[i].DireccionLocal);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==Inquilino[i].DireccionLocal[z] && PatronBusquedaShow[x+1]==Inquilino[i].DireccionLocal[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if (Resultados==0) return Inquilino[i].Legajo;
                        Resultados++;
                        if (LegajoDelMasAltoAsciertos<Asciertos) LegajoDelMasAltoAsciertos = Inquilino[i].Legajo;
                    }
                }
            break;
            case 5 :
                return Inquilino[Resultados].Legajo;
            break;
        }

            return LegajoDelMasAltoAsciertos;

}

long BusquedaShow (char PatronBusquedaShow[50], int EleccionCampo, int Resultado) {
    int d, i, CantRegs=0;

        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            fclose(BaseDeDatos);
        } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}

    if (Dispositivo==1) printf("\n   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");
    if (Dispositivo==1) printf("   |    N  Legajo    |            Titular              |              Inquilino            |         Direccion        |\n");
    if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");

    int x,z, Asciertos=0;
    char LegajoNumero[50];


        switch (EleccionCampo) {
            case 1 :

                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;

                    for (d=0;d<50;d++){
                        LegajoNumero[d]=0;
                    }


                    sprintf(LegajoNumero,"%ld",Inquilino[i].Legajo); //Transforma de Numero a String


                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(LegajoNumero);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==LegajoNumero[z] && PatronBusquedaShow[x+1]==LegajoNumero[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if(Resultado==0) { Dibujar_Registro(i, 0); Resultado++; }
                        else { Dibujar_Registro(i,1); Resultado++; }
                    }
                }

            break;
            case 2 :
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;
                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(Inquilino[i].NombreTitular);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==Inquilino[i].NombreTitular[z] && PatronBusquedaShow[x+1]==Inquilino[i].NombreTitular[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if(Resultado==0) { Dibujar_Registro(i, 0); Resultado++; }
                        else { Dibujar_Registro(i,1); Resultado++; }
                    }
                }
            break;
            case 3 :
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;
                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(Inquilino[i].NombreInquilino);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==Inquilino[i].NombreInquilino[z] && PatronBusquedaShow[x+1]==Inquilino[i].NombreInquilino[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if(Resultado==0) { Dibujar_Registro(i, 0); Resultado++; }
                        else { Dibujar_Registro(i,1); Resultado++; }
                    }
                }
            break;
            case 4 :
                for(i=0;i<CantRegs;i++){ // PARA CADA REGISTRO
                    Asciertos=0;
                    for(x=0;x<strlen(PatronBusquedaShow);x++) { // PARA CADA LETRA DEL PATRON
                        for (z=0;z<strlen(Inquilino[i].DireccionLocal);z++){ //PARA CADA LETRA DEL CAMPO
                            if(PatronBusquedaShow[x]==Inquilino[i].DireccionLocal[z] && PatronBusquedaShow[x+1]==Inquilino[i].DireccionLocal[z+1] && PatronBusquedaShow[x]!=' ') Asciertos++;
                        }
                    }

                    if((Asciertos*2)>(strlen(PatronBusquedaShow))) { //SI SON MAS DE 3 ASCIERTOS, LO IMPRIME
                        if(Resultado==0) { Dibujar_Registro(i, 0); Resultado++; }
                        else { Dibujar_Registro(i,1); Resultado++; }
                    }
                }
            break;
        }

  if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");

  return 0;

}


int Imprime_Todos_inquilinos (int CantRegs, int Resultados) {
    int i;


    if (Dispositivo==1) printf("\n\n   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");
    if (Dispositivo==1) printf("   |    N  Legajo    |            Titular              |              Inquilino            |         Direccion        |\n");
    if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");

    for(i=0;i<CantRegs;i++) {
        if(Resultados==0) { Dibujar_Registro(i,0); Resultados++; }
        else { Dibujar_Registro(i,1); Resultados++; }
    }

  if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");
    return 0;
}


int Listar_Inquilinos () {
    char Caract;
    int CantRegs, Resultado=0;


    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
    if (BaseDeDatos!=NULL) {
        fseek(BaseDeDatos,0,SEEK_END);
        CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
        fclose(BaseDeDatos);
    } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); getch(); Menu_Principal(); return 0;}

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf("\n%s                      +----------+\n", Espacio);
    printf("%s                      | BUSQUEDA |\n", Espacio);
    printf("%s                      +----------+\n", Espacio);
    printf(" (Seleccione un Registro para Salir de este Menu)\n");

    Imprime_Todos_inquilinos(CantRegs,Resultado);

    do {
        fflush(stdin);
        Caract=getch();
        fflush(stdin);
        switch(Caract) {
            case 65 : //SI ES FLECHA PARA ARRIBA
                if(Resultado<0) { Resultado++; }
            break;
            case 72 : //WINDOWS ARRIBA
                if(Resultado<0) { Resultado++; }
            break;
            case 66 : //SI ES FLECHITA PARA ABAJO. Tambien se Activa con [Shift-G]
                if(Resultado>((-1*CantRegs)+1))Resultado--;
            break;
            case 80 : //Windows Abajo
                if(Resultado>((-1*CantRegs)+1))Resultado--;
            break;
            case 10 :
                break;
            break;
            case 13 : // WINDOWS ENTER
                break;
            break;
            case 67 :
                break;
            break;
            case 77 : //WINDOWS DERECHA
                break;
            break;
            case 68 : // Si apreto [Shift-D] se me activa este Menu
                /* break; */
            break;
            case 75 : // Si apreto [Shift-D] se me activa este Menu //WINDOWS IZQUIERDA
                /* break; */
            break;
        }

        if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
        Insertar_Fecha_en_Esquina();
        printf("\n%s                      +----------+\n", Espacio);
        printf("%s                      | BUSQUEDA |\n", Espacio);
        printf("%s                      +----------+\n", Espacio);
        printf(" (Seleccione un Registro para Salir de este Menu)\n");

        Imprime_Todos_inquilinos(CantRegs,Resultado);

    } while (Caract!=10 && Caract!=13 && Caract != 67 && Caract != 77) ; //13 Windows Enter  /77 Windows Derecha



    return Inquilino[(Resultado*(-2))/2].Legajo; //Ecuacion matematica en Resultado, para pasar, por ejemplo, el -2 a 2
}


int Cobrar_Inquilino (int Pos) {
    float Descuento=0, PagoInquilino=0;
    int i;
    int TiempoAjuste=3*Pot(2,(Inquilino[Pos].TipoAjuste-1));
    if(TiempoAjuste==0) TiempoAjuste=10;
    int TiempoDeContrato=(AnioActual()-Inquilino[Pos].FechaInicio.Anio)*12+(MesActual()-Inquilino[Pos].FechaInicio.Mes);
    int Eleccion=0;
    float Precio=0;
    float TotalInteres=0, TotalDeuda=0;;
    char Caract, Descripcion[300]="                                                                                                                                                                                                                                                                                                            ";
////////////
    int CantRegs;
    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
    if (BaseDeDatos!=NULL) {
        fseek(BaseDeDatos,0,SEEK_END);
        CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
        fclose(BaseDeDatos);
    } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}
///////
    for(int i=1;i<12;i++){ // NO LO PONGO EN 0 PORQUE EL INQUILINO NO PUEDE DEBER INTERESES DEL MISMO MES QUE ESTA PAGANDO
        if(Inquilino[Pos].DeudaAlquiler[i]>0) {
            TotalDeuda=TotalDeuda+Inquilino[Pos].DeudaAlquiler[i];
            TotalInteres=TotalInteres+((Inquilino[Pos].DeudaAlquiler[i] * Inquilino[Pos].InteresPorPagoTarde * 30 * i)/(100));
        }
    }


    Precio=Inquilino[Pos].InteresMonto[TiempoDeContrato/TiempoAjuste]+Inquilino[Pos].PrecioABL[0]+Inquilino[Pos].PrecioAysa[0]+Inquilino[Pos].PrecioExpensas[0];

                    printf(ANSI_COLOR_GREEN"\n\n\t ALQUILER:       + $%10.2f",Inquilino[Pos].InteresMonto[TiempoDeContrato/TiempoAjuste]);
                    printf("\n\t ABL:            + $%10.2f",Inquilino[Pos].PrecioABL[0]);
                    printf("\n\t AYSA:           + $%10.2f",Inquilino[Pos].PrecioAysa[0]);
                    printf("\n\t EXPENSAS:       + $%10.2f",Inquilino[Pos].PrecioExpensas[0]);
                    printf("\n\t PENDIENTE:      + $%10.2f",TotalDeuda);
                    printf("\n\t--------------------------------");
                    printf("\n\t TOTAL:          + $%10.2f"ANSI_COLOR_RESET,Precio+TotalDeuda);

                    // CAMBIA ABL
                    printf("\n\n\n\t¿Desea modificar el Precio del ABL para Ahora en Adelante? > ");
                            fflush(stdin);
                            Caract=getch();
                            printf("%c", Caract);
                    if (Caract=='s' || Caract=='S' || Caract=='1') {
                        printf("\n\t Ingrese el Nuevo Precio > $");
                        scanf("%f", &Inquilino[Pos].PrecioABL[0]);
                        LimpiarBuffer();
                    }

                    //CAMBIA AYSA
                    printf("\n\t¿Desea modificar el Precio del AYSA para Ahora en Adelante? > ");
                            fflush(stdin);
                            Caract=getch();
                            printf("%c", Caract);

                    if (Caract=='s' || Caract=='S' || Caract=='1') {
                        printf("\n\t Ingrese el Nuevo Precio > $");
                        scanf("%f", &Inquilino[Pos].PrecioAysa[0]);
                        LimpiarBuffer();
                    }

                    //CAMBIA EXPENSAS
                    printf("\n\t¿Desea modificar el Precio de las Expensas para Ahora en Adelante? > ");
                            fflush(stdin);
                            Caract=getch();
                            printf("%c", Caract);
                    if (Caract=='s' || Caract=='S' || Caract=='1') {
                        printf("\n\t Ingrese el Nuevo Precio > $");
                        scanf("%f", &Inquilino[Pos].PrecioExpensas[0]);
                        LimpiarBuffer();
                    }


    Precio=Inquilino[Pos].InteresMonto[TiempoDeContrato/TiempoAjuste]+Inquilino[Pos].PrecioABL[0]+Inquilino[Pos].PrecioAysa[0]+Inquilino[Pos].PrecioExpensas[0];

    if(TotalDeuda>0) {
        printf("\n\t ¿Desea Aplicar Algun Interes por el Incumplimiento del Pago? (Deuda: $%.2f)", TotalDeuda);
        printf(ANSI_COLOR_CYAN"\n                   1.- Automatico (+$%.2f) \\ 2.- No \\ 3.- Manual"ANSI_COLOR_RESET, TotalInteres);
        printf("\n\n\t Ingrese su eleccion ( 1 , 2 o 3 ) > ");
            fflush(stdin);
            Caract=getch();
            while (Caract=='\n' || Caract>'3' || Caract<'1') {
                printf("%c", Caract);
                fflush(stdin);
                Caract=getch();
            }
            if (Caract == '3' ) {
                    printf("\n\t\t Ingrese el Interes para la Deuda > ");
                    scanf("%f", &TotalInteres);
            }
            if (Caract == '2' ) {
                TotalInteres=0;
            }
            for(i=0;i<12;i++){
                Inquilino[Pos].DeudaAlquiler[i]=0;
                Inquilino[Pos].HistorialPagoAlquiler[i]=1;
            }
            Precio=Precio+TotalDeuda+TotalInteres;
            Inquilino[Pos].InteresFormularCarrito[0]=TotalInteres;
    }

    printf("\n\t ¿Desea Aplicar Algun Descuento? (S/N) > ");
            fflush(stdin);
            Caract=getch();
            printf("%c", Caract);

    if (Caract=='s' || Caract=='S' || Caract=='1') {
        printf("\n\t\t Ingrese el Descuento > $");
            scanf("%f", &Descuento);
        printf("\n\t Ingrese una Descripcion  (No se permite ñ. Presione Enter para Terminar) > \n\n\t\t"ANSI_COLOR_CYAN);
            LimpiarBuffer();
            fgets(Descripcion,sizeof(Descripcion),stdin);
            if( Descripcion [strlen( Descripcion ) - 1] == '\n' ) {
                Descripcion [strlen( Descripcion ) - 1] = 0;
            }
            printf(ANSI_COLOR_RESET);
    }

    printf(ANSI_COLOR_GREEN"\n\t\t\t\t Total a Pagar: $%.2f"ANSI_COLOR_RESET, Precio-Descuento);
    printf("\n\n\t\t Ingrese lo que el Inquilino desea Pagar > $");
        scanf("%f", &PagoInquilino);

    if(PagoInquilino<(Precio-Descuento)) {
        if(Precio-Descuento-PagoInquilino>1){ // HACE QUE SI FALTA MAS DE UN PESO DE DIFERENCIA QUEDE IMPAGO, SINO QUE PASE COMO PAGO E IGUALA LA DIFERENCIA PARA QUE NO HAYA PROBLEMAS
            Inquilino[Pos].HistorialPagoAlquiler[0]=2;
            Inquilino[Pos].DeudaAlquiler[0]=(Precio-Descuento)-PagoInquilino;
        } else {
            Inquilino[Pos].HistorialPagoAlquiler[0]=1;
            PagoInquilino=Precio-Descuento;
            for(i=0;i<12;i++)
                Inquilino[Pos].DeudaAlquiler[i]=0;

        }
    } else Inquilino[Pos].HistorialPagoAlquiler[0]=1;

    Inquilino[Pos].TotalDeCobroInquilino[0]=PagoInquilino; //CAMBIARDO DE precio-descuento
    Inquilino[Pos].TotalNetoAlInquilino[0]=Precio-Descuento;



    char NombreTicket[50]= "  ";
            sprintf(NombreTicket , "Cobros/%ld-%i-%-4i" , Inquilino[Pos].Legajo , MesActual() , AnioActual() );
    FILE *Ticket = fopen(NombreTicket,"w");


    fprintf(Ticket, "\n\n\n\t\t\t\t\t\t\t\t\tRECIBO DE PAGO");
    fprintf(Ticket, "\n\n\t Recibimos el dia de hoy %i/%i/%i del Sr./Sra. Inquilino/a %s que", DiaActual(), MesActual(), AnioActual(), Inquilino[Pos].NombreInquilino);
    fprintf(Ticket, "\n\t\t  ocupa la Propiedad de la calle %s del Titular %s.\n\t", Inquilino[Pos].DireccionLocal,Inquilino[Pos].NombreTitular);

                fprintf(Ticket, " La suma total de $%.2f en concepto de Alquiler ", PagoInquilino);



    fprintf(Ticket, "correspondiente al mes %i del anio %i.\n\n", MesActual(), AnioActual());

                fprintf(Ticket, "\t Descripcion: ");
                for(i=0;i<50;i++){
                    fputc(Descripcion[i],Ticket);
                }
                    fputc('|',Ticket);

                fprintf(Ticket, "\n\t              ");
                for(i=50;i<100;i++){
                    fputc(Descripcion[i],Ticket);
                }
                    fputc('|',Ticket);
                fprintf(Ticket, "   %10s   + $%13.2f","ALQUILER",Inquilino[Pos].InteresMonto[TiempoDeContrato/TiempoAjuste]);

                fprintf(Ticket, "\n\t              ");
                for(i=100;i<150;i++){
                    fputc(Descripcion[i],Ticket);
                }
                    fputc('|',Ticket);
                fprintf(Ticket, "   %10s   + $%13.2f","ABL",Inquilino[Pos].PrecioABL[0]);

                fprintf(Ticket, "\n\t              ");
                for(i=150;i<200;i++){
                    fputc(Descripcion[i],Ticket);
                }
                    fputc('|',Ticket);
                fprintf(Ticket, "   %10s   + $%13.2f","AYSA",Inquilino[Pos].PrecioAysa[0]);

                fprintf(Ticket, "\n\t              ");
                for(i=200;i<250;i++){
                    fputc(Descripcion[i],Ticket);
                }
                    fputc('|',Ticket);
                fprintf(Ticket, "   %10s   + $%13.2f","EXPENSAS", Inquilino[Pos].PrecioExpensas[0]);


                fprintf(Ticket, "\n\t              ");
                for(i=250;i<300;i++){
                    fputc(Descripcion[i],Ticket);
                }
                fprintf(Ticket, "    %10s   + $%13.2f","DEUDA", TotalDeuda);

                fprintf(Ticket, "\n\t%-65s   %10s   + $%13.2f"," ", "INTERESES", TotalInteres);

                fprintf(Ticket, "\n\t%-65s   %10s   - $%13.2f"," ", "DESCUENTO",Descuento);


        fprintf(Ticket, "\n\t%-65s   %s"," ", "------------------------------");
        fprintf(Ticket, "\n\t%-65s   %10s   + $%13.2f"," ", "TOTAL",Precio-Descuento);

        if(PagoInquilino!=0) fprintf(Ticket,"\n\t%-65s   %10s   - $%13.2f"," ","PAGADO",PagoInquilino);
        else fprintf(Ticket,"\n\t%-65s   %10s   - $%13.2f"," ","PAGADO",Precio);

        fprintf(Ticket, "\n\t%-62s      %s"," ","------------------------------");
        if((Precio-Descuento-PagoInquilino)>-1) fprintf(Ticket, "\n\t%-62s      %10s   + $%13.2f"," ","A ABONAR",(Precio-Descuento)-PagoInquilino);
        else fprintf(Ticket, "\n\t%-62s      %10s   - $%13.2f"," ","A ABONAR",(Precio-Descuento)-PagoInquilino);





        fclose(Ticket);


        printf("\n\t ¿Se ha Concretado el pago? (S/N) > ");
            LimpiarBuffer();
            fflush(stdin);
            Caract=getch();
            if (Caract=='s' || Caract=='S' || Caract=='1') {
                FILE *BaseDeDatos = fopen("Inquilinos.bin","w+b");
                if (BaseDeDatos!=NULL) {// SI SE ABRE CORRECTAMENTE
                    rewind(BaseDeDatos);
                    for(i=0;i<CantRegs;i++){
                        fwrite(&Inquilino[i],sizeof(InquilinosSTR),1,BaseDeDatos);
                    }
                    fclose(BaseDeDatos);
                    char SystemOpen[50];
                    sprintf(SystemOpen,"xdg-open '%s' > /dev/null", NombreTicket);
                    system(SystemOpen);
                    sprintf(SystemOpen,"notepad '%s'", NombreTicket);
                    system(SystemOpen);
                    printf("\n\n\n\nPresione Enter Para Continuar...\n\n\n");
                    LimpiarBuffer();
                    getch();
                } else {
                    printf(ANSI_COLOR_RED"\n\nERROR! No se Ha podido guardar. Presione una Tecla para Salir al Menu...\n"ANSI_COLOR_RESET);
                    CargarArchivoEnStruct(1);
                    LimpiarBuffer();
                }
            } else {
                printf ("\nSe ha Elegido NO Guardar. Presione una Tecla para Salir al Menu...");
                CargarArchivoEnStruct(1);
                LimpiarBuffer();
            }

    return 0;
}



int Encontrar_PosVector_Por_Numero_Mes (int Comienzo, int Fin) { //FUNCION RECURSIVA ESCRITA ESPECIFICAMENTE PARA MES_COBRAR, DONDE ENCUENTRA LA POSICION EN UN VECTOR DE MES, HABIANDO INGRESADO EL USUARIO EL VALOR DEL MES
    if(Comienzo==0) Comienzo=12;
    if (Comienzo==Fin)
        return 0;
    else
        return 1+Encontrar_PosVector_Por_Numero_Mes(Comienzo-1, Fin);
}


int Mes_Cobrar (long Legajo) {
    int CantRegs, Pos=0, i, MesesContrato, NumeroMes, CantMesesDeuda=0;
    int TiempoAjuste=3*Pot(2,(Inquilino[Pos].TipoAjuste-1));
    if(TiempoAjuste==0) TiempoAjuste=10;
    char Caract=' ';
    float TotalDeuda=0;

        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            fclose(BaseDeDatos);
        } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}

    for (i=0;i<CantRegs;i++) if(Inquilino[i].Legajo==Legajo) Pos=i;

    for(int i=0;i<12;i++){
        TotalDeuda=TotalDeuda+Inquilino[Pos].DeudaAlquiler[i];
    }

    switch(Inquilino[Pos].TipoContrato) {
        case 1 :
            MesesContrato=0.5*12;
        break;
        case 2 :
            MesesContrato=1*12;
        break;
        case 3 :
            MesesContrato=2*12;
        break;
        case 4 :
            MesesContrato=3*12;
        break;
    }

    int TiempoDeContrato=(AnioActual()-Inquilino[Pos].FechaInicio.Anio)*12+(MesActual()-Inquilino[Pos].FechaInicio.Mes);


    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();

    printf("\n%s                    +--------------------------------+", Espacio);
    printf("\n%s                    | %-30s |", Espacio, Inquilino[Pos].DireccionLocal);
    printf("\n%s                    *--------------------------------+", Espacio);
    printf("\n%s                     \\", Espacio);
    printf("\n%s                      *---> %-26s", Espacio, Inquilino[Pos].NombreTitular);
    printf("\n%s                       \\", Espacio);
    printf("\n%s                        *----> N  %-10ld", Espacio, Inquilino[Pos].Legajo);
    printf("\n\n Inquilino: %s", Inquilino[Pos].NombreInquilino);


    printf("\n --------------------------------------------\n Comenzo el Contrato el %-2i/%i. Transcurrieron %i de %i Meses de Contrato\n\n\n", Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio, TiempoDeContrato, MesesContrato);

    // LISTA EL HISTORIAL DEL Alquiler

    int TiempoDesdeUltimoPago=(AnioActual()-Inquilino[Pos].UltimoPagoAlquiler.Anio)*12+(MesActual()-Inquilino[Pos].UltimoPagoAlquiler.Mes);

    if(TiempoDesdeUltimoPago>12) TiempoDesdeUltimoPago=12;

    printf("\n\tAlquiler:\n");

    for (i=0;i<TiempoDesdeUltimoPago;i++) {

        NumeroMes=(12+(MesActual()-i))%12;
        if(NumeroMes==0) NumeroMes=12;
        printf("\n\n\t\t   %-2i -", NumeroMes);

        Nombre_Mes_Segun_Numero(NumeroMes);
        printf(ANSI_COLOR_YELLOW" %-3s "ANSI_COLOR_RESET, Nombre_Mes);

        printf(" [");
        printf(ANSI_COLOR_RED"   IMPAGO   "ANSI_COLOR_RESET);
        if(i==0) printf("]");
        else printf("] Deuda (Sin Interes): $%.2f", Inquilino[Pos].PrecioABL[0]+Inquilino[Pos].PrecioAysa[0]+(Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste])+Inquilino[Pos].PrecioExpensas[0]);
        CantMesesDeuda++;
    }

    int Posicionmeses=i;


    for(i=0;i<(12-TiempoDesdeUltimoPago);i++) {
        if(TiempoDeContrato>=(i+Posicionmeses)) {
            //DIBUJA EL NUMERO DE MES
            NumeroMes=(12+(MesActual()-(Posicionmeses+i)))%12;
            if(NumeroMes==0) NumeroMes=12;
            printf("\n\n\t\t   %2i -", NumeroMes);
            //DIBUJA EL NOMBRE DEL MES
            Nombre_Mes_Segun_Numero(NumeroMes);
            printf(ANSI_COLOR_YELLOW" %-3s "ANSI_COLOR_RESET, Nombre_Mes);

           switch(Inquilino[Pos].HistorialPagoAlquiler[i]){
                case 0 :
                        printf(" [");
                        printf(ANSI_COLOR_RED"  IMPAGO  "ANSI_COLOR_RESET);
                        printf("] Deuda (Sin Interes): $%.2f", Inquilino[Pos].PrecioABL[i]+Inquilino[Pos].PrecioAysa[i]+(Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste])+Inquilino[Pos].PrecioExpensas[i]);
                    CantMesesDeuda++;
                break;
                case 1:
                    printf(" [");
                    printf(ANSI_COLOR_GREEN"  PAGADO  "ANSI_COLOR_RESET);
                    printf("]");
                break;
                case 2 :
                    printf(" [");
                    printf(ANSI_COLOR_YELLOW"  ADEUDA  "ANSI_COLOR_RESET);
                    printf("] Deuda (Sin Interes): $%.2f", Inquilino[Pos].DeudaAlquiler[i]);
                break;
            }
        }
    }

    printf("\n\n");

    if (CantMesesDeuda>0) {
    printf(ANSI_COLOR_CYAN"\n\t\t            ENTER.- Continuar \\\\ ESC.-Salir\n\n"ANSI_COLOR_RESET);

            fflush(stdin);
            Caract=getch();
            while (Caract!='\n') {
                if(Caract==27) { Menu_Cobrar(); return 0; }
                if(Caract==10) { break; }
                if(Caract==13) { break; } //WINDOWS ENTER
                fflush(stdin);
                Caract=getch();
            }

    // LISTA EL HISTORIAL DEL Alquiler
        printf(" Sincronizando la Ultima Fecha de Pago con la Actual...\n\n");
        TiempoDesdeUltimoPago=(AnioActual()-Inquilino[Pos].UltimoPagoAlquiler.Anio)*12+(MesActual()-Inquilino[Pos].UltimoPagoAlquiler.Mes);
        if(TiempoDesdeUltimoPago>12) TiempoDesdeUltimoPago=12;

        // SI QUIERO PAGAR UN MES EN EL QUE TODAVIA NO EXISTE EN EL VECTOR DEL HISTORIAL DE PAGOS YA QUE EXCEDE LA ULTIMA FECHA DE PAGO, MUEVO EL VECTOR PARA QUE PUEDA LLEGAR HASTA AHI (lo corro X posiciones, segun los meses que lleva sin pagar)
            Inquilino[Pos].UltimoPagoAlquiler.Mes=MesActual();
            Inquilino[Pos].UltimoPagoAlquiler.Anio=AnioActual();
            for(i=11-TiempoDesdeUltimoPago;i>=TiempoDesdeUltimoPago;i--){
                Inquilino[Pos].DeudaAlquiler[i]=Inquilino[Pos].DeudaAlquiler[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].HistorialPagoAlquiler[i]=Inquilino[Pos].HistorialPagoAlquiler[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioABL[i]=Inquilino[Pos].PrecioABL[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioExpensas[i]=Inquilino[Pos].PrecioExpensas[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioAysa[i]=Inquilino[Pos].PrecioAysa[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].InteresFormularCarrito[i]=Inquilino[Pos].InteresFormularCarrito[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].TotalDeCobroInquilino[i]=Inquilino[Pos].TotalDeCobroInquilino[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[i]=Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].TotalNetoAlInquilino[i]=Inquilino[Pos].TotalNetoAlInquilino[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[i]=0;

            }
            for (i=0;i<TiempoDesdeUltimoPago;i++){// NO LO PONGO EN 0 PORQUE EL INQUILINO NO PUEDE DEBER INTERESES DEL MISMO MES QUE ESTA PAGANDO
                Inquilino[Pos].PrecioABL[i]=Inquilino[Pos].PrecioABL[TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioExpensas[i]=Inquilino[Pos].PrecioExpensas[TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioAysa[i]=Inquilino[Pos].PrecioAysa[TiempoDesdeUltimoPago];
                Inquilino[Pos].TotalDeCobroInquilino[i]=0;
                Inquilino[Pos].InteresFormularCarrito[i]=0;
                if(i!=0) {
                    Inquilino[Pos].TotalNetoAlInquilino[i]=Inquilino[Pos].PrecioABL[i]+Inquilino[Pos].PrecioAysa[i]+Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste]+Inquilino[Pos].PrecioExpensas[i]/*  + Inquilino[Pos].DeudaAlquiler[i+1]*/;
                    Inquilino[Pos].DeudaAlquiler[i]=Inquilino[Pos].TotalNetoAlInquilino[i];
                } else Inquilino[Pos].TotalNetoAlInquilino[i]=0;
            }

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();

    printf("\n%s                    +--------------------------------+", Espacio);
    printf("\n%s                    | %-30s |", Espacio, Inquilino[Pos].DireccionLocal);
    printf("\n%s                    *--------------------------------+", Espacio);
    printf("\n%s                     \\", Espacio);
    printf("\n%s                      *---> %-26s", Espacio, Inquilino[Pos].NombreTitular);
    printf("\n%s                       \\", Espacio);
    printf("\n%s                        *----> N  %-10ld", Espacio, Inquilino[Pos].Legajo);
    printf("\n\n Inquilino: %s", Inquilino[Pos].NombreInquilino);


    printf("\n --------------------------------------------\n Comenzo el Contrato el %-2i/%i. Transcurrieron %i de %i Meses de Contrato\n\n\n", Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio, TiempoDeContrato, MesesContrato);

            Cobrar_Inquilino(Pos);
            Mes_Cobrar(Legajo);
    } else { getch(); Menu_Principal(); }
    return 0;

}


int Menu_Cobrar () {
    CargarArchivoEnStruct(1);
    int i=0, Eleccion=0;
    int Resultado=0;
    char Caract, PatronBusquedaShow[50];
    for (i=0;i<50;i++){
        PatronBusquedaShow[i]=0;
    }
    i=0;
    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf("\n%s                      +----------+\n", Espacio);
    printf("%s                      | COBRANZA |\n", Espacio);
    printf("%s                      +----------+\n", Espacio);
    printf("\n\n\n\t Por que Campo Desea Buscar?");
    printf(ANSI_COLOR_CYAN"\n\n\t\t 1.- N  Legajo"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 2.- Nombre Titular"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 3.- Nombre Inquilino"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 4.- Direccion"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 5.- Imprimir Todos\n\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t ESC.- Salir al Menu Anterior\n\n"ANSI_COLOR_RESET);

    printf("\n\n\t Ingrese su eleccion > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'5' || Caract<'1') {
                        fflush(stdin);
                        if (Caract==27) { Menu_Principal(); return 0;}
                        Caract=getch();
                    }
                    Eleccion=atoi(&Caract);

    if (Eleccion==5) { Mes_Cobrar(Listar_Inquilinos()); return 0;}

        if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
        Insertar_Fecha_en_Esquina();
        printf("\n%s                      +----------+\n", Espacio);
        printf("%s                      | COBRANZA |\n", Espacio);
        printf("%s                      +----------+\n", Espacio);
    printf(" (Flecha Izquierda vuelve al Menu Anterior)\n");

        printf("\n\n\n\t (NO usar Mayusculas. Solo usar letras de la 'a' a la 'z' sin la 'ñ' o del '0' al '9')");
        printf("\n\n Patron de Busqueda : %s", PatronBusquedaShow);

        do {
            fflush(stdin);
            Caract=getch();
            fflush(stdin);
            switch(Caract) {
                case 65 : //SI ES FLECHA PARA ARRIBA
                    if(Resultado<0) Resultado++;
                break;
                break;
                case 72 : //WINDOWS ARRIBA
                    if(Resultado<0) Resultado++;
                break;
                case 66 : //SI ES FLECHITA PARA ABAJO. Tambien se Activa con [Shift-G]
                    Resultado--;
                break;
                case 80 : //Windows Abajo
                    Resultado--;
                break;
                case 127 : // SI ES LA TECLA ELIMINAR, RETROCEDE
                    if (i>0) { PatronBusquedaShow[i-1]=0; i--; }
                break;
                case 8 : // WINDOWS ELIMINAR
                    if (i>0) { PatronBusquedaShow[i-1]=0; i--; }
                break;
                case 10 :
                    break;
                break;
                case 13 :  //WINDOWS ENTER
                    break;
                break;
                case 67 :
                    break;
                break;
                case 77 : //WINDOWS DERECHA
                    break;
                break;
                case 68 : // Si apreto [Shift-D] se me activa este Menu
                    Menu_Cobrar(); return 0;
                break;
                case 75 : //WINDOWS IZQUIERDA
                    Menu_Cobrar(); return 0;
                break;
                default : //SINO CONTINUA  NORMAL
                    if(Caract=='a' || Caract=='b' || Caract=='c' || Caract=='d' || Caract=='e' || Caract=='f' || Caract=='g' || Caract=='h' || Caract=='i' || Caract=='j' || Caract=='k' || Caract=='l' || Caract=='m' || Caract=='n' || Caract=='o' || Caract=='p' || Caract=='q' || Caract=='r' || Caract=='s' || Caract=='t' || Caract=='u' || Caract=='v' || Caract=='w' || Caract=='x' || Caract=='y' || Caract=='z' || Caract=='1' || Caract=='2' || Caract=='3' || Caract=='4' || Caract=='5' || Caract=='6' || Caract=='7' || Caract=='8' || Caract=='9' || Caract=='0' || Caract==' ') { PatronBusquedaShow[i]=Caract; i++; }
                break;
            }

            if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
            Insertar_Fecha_en_Esquina();
            printf("\n%s                      +----------+\n", Espacio);
            printf("%s                      | COBRANZA |\n", Espacio);
            printf("%s                      +----------+\n", Espacio);
            printf(" (Flecha Izquierda vuelve al Menu Anterior)\n");

                printf("\n\n\n\t (NO usar Mayusculas. Solo usar letras de la 'a' a la 'z' sin la 'ñ' o del '0' al '9')");
                printf("\n\n Patron de Busqueda: %s\n\n", PatronBusquedaShow);
                BusquedaShow(PatronBusquedaShow, Eleccion, Resultado);

        } while (Caract!=10 && Caract!=13 && Caract != 67 && Caract != 77) ; //13 - WINDOWS ENTER //77 -- Windows Derecha


        Mes_Cobrar(SeleccionBusqueda(PatronBusquedaShow,Eleccion, Resultado));
    return 0;
}


int Pagar_Titular (long Legajo) {
    int CantRegs=0, Pos=0 , i, x, MesesContrato;
    float Total=0, TotalDeuda=0, DeudaDelMes=0, Deuda=0;
    char Caract;

        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            fclose(BaseDeDatos);
        } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}
        for (i=0;i<CantRegs;i++) if(Inquilino[i].Legajo==Legajo) Pos=i;
        int TiempoDeContrato=(AnioActual()-Inquilino[Pos].FechaInicio.Anio)*12+(MesActual()-Inquilino[Pos].FechaInicio.Mes);
        switch(Inquilino[Pos].TipoContrato) {
            case 1 :
                MesesContrato=0.5*12;
            break;
            case 2 :
                MesesContrato=1*12;
            break;
            case 3 :
                MesesContrato=2*12;
            break;
            case 4 :
                MesesContrato=3*12;
            break;
        }
        int TiempoDesdeUltimoPago=(AnioActual()-Inquilino[Pos].UltimoPagoAlquiler.Anio)*12+(MesActual()-Inquilino[Pos].UltimoPagoAlquiler.Mes);
        if(TiempoDesdeUltimoPago>12) TiempoDesdeUltimoPago=12;
        int TiempoDesdeUltimoPagoTitular=(AnioActual()-Inquilino[Pos].UltimoPagoTitular.Anio)*12+(MesActual()-Inquilino[Pos].UltimoPagoTitular.Mes);
        if(TiempoDesdeUltimoPagoTitular>12) TiempoDesdeUltimoPagoTitular=12;
        int TiempoAjuste=3*Pot(2,(Inquilino[Pos].TipoAjuste-1));
        if(TiempoAjuste==0) TiempoAjuste=10;
        for(int i=1;i<12;i++){ // NO LO PONGO EN 0 PORQUE EL INQUILINO NO PUEDE DEBER INTERESES DEL MISMO MES QUE ESTA PAGANDO
            if(Inquilino[Pos].DeudaAlquiler[i]>0) {
                TotalDeuda=TotalDeuda+Inquilino[Pos].DeudaAlquiler[i];
            }
        }

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();

    printf("\n%s                    +--------------------------------+", Espacio);
    printf("\n%s                    | %-30s |", Espacio, Inquilino[Pos].DireccionLocal);
    printf("\n%s                    *--------------------------------+", Espacio);
    printf("\n%s                     \\", Espacio);
    printf("\n%s                      *---> %-26s", Espacio, Inquilino[Pos].NombreTitular);
    printf("\n%s                       \\", Espacio);
    printf("\n%s                        *----> N  %-10ld", Espacio, Inquilino[Pos].Legajo);
    printf("\n\n Inquilino: %s", Inquilino[Pos].NombreInquilino);


    printf("\n --------------------------------------------\n Comenzo el Contrato el %-2i/%i. Transcurrieron %i de %i Meses de Contrato", Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio, TiempoDeContrato, MesesContrato);

    printf("\n\n El Propietario tiene un total a cobrar de ");


                // MUEVE EL VECTOR DEL INQUILINO PARA ESTAR MAS COMODO
            for(i=11-TiempoDesdeUltimoPago;i>=TiempoDesdeUltimoPago;i--){
                Inquilino[Pos].DeudaAlquiler[i]=Inquilino[Pos].DeudaAlquiler[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].HistorialPagoAlquiler[i]=Inquilino[Pos].HistorialPagoAlquiler[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioABL[i]=Inquilino[Pos].PrecioABL[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioExpensas[i]=Inquilino[Pos].PrecioExpensas[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioAysa[i]=Inquilino[Pos].PrecioAysa[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].InteresFormularCarrito[i]=Inquilino[Pos].InteresFormularCarrito[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].TotalDeCobroInquilino[i]=Inquilino[Pos].TotalDeCobroInquilino[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[i]=Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[i-TiempoDesdeUltimoPago];
                Inquilino[Pos].TotalNetoAlInquilino[i]=Inquilino[Pos].TotalNetoAlInquilino[i-TiempoDesdeUltimoPago];
            }
            for (i=0;i<TiempoDesdeUltimoPago;i++){
                Inquilino[Pos].PrecioABL[i]=Inquilino[Pos].PrecioABL[TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioExpensas[i]=Inquilino[Pos].PrecioExpensas[TiempoDesdeUltimoPago];
                Inquilino[Pos].PrecioAysa[i]=Inquilino[Pos].PrecioAysa[TiempoDesdeUltimoPago];
                Inquilino[Pos].TotalDeCobroInquilino[i]=0;
                Inquilino[Pos].HistorialPagoAlquiler[i]=0;
                Inquilino[Pos].InteresFormularCarrito[i]=0;
                Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[i]=0;
                if(i!=0) {
                    Inquilino[Pos].TotalNetoAlInquilino[i]=Inquilino[Pos].PrecioABL[i]+Inquilino[Pos].PrecioAysa[i]+Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste]+Inquilino[Pos].PrecioExpensas[i] /* + Inquilino[Pos].DeudaAlquiler[i+1]*/;
                    Inquilino[Pos].DeudaAlquiler[i]=Inquilino[Pos].TotalNetoAlInquilino[i];
                } else Inquilino[Pos].TotalNetoAlInquilino[i]=0;
            }
                //AHORA MUEVE EL VECTOR DEL TITULAR
            for(i=11;i>=TiempoDesdeUltimoPagoTitular;i--){ // MUEVE EL VECTOR
                Inquilino[Pos].HistorialPagoTitular[i]=Inquilino[Pos].HistorialPagoTitular[i-TiempoDesdeUltimoPagoTitular];
            }

            for(i=0;i<TiempoDesdeUltimoPagoTitular;i++){ // MUEVE EL VECTOR
                Inquilino[Pos].HistorialPagoTitular[i]=0;
            }


int CantMesesDeuda=0,BoolDeuda=0;
        // BUSCA PARA VER SI HAY DEUDA
    for(int i=0;i<12;i++){ // NO LO PONGO EN 0 PORQUE EL INQUILINO NO PUEDE DEBER INTERESES DEL MISMO MES QUE ESTA PAGANDO
            if(Inquilino[Pos].HistorialPagoAlquiler[i]==2) BoolDeuda++;
    }


// REALIZA LOS CALCULOS DEL PRECIO TOTAL
    for(i=0;i<TiempoDesdeUltimoPagoTitular;i++) {
        if(Inquilino[Pos].HistorialPagoAlquiler[i]==1 || Inquilino[Pos].HistorialPagoAlquiler[i]==2) { Total=Total+Inquilino[Pos].TotalDeCobroInquilino[i] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100); CantMesesDeuda++; }
    }
    for(i=TiempoDesdeUltimoPagoTitular;i<12;i++) {
        if((Inquilino[Pos].HistorialPagoAlquiler[i]==1 || Inquilino[Pos].HistorialPagoAlquiler[i]==2) && Inquilino[Pos].HistorialPagoAlquiler[i]==0) { Total=Total+Inquilino[Pos].TotalDeCobroInquilino[i] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100); CantMesesDeuda++; }
    }
    printf(ANSI_COLOR_GREEN"$%.2f\n\n"ANSI_COLOR_RESET, Total );

// DIBUJA LOS MESES PAGADOS, IMPAGOS O INCOMPLETOS
    printf("\tPagos:\n");

    for (i=0;i<TiempoDesdeUltimoPagoTitular;i++) {
        int NumeroMes=(12+(MesActual()-i))%12;
        if(NumeroMes==0) NumeroMes=12;
        printf("\n\n\t\t   %-2i -", NumeroMes);

        Nombre_Mes_Segun_Numero(NumeroMes);
        printf(ANSI_COLOR_YELLOW" %-3s "ANSI_COLOR_RESET, Nombre_Mes);
        /* printf("ep %i\n", Inquilino[Pos].HistorialPagoAlquiler[i]); */
        if (Inquilino[Pos].HistorialPagoAlquiler[i]==2) {
            printf(" [");
            printf(ANSI_COLOR_YELLOW" INCOMPLETO "ANSI_COLOR_RESET);
            printf("] ");
            printf(ANSI_COLOR_YELLOW"--> El Inquilino Falta Pagar $%.2f. " ANSI_COLOR_RESET, Inquilino[Pos].DeudaAlquiler[i]); //SE ACTIVA SI EL INQUILINO DEBE Y QUE SI NO FUE COBRADO ANTERIORMENTE
                    //CONSIGUE LO QUE PAGO DEL MES
                    DeudaDelMes=Total;
                    for(x=0;x<12;x++) {
                        if((x-1)<TiempoDeContrato && Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[x]!=1 && Inquilino[Pos].HistorialPagoAlquiler[x]==1 && Inquilino[Pos].HistorialPagoTitular[x]==0) DeudaDelMes=DeudaDelMes-Inquilino[Pos].PrecioABL[x]-Inquilino[Pos].PrecioAysa[x]-Inquilino[Pos].PrecioExpensas[x]-Inquilino[Pos].InteresMonto[(TiempoDeContrato-x)/TiempoAjuste]-Inquilino[Pos].InteresFormularCarrito[x];

                    }
            printf(ANSI_COLOR_GREEN"Disponible $%.2f" ANSI_COLOR_RESET, Inquilino[Pos].TotalDeCobroInquilino[i] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100)); //SE ACTIVA SI EL INQUILINO DEBE Y QUE SI NO FUE COBRADO ANTERIORMENTE

        } else {
            printf(" [");
            printf(ANSI_COLOR_RED"   IMPAGO   "ANSI_COLOR_RESET);
            printf("] ");
                if(Inquilino[Pos].HistorialPagoAlquiler[i]==0 || i<0) printf(ANSI_COLOR_RED"--> El Inquilino NO Pago el Mes"ANSI_COLOR_RESET);
                if(Inquilino[Pos].HistorialPagoAlquiler[i]==1) {
                    printf(ANSI_COLOR_GREEN"--> Disponible $%.2f" ANSI_COLOR_RESET, Inquilino[Pos].TotalDeCobroInquilino[i] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100));
                }
        }
    }

    for (i=TiempoDesdeUltimoPagoTitular;i<12;i++) {
        if((i-1)<TiempoDeContrato) {
            int NumeroMes=(12+(MesActual()-i))%12;
            if(NumeroMes==0) NumeroMes=12;
            printf("\n\n\t\t   %-2i -", NumeroMes);

            Nombre_Mes_Segun_Numero(NumeroMes);
            printf(ANSI_COLOR_YELLOW" %-3s "ANSI_COLOR_RESET, Nombre_Mes);

            if(Inquilino[Pos].HistorialPagoTitular[i]==1) {
                printf(" [");
                printf(ANSI_COLOR_GREEN"   PAGADO   "ANSI_COLOR_RESET);
                printf("] ");
            }

            if(Inquilino[Pos].HistorialPagoTitular[i]==2) {
                printf(" [");
                printf(ANSI_COLOR_GREEN"   PAGADO   "ANSI_COLOR_RESET);
                printf("] ");
                if(Inquilino[Pos].HistorialPagoAlquiler[i]==2)printf(ANSI_COLOR_YELLOW"--> El Inquilino Falta Pagar $%.2f. " ANSI_COLOR_RESET, Inquilino[Pos].DeudaAlquiler[i]); //SE ACTIVA SI EL INQUILINO DEBE Y QUE SI NO FUE COBRADO ANTERIORMENTE
                if(Inquilino[Pos].HistorialPagoAlquiler[i]==1)printf(ANSI_COLOR_GREEN"--> El Inquilino Faltaba Pagar. Ya Pagado" ANSI_COLOR_RESET); //SE ACTIVA SI EL INQUILINO DEBÍA Y YA FUE COBRADO
            }

            if(Inquilino[Pos].HistorialPagoTitular[i]==0) {
                printf(" [");
                printf(ANSI_COLOR_RED"   IMPAGO   "ANSI_COLOR_RESET);
                printf("] ");
                if(Inquilino[Pos].HistorialPagoAlquiler[i]==0 || i<0) printf(ANSI_COLOR_RED"--> El Inquilino NO Pago el Mes"ANSI_COLOR_RESET);
                if(Inquilino[Pos].HistorialPagoAlquiler[i]==2) {
                    printf(ANSI_COLOR_YELLOW"--> El Inquilino Falta Pagar $%.2f. " ANSI_COLOR_RESET, Inquilino[Pos].DeudaAlquiler[i]); //SE ACTIVA SI EL INQUILINO DEBE Y QUE SI NO FUE COBRADO ANTERIORMENTE

                        DeudaDelMes=Inquilino[Pos].TotalDeCobroInquilino[i]-Inquilino[Pos].DeudaAlquiler[i];
                    for(x=0;x<12;x++) {
                        if(Inquilino[Pos].AQueMesesCorrespondeLaUltimaDeuda[x]==1) DeudaDelMes=DeudaDelMes+Inquilino[Pos].TotalDeCobroInquilino[x]-Inquilino[Pos].PrecioABL[x]-Inquilino[Pos].PrecioAysa[x]-Inquilino[Pos].PrecioExpensas[x]-Inquilino[Pos].InteresMonto[(TiempoDeContrato-x)/TiempoAjuste];
                    }
                    printf(ANSI_COLOR_GREEN"Disponible %.2f" ANSI_COLOR_RESET, DeudaDelMes - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100)); //SE ACTIVA SI EL INQUILINO DEBE Y QUE SI NO FUE COBRADO ANTERIORMENTE
                }

                if(Inquilino[Pos].HistorialPagoAlquiler[i]==1) printf(ANSI_COLOR_GREEN"--> Disponible $%.2f" ANSI_COLOR_RESET, Inquilino[Pos].TotalDeCobroInquilino[i]+Inquilino[Pos].PrecioABL[i]+Inquilino[Pos].PrecioAysa[i]+Inquilino[Pos].PrecioExpensas[i]+Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100)); //INQUILINO PAGO EL MES PERO EL PROPIETARIO TODAVIA NO LO COBRO
            }

        }
    }


    printf("\n\n");

    if (Total>0) {
        printf(ANSI_COLOR_CYAN"\n\t\t            ENTER.- Continuar \\\\ ESC.-Salir\n\n"ANSI_COLOR_RESET);
        if(getch()==27) { CargarArchivoEnStruct(1); Menu_Pagar(); return 0; }

        //GUARDA EL TICKET EN LA CARPETA DE PAGOS
            char NombreTicket[50]= "  ";
                    sprintf(NombreTicket , "Pagos/%ld-%i-%-4i" , Inquilino[Pos].Legajo , MesActual() , AnioActual() );
            FILE *Ticket = fopen(NombreTicket,"w");


            fprintf(Ticket, "\n\n\n\t\t\t\t\t\t\t\t\tRECIBO DE PAGO");
            fprintf(Ticket, "\n\n\t Recibió el dia de hoy %i/%i/%i del Sr./Sra. Titular %s de la Propiedad", DiaActual(), MesActual(), AnioActual(), Inquilino[Pos].NombreTitular);
            fprintf(Ticket, "\n\t  de la calle %s la suma total de $%.2f en concepto de Alquiler\n\t  ", Inquilino[Pos].DireccionLocal, Total);
            fprintf(Ticket, "correspondiente al/los mes/es de ");
                    //IMPRIME LOS NOMBRES DE LOS MESES Y SU LISTADO
                    for(i=CantMesesDeuda-1;i>=TiempoDesdeUltimoPago;i--) {
                    Nombre_Mes_Completo_Segun_Numero((MesActual()-i)%12);
                    if(i==TiempoDesdeUltimoPago+1) fprintf(Ticket,  "%s y ",Nombre_Mes);
                    else if(i==TiempoDesdeUltimoPago) fprintf(Ticket,  "%s.",Nombre_Mes);
                    else fprintf(Ticket,  "%s, ",Nombre_Mes);
                    }

                    //IMPRIME EL LISTADO DEL TOTAL DE LOS MESES
                    for(i=CantMesesDeuda+TiempoDesdeUltimoPago-1;i>=TiempoDesdeUltimoPago;i--) {
                        Nombre_Mes_Completo_Segun_Numero((MesActual()-i)%12);
                        fprintf(Ticket,  "\n\n\t\t\t%33s%-11s","Mes ",Nombre_Mes);
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f","Alquiler: ",Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste]);
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f","ABL: ",Inquilino[Pos].PrecioABL[i]);
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f","AYSA: ",Inquilino[Pos].PrecioAysa[i]);
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f","Expensas: ",Inquilino[Pos].PrecioExpensas[i]);

                        Deuda=0;
                        for(x=11;x>i;x--) {
                            if((x-1)<TiempoDeContrato){
                                Deuda=Deuda+(Inquilino[Pos].TotalNetoAlInquilino[x] - Inquilino[Pos].TotalDeCobroInquilino[x]);
                            }
                        }

                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f","Deuda: ",fabs(Deuda));
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f","Intereses: ",Inquilino[Pos].InteresFormularCarrito[i]);
                        if(Deuda > 1 && Inquilino[Pos].TotalNetoAlInquilino[i]==(Inquilino[Pos].PrecioABL[i]+Inquilino[Pos].PrecioAysa[i]+Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste]+Inquilino[Pos].PrecioExpensas[i])) fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   -$%13.2f","Impago: ",Inquilino[Pos].TotalNetoAlInquilino[i] + Deuda - Inquilino[Pos].TotalDeCobroInquilino[i]); // SI HAY DEUDA Y EL TOTAL A COBRAR ES LA SUMA DE TODOS LOS SERVICIOS SIN LA DEUDA (COSA QUE NO PUEDE PASAR, SOLO CUANDO UNO NO PAGA EL MES, SUMAR LA DEUDA)
                        else fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   -$%13.2f","Impago: ",Inquilino[Pos].TotalNetoAlInquilino[i] -Inquilino[Pos].TotalDeCobroInquilino[i]);
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   -$%13.2f","Comsion: ", Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100);
                        fprintf(Ticket, "\n\t%-65s   %s"," ", "------------------------------");
                        fprintf(Ticket, "\n\t\t\t\t\t\t\t%32s   +$%13.2f\n\n","PAGADO: ", Inquilino[Pos].TotalDeCobroInquilino[i] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100));
                    }
            fclose(Ticket);



        //RECARGA EL APARTADO SUPERIOR Y IMPRIME EN COLORES LO QUE TIENE QUE PAGAR LA INMOBILIARIA
            Deuda=0;

            if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
            Insertar_Fecha_en_Esquina();

            printf("\n%s                    +--------------------------------+", Espacio);
            printf("\n%s                    | %-30s |", Espacio, Inquilino[Pos].DireccionLocal);
            printf("\n%s                    *--------------------------------+", Espacio);
            printf("\n%s                     \\", Espacio);
            printf("\n%s                      *---> %-26s", Espacio, Inquilino[Pos].NombreTitular);
            printf("\n%s                       \\", Espacio);
            printf("\n%s                        *----> N  %-10ld", Espacio, Inquilino[Pos].Legajo);
            printf("\n\n Inquilino: %s", Inquilino[Pos].NombreInquilino);


            printf("\n --------------------------------------------\n Comenzo el Contrato el %-2i/%i. Transcurrieron %i de %i Meses de Contrato\n\n", Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio, TiempoDeContrato, MesesContrato);
                //IMPRIME EL LISTADO DEL TOTAL DE LOS MESES
                for(i=CantMesesDeuda+TiempoDesdeUltimoPago-1;i>=TiempoDesdeUltimoPago;i--) {
                    Nombre_Mes_Completo_Segun_Numero((MesActual()-i)%12);
                    printf( ANSI_COLOR_YELLOW"\n\n\t\t\t%33s%-11s","Mes "ANSI_COLOR_RESET,Nombre_Mes);
                    printf("\n\n\t\t\t\t\t%32s   +$%13.2f","Alquiler: ",Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste]);
                    printf("\n\t\t\t\t\t%32s   +$%13.2f","ABL: ",Inquilino[Pos].PrecioABL[i]);
                    printf("\n\t\t\t\t\t%32s   +$%13.2f","AYSA: ",Inquilino[Pos].PrecioAysa[i]);
                    printf("\n\t\t\t\t\t%32s   +$%13.2f","Expensas: ",Inquilino[Pos].PrecioExpensas[i]);

                    Deuda=0;
                    for(x=11;x>i;x--) {
                        if((x-1)<TiempoDeContrato){
                            Deuda=Deuda+(Inquilino[Pos].TotalNetoAlInquilino[x] - Inquilino[Pos].TotalDeCobroInquilino[x]);
                        }
                    }

                    printf("\n\t\t\t\t\t%32s   +$%13.2f","Deuda: ",fabs(Deuda));
                    printf("\n\t\t\t\t\t%32s   +$%13.2f","Intereses: ",Inquilino[Pos].InteresFormularCarrito[i]);
                    if(Deuda > 1 && Inquilino[Pos].TotalNetoAlInquilino[i]==(Inquilino[Pos].PrecioABL[i]+Inquilino[Pos].PrecioAysa[i]+Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste]+Inquilino[Pos].PrecioExpensas[i])) printf(ANSI_COLOR_RED"\n\t\t\t\t\t%32s   -$%13.2f"ANSI_COLOR_RESET,"Impago: ",Inquilino[Pos].TotalNetoAlInquilino[i] + Deuda - Inquilino[Pos].TotalDeCobroInquilino[i]); //Inquilino[Pos].DeudaAlquiler[i] // SI HAY DEUDA Y EL TOTAL A COBRAR ES LA SUMA DE TODOS LOS SERVICIOS SIN LA DEUDA (COSA QUE NO PUEDE PASAR, SOLO CUANDO UNO NO PAGA EL MES, SUMAR LA DEUDA)
                    else printf(ANSI_COLOR_RED"\n\t\t\t\t\t%32s   -$%13.2f"ANSI_COLOR_RESET,"Impago: ",Inquilino[Pos].TotalNetoAlInquilino[i] -Inquilino[Pos].TotalDeCobroInquilino[i]);
                    printf("\n\t\t\t\t\t%32s   -$%13.2f","Comsion: ", Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100);
                    printf("\n\t\t\t\t\t\t\t   %-35s","---------------------------------");
                    printf(ANSI_COLOR_GREEN"\n\t\t\t\t\t%32s   +$%13.2f\n\n"ANSI_COLOR_RESET,"PAGADO: ", Inquilino[Pos].TotalDeCobroInquilino[i] - (Inquilino[Pos].InteresMonto[(TiempoDeContrato-i)/TiempoAjuste] * Inquilino[Pos].ComisionDeLaInmobiliaria / 100));
                }

                    printf(ANSI_COLOR_GREEN"\n\n\t\t\tTOTAL A PAGAR AL TITULAR -->   +$%13.2f\n\n"ANSI_COLOR_RESET,Total);


        CargarArchivoEnStruct(1);

        for(i=11;i>=TiempoDesdeUltimoPagoTitular;i--){ // MUEVE EL VECTOR
            Inquilino[Pos].HistorialPagoTitular[i]=Inquilino[Pos].HistorialPagoTitular[i-TiempoDesdeUltimoPagoTitular];
        }
        for (i=0;i<TiempoDesdeUltimoPagoTitular;i++) {
            Inquilino[Pos].HistorialPagoTitular[i]=0;
        }
        for(i=0;i<12;i++){ // CORRIGE EL VECTOR
            if(TiempoDesdeUltimoPago<=i && i<TiempoDeContrato) {
                if (Inquilino[Pos].HistorialPagoAlquiler[i-TiempoDesdeUltimoPago]==1 && Inquilino[Pos].HistorialPagoTitular[i]==0 ) Inquilino[Pos].HistorialPagoTitular[i]=1;
                if (Inquilino[Pos].HistorialPagoAlquiler[i-TiempoDesdeUltimoPago]==2 && Inquilino[Pos].HistorialPagoTitular[i]==0 ) Inquilino[Pos].HistorialPagoTitular[i]=2;
            }
            for(x=0;x<12;x++) Inquilino[Pos].InteresFormularCarrito[i]=0;
        }
        Inquilino[Pos].UltimoPagoTitular.Mes=MesActual();
        Inquilino[Pos].UltimoPagoTitular.Anio=AnioActual();


    printf(ANSI_COLOR_CYAN"\n\n        ¿Ya se ha Concretado el Pago? (S/N) > "ANSI_COLOR_RESET);
        fflush(stdin);
        Caract=getch();

        if (Caract=='s' || Caract=='S' || Caract=='1') {
            FILE *BaseDeDatos = fopen("Inquilinos.bin","w+b");
            if (BaseDeDatos!=NULL) {// SI SE ABRE CORRECTAMENTE
                    rewind(BaseDeDatos);
                    for(i=0;i<CantRegs;i++){
                        fwrite(&Inquilino[i],sizeof(InquilinosSTR),1,BaseDeDatos);
                    }
                char SystemOpen[50];
                sprintf(SystemOpen,"xdg-open '%s' > /dev/null", NombreTicket);
                system(SystemOpen);
                sprintf(SystemOpen,"notepad '%s'", NombreTicket);
                system(SystemOpen);
                    printf("\n\n\n\nPresione Enter para Continuar...\n\n\n");
                    LimpiarBuffer();
                    getch();
                CargarArchivoEnStruct(1);
            } else {
                printf(ANSI_COLOR_RED"\n\nERROR! No se Ha podido guardar. Presione una Tecla para Salir al Menu...\n"ANSI_COLOR_RESET);
                getch();
                CargarArchivoEnStruct(1);
            }
            fclose(BaseDeDatos);
        } else {
            printf ("\nSe ha Elegido NO Guardar. Presione una Tecla para Salir al Menu...");
            getch();
            CargarArchivoEnStruct(1);
        }
        Menu_Principal();
    } else {getch(); CargarArchivoEnStruct(1); Menu_Pagar(); return 0;}


    Menu_Pagar();

    return 0;

}

int Buscar_Comprobante (long Legajo) {
    int CantRegs=0, Pos=0 , i, MesesContrato;
    char Caract;
    char NombreTicket[100]= "  ";

        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            fclose(BaseDeDatos);
        } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}
        for (i=0;i<CantRegs;i++) if(Inquilino[i].Legajo==Legajo) Pos=i;
        int TiempoDeContrato=(AnioActual()-Inquilino[Pos].FechaInicio.Anio)*12+(MesActual()-Inquilino[Pos].FechaInicio.Mes);
        switch(Inquilino[Pos].TipoContrato) {
            case 1 :
                MesesContrato=0.5*12;
            break;
            case 2 :
                MesesContrato=1*12;
            break;
            case 3 :
                MesesContrato=2*12;
            break;
            case 4 :
                MesesContrato=3*12;
            break;
        }
        if (TiempoDeContrato>MesesContrato) TiempoDeContrato=MesesContrato;

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();

    printf("\n%s                    +--------------------------------+", Espacio);
    printf("\n%s                    | %-30s |", Espacio, Inquilino[Pos].DireccionLocal);
    printf("\n%s                    *--------------------------------+", Espacio);
    printf("\n%s                     \\", Espacio);
    printf("\n%s                      *---> %-26s", Espacio, Inquilino[Pos].NombreTitular);
    printf("\n%s                       \\", Espacio);
    printf("\n%s                        *----> N  %-10ld", Espacio, Inquilino[Pos].Legajo);
    printf("\n\n Inquilino: %s", Inquilino[Pos].NombreInquilino);


    printf("\n --------------------------------------------\n Comenzo el Contrato el %-2i/%i. Transcurrieron %i de %i Meses de Contrato", Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio, TiempoDeContrato, MesesContrato);


    FILE *Ticket;
    int AnioABuscar=AnioActual(); //LO TENGO QUE DECLARAR AFUERA ASI CUANDO SE PASA UN AÑO PARA ATRAS QUEDA ESE MISMO VALOR DE AÑO EN EL for SIGUIENTE


    printf("\n\n\t\t\t\t       Al INQULINO\t\t\tAl TITULAR");


        for(i=0;i<TiempoDeContrato;i++){
            int MesABuscar=(abs(MesActual()-i)); //fabs es para float, mientras que abs es para enteros
            MesABuscar=MesABuscar%12;

            if(MesABuscar==0) { MesABuscar=12; AnioABuscar=AnioABuscar-1; }
            Nombre_Mes_Segun_Numero(MesABuscar);

            sprintf(NombreTicket , "Cobros/%ld-%i-%-4i" , Inquilino[Pos].Legajo , MesABuscar , AnioABuscar );
            Ticket = fopen(NombreTicket,"r");

                printf(ANSI_COLOR_YELLOW"\n\n\tMes %2i - %-3s de %i\t"ANSI_COLOR_RESET, MesABuscar, Nombre_Mes, AnioABuscar);
                printf("[");
            if(Ticket != NULL) {
                fclose(Ticket);
                printf(ANSI_COLOR_GREEN"       ENCONTRADO       "ANSI_COLOR_RESET);
            } else {
                printf(ANSI_COLOR_RED"   PERDIDO / NO PAGADO  "ANSI_COLOR_RESET);
            }
                printf("]");

            sprintf(NombreTicket , "Pagos/%ld-%i-%-4i" , Inquilino[Pos].Legajo , MesABuscar , AnioABuscar );
            Ticket = fopen(NombreTicket,"r");
                printf("\t[");
            if(Ticket != NULL) {
                fclose(Ticket);
                printf(ANSI_COLOR_GREEN"       ENCONTRADO       "ANSI_COLOR_RESET);
            } else {
                printf(ANSI_COLOR_RED"   PERDIDO / NO PAGADO  "ANSI_COLOR_RESET);
            }
                printf("]");

        }


        if(i>0){ //SI HAY RESULTADOS...
            printf("\n\n\t ¿Referente a que Persona desea Abrir el Comprobante?");
            printf(ANSI_COLOR_CYAN"\n\t\t 1.- Inquilino \\\\ 2.- Titular \\\\ ESC.- Salir\n"ANSI_COLOR_RESET);
                fflush(stdin);
                Caract=getch();
                while (Caract=='\n' || Caract>'2' || Caract<'1') {
                    fflush(stdin);
                    if (Caract==27) { Menu_Pagar(); return 0;}
                    Caract=getch();
                }
                int Persona=atoi(&Caract);

            printf("\n\n\t ¿Que Mes Desea Abrir? > ");
                int MesABuscar;
                scanf("%i", &MesABuscar);
                while(MesABuscar>12 || MesABuscar<1){
                    printf("\nERROR! No existe el Mes. Ingrese De Vuelta > ");
                    scanf("%i", &MesABuscar);
                }

            printf("\n\n\t ¿Que Anio Desea Abrir? > ");
                scanf("%i", &AnioABuscar);
                while(AnioABuscar<2000 || AnioABuscar>2500){
                    printf("\nERROR! No existe el Anio. Ingrese De Vuelta > ");
                    scanf("%i", &AnioABuscar);
                }

            if(Persona==1) sprintf(NombreTicket , "Cobros/%ld-%i-%-4i" , Inquilino[Pos].Legajo , MesABuscar , AnioABuscar );
            else sprintf(NombreTicket , "Pagos/%ld-%i-%-4i" , Inquilino[Pos].Legajo , MesABuscar , AnioABuscar );
            Ticket = fopen(NombreTicket,"r");

            if(Ticket != NULL) {
                    char SystemOpen[50];
                    sprintf(SystemOpen,"xdg-open '%s' > /dev/null", NombreTicket);
                    system(SystemOpen);
                    sprintf(SystemOpen,"notepad '%s'", NombreTicket);
                    system(SystemOpen);
                    printf("\n\n\n\nPresione Enter Para Continuar...\n\n\n");
                    LimpiarBuffer();
                    getch();
                Buscar_Comprobante(Inquilino[Pos].Legajo);
                return 0;
            } else {
                printf(ANSI_COLOR_RED"   ERROR! No se Pudo Abrir. Presione una Tecla para Intentar de Vuelta  "ANSI_COLOR_RESET);
                LimpiarBuffer();
                getch();
                Buscar_Comprobante(Inquilino[Pos].Legajo);
                return 0;
            }
        }

    Menu_Principal();
    return 0;
}


int Menu_Pagar () {
    CargarArchivoEnStruct(1);
    int i=0, CampoBusqueda=0, PagarOComprobante=0;
    int Resultado=0;
    char Caract, PatronBusquedaShow[50];
    for (i=0;i<50;i++){
        PatronBusquedaShow[i]=0;
    }
    i=0; // Vuelve a Declararlo 0 porque el for de arriba la ensució





    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf("\n%s                      +-------+\n", Espacio);
    printf("%s                      | PAGOS |\n", Espacio);
    printf("%s                      +-------+\n", Espacio);
    printf("\n\n\n\t Que esta Buscando?");
    printf(ANSI_COLOR_CYAN"\n\n\t\t 1.- Reliza un Pago"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 2.- Buscar un Comprobante"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t ESC.- Salir al Menu Anterior\n\n"ANSI_COLOR_RESET);
    printf("\n\n\t Ingrese su eleccion > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'2' || Caract<'1') {
                        fflush(stdin);
                        if (Caract==27) { Menu_Principal(); return 0;}
                        Caract=getch();
                    }
                    PagarOComprobante=atoi(&Caract);

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf("\n%s                      +-------+\n", Espacio);
    printf("%s                      | PAGOS |\n", Espacio);
    printf("%s                      +-------+\n", Espacio);
    printf("\n\n\n\t Por que Campo Desea Buscar?");
    printf(ANSI_COLOR_CYAN"\n\n\t\t 1.- N  Legajo"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 2.- Nombre Titular"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 3.- Nombre Inquilino"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 4.- Direccion"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 5.- Imprimir Todos\n\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t ESC.- Salir al Menu Anterior\n\n"ANSI_COLOR_RESET);

    printf("\n\n\t Ingrese su eleccion > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'5' || Caract<'1') {
                        fflush(stdin);
                        if (Caract==27) { Menu_Principal(); return 0;}
                        Caract=getch();
                    }
                    CampoBusqueda=atoi(&Caract);

    if (CampoBusqueda==5) {
        if(PagarOComprobante==1) Pagar_Titular(Listar_Inquilinos());
        if(PagarOComprobante==2) Buscar_Comprobante(Listar_Inquilinos());
        return 0;}

        if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
        Insertar_Fecha_en_Esquina();
        printf("\n%s                      +-------+\n", Espacio);
        printf("%s                      | PAGOS |\n", Espacio);
        printf("%s                      +-------+\n", Espacio);
        printf(" (Flecha Izquierda vuelve al Menu Anterior)\n");

        printf("\n\n\n\t (NO usar Mayusculas. Solo usar letras de la 'a' a la 'z' o del '0' al '9')");
        printf("\n\n Patron de Busqueda : %s", PatronBusquedaShow);

        do {
            fflush(stdin);
            Caract=getch();
            fflush(stdin);
            printf("%ld", strlen(PatronBusquedaShow));
            switch(Caract) {
                case 65 : //SI ES FLECHA PARA ARRIBA
                    if(Resultado<0) Resultado++;
                break;
                break;
                case 72 : //WINDOWS ARRIBA
                    if(Resultado<0) Resultado++;
                break;
                case 66 : //SI ES FLECHITA PARA ABAJO. Tambien se Activa con [Shift-G]
                    Resultado--;
                break;
                case 80 : //Windows Abajo
                    Resultado--;
                break;
                case 127 : // SI ES LA TECLA ELIMINAR, RETROCEDE
                    if (i>0) { PatronBusquedaShow[i-1]=0; i--; }
                break;
                case 8 : // WINDOWS ELIMINAR
                    if (i>0) { PatronBusquedaShow[i-1]=0; i--; }
                break;
                case 10 :
                    break;
                break;
                case 13 :  //WINDOWS ENTER
                    break;
                break;
                case 67 :
                    break;
                break;
                case 77 : //WINDOWS DERECHA
                    break;
                break;
                case 68 : // Si apreto [Shift-D] se me activa este Menu
                    Menu_Cobrar(); return 0;
                break;
                case 75 : //WINDOWS IZQUIERDA
                    Menu_Cobrar(); return 0;
                break;
                default : //SINO CONTINUA  NORMAL
                    if(Caract=='a' || Caract=='b' || Caract=='c' || Caract=='d' || Caract=='e' || Caract=='f' || Caract=='g' || Caract=='h' || Caract=='i' || Caract=='j' || Caract=='k' || Caract=='l' || Caract=='m' || Caract=='n' /*  || Caract=='ñ'*/ || Caract=='o' || Caract=='p' || Caract=='q' || Caract=='r' || Caract=='s' || Caract=='t' || Caract=='u' || Caract=='v' || Caract=='w' || Caract=='x' || Caract=='y' || Caract=='z' || Caract=='1' || Caract=='2' || Caract=='3' || Caract=='4' || Caract=='5' || Caract=='6' || Caract=='7' || Caract=='8' || Caract=='9' || Caract=='0' || Caract==' ') { PatronBusquedaShow[i]=Caract; i++; }
                break;
            }

            if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
            Insertar_Fecha_en_Esquina();
                printf("\n%s                      +-------+\n", Espacio);
                printf("%s                      | PAGOS |\n", Espacio);
                printf("%s                      +-------+\n", Espacio);

                printf("\n\n\n\t (NO usar Mayusculas. Solo usar letras de la 'a' a la 'z' o del '0' al '9')");
                printf("\n\n Patron de Busqueda: %s\n\n", PatronBusquedaShow);
                BusquedaShow(PatronBusquedaShow, CampoBusqueda, Resultado);

        } while (Caract!=10 && Caract!=13 && Caract != 67 && Caract != 77) ; //13 - WINDOWS ENTER //77 -- Windows Derecha


        if(PagarOComprobante==1) Pagar_Titular(SeleccionBusqueda(PatronBusquedaShow,CampoBusqueda, Resultado));
        if(PagarOComprobante==2) Buscar_Comprobante(SeleccionBusqueda(PatronBusquedaShow,CampoBusqueda, Resultado));


    return 0;
}


int Modificar_Inquilino (int Legajo) {
    char LegajoChar[50], Caract;
    int Pos=0, i, Eleccion;
    int CantRegs=0;
    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");

    if (BaseDeDatos!=NULL) {
        fseek(BaseDeDatos,0,SEEK_END);
        CantRegs= ( ftell(BaseDeDatos) ) / sizeof(InquilinosSTR);
        fclose(BaseDeDatos);
    }
    for (i=0;i<CantRegs;i++) if(Inquilino[i].Legajo==Legajo) Pos=i;

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf("\n%s                      +----------+\n", Espacio);
    printf("%s                      | AVANZADO |\n", Espacio);
    printf("%s                      +----------+\n\n", Espacio);

    if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");
    if (Dispositivo==1) printf("   |    N  Legajo    |            Titular              |              Inquilino            |         Direccion        |\n");
    if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");

    sprintf(LegajoChar, "%i", Legajo);
    Dibujar_Registro(Pos,1);

    if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n");

    printf("\n\n\n\t¿Que Campo Desea Modificar?\n");

    printf(ANSI_COLOR_CYAN"\n\t\t 1.- Legajo             \\\\  2.- Direccion Local       \\\\  3.- Nombre Titular       "ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 4.- DNI Titular        \\\\  5.- Direccion Titular     \\\\  6.- Nombre Inquilino     "ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 7.- DNI Inquilino      \\\\  8.- Nombre Garantia       \\\\  9.- DNI Garantia         "ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t10.- Direccion Garantia \\\\ 11.- Fecha Recesion        \\\\ 12.- Interes Por Retraso  "ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t13.- Precio AYSA        \\\\ 14.- Precio ABL            \\\\ 15.- Precio Expensas      "ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t                        \\\\ 16.- Comision Inmobiliaria \\\\"ANSI_COLOR_RESET);

    printf("\n\n\t¿Cual es su Eleccion? > ");
    scanf("%i", &Eleccion);

    switch (Eleccion) {
        case 1 :
            printf("\n\n\t\tContenido del Campo: %ld", Inquilino[Pos].Legajo);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                scanf("%ld", &Inquilino[Pos].Legajo);

        break;
        case 2 :
            printf("\n\n\t\tContenido del Campo: %s", Inquilino[Pos].DireccionLocal);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                fgets(Inquilino[Pos].DireccionLocal, sizeof(Inquilino[Pos].DireccionLocal),stdin);
                if( Inquilino[Pos].DireccionLocal [strlen( Inquilino[Pos].DireccionLocal ) - 1] == '\n' ) {
                    Inquilino[Pos].DireccionLocal [strlen( Inquilino[Pos].DireccionLocal ) - 1] = 0;
                }
        break;
        case 3 :
            printf("\n\n\t\tContenido del Campo: %s", Inquilino[Pos].NombreTitular);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                fgets(Inquilino[Pos].NombreTitular, sizeof(Inquilino[Pos].NombreTitular),stdin);
                if( Inquilino[Pos].NombreTitular [strlen( Inquilino[Pos].NombreTitular ) - 1] == '\n' ) {
                    Inquilino[Pos].NombreTitular [strlen( Inquilino[Pos].NombreTitular ) - 1] = 0;
                }
        break;
        case 4 :
            printf("\n\n\t\tContenido del Campo: %ld", Inquilino[Pos].DNITitular);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                do {
                    scanf("%ld", &Inquilino[Pos].DNITitular);
                    if((Inquilino[Pos].DNITitular/10000000)<1 || (Inquilino[Pos].DNITitular/100000000)>=1) printf(ANSI_COLOR_RED"        DNI Ingresado Incorrrecto. Intente de Vuelta: "ANSI_COLOR_RESET);
                }while((Inquilino[Pos].DNITitular/10000000)<1 || (Inquilino[Pos].DNITitular/100000000)>=1);

        break;
        case 5 :
            printf("\n\n\t\tContenido del Campo: %s", Inquilino[Pos].DireccionTitular);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                fgets(Inquilino[Pos].DireccionTitular, sizeof(Inquilino[Pos].DireccionTitular),stdin);
                if( Inquilino[Pos].DireccionTitular [strlen( Inquilino[Pos].DireccionTitular ) - 1] == '\n' ) {
                    Inquilino[Pos].DireccionTitular [strlen( Inquilino[Pos].DireccionTitular ) - 1] = 0;
                }
        break;
        case 6 :
            printf("\n\n\t\tContenido del Campo: %s", Inquilino[Pos].NombreInquilino);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                fgets(Inquilino[Pos].NombreInquilino, sizeof(Inquilino[Pos].NombreInquilino),stdin);
                if( Inquilino[Pos].NombreInquilino [strlen( Inquilino[Pos].NombreInquilino ) - 1] == '\n' ) {
                    Inquilino[Pos].NombreInquilino [strlen( Inquilino[Pos].NombreInquilino ) - 1] = 0;
                }
        break;
        case 7 :
            printf("\n\n\t\tContenido del Campo: %ld", Inquilino[Pos].DNIInquilino);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                do {
                    scanf("%ld", &Inquilino[Pos].DNIInquilino);
                    if((Inquilino[Pos].DNIInquilino/10000000)<1 || (Inquilino[Pos].DNIInquilino/100000000)>=1) printf(ANSI_COLOR_RED"        DNI Ingresado Incorrrecto. Intente de Vuelta: "ANSI_COLOR_RESET);
                } while((Inquilino[Pos].DNIInquilino/10000000)<1 || (Inquilino[Pos].DNIInquilino/100000000)>=1);

        break;
        case 8 :
            printf("\n\n\t\tContenido del Campo: %s", Inquilino[Pos].GarantiaInquilino.NombreGarant);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                fgets(Inquilino[Pos].GarantiaInquilino.NombreGarant, sizeof(Inquilino[Pos].GarantiaInquilino.NombreGarant),stdin);
                if( Inquilino[Pos].GarantiaInquilino.NombreGarant [strlen( Inquilino[Pos].GarantiaInquilino.NombreGarant ) - 1] == '\n' ) {
                    Inquilino[Pos].GarantiaInquilino.NombreGarant [strlen( Inquilino[Pos].GarantiaInquilino.NombreGarant ) - 1] = 0;
                }
        break;
        case 9 :
            printf("\n\n\t\tContenido del Campo: %ld", Inquilino[Pos].GarantiaInquilino.DNI);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                do {
                    scanf("%ld", &Inquilino[Pos].GarantiaInquilino.DNI);
                    if((Inquilino[Pos].GarantiaInquilino.DNI/10000000)<1 || (Inquilino[Pos].GarantiaInquilino.DNI/100000000)>=1) printf(ANSI_COLOR_RED"        DNI Ingresado Incorrrecto. Intente de Vuelta: "ANSI_COLOR_RESET);
               } while((Inquilino[Pos].GarantiaInquilino.DNI/10000000)<1 || (Inquilino[Pos].GarantiaInquilino.DNI/100000000)>=1);
        break;
        case 10 :
            printf("\n\n\t\tContenido del Campo: %s", Inquilino[Pos].GarantiaInquilino.DireccionGarant);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                fgets(Inquilino[Pos].GarantiaInquilino.DireccionGarant, sizeof(Inquilino[Pos].GarantiaInquilino.DireccionGarant),stdin);
                if( Inquilino[Pos].GarantiaInquilino.DireccionGarant [strlen( Inquilino[Pos].GarantiaInquilino.DireccionGarant ) - 1] == '\n' ) {
                    Inquilino[Pos].GarantiaInquilino.DireccionGarant [strlen( Inquilino[Pos].GarantiaInquilino.DireccionGarant ) - 1] = 0;
                }
        break;
        case 11 :
            printf("\n\n\t\tContenido del Campo: %i/%i/%i", Inquilino[Pos].FechaRecesion.Dia, Inquilino[Pos].FechaRecesion.Mes, Inquilino[Pos].FechaRecesion.Anio);
            printf(ANSI_COLOR_RED"\n           Recuerde NO usar 0 en el Principio del MES ni del DIA!"ANSI_COLOR_RESET);
            printf("\n\tIngrese el Nuevo Contenido del Campo (Formato: d/m/yyyy) > ");
                scanf("%i/%i/%i", &Inquilino[Pos].FechaRecesion.Dia,& Inquilino[Pos].FechaRecesion.Mes, &Inquilino[Pos].FechaRecesion.Anio);

        break;
        case 12 :
            printf("\n\n\t\tContenido del Campo: %i%%", Inquilino[Pos].InteresPorPagoTarde);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                scanf("%i", &Inquilino[Pos].InteresPorPagoTarde);
        break;
        case 13 :
            printf("\n\n\t\tContenido del Campo: $%.2f", Inquilino[Pos].PrecioAysa[0]);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                scanf("%f", &Inquilino[Pos].PrecioAysa[0]);
        break;
        case 14 :
            printf("\n\n\t\tContenido del Campo: $%.2f", Inquilino[Pos].PrecioABL[0]);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                scanf("%f", &Inquilino[Pos].PrecioABL[0]);
        break;
        case 15 :
            printf("\n\n\t\tContenido del Campo: $%.2f", Inquilino[Pos].PrecioExpensas[0]);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                scanf("%f", &Inquilino[Pos].PrecioExpensas[0]);
        break;
        case 16 :
            printf("\n\n\t\tContenido del Campo: %i%%", Inquilino[Pos].ComisionDeLaInmobiliaria);
            printf("\n\tIngrese el Nuevo Contenido del Campo > ");
                scanf("%i", &Inquilino[Pos].ComisionDeLaInmobiliaria);
        break;
    }

    printf("\n\t ¿Esta Seguro de Realizar este Cambio? (S/N) > ");
        LimpiarBuffer();
        fflush(stdin);
        Caract=getch();
        if (Caract=='s' || Caract=='S' || Caract=='1') {
            FILE *BaseDeDatos = fopen("Inquilinos.bin","w+b");
            if (BaseDeDatos!=NULL) {// SI SE ABRE CORRECTAMENTE
                rewind(BaseDeDatos);
                for(i=0;i<CantRegs;i++){
                    fwrite(&Inquilino[i],sizeof(InquilinosSTR),1,BaseDeDatos);
                }
                fclose(BaseDeDatos);
            }
        }
    Menu_Principal();
    return 0;
}

int Informacion_Inquilino (int Legajo) {
    char Caract;
    int i, Pos, CantRegs;
    float AniosContrato;
    char TextoMonto[35]=" ";

        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            CantRegs= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            fclose(BaseDeDatos);
        } else { printf(ANSI_COLOR_RED"ERROR! No Existe una Base de Datos\n"ANSI_COLOR_RESET); return 0;}
        for (i=0;i<CantRegs;i++) if(Inquilino[i].Legajo==Legajo) Pos=i;

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf(ANSI_COLOR_YELLOW"\n\n Informacion del Alquiler...\n"ANSI_COLOR_RESET);

    printf("\n\t\t\t%-35s%ld","N  de Legajo: ",Inquilino[Pos].Legajo );
    printf("\n\t\t\t%-35s%s","Direccion del Local: ",Inquilino[Pos].DireccionLocal);
    printf("\n\t\t\t%-35s%s","Nombre del Titular: ", Inquilino[Pos].NombreTitular);
    printf("\n\t\t\t%-35s%ld","DNI del Titular: ", Inquilino[Pos].DNITitular);
    printf("\n\t\t\t%-35s%s","Direccion del Titular: ", Inquilino[Pos].DireccionTitular);
    printf("\n\t\t\t%-35s%s","Nombre del Inquilino: ", Inquilino[Pos].NombreInquilino);
    printf("\n\t\t\t%-35s%ld","DNI del Inquilino: ", Inquilino[Pos].DNIInquilino);
    printf("\n\t\t\t%-35s%s","Nombre del Garante: ", Inquilino[Pos].GarantiaInquilino.NombreGarant);
    printf("\n\t\t\t%-35s%ld","DNI del Garante: ", Inquilino[Pos].GarantiaInquilino.DNI);
    printf("\n\t\t\t%-35s%s","Direccion del Garante: ", Inquilino[Pos].GarantiaInquilino.DireccionGarant);
    printf("\n\t\t\t%-35s", "Tipo de Contrato: " );

    switch (Inquilino[Pos].TipoContrato) {
        case 1 :
            printf("6 Meses\n");
        break;
        case 2 :
            printf("1 Anio\n");
        break;
        case 3 :
            printf("2 Anios\n");
        break;
        case 4 :
            printf("3 Anios\n");
        break;
    }

    printf("\t\t\t%-35s%i/%i/%i", "Fecha de Inicio del Contrato: ", Inquilino[Pos].FechaInicio.Dia,Inquilino[Pos].FechaInicio.Mes, Inquilino[Pos].FechaInicio.Anio);
    printf("\n\t\t\t%-35s%i/%i/%i","Fecha de Recesion del Contrato: ", Inquilino[Pos].FechaRecesion.Dia, Inquilino[Pos].FechaRecesion.Mes,Inquilino[Pos].FechaRecesion.Anio);
    printf("\n\t\t\t%-35s$%.2f","Precio del Alquiler:",Inquilino[Pos].PrecioAlquiler);
    printf("\n\t\t\t%-35s$%.2f","Precio del ABL:",Inquilino[Pos].PrecioABL[0]);
    printf("\n\t\t\t%-35s$%.2f","Precio de las Expensas:",Inquilino[Pos].PrecioExpensas[0]);
    printf("\n\t\t\t%-35s$%.2f","Precio del AYSA:",Inquilino[Pos].PrecioAysa[0]);
    printf("\n\t\t\t%-35s", "Tipo de Ajuste: ");

    switch (Inquilino[Pos].TipoAjuste) {
        case 10 :
            printf("Sin Ajuste");
        break;
        case 1 :
            printf("Trimestral");
        break;
        case 2 :
            printf("Semestral");
        break;
        case 3 :
            printf("Anual");
        break;
    }

    printf("\n\t\t\t%-35s", "Formato de Ajuste: ");


        switch(Inquilino[Pos].TipoContrato) {
            case 1 :
                AniosContrato=0.5;
            break;
            case 2 :
                AniosContrato=1;
            break;
            case 3 :
                AniosContrato=2;
            break;
            case 4 :
                AniosContrato=3;
            break;
        }

        int CantMontos = (12 * AniosContrato ) / (3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 ));
    switch (Inquilino[Pos].FormatoDeAjuste) {
        case 0 :
            printf("Sin Ajuste");
        break;
        case 1 :
            printf("Por Monto");
            for(i=0; i<CantMontos;i++) {
                sprintf(TextoMonto,"%s%-2i%s%-2i%s", "Monto del Mes ",i*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )), " al ", (i+1)*(3 * Pot( 2 , Inquilino[Pos].TipoAjuste-1 )) , ":    $");
                printf("\n\t\t\t\t%s%.2f", TextoMonto, Inquilino[Pos].InteresMonto[i]);
            }
        break;
        case 2 :
            printf("Por Porcentaje");
            printf("\n\t\t\t\t%-27s%i%%","Porcentaje de Ajuste:",Inquilino[Pos].InteresPorcentaje);

        break;
    }

    printf("\n\t\t\t%-35s%i%%","Interes por Demora de Pago: ", Inquilino[Pos].InteresPorPagoTarde);
    printf("\n\t\t\t%-35s%i%%","Comision de la Inmobiliaria: ", Inquilino[Pos].ComisionDeLaInmobiliaria);


    printf("\n\n\n\tPresione una Tecla para Regresar...");
    getch();
    Menu_Principal();
    return 0;
}


int Menu_Modificar () {
    char Caract, PatronBusquedaShow[50]="   ";
    int Eleccion=0, CampoBusqueda, Resultado=0, i=0;

    for (i=0;i<50;i++){
        PatronBusquedaShow[i]=0;
    }
    i=0;

    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    Insertar_Fecha_en_Esquina();
    printf("\n%s                      +----------+\n", Espacio);
    printf("%s                      | AVANZADO |\n", Espacio);
    printf("%s                      +----------+\n", Espacio);
    printf("\n\n\n\t Que esta Buscando?");
    printf(ANSI_COLOR_CYAN"\n\n\t\t 1.- Modificar Un Inquilino"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 2.- Relizar un Backup"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t 3.- Ver Informacion Completa de un Inquilino"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"\n\t\t ESC.- Salir al Menu Anterior\n\n"ANSI_COLOR_RESET);
    printf("\n\n\t Ingrese su eleccion > ");
                    fflush(stdin);
                    Caract=getch();
                    while (Caract=='\n' || Caract>'3' || Caract<'1') {
                        fflush(stdin);
                        if (Caract==27) { Menu_Principal(); return 0;}
                        Caract=getch();
                    }
                    Eleccion=atoi(&Caract);


    if(Eleccion!=2){
        if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
        Insertar_Fecha_en_Esquina();
        printf("\n%s                      +----------+\n", Espacio);
        printf("%s                      | AVANZADO |\n", Espacio);
        printf("%s                      +----------+\n", Espacio);
        printf("\n\n\n\t Por que Campo Desea Buscar?");
        printf(ANSI_COLOR_CYAN"\n\n\t\t 1.- N  Legajo"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN"\n\t\t 2.- Nombre Titular"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN"\n\t\t 3.- Nombre Inquilino"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN"\n\t\t 4.- Direccion"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN"\n\t\t 5.- Imprimir Todos\n\n"ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN"\n\t\t ESC.- Salir al Menu Anterior\n\n"ANSI_COLOR_RESET);

        printf("\n\n\t Ingrese su eleccion > ");
        fflush(stdin);
        Caract=getch();
        while (Caract=='\n' || Caract>'5' || Caract<'1') {
            fflush(stdin);
            if (Caract==27) { Menu_Principal(); return 0;}
            Caract=getch();
        }
        CampoBusqueda=atoi(&Caract);

        if (CampoBusqueda==5) {
            if(Eleccion==1) Modificar_Inquilino(Listar_Inquilinos());
            if(Eleccion==3) Informacion_Inquilino(Listar_Inquilinos());
            return 0;}

        if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
        Insertar_Fecha_en_Esquina();
        printf("\n%s                      +----------+\n", Espacio);
        printf("%s                      | AVANZADO |\n", Espacio);
        printf("%s                      +----------+\n", Espacio);
        printf(" (Flecha Izquierda vuelve al Menu Anterior)\n");

        printf("\n\n\n\t (NO usar Mayusculas. Solo usar letras de la 'a' a la 'z' o del '0' al '9')");
        printf("\n\n Patron de Busqueda : %s", PatronBusquedaShow);

        do {
            fflush(stdin);
            Caract=getch();
            fflush(stdin);
            /* printf("%ld", strlen(PatronBusquedaShow)); */
            switch(Caract) {
                case 65 : //SI ES FLECHA PARA ARRIBA
                    if(Resultado<0) Resultado++;
                    break;
                    break;
                case 72 : //WINDOWS ARRIBA
                    if(Resultado<0) Resultado++;
                    break;
                case 66 : //SI ES FLECHITA PARA ABAJO. Tambien se Activa con [Shift-G]
                    Resultado--;
                    break;
                case 80 : //Windows Abajo
                    Resultado--;
                    break;
                case 127 : // SI ES LA TECLA ELIMINAR, RETROCEDE
                    if (i>0) { PatronBusquedaShow[i-1]=0; i--; }
                    break;
                case 8 : // WINDOWS ELIMINAR
                    if (i>0) { PatronBusquedaShow[i-1]=0; i--; }
                    break;
                case 10 :
                    break;
                    break;
                case 13 :  //WINDOWS ENTER
                    break;
                    break;
                case 67 :
                    break;
                    break;
                case 77 : //WINDOWS DERECHA
                    break;
                    break;
                case 68 : // Si apreto [Shift-D] se me activa este Menu
                    Menu_Modificar(); return 0;
                    break;
                case 75 : //WINDOWS IZQUIERDA
                    Menu_Modificar(); return 0;
                    break;
                default : //SINO CONTINUA  NORMAL
                    if(Caract=='a' || Caract=='b' || Caract=='c' || Caract=='d' || Caract=='e' || Caract=='f' || Caract=='g' || Caract=='h' || Caract=='i' || Caract=='j' || Caract=='k' || Caract=='l' || Caract=='m' || Caract=='n' || Caract=='o' || Caract=='p' || Caract=='q' || Caract=='r' || Caract=='s' || Caract=='t' || Caract=='u' || Caract=='v' || Caract=='w' || Caract=='x' || Caract=='y' || Caract=='z' || Caract=='1' || Caract=='2' || Caract=='3' || Caract=='4' || Caract=='5' || Caract=='6' || Caract=='7' || Caract=='8' || Caract=='9' || Caract=='0' || Caract==' ') { PatronBusquedaShow[i]=Caract; i++; }
                break;
            }

            if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
            Insertar_Fecha_en_Esquina();
        printf("\n%s                      +----------+\n", Espacio);
        printf("%s                      | AVANZADO |\n", Espacio);
        printf("%s                      +----------+\n", Espacio);

            printf("\n\n\n\t (NO usar Mayusculas. Solo usar letras de la 'a' a la 'z' o del '0' al '9')");
            printf("\n\n Patron de Busqueda: %s\n\n", PatronBusquedaShow);
            BusquedaShow(PatronBusquedaShow, CampoBusqueda, Resultado);

        } while (Caract!=10 && Caract!=13 && Caract != 67 && Caract != 77) ; //13 - WINDOWS ENTER //77 -- Windows Derecha


            if(Eleccion==1) Modificar_Inquilino(SeleccionBusqueda(PatronBusquedaShow, CampoBusqueda, Resultado));
            if(Eleccion==2) Informacion_Inquilino(SeleccionBusqueda(PatronBusquedaShow, CampoBusqueda, Resultado));
    }


    if(Eleccion==2) {
        BackupBaseDeDatos();
        Menu_Principal();
        return 0;
    }

    return 0;
}

int Eliminar_Registro (int Pos) {
    InquilinosSTR InquilinoTemporal;
    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
    FILE *Temporal = fopen("TEMP.bin","wb");
    fseek(BaseDeDatos,0,SEEK_SET);
    fseek(Temporal,0,SEEK_SET);
    int i=0;

    if (BaseDeDatos!=NULL) {
        fread(&InquilinoTemporal,sizeof(InquilinosSTR),1,BaseDeDatos);
        while(!feof(BaseDeDatos)){
            if(i!=Pos) { fwrite(&InquilinoTemporal,sizeof(InquilinosSTR),1,Temporal); }
            i++;
            fread(&InquilinoTemporal,sizeof(InquilinosSTR),1,BaseDeDatos);
        }

    fclose(BaseDeDatos);
    fclose(Temporal);
    remove("Inquilinos.bin");
    rename("TEMP.bin","Inquilinos.bin");
    return 0;
    } else return 1;
}


int BackupBaseDeDatos () {
    char NombreBackup[50] = "   ";
    InquilinosSTR BackupSTR;
    sprintf(NombreBackup, "Backup/Inquilinos %i-%i-%i.bin", AnioActual(), MesActual(), DiaActual());
    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");

    FILE *Backup = fopen(NombreBackup,"wb");

    if (BaseDeDatos!=NULL) {
        fseek(BaseDeDatos,0,SEEK_SET);
            fread(&BackupSTR,sizeof(InquilinosSTR),1,BaseDeDatos);
            while(!feof(BaseDeDatos)){
                fwrite(&BackupSTR,sizeof(InquilinosSTR),1,Backup);
                fread(&BackupSTR,sizeof(InquilinosSTR),1,BaseDeDatos);
            }
        fclose(BaseDeDatos);
        fclose(Backup);


        if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
        printf(ANSI_COLOR_GREEN"\n\n\t Backup Realizado Exitosamente. "ANSI_COLOR_RESET);
        printf("Para Restaurar este Backup Copie Manualmente la Base de Datos");
        printf("\n\t\tde la Fecha deseada (Ejemplo: Inquilinos 2022-2-21) que esta ubicado en la carpeta 'Backup'");
        printf("\n\t\tdentro de la carpeta del programa, cambiandole el nombrea 'Inquilino.bin' y reemplazando la ya existente");
        printf("\n\n\tPresione una Tecla para Continuar...");
        getch();
        Menu_Principal();

    } else return 0;
    return 1;
}


int CargarArchivoEnStruct (int BoolRevisarContratosExpirados) { //BoolRevisarContratosExpirados==0 Si ; BoolRevisarContratosExpirados==1 No
    int x;
    FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");

    if (BaseDeDatos!=NULL) {
        fseek(BaseDeDatos,0,SEEK_END);
        int Pos= ( ftell(BaseDeDatos) ) / sizeof(InquilinosSTR), i=0;
        fseek(BaseDeDatos,0,SEEK_SET);

        fread(&Inquilino[i],sizeof(InquilinosSTR),1,BaseDeDatos);
        while(!feof(BaseDeDatos)){
            i++;
            fread(&Inquilino[i],sizeof(InquilinosSTR),1,BaseDeDatos);
        }
        fclose(BaseDeDatos);



    /* if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n"); */
    /* if (Dispositivo==1) printf("   |    N  Legajo    |            Titular              |              Inquilino            |         Direccion        |\n"); */
    /* if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n"); */


        for (x=0;x<Pos;x++) {
            if(Inquilino[x].Legajo!=0) {
                    if(BoolRevisarContratosExpirados==0) {
                        int TiempoDeContrato=(AnioActual()-Inquilino[x].FechaInicio.Anio)*12+(MesActual()-Inquilino[x].FechaInicio.Mes);
                        int MesesContrato;
                        int Eleccion;
                        switch(Inquilino[x].TipoContrato) {
                            case 1 :
                                MesesContrato=0.5*12;
                            break;
                            case 2 :
                                MesesContrato=1*12;
                            break;
                            case 3 :
                                MesesContrato=2*12;
                            break;
                            case 4 :
                                MesesContrato=3*12;
                            break;
                        }
                        if (TiempoDeContrato>(MesesContrato+3)) { // SI EL TIEMPO DESDE QUE SE CREO EL CONTRATO ES MAYOR A LOS MESES DE CONTRATO PASADOS 90 DIAS, LO BORRA SIN PREGUNTAR
                            Exportar_Inquilino(x);
                            Eliminar_Registro(x);
                            CargarArchivoEnStruct(0);
                            return 0;
                        }
                        if (TiempoDeContrato>MesesContrato) { // SI EL TIEMPO DESDE QUE SE CREO EL CONTRATO ES MAYOR A LOS MESES DE CONTRATO, PREGUNTAR
                            if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
                            Insertar_Fecha_en_Esquina();
                            printf("\n\n\t Se ha Detectado un Contrato que ya ha vencido. ¿Que Desea Hacer?");
                            printf("\n\t Informacion del Contrato:");
                            printf("\n\t\tN  Legajo:         %ld", Inquilino[x].Legajo);
                            printf("\n\t\tDireccion:         %s", Inquilino[x].DireccionLocal);
                            printf("\n\t\tTitular:           %s", Inquilino[x].NombreTitular);
                            printf("\n\t\tInquilino:         %s", Inquilino[x].NombreInquilino);
                            printf("\n\t\tFecha de Inicio:   %i/%i/%i", Inquilino[x].FechaInicio.Dia, Inquilino[x].FechaInicio.Mes, Inquilino[x].FechaInicio.Anio);
                            printf("\n\t\tFecha de Recesion: %i/%i/%i", Inquilino[x].FechaRecesion.Dia, Inquilino[x].FechaRecesion.Mes, Inquilino[x].FechaRecesion.Anio);

                            printf(ANSI_COLOR_CYAN"\n\n\t\t\t 1.- Archivar \\\\ 2.- Archivar y Crear Nuevo  \\\\ 3.- No Hacer Nada"ANSI_COLOR_RESET);
                            printf("\n\n\t Ingrese ( 1 , 2 o 3 ) > ");
                            scanf("%i", &Eleccion);

                            switch (Eleccion) {
                                case 1 :
                                    Exportar_Inquilino(x);
                                    Eliminar_Registro(x);
                                    CargarArchivoEnStruct(0);
                                    return 0;
                                break;
                                case 2 :
                                    Exportar_Inquilino(x);
                                    Eliminar_Registro(x);
                                    CargarArchivoEnStruct(1);
                                    Menu_Agregar();
                                    CargarArchivoEnStruct(0);
                                    return 0;
                                break;
                                case 3 :
                                    printf("\n\n\t Tenga en Cuenta Que el Registro de Este Alquiler sera Archivado Automaticamente a los 3 Meses de la Fecha de Recesion.");
                                    printf("\n\n\t\t Presione Para Continuar...");
                                    LimpiarBuffer();
                                    getch();
                                break;
                            }
                        }
                    }



        /* Dibujar_Registro(x, 1); //PONGO 1 PARA QUE NO DIBUJA LA FLECHITA DE SELECCION */
        /* if (Dispositivo==1) printf("   +-----------------+---------------------------------+-----------------------------------+--------------------------+\n"); */
                }
        }
    }
    return 0;
}




int Menu_Principal () {
    char Eleccion;
    if (Dispositivo==1) {strcpy(Espacio,"                                                 ");}
    else {strcpy(Espacio," ");}
    if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
    printf(ANSI_COLOR_BLUE"%s |     _    _             _ _                     | \n"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_BLUE"%s |    / \\  | | __ _ _   _(_) | ___ _ __ ___  ___  | \n"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_BLUE"%s |   / _ \\ | |/ _` | | | | | |/ _ \\ '__/ _ \\/ __| | \n"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_BLUE"%s |  / ___ \\| | (_| | |_| | | |  __/ | |  __/\\__ \\ | \n"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_BLUE"%s | /_/   \\_\\_|\\__, |\\__,_|_|_|\\___|_|  \\___||___/ | \n"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_BLUE"%s |               |_|                              | \n"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_BLUE"%s --------------------------------------------------"ANSI_COLOR_RESET, Espacio);

        FILE *BaseDeDatos = fopen("Inquilinos.bin","rb");
        if (BaseDeDatos!=NULL) {
            fseek(BaseDeDatos,0,SEEK_END);
            int Pos= ( ftell(BaseDeDatos) ) / ( sizeof(InquilinosSTR)  );
            printf("\n%s                    Alquileres: %i",Espacio, Pos);
            fclose(BaseDeDatos);
        }

    printf("\n\n");
    printf(ANSI_COLOR_GREEN"%s1.- AGREGAR"ANSI_COLOR_RESET, Espacio);
    printf(ANSI_COLOR_GREEN"   2.- COBRAR"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"   3.- PAGAR"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"   4.- AVANZADO\n\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN"%s                   ESC.- Salir\n\n\n"ANSI_COLOR_RESET, Espacio);
    printf("\n\n\n\n");
    printf("Ingrese su Opcion: ");

    fflush(stdin);
    Eleccion=getch();
    printf("%c\n", Eleccion);

    switch (Eleccion) {
        case '1' :
            Menu_Agregar();
            Menu_Principal();
            return 0;
        break;
        case '2' :
            Menu_Cobrar();
            return 0;
        break;
        case '3' :
            Menu_Pagar();
            return 0;
        break;
        case '4' :
            Menu_Modificar();
            return 0;
        break;
        case 27 :
            if(SO==1) Limpiar_Pantalla(); else Limpiar_Pantalla_Win();
            printf("\n\n\n Saliendo... Si todavia no salio, aprete ESC de vuelta\n\n\n\n\n\n");
            return 0;
        break;
        case 10 :
            printf(ANSI_COLOR_RED"\n\n\tError! Opcion ENTER Inexistente."ANSI_COLOR_RESET);
            printf(ANSI_COLOR_CYAN"\n\t\t    Pulse una tecla Para Continuar"ANSI_COLOR_RESET);
            fflush(stdin);
            getch();
            Menu_Principal();
            return 0;
        break;
        case 13 : //WINDOWS ENTER
            printf(ANSI_COLOR_RED"\n\n\tError! Opcion ENTER Inexistente."ANSI_COLOR_RESET);
            printf(ANSI_COLOR_CYAN"\n\t\t    Pulse una tecla Para Continuar"ANSI_COLOR_RESET);
            fflush(stdin);
            getch();
            Menu_Principal();
            return 0;
        break;
        default :
            printf(ANSI_COLOR_RED"\n\n\tError! Opcion %c Inexistente."ANSI_COLOR_RESET, Eleccion);
            printf(ANSI_COLOR_CYAN"\n\t\t    Pulse una tecla Para Continuar"ANSI_COLOR_RESET);
            fflush(stdin);
            getch();
            Menu_Principal();
            return 0;
    }

}


int main () {
    mkdir("Pagos", S_IRWXU);
    mkdir("Backup", S_IRWXU);
    mkdir("Cobros", S_IRWXU);
    mkdir("Contratos Vencidos", S_IRWXU);
    CargarArchivoEnStruct(0);
    /* getch(); */
    Menu_Principal();
    return 0;
}

