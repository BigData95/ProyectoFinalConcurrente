/*
 ============================================================================
 Name        : ProyectoFinalProcesos.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <stdlib.h>
#include <limits.h>


#define NUM_CLIENTES_ADMITIDOS	  100


#define HORA_INICIO_DIA 540  //9hrs
#define HORA_APERTURA 600  //10hrs
#define HORA_FIN_DIA 1380   //23hrs

#define PASILLO						1//Estados del Cliente
#define CABALLOS				2//Estados del Cliente
#define RUSA					3//Estados del Cliente

#define FILA_RUSA				15//Cantidad de clientes maxima que pueden estar formados en la fila
#define NUM_LUGARES_RUSA			8//Cantidad de lugares disponibles del juego
#define TIEMPO_DE_JUEGO_RUSA		2//Numero de minutos que esta en funcionamiento el juego una vez que comience

#define FILA_CABALLOS			20//Cantidad de clientes maxima que pueden estar formados en la fila
#define NUM_LUGARES_CABALLOS		10//Cantidad de lugares disponibles del juego
#define TIEMPO_DE_JUEGO_CABALLOS	1//Numero de minutos que esta en funcionamiento el juego una vez que comience



#define ABIERTO  1
#define CERRADO  0

struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

// fun

// function to create a queue of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(struct Queue* queue)
{  return (queue->size == queue->capacity);  }

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{  return (queue->size == 0); }

void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
   // printf("%d enqueued to queue\n", item);
}


int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}


void inicializaArreglo(int arreglo[], int tam){ //llena los arreglos con ceros

	int i;
	for(i=0; i<tam; i++){
		arreglo[i]=0;
		printf("%d",arreglo[i]);


	}
}



int main(int argc, char* argv[]) {

	int tid; /* rank of process */
	int size; /* number of processes */
	int horaActual = HORA_INICIO_DIA;




	int cupoRusa = 0;
	int cupoFilaRusa = 0;

	int genteFeria = 0;

	////////////////////////////MODIFICAR//////////////////////////
	int estadoFeria = ABIERTO;   //0 FALSE   1 TRUE
	/////////////////////////////////////////////////////////////

	MPI_Status status; /* return status for receive */
	MPI_Request request;

	/* start up MPI */

	MPI_Init(&argc, &argv);

	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &tid);
	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	if (tid == 0) {			//Tiempo
		int cliente;
		int bandera = 0; //0 FALSE  1 TRUE
		//MPI_Send()//MPI_Send();;
		for (horaActual = HORA_INICIO_DIA; horaActual < HORA_FIN_DIA;horaActual++) {
			usleep(1000);
			//printf("La hora actual es: %d:%d\n",horaActual/60,horaActual%60);
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &bandera,&status);
			if (bandera != 0) {
				MPI_Recv(&cliente, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
				MPI_Isend(&horaActual, 1, MPI_INT, status.MPI_SOURCE, 23,MPI_COMM_WORLD, &request);
				//MPI_Send(&horaActual, 1, MPI_INT, status.MPI_SOURCE,23, MPI_COMM_WORLD); //Libera, manda lavrg
				bandera = 0;
			}
		}
		printf("Soy el proceso %d , la feria deberia estar cerrada \n",tid);
		estadoFeria = CERRADO;

		//Libera, manda lavrg Mandar la instruccion para hacer el sierre del parque.
	} else if( tid == PASILLO){
		int cliente;
		int horaLocal = 0;
		int bandera = 0;
		while(horaLocal < 1350){
			MPI_Send(&tid, 1, MPI_INT, 0 ,23, MPI_COMM_WORLD);
			MPI_Recv(&horaLocal, 1, MPI_INT, 0 , MPI_ANY_TAG,MPI_COMM_WORLD, &status);
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &bandera,&status);
			if (bandera != 0) {
				bandera = 0;
				MPI_Recv(&cliente, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
			}
			usleep(1000);
			MPI_Isend(&cliente, 1, MPI_INT, cliente, 23,MPI_COMM_WORLD, &request);

		}


	}else if( tid == CABALLOS){
	    struct Queue* fila = createQueue(FILA_CABALLOS);
	    struct Queue* cupo = createQueue(NUM_LUGARES_CABALLOS);

		int i;
		int despachado;
		int cliente;
		int horaLocal = 0;
		int bandera = 0;
		int horaEspera = 0;

		while(horaLocal < 1350){

			MPI_Send(&tid, 1, MPI_INT, 0 ,23, MPI_COMM_WORLD);
			MPI_Recv(&horaLocal, 1, MPI_INT, 0 , MPI_ANY_TAG,MPI_COMM_WORLD, &status);

			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &bandera,&status);
			if (bandera != 0) {
				bandera = 0;
				MPI_Recv(&cliente, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

				if(!isFull(cupo)){ //PASAN DIRECTO AL JUEGO
					enqueue(cupo,cliente);
					printf("Soy %d y estoy esperando dentro de caballos \n",cliente);

				}else if( isFull(cupo) ){  //YA ESTA LLENO EL JUEGO VA EMPEZAR
					printf("El caballo va a empezar\n");
						while(!isEmpty(cupo)){
							despachado = dequeue(cupo);
							printf("Soy el proceso %d y me estoy divirtiendo en caballos \n",despachado);
							MPI_Isend(&despachado, 1, MPI_INT,despachado  , 23 ,MPI_COMM_WORLD, &request);//LIBERA A TODOS

						}

				}else if (!isFull(fila)){ //SI LA FILA NO ESTA LLENA SE FORMAN
					enqueue(fila,cliente);
					printf("Soy %d y estoy esperando en la fila de caballos\n",cliente);


				}else if(isFull(fila)){ //Si la fila esta llena se van
					printf("NO HAY LUGAR EN LA FILA \n");
					MPI_Isend(&cliente, 1, MPI_INT, cliente, 23,MPI_COMM_WORLD, &request);
				}
			}
			if(horaLocal > 1300){
			while(!isEmpty(cupo) || !isEmpty(fila)){
							printf("ESTOY SACANDO A TODOS \n");
							if(!isEmpty(cupo)){
								despachado = dequeue(cupo);
								MPI_Isend(&despachado, 1, MPI_INT,despachado  , 23 ,MPI_COMM_WORLD, &request);//LIBERA A TODOS

							}
							if(!isEmpty(fila)){
								despachado = dequeue(cupo);
								MPI_Isend(&despachado, 1, MPI_INT,despachado  , 23 ,MPI_COMM_WORLD, &request);//LIBERA A TODOS
								dequeue(fila);
							}


					}
			}
		}
			printf("soy %d fuera del while \n",CABALLOS);


	}else if( tid == RUSA){
	    struct Queue* fila = createQueue(FILA_CABALLOS);
	    struct Queue* cupo = createQueue(NUM_LUGARES_CABALLOS);

		int i;
		int despachado;
		int cliente;
		int horaLocal = 0;
		int bandera = 0;
		int horaEspera = 0;

		while(horaLocal < 1350){

			MPI_Send(&tid, 1, MPI_INT, 0 ,23, MPI_COMM_WORLD);
			MPI_Recv(&horaLocal, 1, MPI_INT, 0 , MPI_ANY_TAG,MPI_COMM_WORLD, &status);

			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &bandera,&status);
			if (bandera != 0) {
				bandera = 0;
				MPI_Recv(&cliente, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

				if(!isFull(cupo)){ //PASAN DIRECTO AL JUEGO
					enqueue(cupo,cliente);
					printf("Soy %d y estoy esperando dentro de rusa \n",cliente);

				}else if( isFull(cupo) ){  //YA ESTA LLENO EL JUEGO VA EMPEZAR
					printf("El rusa va a empezar\n");
						while(!isEmpty(cupo)){
							despachado = dequeue(cupo);
							printf("Soy el proceso %d y me estoy divirtiendo en rusa \n",despachado);
							MPI_Isend(&despachado, 1, MPI_INT,despachado  , 23 ,MPI_COMM_WORLD, &request);//LIBERA A TODOS

						}

				}else if (!isFull(fila)){ //SI LA FILA NO ESTA LLENA SE FORMAN
					enqueue(fila,cliente);
					printf("Soy %d y estoy esperando en la fila de rusa\n",cliente);


				}else if(isFull(fila)){ //Si la fila esta llena se van
					printf("NO HAY LUGAR EN LA FILA \n");
					MPI_Isend(&cliente, 1, MPI_INT, cliente, 23,MPI_COMM_WORLD, &request);
				}
			}
			if(horaLocal > 1300){
			while(!isEmpty(cupo) || !isEmpty(fila)){
							printf("ESTOY SACANDO A TODOS \n");
							if(!isEmpty(cupo)){
								despachado = dequeue(cupo);
								MPI_Isend(&despachado, 1, MPI_INT,despachado  , 23 ,MPI_COMM_WORLD, &request);//LIBERA A TODOS

							}
							if(!isEmpty(fila)){
								despachado = dequeue(cupo);
								MPI_Isend(&despachado, 1, MPI_INT,despachado  , 23 ,MPI_COMM_WORLD, &request);//LIBERA A TODOS
								dequeue(fila);
							}


					}
			}
		}
			printf("soy %d fuera del while \n",RUSA);


	}else {
		int decision;
		int horaLocal = 0;
		while(horaLocal < 1350){
			MPI_Send(&tid, 1, MPI_INT, 0 ,23, MPI_COMM_WORLD);
			MPI_Recv(&horaLocal, 1, MPI_INT, 0 , MPI_ANY_TAG,MPI_COMM_WORLD, &status);
			decision=(rand()%4)+1;
			usleep(1000);

			//1 PASILLO
			//2 MOTANA
			//3 CABALLO
			//4 IRSE
				printf("Soy el proceso %d ,ire a %d y la hora es %d:%d \n",tid,decision,horaLocal/60,horaLocal%60);

				if(decision == CABALLOS){
					MPI_Isend(&tid, 1, MPI_INT, CABALLOS, 23,MPI_COMM_WORLD, &request);
					MPI_Recv(&tid, 1, MPI_INT, CABALLOS , MPI_ANY_TAG,MPI_COMM_WORLD, &status);
					printf("Soy el proceso %d y estoy fuera de caballos ME VOY COMPAS \n",tid);
				}
				if(decision == RUSA){
					MPI_Isend(&tid, 1, MPI_INT, RUSA, 23,MPI_COMM_WORLD, &request);
					MPI_Recv(&tid, 1, MPI_INT, RUSA , MPI_ANY_TAG,MPI_COMM_WORLD, &status);
					printf("Soy el proceso %d y estoy fuera de caballos ME VOY COMPAS \n",tid);
				}
				if(decision == PASILLO){

					MPI_Isend(&tid, 1, MPI_INT, CABALLOS, 23,MPI_COMM_WORLD, &request);
					MPI_Recv(&tid, 1, MPI_INT, CABALLOS , MPI_ANY_TAG,MPI_COMM_WORLD, &status);
					printf("Soy el proceso %d y estoy fuera de caballos ME VOY COMPAS \n",tid);
				}
				if(decision == 4){
					printf("SOY %d Y NO QUIERO ENTRAR A NINGUN JUEGO A LA VERGA COMPA \n",tid);
				}

		}



	}

	if(tid == CABALLOS)
		printf("soy CABALLOS FUERA DE LA FERIA%d\n",CABALLOS);

	MPI_Finalize();

	return 0;
}





