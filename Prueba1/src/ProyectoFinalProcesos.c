
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "mpi.h"


#define horaIncioDia 540  //9hrs
#define horaApertura 600  //10hrs
#define horaFinDia 1380   //23hrs

	struct timespec ts = {0, 100000000L };
	int cupoFeria = 100;

////DECLARACIÓN DE FUNCIONES////

int generarHoraLlegada(){

	srand(time(NULL));

	int horaLlegada = rand() % 781;

	return horaLlegada;

}

int dondeIr(){ //genera la descición de la gente de a donde ir
	//1 PASILLO
	//2 MOTANA
	//3 CABALLO
	//4 IRSE
	srand(time(NULL));
	int dondeIr = (rand()% 4) + 1;

	return dondeIr;
}

void reloj(int tid, int hora){ //prueba para saber si un proceso recibe la hora

	printf("Soy el proceso %d y la hora es %d \n",tid,hora/60);
	//if(hora==540 || hora==1380)printf("soy la persona %d hora %d \n", tid, hora/60);

	return;

}

void ocupado(int tiempo) {

	usleep(tiempo);

	return;

}

////DECLARACIÓN DE FUNCIONES////

int main(int argc, char* argv[]){


	int  tid; /* rank of process */
	int  p;       /* number of processes */
	int horaActual = horaIncioDia;

	MPI_Status status ;   /* return status for receive */


	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &tid);

	MPI_Comm_size(MPI_COMM_WORLD, &p);


	do{

		if(tid == 0){	//Tiempo
				MPI_Bcast(&horaActual,1,MPI_INT,0,MPI_COMM_WORLD);//manda la hora actual a todos los procesos
				horaActual++;
		}

		//Pasillos = 0  Tiempo de estancia en pasillos 2 min   TAG 0
		if(tid == 1){


			int clienteActual = 0;


			MPI_Recv(&clienteActual, 1, MPI_INT, MPI_ANY_SOURCE, 100 ,MPI_COMM_WORLD, &status);

			printf("ME QUEDE ATORADO %d",tid);

			MPI_Bcast(&horaActual,1,MPI_INT,0,MPI_COMM_WORLD);
			//////////////////////////////////////PRUEBA/////////////////////////////////////////////////////////////////////
			MPI_Send(&tid, 1, MPI_INT, 100, status.MPI_SOURCE , MPI_COMM_WORLD); //Libera, manda lavrg

			reloj(tid,horaActual);
			//

		}
		//Montana = 1     Duracion de juego 1 minuto
		if(tid == 2){

			int fila = 0; //15 fila
		    int cupo = 0; //8 cupo


			int clienteActual = 0;
			MPI_Bcast(&horaActual,1,MPI_INT,0,MPI_COMM_WORLD);
			MPI_Recv(&clienteActual, 1, MPI_INT, MPI_ANY_SOURCE, 101 ,MPI_COMM_WORLD, &status);
			//////////////////////////////////////PRUEBA/////////////////////////////////////////////////////////////////////
			MPI_Send(&tid, 1, MPI_INT, 101, clienteActual, MPI_COMM_WORLD); //Libera, manda lavrg

			reloj(tid,horaActual);
		}
		//caballo = 2  duracion 2 minutos
		if(tid == 3){
			int fila = 0; //20
			int cupo = 0; //10

			int clienteActual = 0;
			MPI_Bcast(&horaActual,1,MPI_INT,0,MPI_COMM_WORLD);
			MPI_Recv(&clienteActual, 1, MPI_INT, MPI_ANY_SOURCE, 102 ,MPI_COMM_WORLD, &status);

//////////////////////////////////////PRUEBA/////////////////////////////////////////////////////////////////////
			MPI_Send(&tid, 1, MPI_INT, 102, clienteActual, MPI_COMM_WORLD); //Libera, manda lavrg

			reloj(tid,horaActual);
		}
		//Todos los demas
		if(tid  >= 4){

			int horaLlegada = generarHoraLlegada();
			int queHacer;
			int tid_pasillos=0;


			MPI_Bcast(&horaActual,1 ,MPI_INT,0,MPI_COMM_WORLD);

			printf("sali de los 3 juegos soy la persona %d\n",tid);

////////////////////////////////////////////AQUI SE ROMPE//////////////////////////////////////////////////////////////////////////
			MPI_Send(&tid, 1, MPI_INT, 1, 100,MPI_COMM_WORLD); //pasillos  0
			printf("ya mande a 1");
			MPI_Recv(&tid_pasillos, 1, MPI_INT, MPI_ANY_SOURCE,100,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			tid_pasillos=0;
			printf("SALI DEL PRIMER SEND  %d \n",tid);

			MPI_Send(&tid, 1, MPI_INT, 2, 101,MPI_COMM_WORLD); // Rusa   1
			MPI_Recv(&tid_pasillos, 1, MPI_INT, MPI_ANY_SOURCE,101,	MPI_COMM_WORLD, &status);
			tid_pasillos=0;
			printf("SALI DEL SEGUNDO SEND  %d \n",tid);

			MPI_Send(&tid, 1, MPI_INT, 3, 102,MPI_COMM_WORLD); //caballos   2
			MPI_Recv(&tid_pasillos, 1, MPI_INT, MPI_ANY_SOURCE,102,MPI_COMM_WORLD, &status);

			printf("sali de los 3 juegos %d\n",tid);
			//reloj(tid,horaActual);

		/*	if(cupoFeria > 0){
				cupoFeria--;
				if(horaLlegada == horaActual){ //Llego a la feria
					do{
						queHacer = dondeIr(); //Decido que hacer
						printf("Voy a ir %d\n",queHacer);

						if (queHacer != 4) {//Evita que mande mensaje a tid = 4 y se rompa esta madre
							MPI_Send(&tid, 1, MPI_INT, queHacer, 100,MPI_COMM_WORLD);
							//Esperando que el juego te libere(Esperando el send del juego)
							MPI_Recv(&tid, 1, MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,
									MPI_COMM_WORLD, &status);
						}
						break;
					}while(queHacer != 4);
					cupoFeria++; //Estoy saliendo de la feria
				}
			}else{
				//Codigo que hace que espere
			} */
		}

		//MPI_Barrier(MPI_COMM_WORLD);


	}while(horaActual< horaFinDia);

	MPI_Finalize();


	return 0;

}
