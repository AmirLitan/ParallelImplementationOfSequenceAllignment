build:
	mpicxx -fopenmp -c Fp4.c -o Fp4.o
	mpicxx -fopenmp -c CalculatFunction.c -o CalculatFunction.o 
	mpicxx -fopenmp -c FileFunction.c -o FileFunction.o 
	mpicxx -fopenmp -c openMpFunction.c -o openMpFunction.o
	mpicxx -fopenmp -o Fp4  Fp4.o CalculatFunction.o openMpFunction.o FileFunction.o 

clean:
	rm -f *.o ./Fp4

run:
	mpiexec -np 2 ./Fp4

runOn2:
	mpiexec -np 2 -machinefile  mf  -map-by  node  ./Fp4

