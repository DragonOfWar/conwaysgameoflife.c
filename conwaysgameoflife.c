#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN(x, y) ((x > y) ? y : x)
#define MAX(x, y) ((x > y) ? x : y)
#define CLAMP(n, min, max) (MIN(max, MAX(n, min)))
#define OPT_TERM 1
#define OPT_LOADFILE 2

void rendergrid(int *grid, int m, int n);
void gridcreationmenu(int *m, int *n);
void populategridmenu(int *grid, int n, int m);
void simulate(int *grid, int n, int m);
int getlivingneighbours(int *grid, int m, int n, int cellx, int celly);
int initmenu();
void loadgridfromfile(int **grid, int *m, int *n);

int main()
{
    int *grid = NULL, m, n;

    system("clear");
    switch (initmenu())
    {
    case OPT_TERM:
        gridcreationmenu(&m, &n);
        grid = malloc(m * n * sizeof(int));
        populategridmenu(grid, m, n);
        break;
    case OPT_LOADFILE:
        loadgridfromfile(&grid, &m, &n);
        break;
    default:
        return 1;
    }
    setbuf(stdin, NULL);
    simulate(grid, m, n);
    free(grid);

    return 0;
}

int initmenu()
{
    int opt;
    do
    {
        printf("conwaysgameoflife.c\n1- Create simulation from the terminal\n2- Load simulation from file\n");
        scanf("%d", &opt);
        if (opt <= 0 || opt >= 3)
            printf("Incorrect input\n");
    } while (opt <= 0 || opt >= 3);
    setbuf(stdin, NULL);
    return opt;
}

void rendergrid(int *grid, int m, int n)
{
    int i, j;
    system("clear");

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%c", (grid[i * m + j] == 1) ? '#' : '.');
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
        grid[i * m + j] = 1;
        scanf("%d %d", &i, &j);
    }
}

void loadgridfromfile(int **grid, int *m, int *n)
{
    int err = 1, i, j;
    char location[512];
    FILE *file;
    printf("Loading simulation from file.\nThe file must contain a single line of numbers.\nThe first pair of numbers represent the grid's height and width respectively, and the follow n pairs of numbers represent the coordinates of the live cells.\n");
    while (err)
    {
        printf("Location: ");
        fgets(location, 512, stdin);
        location[strlen(location) - 1] = '\0';
        setbuf(stdin, NULL);
        if ((file = fopen(location, "r")) == NULL)
            printf("Could not open file.\n");
        else
        {
            fscanf(file, "%d %d ", m, n);
            *grid = malloc(sizeof(int) * *m * *n);
            while (!feof(file))
            {
                i = 0;
                j = 0;
                fscanf(file, "%d ", &i);
                fscanf(file, "%d ", &j);
                if (i >= 0 && j >= 0 && i < *m && j < *n)
                    (*grid)[i * *m + j] = 1;
            }
            err = 0;
        }
    }
}

void simulate(int *grid, int m, int n)
{
    int i, j, liven, tmpgrid[m][n];
    char c;
    rendergrid(grid, m, n);
    printf("Press enter to continue, or type 'e' to exit\n");
    c = getchar();

    while (c != 'e')
    {
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                liven = getlivingneighbours(grid, m, n, i, j);

                if (grid[i * m + j] == 1 && liven != 2 && liven != 3)
                    tmpgrid[i][j] = 0;
                else if (grid[i * m + j] == 0 && liven == 3)
                    tmpgrid[i][j] = 1;
                else
                    tmpgrid[i][j] = grid[i * m + j];
            }
        }

        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
                grid[i * m + j] = tmpgrid[i][j];
        }

        rendergrid(grid, m, n);
        printf("Press enter to continue, or type 'e' to exit\n");
        c = getchar();
    }
}

int getlivingneighbours(int *grid, int m, int n, int cellx, int celly)
{
    int i, j, qnt = 0;

    for (i = CLAMP(cellx - 1, 0, m); i <= CLAMP(cellx + 1, 0, m - 1); i++)
    {
        for (j = CLAMP(celly - 1, 0, n); j <= CLAMP(celly + 1, 0, n - 1); j++)
        {
            if (i == cellx && j == celly)
                continue;

            if (grid[i * m + j] == 1)
                qnt++;
        }
    }

    return qnt;
}