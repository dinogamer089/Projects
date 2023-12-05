#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <windows.h>

typedef struct
{
    char User[40];
    int Points;
}Stats;

void Register(Stats Stat);
void Leaderboard();
void Draw(Stats Stat, int *x, int *y, int coinx, int coiny);
void Input(int *p);
void Logic(int p, int *x, int *y, int *g, int *coinx, int *coiny, Stats *Stat);
void Startup(int *x, int *y, int *coinx, int *coiny, Stats *Stat);
void Modify(Stats Stat, int pos);
void Organize();

int main()
{
    Stats Stat;
    int x, y, opc, flag, coinx, coiny;

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
            Startup(&x, &y, &coinx, &coiny, &Stat);
            do
            {
                Draw(Stat, &x, &y, coinx, coiny);
                Input(&flag);
                Logic(flag, &x, &y, &gameover, &coinx, &coiny, &Stat);
            } while (gameover == 0);
            Register(Stat);
            system("cls");
            break;
        case 2:
            //leaderboard
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
    //registro del usuario y se supone que puntos
    FILE *binario;
    Stats Stat2;
    int pos = -1, nr, flag = 0;

    binario = fopen("Stats.dat", "ab");
    if (binario == NULL)
    {
        printf("Couldn't open file");
        exit(1);
    }
    printf("\nUsername: ");
    scanf("%39s", Stat.User);
    fclose(binario);
    binario = fopen("Stats.dat", "rb+");
    if (binario == NULL)
    {
        printf("Couldn't open file");
        exit(1);
    }
    while(fread(&Stat2, sizeof(Stats),1,binario)>0)
    {
        if(strcmp(Stat.User, Stat2.User) == 0)
        {
            pos = nr;
            Modify(Stat, pos);
            flag = 1;
        }
        nr++;
    }
    if (!flag)
    {
        fwrite(&Stat, sizeof(Stats), 1, binario);
    }
    fclose(binario);
}

void Leaderboard()
{
    FILE *binario;
    Stats Stat;
    //abrir el archivo e imprimir todos los datos de los usuarios
    system("cls");
    if((binario=fopen("Stats.dat","rb")) == NULL)
    {
        printf("Couldn't open file");
        exit(1);
    }
    else
    {
        while(fread(&Stat, sizeof(Stats),1,binario)>0)
        {
            printf("%s \t %d \n",Stat.User, Stat.Points);
        }
        fclose(binario);
    }
}

void Draw(Stats Stat, int *x, int *y, int coinx, int coiny)
{
    //dibujar el juego
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
                if ((a == *x) && (b == *y))
                {
                    printf("0");
                }
                else if ((a == coiny) && (b == coinx))
                {
                    printf("#");
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

void Logic(int p, int *x, int *y, int *g, int *coinx, int *coiny, Stats *Stat) 
{ 
    //comandos de logica como el movimiento del "carro" o de la moneda
    int width = 30, height = 10;
 
    switch (p) { 
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
    
    if (*x < 1 || *x >= height - 1 || *y < 1 || *y >= width - 1)
    {
        *g = 1;
    }
    sleep(0.8);
}

void Startup(int *x, int *y, int *coinx, int *coiny, Stats *Stat)
{
    //iniciar los valores de todo
    int width = 30, height = 10;

    srand(time(NULL));
    *x = height / 2;
    *y = width / 2;
    *coiny = rand() % 8;
    *coinx = 28;
    Stat->Points = 0;
}

void Modify(Stats Stat, int pos)
{
    FILE *binario;

    if( (binario=fopen("Stats.dat","rb+"))==NULL)
    {
        printf("Couldn't open file"); 
        exit(1);
    }
    else
    {
        fseek(binario, pos*sizeof(Stats),0);
        fwrite(&Stat, sizeof(Stats),1,binario);
        fclose(binario);
    }
}

void Organize()
{
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