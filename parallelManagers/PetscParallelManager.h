#ifndef _PETSC_PARALLEL_MANAGER_H_
#define _PETSC_PARALLEL_MANAGER_H_

//#include "PetscParallelManager.h"
#include "../stencils/PressureBufferFillStencil.h"
#include "../stencils/PressureBufferReadStencil.h"
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
		//TODO: VelocityBufferFillStencil & _velocityBufferFillStencil;
		//TODO: VelocityBufferReadStencil & _velocityBufferReadStencil;
		ParallelBoundaryIterator<FlowField> & _parallelPressureFillBoundaryIterator;
		ParallelBoundaryIterator<FlowField> & _parallelPressureReadBoundaryIterator;

    public:

        /** Constructor
         * @param parameters Reference to the parameters
         */
        PetscParallelManager(Parameters & parameters, PressureBufferFillStencil & pressureBufferFillStencil, PressureBufferReadStencil & pressureBufferReadStencil, ParallelBoundaryIterator<FlowField> & parallelPressureFillBoundaryIterator, ParallelBoundaryIterator<FlowField> & parallelPressureReadBoundaryIterator);

        /** Destructor */
        //~PetscParallelManager();

        void communicatePressure();

        //TODO: void communicateVelocities();

};


#include "PetscParallelManager.cpph"

#endif
