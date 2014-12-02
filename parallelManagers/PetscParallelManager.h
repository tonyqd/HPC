#ifndef _PETSC_PARALLEL_MANAGER_H_
#define _PETSC_PARALLEL_MANAGER_H_

#include "../stencils/PressureBufferFillStencil.h"
#include "../stencils/PressureBufferReadStencil.h"
#include "../stencils/VelocityBufferFillStencil.h"
#include "../stencils/VelocityBufferReadStencil.h"
#include "../Iterators.h"
#include "../Parameters.h"
#include "../Definitions.h"
#include <mpi.h>


/** WS2.1.4. Class integrating reading (writing) from (to) parallel buffers.
 */
template <class FlowField>
class PetscParallelManager {

    private:

        Parameters & _parameters;   //! Reference to the parameters

        PressureBufferFillStencil & _pressureBufferFillStencil;
	  PressureBufferReadStencil & _pressureBufferReadStencil;
	  VelocityBufferFillStencil & _velocityBufferFillStencil;
	  VelocityBufferReadStencil & _velocityBufferReadStencil;
	  ParallelBoundaryIterator<FlowField> & _parallelPressureFillBoundaryIterator;
	  ParallelBoundaryIterator<FlowField> & _parallelPressureReadBoundaryIterator;
	  ParallelBoundaryIterator<FlowField> & _parallelVelocityFillBoundaryIterator;
	  ParallelBoundaryIterator<FlowField> & _parallelVelocityReadBoundaryIterator;

    public:

        /** Constructor
         * @param parameters Reference to the parameters
         */
	  	  PetscParallelManager(Parameters & parameters, PressureBufferFillStencil & pressureBufferFillStencil, PressureBufferReadStencil & pressureBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelPressureFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelPressureReadBoundaryIterator,
        		VelocityBufferFillStencil & velocityBufferFillStencil, VelocityBufferReadStencil & velocityBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelVelocityFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelVelocityReadBoundaryIterator);

        /** Destructor */
        //~PetscParallelManager();

        void communicatePressure();

        void communicateVelocities();

};


//#include "PetscParallelManager.cpph"
template <class FlowField> PetscParallelManager<FlowField>::PetscParallelManager(Parameters & parameters, PressureBufferFillStencil & pressureBufferFillStencil, PressureBufferReadStencil & pressureBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelPressureFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelPressureReadBoundaryIterator,
		VelocityBufferFillStencil & velocityBufferFillStencil, VelocityBufferReadStencil & velocityBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelVelocityFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelVelocityReadBoundaryIterator):
_parameters(parameters),
_pressureBufferFillStencil(pressureBufferFillStencil),
_pressureBufferReadStencil(pressureBufferReadStencil),
_parallelPressureFillBoundaryIterator(parallelPressureFillBoundaryIterator),
_parallelPressureReadBoundaryIterator(parallelPressureReadBoundaryIterator),
_velocityBufferFillStencil(velocityBufferFillStencil),
_velocityBufferReadStencil(velocityBufferReadStencil),
_parallelVelocityFillBoundaryIterator(parallelVelocityFillBoundaryIterator),
_parallelVelocityReadBoundaryIterator(parallelVelocityReadBoundaryIterator) {

}


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

template <class FlowField>
void PetscParallelManager<FlowField>::communicateVelocities() {

	_parallelVelocityFillBoundaryIterator.iterate();

	// Copy from left to right
	MPI_Send(_velocityBufferFillStencil.leftVelocityFillBuffer, (_velocityBufferFillStencil.localSize[1])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.rightNb, 0, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.rightVelocityReadBuffer, (_velocityBufferFillStencil.localSize[1])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.leftNb, 0, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from right to left
	MPI_Send(_velocityBufferFillStencil.rightVelocityFillBuffer, (_velocityBufferFillStencil.localSize[1])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.leftNb, 1, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.leftVelocityReadBuffer, (_velocityBufferFillStencil.localSize[1])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.rightNb, 1, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from top to bottom
	MPI_Send(_velocityBufferFillStencil.topVelocityFillBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 2, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.bottomVelocityReadBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.topNb, 2, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from bottom to top
	MPI_Send(_velocityBufferFillStencil.bottomVelocityFillBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.topNb, 3, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.topVelocityReadBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 3, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from front to back
	MPI_Send(_velocityBufferFillStencil.frontVelocityFillBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.backNb, 4, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.backVelocityReadBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.frontNb, 4, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from back to front
	MPI_Send(_velocityBufferFillStencil.backVelocityFillBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.frontNb, 5, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.frontVelocityReadBuffer, (_velocityBufferFillStencil.localSize[0])*(_velocityBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.backNb, 6, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	_parallelVelocityReadBoundaryIterator.iterate();
}


#endif
