#include "TurbulentViscosityStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

TurbulentViscosityStencil::TurbulentViscosityStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}



void TurbulentViscosityStencil::apply ( FlowField & flowField,  int i, int j ){

}

void TurbulentViscosityStencil::apply ( FlowField & flowField,  int i, int j, int k ){

	 const int obstacle = flowField.getFlags().getValue(i, j, k);

	 if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid

		 loadLocalVelocity3D(  flowField, _localVelocity, i, j, k);
	     loadLocalMeshsize3D(_parameters, _localMeshsize, i, j, k);

		 FLOAT S11 = 2.0*dudx(_localVelocity, _localMeshsize);
		 FLOAT S22 = 2.0*dvdy(_localVelocity, _localMeshsize);
		 FLOAT S33 = 2.0*dwdz(_localVelocity, _localMeshsize);
		 FLOAT S12 = dudy(_localVelocity, _localMeshsize) + dvdx(_localVelocity, _localMeshsize);
		 FLOAT S13 =
		 FLOAT S23 = 1;


	 }
}

