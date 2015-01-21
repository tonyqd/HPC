#include "CheckpointWriteStencil.h"
#include "fstream"
#include "string"
#include "sstream"
#include <stdio.h>
#include <stdlib.h>

CheckpointWriteStencil::CheckpointWriteStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {
	sprintf(filename, "%d", parameters.parallel.rank);
}

CheckpointWriteStencil::~CheckpointWriteStencil () {

}
void CheckpointWriteStencil::apply ( FlowField & flowField, int i, int j ){

	FLOAT pressure_tmp;
	FLOAT velocity_tmp[3];

	FLOAT & pressure = flowField.getPressure().getScalar(i,j);
	FLOAT * velocity = flowField.getVelocity().getVector(i,j);

	fwrite(velocity, sizeof(FLOAT), 3, outputFile);
	fwrite(&pressure, sizeof(FLOAT), 1, outputFile);

}

void CheckpointWriteStencil::apply ( FlowField & flowField, int i, int j, int k ){

	FLOAT pressure_tmp;
	FLOAT velocity_tmp[3];

	FLOAT & pressure = flowField.getPressure().getScalar(i,j,k);
	FLOAT * velocity = flowField.getVelocity().getVector(i,j,k);

	fwrite(velocity, sizeof(FLOAT), 3, outputFile);
	fwrite(&pressure, sizeof(FLOAT), 1, outputFile);
        

}

void CheckpointWriteStencil::open (){

	outputFile = fopen(filename, "wb");

}

void CheckpointWriteStencil::close (){

	fclose(outputFile);

}
