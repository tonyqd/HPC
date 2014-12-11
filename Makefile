#### The petsc environment can also be fixed in the makefile
PETSC_DIR =/home/matthias/Downloads/petsc-3.5.2
#PETSC_DIR = /usr/work/petsc-3.5.2
#PETSC_ARCH = arch-linux2-c-debug

include ${PETSC_DIR}/conf/variables
#PETSC_KSP_LIB=-Wall -Werror -O3 -pg -L/lrz/sys/libraries/petsc/3.3-p2/real_mpi.intel_121_opt/linux-gnu-intel/lib  -lpetsc -lX11 -lpthread -L/lrz/sys/intel/mkl103u9/lib/em64t -Wl,--start-group -Wl,-rpath,/lrz/sys/intel/mkl103u9/lib/em64t -mkl -Wl,--end-group -liomp5 -lpthread -lm -L/lrz/sys/intel/mpi_40_3_008/intel64/lib -L/lrz/mnt/sys.x86_64/intel/icc_121_339/composer_xe_2011_sp1.11.339/compiler/lib/intel64 -L/usr/lib64/gcc/x86_64-suse-linux/4.3 -L/usr/x86_64-suse-linux/lib -Wl,-rpath,/lrz/mnt/sys.x86_64/libraries/petsc/3.3-p2/real_mpi.intel_121_opt/-Xlinker -L/lrz/mnt/sys.x86_64/intel/ifort_121_339/composer_xe_2011_sp1.11.339/compiler/lib/intel64 -Wl,-rpath,/lrz/sys/intel/mpi_40_3_008/intel64/lib -Wl,-rpath,/opt/intel/mpi-rt/4.0.3 -lifport -lifcore -lm -lm -ldl -lmpi -lmpigf -lmpigi -lpthread -lrt -limf -lsvml -lipgo -ldecimal -lcilkrts -lstdc++ -lgcc_s -lirc -lirc_s -ldl

# default gnu compiler (currently not used)
# CC = g++
# compiler wrapper for mac-cluster
CC = mpiCC
# compiler on Ubuntu
#
#
#CC = mpic++
CFLAGS = -Wall -O3 #-Werror 
SRCDIR = ./
INCLUDE = -I. -Istencils ${PETSC_CC_INCLUDES}


NSMAIN = main.o

OBJ = DataStructures.o Configuration.o 3rdparty/tinyxml2/tinyxml2.o SimpleTimer.o

NSOBJ = FlowField.o LinearSolver.o Meshsize.o\
stencils/MaxUStencil.o stencils/MovingWallStencils.o stencils/PeriodicBoundaryStencils.o\
stencils/FGHStencil.o stencils/TurbulentViscosityStencil.o stencils/FGHStencilTurbulent.o solvers/SORSolver.o solvers/PetscSolver.o \
stencils/RHSStencil.o stencils/VelocityStencil.o \
stencils/VTKStencil.o stencils/VTKStencilTurbulent.o \
parallelManagers/PetscParallelConfiguration.o\
GlobalBoundaryFactory.o\
stencils/BFStepInitStencil.o stencils/NeumannBoundaryStencils.o stencils/BFInputStencils.o\
stencils/PressureBufferFillStencil.o \
stencils/PressureBufferReadStencil.o \
stencils/VelocityBufferFillStencil.o \
stencils/VelocityBufferReadStencil.o \
stencils/ViscosityBufferFillStencil.o \
stencils/ViscosityBufferReadStencil.o \

#parallelManagers/PetscParallelManager.o \

all: ns

ns: $(OBJ) $(NSOBJ) $(NSMAIN)
	$(CC) -o ns $(OBJ) $(NSOBJ) $(NSMAIN) $(PETSC_KSP_LIB) -lstdc++ $(CFLAGS)


%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $*.o $*.cpp $(PETSC_KSP_LIB) -lstdc++

cleanall clean:
	for name in  ns main.o $(NSOBJ) $(OBJ) ; do \
	if [ -f $$name ]; then rm $$name; fi; \
	done;
