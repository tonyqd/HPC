#ifndef _CHECKPOINT_MANAGER_H_
#define _CHECKPOINT_MANAGER_H_

#include "../stencils/CheckpointWriteStencil.h"
#include "../stencils/CheckpointReadStencil.h"

#include "../Iterators.h"
#include "../Parameters.h"
#include "../Definitions.h"
#include <mpi.h>


template <class FlowField>
class CheckpointManager {

private:

	FlowField & _flowField;

	Parameters & _parameters;   //! Reference to the parameters

	CheckpointWriteStencil & _checkpointWriteStencil;
	FieldIterator<FlowField> & _checkpointWriteIterator;

	CheckpointReadStencil & _checkpointReadStencil;
	FieldIterator<FlowField> & _checkpointReadIterator;

public:

	/** Constructor
	 * @param parameters Reference to the parameters
	 */
	CheckpointManager(FlowField & flowField, Parameters & parameters, CheckpointWriteStencil & checkpointWriteStencil, FieldIterator<FlowField> & checkpointWriteIterator, CheckpointReadStencil & checkpointReadStencil, FieldIterator<FlowField> & checkpointReadIterator);

	/** Destructor */
	//~PetscParallelManager();

	void writeMultFiles();

	void readMultFiles();

	void writeSingleFile();

};

template <class FlowField> CheckpointManager<FlowField>::CheckpointManager(FlowField & flowField, Parameters & parameters, CheckpointWriteStencil & checkpointWriteStencil, FieldIterator<FlowField> & checkpointWriteIterator, CheckpointReadStencil & checkpointReadStencil, FieldIterator<FlowField> & checkpointReadIterator):
		_flowField(flowField),
		_parameters(parameters),
		_checkpointWriteStencil(checkpointWriteStencil),
		_checkpointWriteIterator(checkpointWriteIterator),
		_checkpointReadStencil(checkpointReadStencil),
		_checkpointReadIterator(checkpointReadIterator){}

template <class FlowField>
void CheckpointManager<FlowField>::writeMultFiles() {
	_checkpointWriteStencil.open();
	_checkpointWriteIterator.iterate();
	_checkpointWriteStencil.close();
}

template <class FlowField>
void CheckpointManager<FlowField>::readMultFiles() {
	_checkpointReadStencil.open();
	_checkpointReadIterator.iterate();
	_checkpointReadStencil.close();
}

#endif
