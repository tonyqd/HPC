#include "PetscParallelManager.h"
#include "../stencils/PressureBufferFillStencil.h"
#include "../stencils/PressureBufferReadStencil.h"
#include "../Iterators.h"
#include "../Parameters.h"
#include "../Definitions.h"
#include <mpi.h>


template <class FlowField>
void PetscParallelManager<FlowField>::communicatePressure() {

	_parallelPressureFillBoundaryIterator.iterate();

	// Copy from left to right
	MPI_Send(_pressureBufferFillStencil.leftPressureFillBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.rightNb, 0, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.rightPressureReadBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.leftNb, 0, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from right to left
	MPI_Send(_pressureBufferFillStencil.rightPressureFillBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.leftNb, 1, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.leftPressureReadBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.rightNb, 1, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from top to bottom
	MPI_Send(_pressureBufferFillStencil.topPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 2, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.bottomPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.topNb, 2, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from bottom to top
	MPI_Send(_pressureBufferFillStencil.bottomPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.topNb, 3, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.topPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 3, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from front to back
	MPI_Send(_pressureBufferFillStencil.frontPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.backNb, 4, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.backPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.frontNb, 4, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from back to front
	MPI_Send(_pressureBufferFillStencil.backPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.frontNb, 5, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.frontPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.backNb, 6, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	_parallelPressureReadBoundaryIterator.iterate();

}
