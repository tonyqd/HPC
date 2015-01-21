#ifndef _CHECKPOINT_WRITE_STENCIL_H_
#define _CHECKPOINT_WRITE_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"



class CheckpointWriteStencil : public FieldStencil<FlowField> {

private:

	FILE *outputFile;
	char filename[10];

public:

	/** Constructor
	 *
	 * @param prefix String with the prefix of the name of the VTK files
	 */
	CheckpointWriteStencil ( const Parameters & parameters );

	~CheckpointWriteStencil ();

	/** 2D operation for one position
	 *
	 * @param flowField State of the flow field
	 * @param i Position in the x direction
	 * @param j Position in the y direction
	 */
	void apply ( FlowField & flowField, int i, int j );

	/** 3D operation for one position
	 *
	 * @param flowField State of the flow field
	 * @param i Position in the x direction
	 * @param j Position in the y direction
	 * @param k Position in the z direction
	 */
	void apply ( FlowField & flowField, int i, int j, int k );

	/** Writes the information to the file
	 * @param flowField Flow field to be written
	 */

	void open ();

	void close ();

};

#endif
