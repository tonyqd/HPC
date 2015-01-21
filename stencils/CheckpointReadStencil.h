#ifndef _CHECKPOINT_READ_STENCIL_H_
#define _CHECKPOINT_READ_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"



class CheckpointReadStencil : public FieldStencil<FlowField> {

	FILE * inputFile;
	char filename[10];

public:

	/** Constructor
	 *
	 * @param prefix String with the prefix of the name of the VTK files
	 */
	CheckpointReadStencil ( const Parameters & parameters );

	~CheckpointReadStencil ();

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
