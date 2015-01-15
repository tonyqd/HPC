#include "TurbulentViscosityStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

TurbulentViscosityStencil::TurbulentViscosityStencil ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}



void TurbulentViscosityStencil::apply ( FlowField & flowField,  int i, int j ){

	 const int obstacle = flowField.getFlags().getValue(i, j);

	 if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid

		 loadLocalVelocity2D(  flowField, _localVelocity, i, j);
	     loadLocalMeshsize2D(_parameters, _localMeshsize, i, j);

		 FLOAT S11 = 0.5*2.0*dudx(_localVelocity, _localMeshsize);
		 FLOAT S22 = 0.5*2.0*dvdy(_localVelocity, _localMeshsize);
		 FLOAT S12 = 0.5*(dudy(_localVelocity, _localMeshsize) + dvdx(_localVelocity, _localMeshsize));

		 //default
		 FLOAT lm = 0.0;
		 FLOAT delta;

		 //laminar reference case
		 if(_parameters.turbulent.model==1){
			 lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j);
		 }

		 //laminar flat plate Boundary layer
		 if(_parameters.turbulent.model==2){
			 lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j);
			 delta = 0.09*4.91*pow((_parameters.geometry.lengthX*(1.0-_parameters.bfStep.xRatio))/_parameters.flow.Re,0.5)*pow(_parameters.meshsize->getPosX(i,j) + _parameters.meshsize->getDx(i,j),0.5);
			 if(lm > delta){
				 lm = delta;
			 }
		 }

		 //turbulent flat plate Boundary layer
		 if(_parameters.turbulent.model==3){
			 lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j);
			 delta = 0.09*0.382*pow((_parameters.geometry.lengthX*(1.0-_parameters.bfStep.xRatio))/_parameters.flow.Re,0.2)*pow(_parameters.meshsize->getPosX(i,j) + _parameters.meshsize->getDx(i,j),0.8);
			 if(lm > delta){
				 lm = delta;
			 }
		 }

		 flowField.getViscosity().getScalar(i,j) = lm * lm * sqrt(2.0*((S11*S11 + S22*S22)+2.0*(S12*S12)));

	 }
}

void TurbulentViscosityStencil::apply ( FlowField & flowField,  int i, int j, int k ){

	 const int obstacle = flowField.getFlags().getValue(i, j, k);

	 if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid

	     loadLocalVelocity3D(  flowField, _localVelocity, i, j, k);
	     loadLocalMeshsize3D(_parameters, _localMeshsize, i, j, k);

		 FLOAT S11 = 0.5*2.0*dudx(_localVelocity, _localMeshsize);
		 FLOAT S22 = 0.5*2.0*dvdy(_localVelocity, _localMeshsize);
		 FLOAT S33 = 0.5*2.0*dwdz(_localVelocity, _localMeshsize);
		 FLOAT S12 = 0.5*(dudy(_localVelocity, _localMeshsize) + dvdx(_localVelocity, _localMeshsize));
		 FLOAT S13 = 0.5*(dudz(_localVelocity, _localMeshsize) + dwdx(_localVelocity, _localMeshsize));
		 FLOAT S23 = 0.5*(dvdz(_localVelocity, _localMeshsize) + dwdy(_localVelocity, _localMeshsize));

		 //default
		 FLOAT lm = 0.0;
		 FLOAT delta;

		 //laminar reference case
		 if(_parameters.turbulent.model==1){
			 lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j,k);
		 }

		 //laminar flat plate Boundary layer
		 if(_parameters.turbulent.model==2){
			 lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j,k);
			 delta = 0.09*4.91*pow((_parameters.geometry.lengthX*(1.0-_parameters.bfStep.xRatio))/_parameters.flow.Re,0.5)*pow(_parameters.meshsize->getPosX(i,j,k) + _parameters.meshsize->getDx(i,j,k),0.5);
			 if(lm > delta){
				 lm = delta;
			 }
		 }

		 //turbulent flat plate Boundary layer
		 if(_parameters.turbulent.model==3){
			 lm = _parameters.turbulent.kappa*flowField.getDistance().getScalar(i,j,k);
			 delta = 0.09*0.382*pow((_parameters.geometry.lengthX*(1.0-_parameters.bfStep.xRatio))/_parameters.flow.Re,0.2)*pow(_parameters.meshsize->getPosX(i,j,k) + _parameters.meshsize->getDx(i,j,k),0.8);
			 if(lm > delta){
				 lm = delta;
			 }
		 }

		 flowField.getViscosity().getScalar(i,j,k) = lm * lm * sqrt(2.0*((S11*S11 + S22*S22 + S33*S33)+2.0*(S12*S12 + S13*S13 + S23*S23)));

	 }

}

