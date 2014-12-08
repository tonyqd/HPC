#ifndef _DISTANCE_STENCIL_H_
#define _DISTANCE_STENCIL_H_

#include "../Stencil.h"
#include "../Parameters.h"
#include "../FlowField.h"

/** Stencil to compute the distance to the nearest wall.*/

class DistanceStencil : public FieldStencil<FlowField> {

    public:

        /** Constructor
         * @param parameters Parameters of the problem
         */
        DistanceStencil(const Parameters & parameters) : FieldStencil<FlowField> ( parameters ){

        }

        /** Apply the stencil in 2D
         * @param flowField Flow field information
         * @param i Position in the X direction
         * @param j Position in the Y direction
         */
        void apply ( FlowField & flowField, int i, int j ){

        	flowField.getDistance().getScalar(i,j,0) = 1;
        }



        /** Apply the stencil in 3D
         * @param flowField Flow field information
         * @param i Position in the X direction
         * @param j Position in the Y direction
         * @param k Position in the Z direction
         */
        void apply ( FlowField & flowField, int i, int j, int k ){

        	const int obstacle = flowField.getFlags().getValue(i,j,k);
        	if( (obstacle & OBSTACLE_SELF) == 0 ) {
        		FLOAT distance;
        		FLOAT tmp;

        		if(_parameters.simulation.scenario == "channel"){
        			if(_parameters.meshsize->getPosX(i,j,k) < _parameters.bfStep.xRatio*_parameters.geometry.lengthX){
        				distance = _parameters.meshsize->getPosY(i,j,k) + 0.5*(_parameters.meshsize->getDy(i,j,k)) - _parameters.bfStep.yRatio*_parameters.geometry.lengthY;
        				if ( (tmp = _parameters.geometry.lengthY - (_parameters.meshsize->getPosY(i,j,k) + 0.5*(_parameters.meshsize->getDy(i,j,k))) -  _parameters.bfStep.yRatio*_parameters.geometry.lengthY) <  _parameters.meshsize->getPosY(i,j,k) ) {
        					distance = tmp;
        				}
        			}
        			else{
        				distance = _parameters.meshsize->getPosY(i,j,k) + 0.5*(_parameters.meshsize->getDy(i,j,k));

        				if ( (tmp = _parameters.geometry.lengthY - (_parameters.meshsize->getPosY(i,j,k) + 0.5*(_parameters.meshsize->getDy(i,j,k)))) <  _parameters.meshsize->getPosY(i,j,k) ) {
      						distance = tmp;
        				}
     					if((tmp = (_parameters.meshsize->getPosZ(i,j,k) + 0.5*(_parameters.meshsize->getDz(i,j,k)))) < distance){
     						distance = tmp;
     					}
       					if((tmp = _parameters.geometry.lengthZ - (_parameters.meshsize->getPosZ(i,j,k) + 0.5*(_parameters.meshsize->getDz(i,j,k)))) < distance){
      						distance = tmp;
      					}
       					flowField.getDistance().getScalar(i,j,k) = distance;
        			}
        		}

        		if(_parameters.simulation.scenario != "channel"){
					distance = _parameters.meshsize->getPosY(i,j,k) + 0.5*(_parameters.meshsize->getDy(i,j,k));
					if ( (tmp = _parameters.geometry.lengthY - (_parameters.meshsize->getPosY(i,j,k) + 0.5*(_parameters.meshsize->getDy(i,j,k)))) <  _parameters.meshsize->getPosY(i,j,k) ) {
						distance = tmp;
					}
					if((tmp = (_parameters.meshsize->getPosZ(i,j,k) + 0.5*(_parameters.meshsize->getDz(i,j,k)))) < distance){
						distance = tmp;
					}
					if((tmp = _parameters.geometry.lengthZ - (_parameters.meshsize->getPosZ(i,j,k) + 0.5*(_parameters.meshsize->getDz(i,j,k)))) < distance){
						distance = tmp;
					}
					if((tmp = (_parameters.meshsize->getPosX(i,j,k) + 0.5*(_parameters.meshsize->getDx(i,j,k)))) < distance){
						distance = tmp;
					}
					if ( (tmp = _parameters.geometry.lengthX - (_parameters.meshsize->getPosX(i,j,k) + 0.5*(_parameters.meshsize->getDx(i,j,k)))) < distance){
						distance = tmp;
					}
					flowField.getDistance().getScalar(i,j,k) = distance;
        		}





        	}

        }
};

#endif
