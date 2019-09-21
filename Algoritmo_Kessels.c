#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>

/*
 * U.N.A.M
 * Facultad de ciencias
 * Montoya Montes Pedro
 * 31219536-2
 *
 * Proyecto1, el cual se implemente el algoritmo de torneo, la cual implementa
 * el algoritmo de Kessels para resolver el problema de excluisón mutua.
 */

int main(int argc, char** argv){
	
	//Variables globales
	int numHilos, idHilo;
	int i,nodo,n,local,id;

	//verifica el numero de hilos que se paso como parametro
	if (argc < 2)
	{
		printf("por favor especifique el numero de hilos\n");
		exit(1);
	}

	sscanf(argv[1], "%i", &numHilos);

	if (numHilos < 1)
	{
		printf("Numero de hilos no valido (%i)\n", numHilos);
		exit(1);
	}

	printf ("iniciando calculo con 2 ^ %i hilos\n", numHilos);
	
	//Crea el número de hilos potencia de 2
	omp_set_num_threads(pow(2,numHilos));

	//Creación de las variables	
	long int suma, sumaHilo;
	suma=0;
	n = pow(2,numHilos);
	int b [n-1][2] ;
	int turn [n-1][2];
	int edge [n-1];

	//Inicialización de variables
	for (int entero = 0; entero < n; ++entero)
	{
		b[entero][0]=0;
		b[entero][1]=0;
		turn[entero][0]=0;
		turn[entero][1]=0;
	}

	//inicia seccion paralela
	#pragma omp parallel private(i, local, nodo, id,sumaHilo)
    {
		i = omp_get_thread_num();//obtenemos el hilo
		for (int f_loop = 0; f_loop < 10; ++f_loop)
		{
			nodo = i + n;

			while(nodo > 1)
			{
				id = nodo % 2;
				nodo = nodo/2; 
				b[nodo][id] = 1;
				local = (turn[nodo] [1-id] + id) % 2;//Nos dice que hijo es.
				turn[nodo][id] = local;

				while(!(b[nodo][1- id] == 0 || local != ((turn[nodo][1-id] +id)% 2)))
				{
					//await, sección de espera.
				}

				edge[nodo] = id;
				
				printf("Ganó el hilo con id %i con nodo %i \n", i, nodo);//Imprimimos el hilo ganador con el nodo de donde ganó.
			}//fin while

			//indica que la siguiente seccion se realiza con acceso exclusivo
			printf("Entró el hilo con id %i \n", omp_get_thread_num());
		
			//Sección donde hace una suma, basicamente hacer tiempo.
			sumaHilo = 0;
	 		int s;
	 		for (s = idHilo; s < 1000000000; s+=numHilos)
	 		{
	 			sumaHilo+=i;
	 		}//fin for

			printf("Salió el hilo con id %i \n", omp_get_thread_num());
			
			//Código de salida
			while(nodo < n){
		    	b[nodo][edge[nodo]] = 0;
		    	nodo = 2*nodo + edge[nodo];
		    }//fin while (código salida)
		}//fin for 1-10
	}//fin de seccion paralela
}