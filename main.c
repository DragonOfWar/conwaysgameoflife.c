#include <stdio.h>
#include <stdlib.h>

void rendergrid(int **grid, int m, int n);
void mallocmatrix(int ***matrix, int m, int n);
void gridcreationmenu(int ***grid, int *m, int *n);
void populategridmenu(int **grid, int n, int m);
void simulate(int **grid, int n, int m);
void freematrix(int **matrix, int m);
void clearbuffer();
int getlivingneighbours(int **grid, int m, int n, int cellx, int celly);
int clamp(int n, int min, int max);

int main()
{
    int **grid, m, n;

    system("clear");
    gridcreationmenu(&grid, &m, &n);
    populategridmenu(grid, m, n);
    clearbuffer();
    simulate(grid, m, m);
    freematrix(grid, m);

    return 0;
}

void rendergrid(int **grid, int m, int n)
{
    int i, j;
    system("clear");

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%c", (grid[i][j] == 1) ? '#' : '.');
        printf("\n");
    }
}

void gridcreationmenu(int ***grid, int *m, int *n)
{
    printf("Grid width: ");
    scanf("%d", n);
    printf("Grid height: ");
    scanf("%d", m);

    mallocmatrix(grid, *m, *n);
}

void populategridmenu(int **grid, int m, int n)
{
    int i, j;
    printf("Type the location of live cells.\n");
    printf("Type two numbers, representing the position of the live cell.\n");
    printf("If the position is out of bounds of the grid, the input will end and the simulation will start.\n");

    scanf("%d %d", &i, &j);
    while (i >= 0 && j >= 0 && i < m && j < n)
    {
        grid[i][j] = 1;
        scanf("%d %d", &i, &j);
    }
}

void simulate(int **grid, int m, int n)
{
    int i, j, liven, tmpgrid[m][n];
    char c;
    rendergrid(grid, m, n);
    printf("Press enter to continue, or type 'e' to exit\n");
    c = getchar();

    while(c != 'e')
    {
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++){
                liven = getlivingneighbours(grid, m, n, i, j);

                if (grid[i][j] == 1 && liven != 2 && liven != 3)
                    tmpgrid[i][j] = 0;
                else if (grid[i][j] == 0 && liven == 3)
                    tmpgrid[i][j] = 1;
                else
                    tmpgrid[i][j] = grid[i][j];
            }
        }

        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j ++)
                grid[i][j] = tmpgrid[i][j];
        }

        rendergrid(grid, m, n);
        printf("Press enter to continue, or type 'e' to exit\n");
        c = getchar();
    }
}

int getlivingneighbours(int **grid, int m, int n, int cellx, int celly)
{
    int i, j, qnt = 0;

    for (i = clamp (cellx - 1, 0, m); i <= clamp (cellx + 1, 0, m-1); i++)
    {
        for (j = clamp (celly - 1, 0, n); j <= clamp (celly + 1, 0, n-1); j++)
        {
            if (i == cellx && j == celly)
                continue;
            
            if (grid[i][j] == 1)
                qnt++;
        }
    }

    return qnt;
}

void mallocmatrix(int ***matrix, int m, int n)
{
    int i;

    *matrix = malloc(m * sizeof(int*));
    for (i = 0; i < m; i++)
        (*matrix)[i] = malloc(n * sizeof(int));
}

void freematrix(int **matrix, int m)
{
    int i;
    for(i = 0; i < m; i++)
        free(matrix[i]);
    free(matrix);
}

int clamp(int n, int min, int max)
{
    if (n < min)
        return min;
    if (n > max)
        return max;
    
    return n;
}

void clearbuffer()
{
    while ((getchar()) != '\n'); 
}