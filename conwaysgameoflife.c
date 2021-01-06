#include <stdio.h>
#include <stdlib.h>
#define CLEARBUFFER() while ((getchar()) != '\n');
#define MIN(x, y) ((x > y) ? y : x)
#define MAX(x, y) ((x > y) ? x : y)
#define CLAMP(n, min, max) (MIN(max, MAX(n, min)))

void rendergrid(int *grid, int m, int n);
void gridcreationmenu(int *m, int *n);
void populategridmenu(int *grid, int n, int m);
void simulate(int *grid, int n, int m);
int getlivingneighbours(int *grid, int m, int n, int cellx, int celly);

int main()
{
    int *grid = NULL, m, n;

    system("clear");
    gridcreationmenu(&m, &n);
    grid = malloc(m * n * sizeof(int));
    populategridmenu(grid, m, n);
    CLEARBUFFER();
    simulate(grid, m, n);
    free(grid);

    return 0;
}

void rendergrid(int *grid, int m, int n)
{
    int i, j;
    system("clear");

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%c", (grid[i*m + j] == 1) ? '#' : '.');
        printf("\n");
    }
}

void gridcreationmenu(int *m, int *n)
{
    printf("Grid width: ");
    scanf("%d", n);
    printf("Grid height: ");
    scanf("%d", m);
}

void populategridmenu(int *grid, int m, int n)
{
    int i, j;
    printf("Type the location of live cells.\n");
    printf("Type two numbers, representing the position of the live cell.\n");
    printf("If the position is out of bounds of the grid, the input will end and the simulation will start.\n");

    scanf("%d %d", &i, &j);
    while (i >= 0 && j >= 0 && i < m && j < n)
    {
        grid[i*m + j] = 1;
        scanf("%d %d", &i, &j);
    }
}

void simulate(int *grid, int m, int n)
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

                if (grid[i*m + j] == 1 && liven != 2 && liven != 3)
                    tmpgrid[i][j] = 0;
                else if (grid[i*m + j] == 0 && liven == 3)
                    tmpgrid[i][j] = 1;
                else
                    tmpgrid[i][j] = grid[i*m + j];
            }
        }

        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j ++)
                grid[i*m + j] = tmpgrid[i][j];
        }

        rendergrid(grid, m, n);
        printf("Press enter to continue, or type 'e' to exit\n");
        c = getchar();
    }
}

int getlivingneighbours(int *grid, int m, int n, int cellx, int celly)
{
    int i, j, qnt = 0;

    for (i = CLAMP (cellx - 1, 0, m); i <= CLAMP (cellx + 1, 0, m-1); i++)
    {
        for (j = CLAMP (celly - 1, 0, n); j <= CLAMP (celly + 1, 0, n-1); j++)
        {
            if (i == cellx && j == celly)
                continue;
            
            if (grid[i*m + j] == 1)
                qnt++;
        }
    }

    return qnt;
}