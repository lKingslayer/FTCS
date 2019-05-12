//
//  main.c
//  
//
//  Created by Runcheng Liu on 5/12/19.
//

#include <stdio.h>

const double L = 100, W = 10, H = 5;
const double k = 0.1;
const double dx = 0.1, dy = 0.1, dz = 0.1, dt = 0.01;
const double max_t = 20;
const double s = (k * dt)/(dx*dx);
const int Nx = L / dx + 1, Ny = W / dy + 1, Nz = H / dz + 1, Nt = max_t / dt + 1;
double T[2][Nx][Ny][Nz];

double T_start = 0;

void Initalizing()
{
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            for (int k = 0; k < Nz; k++)
                T[0][i][j][k] = T_start;
    // initializing up points
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
        {
            T[0][i][j][Nz - 1] = 40;
            T[1][i][j][Nz - 1] = 40;
        }
    // initializing down points
    for (int i = 0; i < Nx; i++)
        T[0][i][0][0] = 100 - (75.0 / (Nx - 1.0))*i;
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            T[0][i][j][0] = T[0][i][0][0] * (1 - (0.8 / (Ny - 1.0))*j);
    // initializing front and back points
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][i][0][j] = 100 - (75.0 / (Nx - 1.0))*i;
            T[0][i][Ny - 1][j] = 100 - (75.0 / (Nx - 1.0))*i;
        }
    // initializing left points
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][0][i][j] = 100;
            T[1][0][i][j] = 100;
        }
    // initializing right points
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][Nx - 1][i][j] = 20;
            T[1][Nx - 1][i][j] = 20;
        }
}

void updateState_1(int t)
{
    // update interior points
    // printf("start (0,0,0) = %lf (0,0,1) = %lf (0,1,1) = %lf (0,1,0) = %lf (1,0,0) = %lf (1,1,0) = %lf (1,1,1) = %lf (1,0,1) = %lf\n",T[t][0][0][0],T[t][0][0][1],T[t][0][1][1],T[t][0][1][0],T[t][1][0][0],T[t][1][1][0],T[t][1][1][1],T[t][1][0][1]);
    for (int i = 1; i <= Nx - 2; i++)
        for (int j = 1; j <= Ny - 2; j++)
            for (int k = 1; k <= Nz - 2; k++)
                T[t + 1][i][j][k] = T[t][i][j][k] + s * (T[t][i + 1][j][k] - 2 * T[t][i][j][k] + T[t][i - 1][j][k]) + s * (T[t][i][j + 1][k] - 2 * T[t][i][j][k] + T[t][i][j - 1][k]) + s * (T[t][i][j][k + 1] - 2 * T[t][i][j][k] + T[t][i][j][k - 1]);
    // printf("(0,0,0) = %lf (0,0,1) = %lf (0,1,1) = %lf (0,1,0) = %lf (1,0,0) = %lf (1,1,0) = %lf (1,1,1) = %lf (1,0,1) = %lf\n",T[t][0][0][0],T[t][0][0][1],T[t][0][1][1],T[t][0][1][0],T[t][1][0][0],T[t][1][1][0],T[t][1][1][1],T[t][1][0][1]);
    // update front points
    for (int i = 1; i <= Nx - 2; i++)
            for (int k = 1; k <= Nz - 2; k++)
                T[t + 1][i][0][k] = T[t][i][0][k] + s * (T[t][i + 1][0][k] - 2 * T[t][i][0][k] + T[t][i - 1][0][k]) + s * (T[t][i][0][k] - 2 * T[t][i][1][k] + T[t][i][2][k]) + s * (T[t][i][0][k + 1] - 2 * T[t][i][0][k] + T[t][i][0][k - 1]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][0][0] = T[t][i][0][0] + s * (T[t][i + 1][0][0] - 2 * T[t][i][0][0] + T[t][i - 1][0][0]) + s * (T[t][i][0][0] - 2 * T[t][i][1][0] + T[t][i][2][0]) + s * (T[t][i][0][0] - 2 * T[t][i][0][1] + T[t][i][0][2]);
    // update back points
    for (int i = 1; i <= Nx - 2; i++)
        for (int k = 1; k <= Nz - 2; k++)
            T[t + 1][i][Ny - 1][k] = T[t][i][Ny - 1][k] + s * (T[t][i + 1][Ny - 1][k] - 2 * T[t][i][Ny - 1][k] + T[t][i - 1][Ny - 1][k]) + s * (T[t][i][Ny - 1][k] - 2 * T[t][i][Ny - 2][k] + T[t][i][Ny - 3][k]) + s * (T[t][i][Ny - 1][k + 1] - 2 * T[t][i][Ny - 1][k] + T[t][i][Ny - 1][k - 1]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][Ny - 1][0] = T[t][i][Ny - 1][0] + s * (T[t][i + 1][Ny - 1][0] - 2 * T[t][i][Ny - 1][0] + T[t][i - 1][Ny - 1][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 2][0] + T[t][i][Ny - 3][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 1][1] + T[t][i][Ny - 1][2]);
    // update down points
    for (int i = 1; i <= Nx - 2; i++)
        for (int j = 1; j <= Ny - 2; j++)
                T[t + 1][i][j][0] = T[t][i][j][0] + s * (T[t][i + 1][j][0] - 2 * T[t][i][j][0] + T[t][i - 1][j][0]) + s * (T[t][i][j + 1][0] - 2 * T[t][i][j][0] + T[t][i][j - 1][0]) + s * (T[t][i][j][0] - 2 * T[t][i][j][1] + T[t][i][j][2]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][0][0] = T[t][i][0][0] + s * (T[t][i + 1][0][0] - 2 * T[t][i][0][0] + T[t][i - 1][0][0]) + s * (T[t][i][0][0] - 2 * T[t][i][1][0] + T[t][i][2][0]) + s * (T[t][i][0][0] - 2 * T[t][i][0][1] + T[t][i][0][2]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][Ny - 1][0] = T[t][i][Ny - 1][0] + s * (T[t][i + 1][Ny - 1][0] - 2 * T[t][i][Ny - 1][0] + T[t][i - 1][Ny - 1][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 2][0] + T[t][i][Ny - 3][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 1][1] + T[t][i][Ny - 1][2]);
}

void updateState_2()
{
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            for (int k = 0; k < Nz; k++)
                T[0][i][j][k] = T[1][i][j][k];
}

double diffence(int t)
{
    double sum = 0;
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            for (int k = 0; k < Nz; k++)
                sum = sum + T[t + 1][i][j][k] - T[t][i][j][k];
    return sum/(Nx*Ny*Nz);
}

double average_temp(int t)
{
    double sum = 0;
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            for (int k = 0; k < Nz; k++)
                sum = sum + T[t][i][j][k];
    return sum / (Nx * Ny * Nz);
}

int main()
{
    printf("%d %d %d %d\n",Nt,Nx,Ny,Nz);
    
    printf("Initalizing.\n");
    Initalizing();
    double avg_t = average_temp(0);
    printf("avg_t %lf\n", avg_t);
    
    printf("Simualtingf.\n");
    int check_interval = 20;
    for (int t = 1; t < Nt; ++t)
    {
        updateState_1(0);
        
        if (t % check_interval == 0)
        {
            double diff = diffence(0);
            double avg_t = average_temp(0);
            printf("t = %d, diff = %lf, avg_t = %lf\n", t, diff, avg_t);
            //            printf("%lf\n", T[0][1][1][1]);
        }
        
        updateState_2();
    }
    
    return 0;
}
