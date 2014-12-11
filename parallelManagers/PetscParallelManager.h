#ifndef _PETSC_PARALLEL_MANAGER_H_
#define _PETSC_PARALLEL_MANAGER_H_

#include "../stencils/PressureBufferFillStencil.h"
#include "../stencils/PressureBufferReadStencil.h"
#include "../stencils/VelocityBufferFillStencil.h"
#include "../stencils/VelocityBufferReadStencil.h"
#include "../stencils/ViscosityBufferFillStencil.h"
#include "../stencils/ViscosityBufferReadStencil.h"

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

	  ViscosityBufferFillStencil & _viscosityBufferFillStencil;
	  ViscosityBufferReadStencil & _viscosityBufferReadStencil;
	  ParallelBoundaryIterator<FlowField> & _parallelViscosityFillBoundaryIterator;
	  ParallelBoundaryIterator<FlowField> & _parallelViscosityReadBoundaryIterator;


    public:

        /** Constructor
         * @param parameters Reference to the parameters
         */
	  	  PetscParallelManager(Parameters & parameters, PressureBufferFillStencil & pressureBufferFillStencil, PressureBufferReadStencil & pressureBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelPressureFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelPressureReadBoundaryIterator,
        		VelocityBufferFillStencil & velocityBufferFillStencil, VelocityBufferReadStencil & velocityBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelVelocityFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelVelocityReadBoundaryIterator,
        		ViscosityBufferFillStencil & viscosityBufferFillStencil, ViscosityBufferReadStencil & viscosityBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelViscosityFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelViscosityReadBoundaryIterator);

        /** Destructor */
        //~PetscParallelManager();

        void communicatePressure();

        void communicateVelocities();

        void communicateViscosity();


};


//#include "PetscParallelManager.cpph"
template <class FlowField> PetscParallelManager<FlowField>::PetscParallelManager(Parameters & parameters, PressureBufferFillStencil & pressureBufferFillStencil, PressureBufferReadStencil & pressureBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelPressureFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelPressureReadBoundaryIterator,
		VelocityBufferFillStencil & velocityBufferFillStencil, VelocityBufferReadStencil & velocityBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelVelocityFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelVelocityReadBoundaryIterator,
		ViscosityBufferFillStencil & viscosityBufferFillStencil, ViscosityBufferReadStencil & viscosityBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelViscosityFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelViscosityReadBoundaryIterator):
_parameters(parameters),
_pressureBufferFillStencil(pressureBufferFillStencil),
_pressureBufferReadStencil(pressureBufferReadStencil),
_parallelPressureFillBoundaryIterator(parallelPressureFillBoundaryIterator),
_parallelPressureReadBoundaryIterator(parallelPressureReadBoundaryIterator),
_velocityBufferFillStencil(velocityBufferFillStencil),
_velocityBufferReadStencil(velocityBufferReadStencil),
_parallelVelocityFillBoundaryIterator(parallelVelocityFillBoundaryIterator),
_parallelVelocityReadBoundaryIterator(parallelVelocityReadBoundaryIterator),
_viscosityBufferFillStencil(viscosityBufferFillStencil),
_viscosityBufferReadStencil(viscosityBufferReadStencil),
_parallelViscosityFillBoundaryIterator(parallelViscosityFillBoundaryIterator),
_parallelViscosityReadBoundaryIterator(parallelViscosityReadBoundaryIterator) {}


template <class FlowField>
void PetscParallelManager<FlowField>::communicatePressure() {

	_parallelPressureFillBoundaryIterator.iterate();

	// Copy from left to right
	MPI_Send(_pressureBufferFillStencil.leftPressureFillBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.leftNb, 0, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.rightPressureReadBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.rightNb, 0, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from right to left
	MPI_Send(_pressureBufferFillStencil.rightPressureFillBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.rightNb, 1, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.leftPressureReadBuffer, (_pressureBufferFillStencil.localSize[1])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.leftNb, 1, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	_parallelPressureReadBoundaryIterator.iterate();
	_parallelPressureFillBoundaryIterator.iterate();
	// Copy from top to bottom
	MPI_Send(_pressureBufferFillStencil.topPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.topNb, 2, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.bottomPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 2, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from bottom to top
	MPI_Send(_pressureBufferFillStencil.bottomPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 3, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.topPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[2]), MY_MPI_FLOAT, _parameters.parallel.topNb, 3, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);
	_parallelPressureReadBoundaryIterator.iterate();
	// Copy from front to back
	_parallelPressureFillBoundaryIterator.iterate();
	MPI_Send(_pressureBufferFillStencil.frontPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.frontNb, 4, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.backPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.backNb, 4, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from back to front
	MPI_Send(_pressureBufferFillStencil.backPressureFillBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.backNb, 5, PETSC_COMM_WORLD);
	MPI_Recv(_pressureBufferReadStencil.frontPressureReadBuffer, (_pressureBufferFillStencil.localSize[0])*(_pressureBufferFillStencil.localSize[1]), MY_MPI_FLOAT, _parameters.parallel.frontNb, 5, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	_parallelPressureReadBoundaryIterator.iterate();

}

template <class FlowField>
void PetscParallelManager<FlowField>::communicateVelocities() {

	int dim = _parameters.geometry.dim;

	_parallelVelocityFillBoundaryIterator.iterate();

	// Copy from left to right
	MPI_Send(_velocityBufferFillStencil.leftVelocityFillBuffer, ((_velocityBufferFillStencil.localSize[1] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * dim), MY_MPI_FLOAT, _parameters.parallel.leftNb, 0, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.rightVelocityReadBuffer, ((_velocityBufferFillStencil.localSize[1] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * dim), MY_MPI_FLOAT, _parameters.parallel.rightNb, 0, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from right to left
	MPI_Send(_velocityBufferFillStencil.rightVelocityFillBuffer, ((_velocityBufferFillStencil.localSize[1] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * 2*(dim )), MY_MPI_FLOAT, _parameters.parallel.rightNb, 1, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.leftVelocityReadBuffer, ((_velocityBufferFillStencil.localSize[1] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * 2*(dim )), MY_MPI_FLOAT, _parameters.parallel.leftNb, 1, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);
	_parallelVelocityReadBoundaryIterator.iterate();

	// Copy from bottom to top
	_parallelVelocityFillBoundaryIterator.iterate();
	MPI_Send(_velocityBufferFillStencil.bottomVelocityFillBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * dim), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 2, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.topVelocityReadBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * dim), MY_MPI_FLOAT, _parameters.parallel.topNb, 2, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from top to bottom
	MPI_Send(_velocityBufferFillStencil.topVelocityFillBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * 2*(dim)), MY_MPI_FLOAT, _parameters.parallel.topNb, 3, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.bottomVelocityReadBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[2] + 3) * 2*(dim )), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 3, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);
	_parallelVelocityReadBoundaryIterator.iterate();

	// Copy from front to back
	_parallelVelocityFillBoundaryIterator.iterate();
	MPI_Send(_velocityBufferFillStencil.frontVelocityFillBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[1] + 3) * dim), MY_MPI_FLOAT, _parameters.parallel.frontNb, 4, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.backVelocityReadBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[1] + 3) * dim), MY_MPI_FLOAT, _parameters.parallel.backNb, 4, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from back to front
	MPI_Send(_velocityBufferFillStencil.backVelocityFillBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[1] + 3) * 2*(dim )), MY_MPI_FLOAT, _parameters.parallel.backNb, 5, PETSC_COMM_WORLD);
	MPI_Recv(_velocityBufferReadStencil.frontVelocityReadBuffer, ((_velocityBufferFillStencil.localSize[0] + 3) * (_velocityBufferFillStencil.localSize[1] + 3) * 2*(dim )), MY_MPI_FLOAT, _parameters.parallel.frontNb, 5, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	_parallelVelocityReadBoundaryIterator.iterate();
}

template <class FlowField>
void PetscParallelManager<FlowField>::communicateViscosity() {

	int dim = _parameters.geometry.dim;

	_parallelViscosityFillBoundaryIterator.iterate();

	// Copy from left to right
	MPI_Send(_viscosityBufferFillStencil.leftViscosityFillBuffer, ((_viscosityBufferFillStencil.localSize[1] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3) ), MY_MPI_FLOAT, _parameters.parallel.leftNb, 0, PETSC_COMM_WORLD);
	MPI_Recv(_viscosityBufferReadStencil.rightViscosityReadBuffer, ((_viscosityBufferFillStencil.localSize[1] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3)), MY_MPI_FLOAT, _parameters.parallel.rightNb, 0, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from right to left
	MPI_Send(_viscosityBufferFillStencil.rightViscosityFillBuffer, ((_viscosityBufferFillStencil.localSize[1] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3)), MY_MPI_FLOAT, _parameters.parallel.rightNb, 1, PETSC_COMM_WORLD);
	MPI_Recv(_viscosityBufferReadStencil.leftViscosityReadBuffer, ((_viscosityBufferFillStencil.localSize[1] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3) ), MY_MPI_FLOAT, _parameters.parallel.leftNb, 1, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);
	_parallelViscosityReadBoundaryIterator.iterate();

	// Copy from bottom to top
	_parallelViscosityFillBoundaryIterator.iterate();
	MPI_Send(_viscosityBufferFillStencil.bottomViscosityFillBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3)), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 2, PETSC_COMM_WORLD);
	MPI_Recv(_viscosityBufferReadStencil.topViscosityReadBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3) ), MY_MPI_FLOAT, _parameters.parallel.topNb, 2, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from top to bottom
	MPI_Send(_viscosityBufferFillStencil.topViscosityFillBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3) ), MY_MPI_FLOAT, _parameters.parallel.topNb, 3, PETSC_COMM_WORLD);
	MPI_Recv(_viscosityBufferReadStencil.bottomViscosityReadBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[2] + 3) ), MY_MPI_FLOAT, _parameters.parallel.bottomNb, 3, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);
	_parallelViscosityReadBoundaryIterator.iterate();

	// Copy from front to back
	_parallelViscosityFillBoundaryIterator.iterate();
	MPI_Send(_viscosityBufferFillStencil.frontViscosityFillBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[1] + 3) ), MY_MPI_FLOAT, _parameters.parallel.frontNb, 4, PETSC_COMM_WORLD);
	MPI_Recv(_viscosityBufferReadStencil.backViscosityReadBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[1] + 3)), MY_MPI_FLOAT, _parameters.parallel.backNb, 4, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	// Copy from back to front
	MPI_Send(_viscosityBufferFillStencil.backViscosityFillBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[1] + 3) ), MY_MPI_FLOAT, _parameters.parallel.backNb, 5, PETSC_COMM_WORLD);
	MPI_Recv(_viscosityBufferReadStencil.frontViscosityReadBuffer, ((_viscosityBufferFillStencil.localSize[0] + 3) * (_viscosityBufferFillStencil.localSize[1] + 3)), MY_MPI_FLOAT, _parameters.parallel.frontNb, 5, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);

	_parallelViscosityReadBoundaryIterator.iterate();
}

#endif
