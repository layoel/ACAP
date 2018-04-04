#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{
  register double width, x;
  double sum, local_sum;
  register int intervals, i;
  int num_proc, id_proc; //num_proc = size, id_proc = rank
  
  struct timespec creacion1,creacion2, destruccion1, destruccion2, calculo1, calculo2, calculo3, calculo4, result1, result2,total1, total2;
  double t_creacion, t_destruccion, t_calculo, t_resultados, t_total; 

  if(argc < 2)     {
    fprintf(stderr,"\nFalta nº intervalos\n");
    exit(-1);
  }

  /* get the number of intervals */
  intervals = atoi(argv[1]);
  if (intervals<1) intervals=1;

  clock_gettime(CLOCK_REALTIME,&total1);//inicio tiempo total paralelo
  clock_gettime(CLOCK_REALTIME,&creacion1);//inicio tiempo de creacion
      if (MPI_Init(&argc, &argv) != MPI_SUCCESS) 
        exit(1);
  clock_gettime(CLOCK_REALTIME,&creacion2);//fin tiempo creacion


  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);

  clock_gettime(CLOCK_REALTIME,&calculo1); //inicio tiempo de calculo
      width = 1.0 / intervals;
      local_sum = 0;

      /* do the computation */
      sum = 0;

      for (i=id_proc; i<intervals; i+=num_proc) {
        x = (i + 1.0) * width;
        local_sum += 4.0 / (1.0 + x * x);
      }
      local_sum *= width;
      //calculo del error
      
  clock_gettime(CLOCK_REALTIME,&calculo2); //fin timepo de calculo


   clock_gettime(CLOCK_REALTIME,&result1);//inicio recepcion de resultados
    MPI_Reduce(&local_sum ,&sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   clock_gettime(CLOCK_REALTIME,&result2);//fin recepcion de resultados

   clock_gettime(CLOCK_REALTIME,&calculo3); //inicio tiempo de calculo
      register double enu = fabs(3.141592653589793238462643 - sum);
      register double error = enu/3.141592653589793238462643*100;
  clock_gettime(CLOCK_REALTIME,&calculo4); //fin tiempo de calculo

  clock_gettime(CLOCK_REALTIME,&destruccion1);//inicio tiempo destruccion
    MPI_Finalize();
  clock_gettime(CLOCK_REALTIME,&destruccion2);//fin tiempo destruccion
  clock_gettime(CLOCK_REALTIME,&total2);//fin tiempo total paralelo
//calculo de tiempos
  t_creacion=(double) (creacion2.tv_sec-creacion1.tv_sec)+
  (double) ((creacion2.tv_nsec-creacion1.tv_nsec)/(1.e+9));

  t_calculo=(double) (calculo2.tv_sec-calculo1.tv_sec)+
  (double) ((calculo2.tv_nsec-calculo1.tv_nsec)/(1.e+9)) + (double) (calculo4.tv_sec-calculo3.tv_sec)+
  (double) ((calculo4.tv_nsec-calculo3.tv_nsec)/(1.e+9));

  t_destruccion=(double) (destruccion2.tv_sec-destruccion1.tv_sec)+
  (double) ((destruccion2.tv_nsec-destruccion1.tv_nsec)/(1.e+9));
  
  t_resultados=(double) (result2.tv_sec-result1.tv_sec)+
  (double) ((result2.tv_nsec-result1.tv_nsec)/(1.e+9));

   t_total=(double) (total2.tv_sec-total1.tv_sec)+
  (double) ((total2.tv_nsec-total1.tv_nsec)/(1.e+9));
//imprimiendo resultados
  if (!id_proc) {
    //printf("Calculo estimado de pi: %.14f\n", sum);
    //printf("Iteraciones: %d.\t Procesos: %d.\t Tiempo:\t%8.6f\t  Pi: %26.24f.\t\n ",intervals,num_proc,t_resultados,sum);
//printf("intervalos\tprocesos\ttcreacion\ttcalculo\ttdestruccion\ttresultado\t calculo-estimado-pi\terror\n");
     printf("%d\t%d\t%8.6f\t%8.6f\t%8.6f\t%8.6f\t%.14f\t%.14f\t%8.6f  \n",intervals,num_proc,t_creacion,t_calculo,t_destruccion, t_resultados,sum,error, t_total );
    //printf("Porcentaje de error del calculo pi: %.14f\n", error);
  }
  return(0);
}
