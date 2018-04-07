// Swapnil Paratey
// ECEN 5653 - Summer 2015 [Real Time Digital Media]
// Code used to convert individual pixel into a color boundary
// using Sobel egde detection
// Use "gcc -o sobel sobel.c"
// Can also use "gcc -O3 -mssse3 -malign-double -S -o sobel sobel.c" for hardware speedup

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <mpi.h>

typedef unsigned int UINT32;
typedef unsigned long long int UINT64;
typedef unsigned char UINT8;
typedef double FLOAT;

#define FILTER_RADIUS 1
#define THRESHOLD 20
#define Ro 0
#define Gr 1
#define Bl 2

int main(int argc, char **argv)
{
    if (argc != 4){
        fprintf(stderr, "USO: <nombre_imagen> <Ancho_imagen> <Alto_imagen>\n");
        return 1;
    }


    int fdin, fdout, bytesLeft, bytesRead;
    int i, j, temp, dx, dy;
    long int IMAGE_WIDTH , IMAGE_HEIGHT;
    FLOAT sumRX, sumRY, sumGX, sumGY, sumBX, sumBY;
    FLOAT pixelR, pixelG, pixelB;
    const float SobelMatrix[9] = {-1,0,1,-2,0,2,-1,0,1};
    struct timeval StartTime, StopTime;
    unsigned int microsecs;
    int trozoimg;

    //MPI VARIABLES
    int num_proc, id_proc;

    IMAGE_WIDTH = strtol(argv[2], NULL, 10);
    IMAGE_HEIGHT = strtol(argv[3], NULL, 10);
    
    UINT8 header[22];
    UINT8 *R;
    UINT8 *G;
    UINT8 *B;
    UINT8 *convR;
    UINT8 *convG;
    UINT8 *convB;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_size(MPI_COMM_WORLD, &id_proc);    



    MPI_Request OKR[num_proc]; 
    MPI_Request OKG[num_proc]; 
    MPI_Request OKB[num_proc];

    MPI_Status okr, okg, okb;

printf("\nError divide por cero\n");
    if((num_proc %2) == 0 )
        trozoimg = (IMAGE_HEIGHT/num_proc);
    else{
        if (id_proc != (num_proc -1) && (num_proc > 1))
            trozoimg = (IMAGE_HEIGHT/num_proc);
        else trozoimg = (IMAGE_HEIGHT/num_proc) + (IMAGE_HEIGHT%num_proc);
    }


    if (id_proc == 0){
        R = (UINT8*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(UINT8));
        G = (UINT8*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(UINT8));
        B = (UINT8*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(UINT8));
        convR = (UINT8*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(UINT8));
        convG = (UINT8*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(UINT8));
        convB = (UINT8*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(UINT8));

        if (R == NULL){
            fprintf(stderr, "Error al reservar R\n");
            return 1;
        }

        if (G == NULL){
            fprintf(stderr, "Error al reservar G\n");
            return 1;
        }
        
        if (B == NULL){
            fprintf(stderr, "Error al reservar B\n");
            return 1;
        }

        if (convR == NULL){
            fprintf(stderr, "Error al reservar convR\n");
            return 1;
        }
        
        if (convG == NULL){
            fprintf(stderr, "Error al reservar convG\n");
            return 1;
        }

        if (convB == NULL){
            fprintf(stderr, "Error al reservar convB\n");
            return 1;
        }

        if((fdin = open(argv[1], O_RDONLY, 0644)) < 0)
        {
            printf("Error opening imagen\n");
        }

        printf("IMAGE OPENED %d\n", fdin);
        
        if((fdout = open("filtro_sobel.ppm", (O_RDWR | O_CREAT), 0666)) < 0)
        {
            printf("Error opening filtro_sobel.ppm\n");
        }
        printf("OUTPUT FILE OPENED %d\n", fdout);
        
        bytesLeft=21;
        do
        {
            bytesRead=read(fdin, (void *)header, bytesLeft);
            bytesLeft -= bytesRead;
        } while(bytesLeft > 0);
        header[21]='\0'; 
        
        // Read and assign RGB components of input image
        gettimeofday(&StartTime, 0);
        for(i=0; i<(IMAGE_WIDTH*IMAGE_HEIGHT); i++)
        {
            read(fdin, (void *)&R[i], 1); convR[i]=R[i];
            read(fdin, (void *)&G[i], 1); convG[i]=G[i];
            read(fdin, (void *)&B[i], 1); convB[i]=B[i];
        }
        gettimeofday(&StopTime, 0);
        
        microsecs=((StopTime.tv_sec - StartTime.tv_sec)*1000000) ;
        if(StopTime.tv_usec > StartTime.tv_usec)
        microsecs+=(StopTime.tv_usec - StartTime.tv_usec);
        else
        microsecs-=(StartTime.tv_usec - StopTime.tv_usec);
        printf("Time required to open the file: %d ms\n", microsecs/1000);
        // Testing the image pixels
        // printf("%d %d %d\n", R[40], G[40], B[40]);

        if (num_proc > 1){
            for (i = (id_proc+1)*trozoimg-1 , j = id_proc+1; i < IMAGE_HEIGHT; i+=trozoimg, ++j ){
                if (j == num_proc-1){
                    MPI_Isend(R+i, (trozoimg+(IMAGE_HEIGHT%num_proc)+1)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, j, Ro, MPI_COMM_WORLD, &OKR[j]);  
                    MPI_Isend(G+i, (trozoimg+(IMAGE_HEIGHT%num_proc)+1)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, j, Gr, MPI_COMM_WORLD, &OKG[j]);  
                    MPI_Isend(B+i, (trozoimg+(IMAGE_HEIGHT%num_proc)+1)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, j, Bl, MPI_COMM_WORLD, &OKB[j]); 
                }
                MPI_Isend(R+i, (trozoimg+2)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, j, Ro, MPI_COMM_WORLD, &OKR[j]);
                MPI_Isend(G+i, (trozoimg+2)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, j, Gr, MPI_COMM_WORLD, &OKG[j]);
                MPI_Isend(B+i, (trozoimg+2)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, j, Bl, MPI_COMM_WORLD, &OKB[j]);
                
            }
        }

        gettimeofday(&StartTime, 0);
        for(i = 1; i < (trozoimg - 1); i++)
        {
            for(j = 1; j < (IMAGE_WIDTH - 1); j++)
            {
                sumRX = 0; sumRY = 0; sumGX = 0; sumGY = 0; sumBX = 0; sumBY = 0;
                for(dy = -FILTER_RADIUS; dy <= FILTER_RADIUS; dy++) 
                {
                    for(dx = -FILTER_RADIUS; dx <= FILTER_RADIUS; dx++) 
                    {
                        pixelR = R[i*IMAGE_WIDTH + j +  (dy * IMAGE_WIDTH + dx)];
                        sumRX += pixelR * SobelMatrix[(dy + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dx+FILTER_RADIUS)];
                        sumRY += pixelR * SobelMatrix[(dx + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dy+FILTER_RADIUS)];
                        
                        pixelG = G[i*IMAGE_WIDTH + j +  (dy * IMAGE_WIDTH + dx)];
                        sumGX += pixelG * SobelMatrix[(dy + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dx+FILTER_RADIUS)];
                        sumGY += pixelG * SobelMatrix[(dx + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dy+FILTER_RADIUS)];
                        
                        pixelB = B[i*IMAGE_WIDTH + j +  (dy * IMAGE_WIDTH + dx)];
                        sumBX += pixelB * SobelMatrix[(dy + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dx+FILTER_RADIUS)];
                        sumBY += pixelB * SobelMatrix[(dx + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dy+FILTER_RADIUS)];
                    }
                    // printf("%f %f\n", sumX, sumY);
                }
                convR[i-1 * IMAGE_WIDTH + j-1] = (abs(sumRX) + abs(sumRY)) > THRESHOLD ? 255 : 0;
                convG[i-1 * IMAGE_WIDTH + j-1] = (abs(sumGX) + abs(sumGY)) > THRESHOLD ? 255 : 0;
                convB[i-1 * IMAGE_WIDTH + j-1] = (abs(sumBX) + abs(sumBY)) > THRESHOLD ? 255 : 0;
            }
        }
        gettimeofday(&StopTime, 0);

    }
    else{
        R = (UINT8*)malloc((2+trozoimg)*IMAGE_WIDTH*sizeof(UINT8));
        G = (UINT8*)malloc((2+trozoimg)*IMAGE_WIDTH*sizeof(UINT8));
        B = (UINT8*)malloc((2+trozoimg)*IMAGE_WIDTH*sizeof(UINT8));
        convR = (UINT8*)malloc((2+trozoimg)*IMAGE_WIDTH*sizeof(UINT8));
        convG = (UINT8*)malloc((2+trozoimg)*IMAGE_WIDTH*sizeof(UINT8));
        convB = (UINT8*)malloc((2+trozoimg)*IMAGE_WIDTH*sizeof(UINT8));
        
        

        if (id_proc == num_proc-1){
            MPI_Irecv(R, (trozoimg+1)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, 0, Ro, MPI_COMM_WORLD, &OKR[id_proc]);
            MPI_Irecv(G, (trozoimg+1)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, 0, Gr, MPI_COMM_WORLD, &OKG[id_proc]);
            MPI_Irecv(B, (trozoimg+1)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, 0, Bl, MPI_COMM_WORLD, &OKB[id_proc]);
            MPI_Wait(&OKR[id_proc], &okr);
            MPI_Wait(&OKG[id_proc], &okg);
            MPI_Wait(&OKB[id_proc], &okb);

            
            for ( i = (trozoimg)*IMAGE_WIDTH ; i < (trozoimg+1)*IMAGE_WIDTH; ++i){
                R[i] = 0;
                G[i] = 0;
                B[i] = 0; 
            }
        }
        else{
            MPI_Irecv(R, (trozoimg+2)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, 0, Ro, MPI_COMM_WORLD, &OKR[id_proc]);
            MPI_Irecv(G, (trozoimg+2)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, 0, Gr, MPI_COMM_WORLD, &OKG[id_proc]);
            MPI_Irecv(B, (trozoimg+2)*IMAGE_WIDTH*sizeof(UINT8), MPI_UNSIGNED_CHAR, 0, Bl, MPI_COMM_WORLD, &OKB[id_proc]);
            MPI_Wait(&OKR[id_proc], &okr);
            MPI_Wait(&OKG[id_proc], &okg);
            MPI_Wait(&OKB[id_proc], &okb);
        }

        gettimeofday(&StartTime, 0);
        for(i = 1; i < (trozoimg - 1); i++)
        {
            for(j = 1; j < (IMAGE_WIDTH - 1); j++)
            {
                sumRX = 0; sumRY = 0; sumGX = 0; sumGY = 0; sumBX = 0; sumBY = 0;
                for(dy = -FILTER_RADIUS; dy <= FILTER_RADIUS; dy++) 
                {
                    for(dx = -FILTER_RADIUS; dx <= FILTER_RADIUS; dx++) 
                    {
                        pixelR = R[i*IMAGE_WIDTH + j +  (dy * IMAGE_WIDTH + dx)];
                        sumRX += pixelR * SobelMatrix[(dy + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dx+FILTER_RADIUS)];
                        sumRY += pixelR * SobelMatrix[(dx + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dy+FILTER_RADIUS)];
                        
                        pixelG = G[i*IMAGE_WIDTH + j +  (dy * IMAGE_WIDTH + dx)];
                        sumGX += pixelG * SobelMatrix[(dy + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dx+FILTER_RADIUS)];
                        sumGY += pixelG * SobelMatrix[(dx + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dy+FILTER_RADIUS)];
                        
                        pixelB = B[i*IMAGE_WIDTH + j +  (dy * IMAGE_WIDTH + dx)];
                        sumBX += pixelB * SobelMatrix[(dy + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dx+FILTER_RADIUS)];
                        sumBY += pixelB * SobelMatrix[(dx + FILTER_RADIUS) * FILTER_RADIUS * 2 + (dy+FILTER_RADIUS)];
                    }
                    // printf("%f %f\n", sumX, sumY);
                }
                convR[i-1 * IMAGE_WIDTH + j] = (abs(sumRX) + abs(sumRY)) > THRESHOLD ? 255 : 0;
                convG[i-1 * IMAGE_WIDTH + j] = (abs(sumGX) + abs(sumGY)) > THRESHOLD ? 255 : 0;
                convB[i-1 * IMAGE_WIDTH + j] = (abs(sumBX) + abs(sumBY)) > THRESHOLD ? 255 : 0;
            }
        }
        gettimeofday(&StopTime, 0);
    }
   
    
    
    microsecs=((StopTime.tv_sec - StartTime.tv_sec)*1000000) ;
    if(StopTime.tv_usec > StartTime.tv_usec)
	microsecs+=(StopTime.tv_usec - StartTime.tv_usec);
    else
	microsecs-=(StartTime.tv_usec - StopTime.tv_usec);
    printf("Sobel edge detection time: %d ms\n", microsecs/1000);    
    // Write out the final image
    
    gettimeofday(&StartTime, 0);
    write(fdout, (void *)header, 21);
    for(i=0; i<IMAGE_WIDTH*IMAGE_HEIGHT; i++)
    {
        write(fdout, (void *)&convR[i], 1);
        write(fdout, (void *)&convG[i], 1);
        write(fdout, (void *)&convB[i], 1);
    }
    gettimeofday(&StopTime, 0);
    
    microsecs=((StopTime.tv_sec - StartTime.tv_sec)*1000000) ;
    if(StopTime.tv_usec > StartTime.tv_usec)
	microsecs+=(StopTime.tv_usec - StartTime.tv_usec);
    else
	microsecs-=(StartTime.tv_usec - StopTime.tv_usec);
    printf("Time required to write file: %d ms\n", microsecs/1000);
    
    close(fdin);
    close(fdout);

    free(R); free(G); free(B);
    free(convR); free (convG) ; free(convB);


    return 0;
}
