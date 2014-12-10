#include "TurbulentViscosityStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

TurbulentViscosityStencil::TurbulentViscosityStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}



void TurbulentViscosityStencil::apply ( FlowField & flowField,  int i, int j ){

	 const int obstacle = flowField.getFlags().getValue(i, j);

	 if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid

		 loadLocalVelocity2D(  flowField, _localVelocity, i, j);
	     loadLocalMeshsize2D(_parameters, _localMeshsize, i, j);

		 FLOAT S11 = 2.0*dudx(_localVelocity, _localMeshsize);
		 FLOAT S22 = 2.0*dvdy(_localVelocity, _localMeshsize);
		 FLOAT S12 = dudy(_localVelocity, _localMeshsize) + dvdx(_localVelocity, _localMeshsize);

		 FLOAT lm = 1; //mixing length input

		 flowField.getViscosity().getScalar(i,j) = lm * lm * sqrt(2.0*((S11*S11 + S22*S22)+2.0*(S12*S12)));

	 }
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
		 FLOAT S13 = dudz(_localVelocity, _localMeshsize) + dwdx(_localVelocity, _localMeshsize);
		 FLOAT S23 = dvdz(_localVelocity, _localMeshsize) + dwdy(_localVelocity, _localMeshsize);

		 FLOAT tmp = flowField.getDistance().getScalar(i,j,k);
		 FLOAT lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j,k); //mixing length input
		 FLOAT delta = 1; //needs to be changed

		 if(lm > 0.09*delta){
			 lm = 0.09*delta;
		 }

		 flowField.getViscosity().getScalar(i,j,k) = lm * lm * sqrt(2.0*((S11*S11 + S22*S22 + S33*S33)+2.0*(S12*S12 + S13*S13 + S23*S23)));

	 }

}

