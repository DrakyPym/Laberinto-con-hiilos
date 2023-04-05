#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
//#include <stdlib.h>

#define TAMANO_Y 20
#define TAMANO_X 20

int abortar = 0;                      // Variable que determinara si abortara un hilo
int laberinto[TAMANO_Y][TAMANO_X] = { // Laberinto a recorrer
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ,1 ,0},
    {0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 ,1 ,0},
    {0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1 ,1 ,0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 ,1 ,0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 ,1 ,0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0 ,1 ,0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 ,1 ,0},
    {0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 ,0 ,0},
    {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 ,0 ,0},
    {0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

struct datosHilo
{ // Estructura que almacena los datos que se le pasaran al hilo
    int *posXH;
    int *posYH;
    int posFXH;
    int posFYH;
};

// Prototipos de las funciones
void imprimirLaberinto();
void mover(int *posX, int *posY, int direccion);
void *recorridoHiloDerecha(void *arg);
void *recorridoHiloIzquierda(void *arg);
void *recorridoHiloArriba(void *arg);
void *recorridoHiloAbajo(void *arg);
void recorrerLab(int *posX, int *posY, int posFinalX, int posFinalY);

int main()
{
    int posX = 1;
    int posY = 1;
    //int posFinalX = 18;
    //int posfinalY = 18;
    int posFinalX = 18;
    int posfinalY = 1;

    recorrerLab(&posX, &posY, posFinalX, posfinalY);
    imprimirLaberinto();
    //printf("Valor de abortar: %i\n", abortar);
    return 0;
}

void imprimirLaberinto()
{
    printf("\n");
    int *p = &laberinto[0][0]; // Se inicializa el puntero al inicio de la matriz
    int i;
    for (i = 0; i < TAMANO_X * TAMANO_Y; i++)
    {
        printf("%i ", *p); // Se imprime el valor al que apunta el puntero
        p++;               // Se incrementa el puntero para apuntar al siguiente elemento de la matriz
        if ((i + 1) % TAMANO_X == 0)
        {                 // Se verifica si se llegó al final de una fila
            printf("\n"); // se imprime un salto de línea para empezar la siguiente fila
        }
    }
    printf("\n");
}

void mover(int *posX, int *posY, int direccion)
{
    switch (direccion)
    {
    case 0: // Izquierda
        if (laberinto[*posY][(*posX) - 1] == 1)
        {                                // verifica si se puede mover y de ser posible se mueve
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
            (*posX)--;                   // Se mueve
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
        }
        else
            printf("\nError en el movimiento, choca con una pared\n");
        break;
    case 1: // Derecha
        if (laberinto[*posY][(*posX) + 1] == 1)
        {                                // verifica si se puede mover y de ser posible se mueve
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
            (*posX)++;                   // Se mueve
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
        }
        else
            printf("\nError en el movimiento, choca con una pared\n");
        break;
    case 2: // Abajo
        if (laberinto[(*posY) + 1][*posX] == 1)
        {                                // verifica si se puede mover y de ser posible se mueve
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
            (*posY)++;
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
        }
        else
            printf("\nError en el movimiento, choca con una pared\n");
        break;
    case 3: // Arriba
        if (laberinto[(*posY) - 1][*posX] == 1)
        {                                // verifica si se puede mover y de ser posible se mueve
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
            (*posY)--;
            laberinto[*posY][*posX] = 2; // Se asigna un 2 a la posicion actual, para ir dejando un rastro
        }
        else
            printf("\nError en el movimiento, choca con una pared\n");
        break;
    default:
        printf("\nError en el movimiento, no se esta recibiendo un parametro valido de la funcion mover\n");
        break;
    }
}

void *recorridoHiloDerecha(void *arg)
{
    struct datosHilo *datos = (struct datosHilo *)arg;
    if (abortar == 1)
        pthread_exit(NULL); // Finaliza el hilo
    int posX = *(datos->posXH);
    int posY = *(datos->posYH);
    int posFinalX = datos->posFXH;
    int posFinalY = datos->posFYH;
    mover(&posX, &posY, 1);
    recorrerLab(&posX, &posY, posFinalX, posFinalY);
    return NULL;
}

void *recorridoHiloIzquierda(void *arg)
{
    struct datosHilo *datos = (struct datosHilo *)arg;
    if (abortar == 1)
        pthread_exit(NULL); // finaliza el hilo
    int posX = *(datos->posXH);
    int posY = *(datos->posYH);
    int posFinalX = datos->posFXH;
    int posFinalY = datos->posFYH;
    mover(&posX, &posY, 0);
    recorrerLab(&posX, &posY, posFinalX, posFinalY);
    return NULL;
}

void *recorridoHiloArriba(void *arg)
{
    struct datosHilo *datos = (struct datosHilo *)arg;
    if (abortar == 1)
        pthread_exit(NULL); // Finaliza el hilo
    int posX = *(datos->posXH);
    int posY = *(datos->posYH);
    int posFinalX = datos->posFXH;
    int posFinalY = datos->posFYH;
    mover(&posX, &posY, 3);
    recorrerLab(&posX, &posY, posFinalX, posFinalY);
    return NULL;
}

void *recorridoHiloAbajo(void *arg)
{
    struct datosHilo *datos = (struct datosHilo *)arg;
    if (abortar == 1)
        pthread_exit(NULL); // Flinaliza el hilo
    int posX = *(datos->posXH);
    int posY = *(datos->posYH);
    int posFinalX = datos->posFXH;
    int posFinalY = datos->posFYH;
    mover(&posX, &posY, 2);
    recorrerLab(&posX, &posY, posFinalX, posFinalY);

    return NULL;
}

void recorrerLab(int *posX, int *posY, int posFinalX, int posFinalY)
{
    pthread_t hilo1 = 0; // Identificador del hilo1
    int inicioHilo1 = 0; // Variable que determina si el hijo1 ha sido creado
    pthread_t hilo2 = 0;
    int inicioHilo2 = 0;
    pthread_t hilo3 = 0;
    int inicioHilo3 = 0;
    pthread_t hilo4 = 0;
    int inicioHilo4 = 0;
    if (posFinalX == (*posX) && posFinalY == (*posY))
    {
        abortar = 1;
        printf("\nHemos llegado al final\n");
        printf("Presione entrar para continuar...");
        getchar();
    }
    else
    {
        //Corrobora para donde hay caminos
        int derecha = 0;
        int izquierda = 0;
        int arriba = 0;
        int abajo = 0;
        int nCaminos = 0;
        if (laberinto[*posY][(*posX) + 1] == 1)
            derecha = 1; // Existe camino a la derecha
        if (laberinto[*posY][(*posX) - 1] == 1)
            izquierda = 1; // Existe camino a la izquierda
        if (laberinto[(*posY) + 1][*posX] == 1)
            abajo = 1; // Existe camino para abajo
        if (laberinto[(*posY) - 1][*posX] == 1)
            arriba = 1;                                  // Existe camino para arriba
        nCaminos = derecha + izquierda + arriba + abajo; // Guarda el numero de caminos
        if (nCaminos == 1)
        { // Solo existe un camino
            if (derecha == 1)
            { // Si existe camino a la derecha, se mueve a la derecha
                mover(posX, posY, 1);
                recorrerLab(posX, posY, posFinalX, posFinalY); // Hace una llamada recursiva a la funcion, para seguir recorriendo el laberinto
            }
            if (izquierda == 1)
            { // Si existe un camino a la izquierda, se mueve a la izquierda
                mover(posX, posY, 0);
                recorrerLab(posX, posY, posFinalX, posFinalY); // Hace una llamada recursiva a la funcion, para seguir recorriendo el laberinto
            }
            if (arriba == 1)
            { // Si existe un camino para arriba, se mueve para arriba
                mover(posX, posY, 3);
                recorrerLab(posX, posY, posFinalX, posFinalY); // Hace una llamada recursiva a la funcion, para seguir recorriendo el laberinto
            }
            if (abajo == 1)
            { // Si existe un camino para abajo, se mueve para abajo
                mover(posX, posY, 2);
                recorrerLab(posX, posY, posFinalX, posFinalY); // Hace una llamada recursiva a la funcion, para seguir recorriendo el laberinto
            }
        }
        else
        {                                                                // Existe mas de un camino
            struct datosHilo datos = {posX, posY, posFinalX, posFinalY}; // Se guardan los datos que se le pasaran al hilo
            if (derecha == 1)
            {                                                                // Hay camino a la derecha
                pthread_create(&hilo1, NULL, &recorridoHiloDerecha, &datos); // Se crea el hilo 1
                inicioHilo1 = 1;
            }
            if (arriba == 1)
            {                                                               // Hay un camino para arriba
                pthread_create(&hilo2, NULL, &recorridoHiloArriba, &datos); // Se crea el hilo 2
                inicioHilo2 = 1;
            }
            if (izquierda == 1)
            {                                                                  // Hay un camino para la izquierda
                pthread_create(&hilo3, NULL, &recorridoHiloIzquierda, &datos); // Se crea el hilo 3
                inicioHilo3 = 1;
            }
            if (abajo == 1)
            {                                                              // Hay un camino para abajo
                pthread_create(&hilo4, NULL, &recorridoHiloAbajo, &datos); // Se crea el hilo 4
                inicioHilo4 = 1;
            }
            if (inicioHilo1 == 1)
                pthread_join(hilo1, NULL); // Espera al hilo 1 si ha sido creado
            if (inicioHilo2 == 1)
                pthread_join(hilo2, NULL); // Espera al hilo 2 si ha sido creado
            if (inicioHilo3 == 1)
                pthread_join(hilo3, NULL); // Espera al hilo 3 si ha sido creado
            if (inicioHilo4 == 1)
                pthread_join(hilo4, NULL); // Espera al hilo 4 si ha sido creado
        }
    }
}