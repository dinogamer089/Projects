#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

typedef struct
{
    char User[40];
    int Points;
}Stats;

void Register(Stats Stat);
void Leaderboard();
void Draw(Stats Stat, int x, int y, int coinx, int coiny, int obx, int oby, int ob2x, int ob2y);
void Input(int *p);
void Logic(int p, int *x, int *y, int *g, int *coinx, int *coiny, Stats *Stat, int *obx, int *oby, int *ob2x, int *ob2y);
void Startup(int *x, int *y, int *coinx, int *coiny, Stats *Stat, int *obx, int *oby, int *ob2x, int *ob2y);
void Organize();

int main()
{
    Stats Stat;
    int x, y, opc, flag, coinx, coiny, obx, oby, ob2x, ob2y;

    do
    {
        int gameover = 0;
        //menu
        puts("1) Play");
        puts("2) Leaderboard");
        puts("3) Exit");
        scanf("%d", &opc);
        switch (opc)
        {
        case 1:
            //juego
            Startup(&x, &y, &coinx, &coiny, &Stat, &obx, &oby, &ob2x, &ob2y);
            do
            {
                Draw(Stat, x, y, coinx, coiny, obx, oby, ob2x, ob2y);
                Input(&flag);
                Logic(flag, &x, &y, &gameover, &coinx, &coiny, &Stat, &obx, &oby, &ob2x, &ob2y);
            } while (gameover == 0);
            Register(Stat);
            system("cls");
            break;
        case 2:
            //organizar y despues mostrar leaderboard
            Organize();
            Leaderboard(&Stat);
            break;
        case 3:
            //terminar el juego
            puts("Exiting...");
            break;
        default:
            puts("Option invalid.");
            break;
        }
    } while (opc != 3);   
}

void Register(Stats Stat)
{
    FILE *binario, *temp;
    Stats Stat2;
    int flag = 0;
    //crear archivo "binario"
    binario = fopen("Stats.dat", "ab+");
    if (binario == NULL)
    {
        printf("Couldn't open file");
        exit(1);
    }
    //crear archivo temporal
    temp = fopen("temp.dat", "wb+");
    if (temp == NULL)
    {
        fclose(binario);
        printf("Couldn't open temp file");
        exit(1);
    }

    printf("\nUsername: ");
    scanf("%39s", Stat.User);

    rewind(binario);
    while (fread(&Stat2, sizeof(Stats), 1, binario) > 0)
    {   //checar si nuevo username ya existe
        if (strcmp(Stat.User, Stat2.User) == 0)
        {
           if (Stat.Points > Stat2.Points)
            {
                fwrite(&Stat, sizeof(Stats), 1, temp);
            }
            else
            {
                fwrite(&Stat2, sizeof(Stats), 1, temp);
            }
            //asignar bandera
            flag = 1;
        }
        else
        {
            fwrite(&Stat2, sizeof(Stats), 1, temp);
        }
    }

    if (!flag)
    {
        //Si no se encuentra usuario, escribe el nuevo dato
        fseek(temp, 0, SEEK_END);
        fwrite(&Stat, sizeof(Stats), 1, temp);
    }

    fclose(binario);
    fclose(temp);

    //Reemplazar el archivo viejo por el nuevo
    remove("Stats.dat");
    rename("temp.dat", "Stats.dat");
}

void Leaderboard()
{
    FILE *binario;
    Stats Stat;
    //Abrir el archivo e imprimir todos los datos de los usuarios
    system("cls");
    if((binario=fopen("Stats.dat","rb")) == NULL)
    {
        printf("Couldn't open file");
        exit(1);
    }
    else
    {
        printf("------------------------- \n");
        printf("| User \t\tPoints \t| \n");
        while(fread(&Stat, sizeof(Stats),1,binario)>0)
        {
            printf("| %-6s \t%d \t|\n",Stat.User, Stat.Points);
        }
        printf("------------------------- \n");
        fclose(binario);
    }
}

void Draw(Stats Stat, int x, int y, int coinx, int coiny, int obx, int oby, int ob2x, int ob2y)
{
    //Dibujar el juego
    int width = 30, height = 10, b = 0, a = 0;

    system("cls");
    for (a = 0; a < height; a++) 
    {
        for (b = 0; b < width; b++ ) 
        {

            if ((a == 0) || (a == height-1) || (b == width-1) || (b == 0))
            {  
                printf("*");
            }
            else
            {
                if ((a == x) && (b == y))
                {
                    printf("0");
                }
                else if ((a == coiny) && (b == coinx))
                {
                    printf("#");
                }
                else if ((a == oby) && (b == obx) && (oby != coiny) && (oby != ob2y))
                {
                    printf("1");
                }
                else if ((a == ob2y) && (b == ob2x) && (ob2y != coiny) && (ob2y != oby))
                {
                    printf("1");
                }
                
                else
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    printf("Score: %d", Stat.Points);
}

void Input(int *p) 
{
    //detectar la tecla presionada
    *p = 0;

    if (kbhit()) { 
        switch (getch()) {  
        case 's': 
            *p = 1; 
            break; 
        case 'w': 
            *p = 2; 
            break; 
        } 
    } 
}

void Logic(int p, int *x, int *y, int *g, int *coinx, int *coiny, Stats *Stat, int *obx, int *oby, int *ob2x, int *ob2y) 
{ 
    //comandos de logica como el movimiento del "carro" o de la moneda
    int width = 30, height = 10;
 
    switch (p) 
    { 
    case 1: 
        (*x)++; 
        break; 
    case 2: 
        (*x)--; 
        break;  
    default: 
        break; 
    }

    srand(time(NULL));
    if ((*coiny == *x) && (*y == *coinx))
    {
        Stat->Points++;
        *coinx = 28;
        *coiny = rand() % 8;
    }
    else
    {
        (*coinx)--;
    }
    if (*coinx == 0)
    {
        *coinx = 28;
        *coiny = rand() % 8;
    }
    if (((*oby == *x) && (*y == *obx)) || ((*ob2y == *x) && (*y == *ob2x)))
    {
        *g = 1;
    }
    else
    {
        (*obx)--;
        (*ob2x)--;
    }
    if (*obx == 0)
    {
        *obx = 28;
        *oby = rand() % 8;
        while (*oby == *coiny)
        {
            *oby = rand() % 8;
        }
    }
    if (*ob2x == 0)
    {
        *ob2x = 28;
        *ob2y = rand() % 8;
        while (*ob2y == *coiny)
        {
            *ob2y = rand() % 8;
        }
    }
    if (*x < 1 || *x >= height - 1 || *y < 1 || *y >= width - 1)
    {
        *g = 1;
    }
    sleep(0.8);
}

void Startup(int *x, int *y, int *coinx, int *coiny, Stats *Stat, int *obx, int *oby, int *ob2x, int *ob2y)
{
    //iniciar los valores de todo
    int width = 30, height = 10;

    srand(time(NULL));
    *x = height / 2;
    *y = width / 2;
    *coiny = rand() % 8;
    *coinx = 28;
    Stat->Points = 0;
    *obx = 28;
    *oby = rand() % 8;
    *ob2x = 28;
    *ob2y = rand() % 8;
}

void Organize()
{
    //Organizar los usuarios por puntos
    FILE *binario;
    Stats Stat, Stat2, aux;
    int x, y, n;

    if ((binario = fopen("Stats.dat", "rb+")) == NULL)
    {
        printf("Couldn't open file"); 
        exit(1);
    }
    else
    {
        fseek(binario, 0L, 2);
        n = ftell(binario)/sizeof(Stats);
        rewind(binario);
        for (x = 1; x < n; x++)
        {
            for (y = 0; y < n - x; y++)
            {
                fseek(binario, y*sizeof(Stats), 0);
                fread(&Stat, sizeof(Stats), 1, binario);
                fseek(binario, (y+1)*sizeof(Stats), 0);
                fread(&Stat2, sizeof(Stats), 1, binario);
                if (Stat.Points < Stat2.Points)
                {
                    aux = Stat;
                    Stat = Stat2;
                    Stat2 = aux;
                    fseek(binario, y * sizeof(Stats), 0);
                    fwrite(&Stat, sizeof(Stats), 1, binario);
                    fseek(binario, (y + 1) * sizeof(Stats), 0);
                    fwrite(&Stat2, sizeof(Stats), 1, binario);
                }
            }
        }
        fclose(binario);
    }
    
}