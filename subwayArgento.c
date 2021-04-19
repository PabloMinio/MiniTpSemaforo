#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50
sem_t mutex1;  // validar si esta en uso o no

//creo estructura de semaforos
struct semaforos
{
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_agregarCarne;
    sem_t sem_empanizar;
    sem_t sem_fritar;

};

//creo los pasos con los ingredientes
struct paso
{
    char accion [LIMITE];
    char ingredientes[4][LIMITE];

};

//creo los parametros de los hilos
struct parametro
{
    int equipo_param;
    struct semaforos semaforos_param;
    struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn)
{
    struct parametro *mydata = data;

    int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0])); //calculo la longitud del array de pasos

    int i;	 //indice para recorrer array de pasos

    for(i = 0; i < sizeArray; i ++)
    {

        if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0) 	//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
        {
            printf("\n");
            printf("\tEquipo %d - accion %s \n ", mydata->equipo_param, mydata->pasos_param[i].accion);

            int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) ); //calculo la longitud del array de ingredientes

            int h; //indice para recorrer array de ingredientes

            printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
            for(h = 0; h < sizeArrayIngredientes; h++)
            {

                if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0)   //consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
                {
                    printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
                }
            }
        }
    }
}

void* cortar(void *data)
{
    char *accion = "cortar";	//creo el nombre de la accion de la funcion
    struct parametro *mydata = data;	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
    imprimirAccion(mydata,accion);	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
    usleep( 50000 );	//uso sleep para simular que pasa tiempo
    sem_post(&mydata->semaforos_param.sem_mezclar); 	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    pthread_exit(NULL);
}

void* mezclar(void *data)
{
    char *accion = "mezclar"; 	//creo el nombre de la accion de la funcion
    struct parametro *mydata = data; 	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
    sem_wait (&mydata->semaforos_param.sem_mezclar);  //espera a que la accion cortar lo active
    imprimirAccion(mydata,accion);	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
    usleep( 20000 ); 	//uso sleep para simular que pasa tiempo
    sem_post(&mydata->semaforos_param.sem_salar);
    pthread_exit(NULL);
}

void* salar (void *data)
{
    char *accion = "salar";		//creo el nombre de la accion de la funcion
    struct parametro *mydata = data;	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
    sem_wait (&mydata->semaforos_param.sem_salar);  //espera a que la accion mezclar lo active
    sem_wait (&mutex1);
    printf("\n");
    printf("\tEquipo %d --Salando la mezcla--\n",mydata->equipo_param);
    usleep( 60000 );		//uso sleep para simular que pasa tiempo
    sem_post (&mutex1);
    sem_post(&mydata->semaforos_param.sem_agregarCarne); //doy la señal a la siguiente accion (salar me habilita agregarCarne)
    pthread_exit(NULL);
}

void* agregarCarne (void *data)
{
    char *accion = "agregarCarne";	//creo el nombre de la accion de la funcion
    struct parametro *mydata = data;	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
    sem_wait (&mydata->semaforos_param.sem_agregarCarne);  //espera a que la accion mezclar lo active
    printf("\n");
    printf("\tEquipo %d --Agregando carne a la mezcla--\n",mydata->equipo_param);
    usleep( 40000 );	//uso sleep para simular que pasa tiempo
    sem_post(&mydata->semaforos_param.sem_empanizar);	//doy la señal a la siguiente accion (agregarCarne me habilita empanizar)
    pthread_exit(NULL);
}

void* empanizar (void *data)
{

    char *accion = "empanizar";	//creo el nombre de la accion de la funcion
    struct parametro *mydata = data;	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
    sem_wait (&mydata->semaforos_param.sem_empanizar);  //espera a que la accion mezclar lo active
    printf("\n");
    printf("\tEquipo %d --Empanando la carne--\n",mydata->equipo_param);
    usleep( 80000 );	//uso sleep para simular que que pasa tiempo
    sem_post(&mydata->semaforos_param.sem_fritar);	//doy la señal a la siguiente accion (empanizar me habilita fritar)
    pthread_exit(NULL);
}

void* fritar (void *data)
{
    char *accion = "fritar";	//creo el nombre de la accion de la funcion
    struct parametro *mydata = data;	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
    sem_wait (&mydata->semaforos_param.sem_fritar);  //espera a que la accion mezclar lo active
    sem_wait (&mutex1);
    printf("\n");
    printf("\tEquipo %d --Cocinando la milanesa--\n",mydata->equipo_param);
    usleep( 300000 ); 	//uso sleep para simular que que pasa tiempo
    sem_post (&mutex1);
    printf("\n");
    printf("\tEquipo %d --Milanesa cocinada--\n",mydata->equipo_param);

    pthread_exit(NULL);
}

void* cortarExtras(void *data)
{
	char *accion = "cortarExtras";	//creo el nombre de la accion de la funcion
	struct parametro *mydata = data; 	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
	imprimirAccion(mydata,accion); 	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	usleep( 50000 );	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
    pthread_exit(NULL);
}

void* ejecutarReceta(void *i)
{
    //variables semaforos
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_agregarCarne;
    sem_t sem_empanizar;
    sem_t sem_fritar;

    //variables hilos
    pthread_t p1;
    pthread_t p2;
    pthread_t p3;
    pthread_t p4;
    pthread_t p5;
    pthread_t p6;
    pthread_t p7;

    //numero del equipo (casteo el puntero a un int)
    int p = *((int *) i);

    printf("Ejecutando equipo %d \n", p);

    //reservo memoria para el struct
    struct parametro *pthread_data = malloc(sizeof(struct parametro));

    //seteo numero de grupo
    pthread_data->equipo_param = p;

    //seteo semaforos
    pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
    pthread_data->semaforos_param.sem_salar = sem_salar;
    pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
    pthread_data->semaforos_param.sem_empanizar = sem_empanizar;
    pthread_data->semaforos_param.sem_fritar = sem_fritar;

    //seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
    strcpy(pthread_data->pasos_param[0].accion, "cortar");
    strcpy(pthread_data->pasos_param[0].ingredientes[0], "diente de ajo 1");
    strcpy(pthread_data->pasos_param[0].ingredientes[1], "diente de ajo 2");
    strcpy(pthread_data->pasos_param[0].ingredientes[2], "perejil");

    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
    strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo cortado");
    strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
    strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	
	strcpy(pthread_data->pasos_param[5].accion, "cortarExtras");
	strcpy(pthread_data->pasos_param[5].ingredientes[0], "lechuga");
	strcpy(pthread_data->pasos_param[5].ingredientes[1], "tomate");
	strcpy(pthread_data->pasos_param[5].ingredientes[2], "pepino");
	strcpy(pthread_data->pasos_param[5].ingredientes[3], "cebolla morada");

    //inicializo los semaforos
    sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_empanizar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_fritar),0,0);

    //creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos
    int rc;
    rc = pthread_create(&p1,                                //identificador unico
                        NULL,                          //atributos del thread
                        cortar,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

    rc = pthread_create(&p2,                                //identificador unico
                        NULL,                          //atributos del thread
                        mezclar,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

    rc = pthread_create(&p3,                                //identificador unico
                        NULL,                          //atributos del thread
                        salar,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p4,                                //identificador unico
                        NULL,                          //atributos del thread
                        agregarCarne,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p5,                                //identificador unico
                        NULL,                          //atributos del thread
                        empanizar,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p6,                                //identificador unico
                        NULL,                          //atributos del thread
                        fritar,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p7,                                //identificador unico
                        NULL,                          //atributos del thread
                        cortarExtras,                    //funcion a ejecutar
                        pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

    //join de todos los hilos
    pthread_join (p1,NULL);
    pthread_join (p2,NULL);
    pthread_join (p3,NULL);
    pthread_join (p4,NULL);
    pthread_join (p5,NULL);
    pthread_join (p6,NULL);
	pthread_join (p7,NULL);

    //valido que el hilo se alla creado bien
    if (rc)
    {
        printf("Error:unable to create thread, %d \n", rc);
        exit(-1);
    }

    //destruccion de los semaforos
    sem_destroy(&sem_mezclar);
    sem_destroy(&sem_salar);
    sem_destroy(&sem_agregarCarne);
    sem_destroy(&sem_empanizar);
    sem_destroy(&sem_fritar);

    //salida del hilo
    pthread_exit(NULL);
}


int main ()
{
    int rc;
	
    int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
    int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
    int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
    int *equipoNombre4 =malloc(sizeof(*equipoNombre4));

    *equipoNombre1 = 1;
    *equipoNombre2 = 2;
    *equipoNombre3 = 3;
    *equipoNombre4 = 4;

    pthread_t equipo1;
    pthread_t equipo2;
    pthread_t equipo3;
    pthread_t equipo4;

    sem_init(&mutex1, 0, 1);

    rc = pthread_create(&equipo1,                          //identificador unico
                        NULL,                          //atributos del thread
                        ejecutarReceta,             //funcion a ejecutar
                        equipoNombre1);

    rc = pthread_create(&equipo2,                           //identificador unico
                        NULL,                          //atributos del thread
                        ejecutarReceta,             //funcion a ejecutar
                        equipoNombre2);
    rc = pthread_create(&equipo3,                           //identificador unico
                        NULL,                          //atributos del thread
                        ejecutarReceta,             //funcion a ejecutar
                        equipoNombre3);
	rc = pthread_create(&equipo4,                           //identificador unico
                                NULL,                          //atributos del thread
                                    ejecutarReceta,             //funcion a ejecutar
                                    equipoNombre4); 

    if (rc)
    {
        printf("Error:unable to create thread, %d \n", rc);
        exit(-1);
    }

    pthread_join (equipo1,NULL);
    pthread_join (equipo2,NULL);
    pthread_join (equipo3,NULL);
    pthread_join (equipo4,NULL);

    pthread_exit(NULL);
}

