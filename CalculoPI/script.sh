#!/bin/bash

rm datos/*.dat

#secuencial O1
echo "Compila con -O1"
mpicc cpi-par_medio.c -O1 -lrt -o cpi-par_medio -w
mpicc cpi-par_exceso.c -O1 -lrt -o cpi-par_exceso -w
mpicc cpi-par_defecto.c -O1 -lrt -o cpi-par_defecto -w

COMPILADOR='mpicc'
EX='mpirun'


echo "Fin de la compilacion OK"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -np 1 ./cpi-par_defecto $i >> datos/cpi-par_defecto1.dat && echo "\n#################################\n"; done
echo "He terminado secuencial Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -np 1 ./cpi-par_exceso $i >> datos/cpi-par_exceso1.dat && echo "\n#################################\n"; done
echo "He terminado secuencial Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -np 1 ./cpi-par_medio $i >> datos/cpi-par_medio1.dat && echo "\n#################################\n"; done
echo "He terminado secuencial Medio"


#2procesos O1
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 2 ./cpi-par_defecto $i >> datos/cpi-par_defecto2p.dat && echo "\n#################################\n"; done
echo "He terminado 2procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 2 ./cpi-par_exceso $i >> datos/cpi-par_exceso2p.dat && echo "\n#################################\n"; done
echo "He terminado 2procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 2 ./cpi-par_medio $i >> datos/cpi-par_medio2p.dat && echo "\n#################################\n"; done
echo "He terminado 2procesos en paralelo por Medio"

#3procesos O1
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 3 ./cpi-par_defecto $i >> datos/cpi-par_defecto3p.dat && echo "\n#################################\n"; done
echo "He terminado 3procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 3 ./cpi-par_exceso $i >> datos/cpi-par_exceso3p.dat && echo "\n#################################\n"; done
echo "He terminado 3procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 3 ./cpi-par_medio $i >> datos/cpi-par_medio3p.dat && echo "\n#################################\n"; done
echo "He terminado 3procesos en paralelo por Medio"

#4procesos O1
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 4 ./cpi-par_defecto $i >> datos/cpi-par_defecto4p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 4 ./cpi-par_exceso $i >> datos/cpi-par_exceso4p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 4 ./cpi-par_medio $i >> datos/cpi-par_medio4p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Medio"


#6procesos O1
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 5 ./cpi-par_defecto $i >> datos/cpi-par_defecto5p.dat && echo "\n#################################\n"; done
echo "He terminado 6procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 5 ./cpi-par_exceso $i >> datos/cpi-par_exceso5p.dat && echo "\n#################################\n"; done
echo "He terminado 6procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 5 ./cpi-par_medio $i >> datos/cpi-par_medio5p.dat && echo "\n#################################\n"; done
echo "He terminado 6procesos en paralelo por Medio"

#8procesos O1
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 6 ./cpi-par_defecto $i >> datos/cpi-par_defecto6p.dat && echo "\n#################################\n"; done
echo "He terminado 8procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 6 ./cpi-par_exceso $i >> datos/cpi-par_exceso6p.dat && echo "\n#################################\n"; done
echo "He terminado 8procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 6 ./cpi-par_medio $i >> datos/cpi-par_medio6p.dat && echo "\n#################################\n"; done
echo "He terminado 8procesos en paralelo por Medio"

############################################################################

#secuencial O2
echo "Compila con -O2"
mpicc cpi-par_medio.c -O2 -lrt -o cpi-par_medio -w
mpicc cpi-par_exceso.c -O2 -lrt -o cpi-par_exceso -w
mpicc cpi-par_defecto.c -O2 -lrt -o cpi-par_defecto -w

COMPILADOR='mpicc'
EX='mpirun'


echo "Fin de la compilacion OK"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -np 1 ./cpi-par_defecto $i >> datos/cpi-par_defecto2.dat && echo "\n#################################\n"; done
echo "He terminado secuencial Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -np 1 ./cpi-par_exceso $i >> datos/cpi-par_exceso2.dat && echo "\n#################################\n"; done
echo "He terminado secuencial Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -np 1 ./cpi-par_medio $i >> datos/cpi-par_medio2.dat && echo "\n#################################\n"; done
echo "He terminado secuencial Medio"

#2procesos O2
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 2 ./cpi-par_defecto $i >> datos/cpi-par_defecto2-2p.dat && echo "\n#################################\n"; done
echo "He terminado 2procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 2 ./cpi-par_exceso $i >> datos/cpi-par_exceso2-2p.dat && echo "\n#################################\n"; done
echo "He terminado 2procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 2 ./cpi-par_medio $i >> datos/cpi-par_medio2-2p.dat && echo "\n#################################\n"; done
echo "He terminado 2procesos en paralelo por Medio"

#3procesos O2
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 3 ./cpi-par_defecto $i >> datos/cpi-par_defecto2-3p.dat && echo "\n#################################\n"; done
echo "He terminado 3procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 3 ./cpi-par_exceso $i >> datos/cpi-par_exceso2-3p.dat && echo "\n#################################\n"; done
echo "He terminado 3procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 3 ./cpi-par_medio $i >> datos/cpi-par_medio2-3p.dat && echo "\n#################################\n"; done
echo "He terminado 3procesos en paralelo por Medio"

#4procesos O2
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 4 ./cpi-par_defecto $i >> datos/cpi-par_defecto2-4p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 4 ./cpi-par_exceso $i >> datos/cpi-par_exceso2-4p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 4 ./cpi-par_medio $i >> datos/cpi-par_medio2-4p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Medio"

#6procesos O2
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 5 ./cpi-par_defecto $i >> datos/cpi-par_defecto2-5p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 5 ./cpi-par_exceso $i >> datos/cpi-par_exceso2-5p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 5 ./cpi-par_medio $i >> datos/cpi-par_medio2-5p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Medio"

#8procesos O2
for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_defecto.dat && mpirun -hostfile host.txt -np 6 ./cpi-par_defecto $i >> datos/cpi-par_defecto2-6p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Defecto"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_exceso.dat && mpirun -hostfile host.txt -np 6 ./cpi-par_exceso $i >> datos/cpi-par_exceso2-6p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Exceso"

for((i=100; i < 1000000000;i*=10));do echo "Intervalo = " $i >> cpi-par_medio.dat && mpirun -hostfile host.txt -np 6 ./cpi-par_medio $i >> datos/cpi-par_medio2-6p.dat && echo "\n#################################\n"; done
echo "He terminado 4procesos en paralelo por Medio"
