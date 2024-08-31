#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Largura e altura (Quantidade de caracteres)
#define width 110
#define height 55

// Quantidade de frames a serem exibidos
#define frames 3000

// Mudança nos ângulos a cada frame
#define deltaAlpha 0.005
#define deltaBeta 0.005
#define deltaGama 0.005

// Caractere para desenhar o cubo
#define cubeCaracter 'N'

// Função que realiza o produto de uma matriz por um vetor
void dot(int rows, int cols, double a[rows][cols], double v[rows], double result[rows])
{
    for (int i = 0; i < rows; i++)
    {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++)
        {
            result[i] += a[i][j] * v[j];
        }
    }
}

// Função que realiza a projeção ortogonal de um vetor 3d para um vetor 2d
void project(double v[3], double r[2])
{
    double projection[2][3] = {
        {1, 0, 0},
        {0, 1, 0}};

    dot(2, 3, projection, v, r);
}

// Função de rotação unificada para 3 angulos em relação aos 3 eixos de rotação sendo aplicada a vetores 3d
void rotate(double a, double b, double y, double vector[3], double result[3])
{
    double rotationMatrix[3][3] = {
        {cos(a) * cos(y) - sin(a) * cos(b) * sin(y), sin(a) * cos(y) + cos(a) * cos(b) * sin(y), sin(b) * sin(y)},
        {-cos(a) * sin(y) - sin(a) * cos(b) * cos(y), -sin(a) * sin(y) + cos(a) * cos(b) * cos(y), sin(b) * cos(y)},
        {sin(a) * sin(b), -cos(a) * sin(b), cos(b)}};

    dot(3, 3, rotationMatrix, vector, result);
}

// Criação dos vertices de um cubo centrado na origem (0, 0, 0) e de lado 2
double cube[8][3] = {
    {-1, -1, -1},
    {1, -1, -1},
    {1, -1, 1},
    {-1, -1, 1},
    {-1, 1, -1},
    {1, 1, -1},
    {1, 1, 1},
    {-1, 1, 1}};

// Matriz com as informações de quais pares de vertices formam uma aresta
int cubeEdges[12][2] = {
    {0, 1},
    {1, 2},
    {2, 3},
    {0, 3},

    {4, 5},
    {5, 6},
    {6, 7},
    {4, 7},

    {0, 4},
    {1, 5},
    {2, 6},
    {3, 7}};

// Algoritmo de Bresenham para desenhar linhas em posições discretas
void drawLine(int x0, int y0, int x1, int y1, char image[height][width])
{
    // Delta X e delta Y
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1; // Direção do passo em x
    int sy = (y0 < y1) ? 1 : -1; // Direção do passo em y

    int err = dx - dy;

    while (1)
    {
        // Colocando o caractere na linha
        image[y0][x0] = cubeCaracter;

        if (x0 == x1 && y0 == y1)
            break; // Se alcançou o ponto final, pare

        int e2 = 2 * err;

        // Atualizando o x
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        // Atualizando o y
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// Transforma os valores reais em valores inteiros aumentando a escala do cubo para o tamanho da tela
void getDiscreteCube(double projectedCube[8][2], int discreteCube[8][2])
{
    for (int i = 0; i < 8; i++)
    {
        // Arredonda e salva os vertices
        int x = (int)round(projectedCube[i][0] * width / 4 + width / 2);
        int y = (int)round(projectedCube[i][1] * height / 4 + height / 2);

        // Salva na matriz de potos inteiros
        discreteCube[i][0] = x;
        discreteCube[i][1] = y;
    }
}

// Função que conecta os vertices do cubo na ordem certa e os coloca na matriz de caracteres
void conectEdges(int cubeEdges[12][2], int discreteCube[8][2], char image[height][width])
{
    for (int i = 0; i < 12; i++)
    {
        // Arestas
        int v0, v1;
        v0 = cubeEdges[i][0];
        v1 = cubeEdges[i][1];

        // Cordenadas dos vertices
        int x0, y0, x1, y1;
        x0 = discreteCube[v0][0];
        y0 = discreteCube[v0][1];

        x1 = discreteCube[v1][0];
        y1 = discreteCube[v1][1];

        // Colocando a linha na matriz de caracteres
        drawLine(x0, y0, x1, y1, image);
    }
}

// Função que aplica as transformações para cada vertice do cubo e projeta
void rotateProject(double alpha, double beta, double gama, double cube[8][3], double projectedCube[8][2])
{
    // Matriz que vai armazenar os vetores 3D do cubo rotacionado
    double rotatedCube[8][3];

    // Realizando as operações de rotação e projeção para cada vertice do cubo
    for (int i = 0; i < 8; i++)
    {
        rotate(alpha, beta, gama, cube[i], rotatedCube[i]);
        project(rotatedCube[i], projectedCube[i]);
    }
}

// Função que desenha na tela a matriz de caracteres
void draw(double projectedCube[8][2], int cubeEdges[12][2])
{
    // Limpa a tela do prompt no windows
    system("cls");

    // Cria uma matriz de caracteres para desenhar na tela
    char image[height][width] = {0};

    // Cria uma matriz para salvar os valores inteiros arredondados dos vertices do cubo
    int discreteCube[8][2];

    // Aplicando a função para tornar inteiro as cordenadas 2D dos vertices
    getDiscreteCube(projectedCube, discreteCube);

    // Conectando os vertices com uma linha
    conectEdges(cubeEdges, discreteCube, image);

    // Desenhando na tela a matriz de caracteres usando a função printf();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image[i][j] == 0)
                image[i][j] = ' ';
        }
        // Desenhando na tela uma string formada por um array de caracteres
        printf("%s\n", image[i]);
    }

    //Obs testei a matriz de caracteres linearizada mas não houve melhora perceptivel na performance
}

// Função principal de execução do C
int main()
{
    // Definindo os angulos com os eixos iniciais
    double alpha = M_PI / 4;
    double beta = M_PI / 8;
    double gama = M_PI / 16;

    // Matriz que vai armazenar a projeção ortogonal do cubo rotacionado
    double projectedCube[8][2];

    // Desenhando cada frame da animação
    for (int f = 0; f < frames; f++)
    {
        // Realizando incremento aos ângulos
        alpha += deltaAlpha;
        beta += deltaBeta;
        gama += deltaGama;

        // Aplicando as transformações lineares ao cubo
        rotateProject(alpha, beta, gama, cube, projectedCube);

        // Desenhando na tela
        draw(projectedCube, cubeEdges);
    }

    return 0;
}