//
//  main.c
//
//
//  Created by Runcheng Liu on 5/12/19.
//

#include <stdio.h>

const double L = 100, W = 10, H = 5;
const double k = 0.3;
const double dx = 0.5, dy = 0.5, dz = 0.5, dt = 0.01;
const double max_t = 500;
const double s = (k * dt)/(dx*dx);
const int Nx = L / dx + 1, Ny = W / dy + 1, Nz = H / dz + 1, Nt = max_t / dt + 1;
double T[2][Nx][Ny][Nz];

double T_start = 30;

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
            T[1][i][j][Nz - 1] = 40.0;
        }
    
    // initializing down points
    for (int i = 1; i < Nx - 1; i++)
    {
        T[0][i][0][0] = 100.0 - (75.0 / (Nx - 1.0))*i;
    }
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 1; j < Ny - 1; j++)
        {
            T[0][i][j][0] = T[0][i][0][0] * (1.0 - (0.8 / (Ny - 1.0))*j);
        }
    
    // initializing front points
    for (int i = 1; i < Nx - 1; i++)
        for (int j = 1; j < Nz; j++)
        {
            T[0][i][0][j] = 100.0 - (75.0 / (Nx - 1.0))*i;
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
            T[1][0][i][j] = 100.0;
        }
    
    // initializing right points
    for (int i = 0; i < Ny; i++)
        for (int j = 0; j < Nz; j++)
        {
            T[0][Nx - 1][i][j] = 20.0;
            T[1][Nx - 1][i][j] = 20.0;
        }
}

void updateState_1(int t)
{
    // update interior points
    for (int i = 1; i <= Nx - 2; i++)
        for (int j = 1; j <= Ny - 2; j++)
            for (int k = 1; k <= Nz - 2; k++)
                T[t + 1][i][j][k] = T[t][i][j][k] + s * (T[t][i + 1][j][k] - 2 * T[t][i][j][k] + T[t][i - 1][j][k]) + s * (T[t][i][j + 1][k] - 2 * T[t][i][j][k] + T[t][i][j - 1][k]) + s * (T[t][i][j][k + 1] - 2 * T[t][i][j][k] + T[t][i][j][k - 1]);
    
    // update front points
    for (int i = 1; i <= Nx - 2; i++)
            for (int k = 1; k <= Nz - 2; k++)
                T[t + 1][i][0][k] = T[t][i][0][k] + s * (T[t][i + 1][0][k] - 2 * T[t][i][0][k] + T[t][i - 1][0][k]) + 0.5 * s * (8 * T[t][i][1][k] - T[t][i][2][k] - 7 * T[t][i][0][k]) + s * (T[t][i][0][k + 1] - 2 * T[t][i][0][k] + T[t][i][0][k - 1]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][0][0] = T[t][i][0][0] + s * (T[t][i + 1][0][0] - 2 * T[t][i][0][0] + T[t][i - 1][0][0]) + 0.5 * s * (8 * T[t][i][1][0] - T[t][i][2][0] - 7 * T[t][i][0][0]) + 0.5 * s * (8 * T[t][i][0][1] - T[t][i][0][2] - 7 * T[t][i][0][0]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][0][Nz - 1] = T[t][i][0][Nz - 1] + s * (T[t][i + 1][0][Nz - 1] - 2 * T[t][i][0][Nz - 1] + T[t][i - 1][0][Nz - 1]) + 0.5 * s * (8 * T[t][i][1][Nz - 1] - T[t][i][2][Nz - 1] - 7 * T[t][i][0][ Nz - 1]) + 0.5 * s * (8 * T[t][i][0][Nz - 2] - T[t][i][0][Nz - 3] - 7 * T[t][i][0][Nz - 1]);
    
    // update back points
    for (int i = 1; i <= Nx - 2; i++)
        for (int k = 1; k <= Nz - 2; k++)
            T[t + 1][i][Ny - 1][k] = T[t][i][Ny - 1][k] + s * (T[t][i + 1][Ny - 1][k] - 2 * T[t][i][Ny - 1][k] + T[t][i - 1][Ny - 1][k]) + 0.5 * s * (8 * T[t][i][Ny - 2][k] - T[t][i][Ny - 3][k] - 7 * T[t][i][Ny - 1][k]) + s * (T[t][i][Ny - 1][k + 1] - 2 * T[t][i][Ny - 1][k] + T[t][i][Ny - 1][k - 1]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][Ny - 1][0] = T[t][i][Ny - 1][0] + s * (T[t][i + 1][Ny - 1][0] - 2 * T[t][i][Ny - 1][0] + T[t][i - 1][Ny - 1][0]) + 0.5 * s * (8 * T[t][i][Ny - 2][0] - T[t][i][Ny - 3][0] - 7 * T[t][i][Ny - 1][0]) + 0.5 * s * (8 * T[t][i][Ny - 1][1] - T[t][i][Ny - 1][2] - 7 * T[t][i][Ny - 1][0]);
    for (int i = 1; i <= Nx - 2; i++)
        T[t + 1][i][Ny - 1][Nz - 1] = T[t][i][Ny - 1][Nz - 1] + s * (T[t][i + 1][Ny - 1][Nz - 1] - 2 * T[t][i][Ny - 1][Nz - 1] + T[t][i - 1][Ny - 1][Nz - 1]) + 0.5 * s * (8 * T[t][i][Ny - 2][Nz - 1] - T[t][i][Ny - 3][Nz - 1] - 7 * T[t][i][Ny - 1][Nz - 1]) + 0.5 * s * (8 * T[t][i][Ny - 1][Nz - 2] - T[t][i][Ny - 1][Nz - 3] - 7 * T[t][i][Ny - 1][Nz - 1]);

    // update down points
    for (int i = 1; i <= Nx - 2; i++)
        for (int j = 1; j <= Ny - 2; j++)
                T[t + 1][i][j][0] = T[t][i][j][0] + s * (T[t][i + 1][j][0] - 2 * T[t][i][j][0] + T[t][i - 1][j][0]) + s * (T[t][i][j + 1][0] - 2 * T[t][i][j][0] + T[t][i][j - 1][0]) + 0.5 * s * (8 * T[t][i][j][1] - T[t][i][j][2] - 7 * T[t][i][j][0]);
//    for (int i = 1; i <= Nx - 2; i++)
//        T[t + 1][i][0][0] = T[t][i][0][0] + s * (T[t][i + 1][0][0] - 2 * T[t][i][0][0] + T[t][i - 1][0][0]) + s * (T[t][i][0][0] - 2 * T[t][i][1][0] + T[t][i][2][0]) + s * (T[t][i][0][0] - 2 * T[t][i][0][1] + T[t][i][0][2]);
//    for (int i = 1; i <= Nx - 2; i++)
//        T[t + 1][i][Ny - 1][0] = T[t][i][Ny - 1][0] + s * (T[t][i + 1][Ny - 1][0] - 2 * T[t][i][Ny - 1][0] + T[t][i - 1][Ny - 1][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 2][0] + T[t][i][Ny - 3][0]) + s * (T[t][i][Ny - 1][0] - 2 * T[t][i][Ny - 1][1] + T[t][i][Ny - 1][2]);
    
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

void snap_shot(int t) {
    char filename[50]; 
    sprintf(filename, "./snapshot_%d.txt", t);
    FILE *snapshot_fp;
    snapshot_fp = fopen(filename, "w");  
    fprintf(snapshot_fp,"t = %d :\n",t);
    for (int i = 0; i < Nx; i++)
        for (int j = 0; j < Ny; j++)
            for (int k = 0; k < Nz; k++) {
                fprintf(snapshot_fp,"(%d,%d,%d) = %lf\n", i, j, k, T[1][i][j][k]);
            }
    fflush(snapshot_fp);
    fclose(snapshot_fp);  
}

int main()
{
    FILE * fp;
    fp = fopen("./output.log","w");
    
    fprintf(fp, "%d %d %d %d\n",Nt,Nx,Ny,Nz);
    fprintf(fp, "s = %lf\n",s);
    fprintf(fp, "Initalizing.\n");
    
    Initalizing();
    
    double avg_t = average_temp(0);
    fprintf(fp, "%lf\n", T[0][0][1][1]);
    fprintf(fp, "avg_t %lf\n", avg_t);
    fprintf(fp, "Simualtingf.\n");
    
    int check_interval = 50;
    int save_interval = 50000;
    int start_snapshot = 0;
    for (int t = 1; t < Nt; ++t)
    {
        updateState_1(0);
//        printf("%lf\n", T[1][1][1][1]);
<<<<<<< HEAD
//        if (t > 35000)
//        {
//            fprintf(fp,"t = %d :\n",t);
//            fflush(fp);
//            for (int i = 0; i < Nx; i++)
//                for (int j = 0; j < Ny; j++)
//                    for (int k = 0; k < Nz; k++)
//                    {
//                        fprintf(fp,"(%d,%d,%d) = %lf\n",i,j,k,T[1][i][j][k]);
//                        fflush(fp);
//                    }
//        }
=======
        if ((t > start_snapshot) && (t % save_interval == 0))
        {
            fprintf(fp, "Snapshooting t = %d\n", t); 
            snap_shot(t); 
        }
>>>>>>> a0bb77f90207b6599d977afec9384edb6c6fe121
        if (t % check_interval == 0)
        {
            double diff = diffence(0);
            double avg_t = average_temp(1);
            fprintf(fp,"t = %d, difference = %lf, avg_t = %lf\n", t, diff, avg_t);
            printf("t = %d, difference = %lf, avg_t = %lf\n", t, diff, avg_t);
//            printf("%lf\n", T[0][1][0][1]);
//            printf("(1,0,Nz-1) = %lf\n",T[1][1][0][Nz-1]);
//            printf("(0,1,1)_final = %lf\n",T[0][0][1][1]);
//            printf("(1, 1, Nz - 2) = %lf\n", T[0][1][1][Nz - 2]);
            
//            printf("(1, 1, Nz - 1) = %lf\n", T[0][1][1][Nz - 1]);
//            printf("(1, 1, Nz - 3) = %lf\n", T[0][1][1][Nz - 3]);

//            printf("(1, 0, Nz - 2) = %lf\n", T[0][1][0][Nz - 2]);
//            printf("(1, 2, Nz - 2) = %lf\n", T[0][1][2][Nz - 2]);
//            printf("(0, 1, Nz - 2) = %lf\n", T[0][0][1][Nz - 2]);
//            printf("(2, 1, Nz - 2) = %lf\n", T[0][2][1][Nz - 2]);
//            printf("\n\n\n");
        }
        updateState_2();
//        updateState_2();
//        printf("(0,1,1)_final = %lf\n",T[0][0][1][1]);
        // flush once in a loop is enough 
        fflush(fp); 
    }
    fclose (fp);
    return 0;
}
