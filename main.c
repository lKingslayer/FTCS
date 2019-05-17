//
//  main.c
//  
//
//  Created by Runcheng Liu on 5/12/19.
//

#include <stdio.h>
#include <stdlib.h> 

const double L = 100, W = 10, H = 5;
const double k = 0.1;
const double dx = 0.1, dy = 0.1, dz = 0.1, dt = 0.001;
const double max_t = 2;
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
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 1; j < Ny - 1; j++)
        {
            T[0][i][j][Nz - 1] = 40.0;
//            T[1][i][j][Nz - 1] = 40.0;
        }
    
    // initializing down points
    for (int i = 1; i < Nx - 1; i++)
        T[0][i][0][0] = 100.0 - (75.0 / (Nx - 1.0))*i;
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 0; j < Ny - 1; j++)
            T[0][i][j][0] = T[0][i][0][0] * (1.0 - (0.8 / (Ny - 1.0))*j);
    
    // initializing front points
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 1; j < Nz; j++)
        {
            T[0][i][0][j] = 100.0 - (75.0 / (Nx - 1.0))*i;
//            T[0][i][Ny - 1][j] = 100.0 - (75.0 / (Nx - 1.0))*i;
        }
    
    //initializing back points
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][i][Ny - 1][j] = 100.0 - (75.0 / (Nx - 1.0))*i;
        }
    
    // initializing left points
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][0][i][j] = 100.0;
//            T[1][0][i][j] = 100.0;
        }
    
    // initializing right points
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][Nx - 1][i][j] = 20.0;
//            T[1][Nx - 1][i][j] = 20.0;
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
//     printf("(0,0,0) = %lf (0,0,1) = %lf (0,1,1) = %lf (0,1,0) = %lf (1,0,0) = %lf (1,1,0) = %lf (1,1,1) = %lf (1,0,1) = %lf\n",T[][0][0][0],T[t][0][0][1],T[t][0][1][1],T[t][0][1][0],T[t][1][0][0],T[t][1][1][0],T[t][1][1][1],T[t][1][0][1]);
    // update front points
    for (int i = 1; i <= Nx - 2; i++)
            for (int k = 1; k <= Nz - 2; k++)
                T[t + 1][i][0][k] = T[t][i][0][k] + s * (T[t][i + 1][0][k] - 2 * T[t][i][0][k] + T[t][i - 1][0][k]) + s * (T[t][i][0][k] - 2 * T[t][i][1][k] + T[t][i][2][k]) + s * (T[t][i][0][k + 1] - 2 * T[t][i][0][k] + T[t][i][0][k - 1]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][0][0] = T[t][i][0][0] + s * (T[t][i + 1][0][0] - 2 * T[t][i][0][0] + T[t][i - 1][0][0]) + s * (T[t][i][0][0] - 2 * T[t][i][1][0] + T[t][i][2][0]) + s * (T[t][i][0][0] - 2 * T[t][i][0][1] + T[t][i][0][2]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][0][Nz - 1] = T[t][i][0][Nz - 1] + s * (T[t][i + 1][0][Nz - 1] - 2 * T[t][i][0][Nz - 1] + T[t][i - 1][0][Nz - 1]) + s * (T[t][i][0][Nz - 1] - 2 * T[t][i][1][Nz - 1] + T[t][i][2][Nz - 1]) + s * (T[t][i][0][Nz - 1] - 2 * T[t][i][0][Nz - 2] + T[t][i][0][Nz - 3]);
    // update back points
    for (int i = 1; i <= Nx - 2; i++)
        for (int k = 1; k <= Nz - 2; k++)
            T[t + 1][i][Ny - 1][k] = T[t][i][Ny - 1][k] + s * (T[t][i + 1][Ny - 1][k] - 2 * T[t][i][Ny - 1][k] + T[t][i - 1][Ny - 1][k]) + s * (T[t][i][Ny - 1][k] - 2 * T[t][i][Ny - 2][k] + T[t][i][Ny - 3][k]) + s * (T[t][i][Ny - 1][k + 1] - 2 * T[t][i][Ny - 1][k] + T[t][i][Ny - 1][k - 1]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][Ny - 1][0] = T[t][i][Ny - 1][0] + s * (T[t][i + 1][Ny - 1][0] - 2 * T[t][i][Ny - 1][0] + T[t][i - 1][Ny - 1][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 2][0] + T[t][i][Ny - 3][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 1][1] + T[t][i][Ny - 1][2]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][Ny - 1][Nz - 1] = T[t][i][Ny - 1][Nz - 1] + s * (T[t][i + 1][Ny - 1][Nz - 1] - 2 * T[t][i][Ny - 1][Nz - 1] + T[t][i - 1][Ny - 1][Nz - 1]) + s * (T[t][i][Ny - 1][Nz - 1] - 2 * T[t][i][Ny - 2][Nz - 1] + T[t][i][Ny - 3][Nz - 1]) + s * (T[t][i][Ny - 1][Nz - 1] - 2 * T[t][i][Ny - 1][Nz - 2] + T[t][i][Ny - 1][Nz - 3]);
    // update down points
    for (int i = 1; i <= Nx - 2; i++)
        for (int j = 1; j <= Ny - 2; j++)
                T[t + 1][i][j][0] = T[t][i][j][0] + s * (T[t][i + 1][j][0] - 2 * T[t][i][j][0] + T[t][i - 1][j][0]) + s * (T[t][i][j + 1][0] - 2 * T[t][i][j][0] + T[t][i][j - 1][0]) + s * (T[t][i][j][0] - 2 * T[t][i][j][1] + T[t][i][j][2]);
//    for (int i = 1; i <= Nx - 2; i++)
//        T[t + 1][i][0][0] = T[t][i][0][0] + s * (T[t][i + 1][0][0] - 2 * T[t][i][0][0] + T[t][i - 1][0][0]) + s * (T[t][i][0][0] - 2 * T[t][i][1][0] + T[t][i][2][0]) + s * (T[t][i][0][0] - 2 * T[t][i][0][1] + T[t][i][0][2]);
//    for (int i = 1; i <= Nx - 2; i++)
//        T[t + 1][i][Ny - 1][0] = T[t][i][Ny - 1][0] + s * (T[t][i + 1][Ny - 1][0] - 2 * T[t][i][Ny - 1][0] + T[t][i - 1][Ny - 1][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 2][0] + T[t][i][Ny - 3][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 1][1] + T[t][i][Ny - 1][2]);
    
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 1; j < Ny - 1; j++)
            T[1][i][j][Nz - 1] = 40.0;
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
            T[1][0][i][j] = 100.0;
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
            T[1][Nx - 1][i][j] = 20.0;
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

void snapshot(char *filename) {
    // take a snapshot of t = 0 image 

    FILE *fp;

    fp = fopen(filename, "w+");

    // write sequentially 
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            for (int k = 0; k < Nz; k++)
                fprintf(fp, "%.10f, ", T[0][i][j][k]);

    fclose(fp);
}

int main()
{
    printf("%d %d %d %d\n",Nt,Nx,Ny,Nz);
    
    printf("Initalizing.\n");
    Initalizing();

    // take snap shot after initalization 
    char *init_temp_filename = "./snapshot/0.txt";
    printf("saving init temp to file %s\n", init_temp_filename);
    snapshot(init_temp_filename);

    double avg_t = average_temp(0);
    printf("%lf\n", T[0][0][1][1]);
    printf("avg_t %lf\n", avg_t);
    printf("Simualtingf.\n");
    int check_interval = 100;
    int snapshot_interval = 100; 
    for (int t = 1; t < Nt; ++t)
    {
        updateState_1(0);
        updateState_2();
//        update_const_points();
        if (t % check_interval == 0)
        {
//            double diff = diffence(0);
            double avg_t = average_temp(0);
            printf("t = %d, avg_t = %lf\n", t, avg_t);
            printf("%lf\n", T[0][1][1][1]);
//            printf("(1,0,Nz-1) = %lf\n",T[1][1][0][Nz-1]);
//            printf("(0,1,1)_final = %lf\n",T[0][0][1][1]);
        }
//        updateState_2();
//        printf("(0,1,1)_final = %lf\n",T[0][0][1][1]);

        // take a snapshot
        if (t % snapshot_interval == 0)
        {
            // prepare filename 
            const int len_filename = 20; 
            char *filename = (char*)malloc(len_filename * sizeof(char));
            sprintf(filename, "./snapshot/%d.txt", t);

            printf("taking snapshot at time = %d\n", t); 
            snapshot(filename); 
        }
    }
    
    return 0;
}
