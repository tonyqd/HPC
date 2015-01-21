#include "CheckpointReadStencil.h"
#include <stdio.h>
#include <stdlib.h>

CheckpointReadStencil::CheckpointReadStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {
	sprintf(filename, "%d", parameters.parallel.rank);

}

CheckpointReadStencil::~CheckpointReadStencil () {

}
void CheckpointReadStencil::apply ( FlowField & flowField, int i, int j ){


	FLOAT pressure_tmp;
	FLOAT velocity_tmp[3];

	FLOAT & pressure = flowField.getPressure().getScalar(i,j);
	FLOAT * velocity = flowField.getVelocity().getVector(i,j);

	fread(velocity_tmp, sizeof(FLOAT), 3, inputFile);
	fread(&pressure_tmp, sizeof(FLOAT), 1, inputFile);

	pressure = pressure_tmp;
	velocity[0] = velocity_tmp[0];
	velocity[1] = velocity_tmp[1];
	velocity[2] = velocity_tmp[2];

}

void CheckpointReadStencil::apply ( FlowField & flowField, int i, int j, int k ){


	FLOAT pressure_tmp;
	FLOAT velocity_tmp[3];

	FLOAT & pressure = flowField.getPressure().getScalar(i,j,k);
	FLOAT * velocity = flowField.getVelocity().getVector(i,j,k);

	fread(velocity_tmp, sizeof(FLOAT), 3, inputFile);
	fread(&pressure_tmp, sizeof(FLOAT), 1, inputFile);

	pressure = pressure_tmp;
	velocity[0] = velocity_tmp[0];
	velocity[1] = velocity_tmp[1];
	velocity[2] = velocity_tmp[2];


}


void CheckpointReadStencil::open (){

	inputFile = fopen(filename, "rb");
}

void CheckpointReadStencil::close (){

	fclose(inputFile);


}
