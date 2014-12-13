#ifndef _MAX_VISCOSITY_STENCIL_H_
#define _MAX_VISCOSITY_STENCIL_H_

#include "../Stencil.h"
#include "../Parameters.h"
#include "../FlowField.h"


/** this class computes the maximum value of max(viscosity) for all grid cells.
 */
class MaxViscosityStencil : public FieldStencil<FlowField> {

    public:

		FLOAT _maxViscosity;

        /** Constructor
         *
         * @param parameters Parameters of the problem
         */
        MaxViscosityStencil (const Parameters & parameters);

        //@ brief Body iterations
        //@{
        void apply (FlowField & flowField, int i, int j){

        	if (flowField.getViscosity().getScalar(i,j) > _maxViscosity){
        		_maxViscosity = flowField.getViscosity().getScalar(i,j);

        	}

        }


        void apply (FlowField & flowField, int i, int j, int k){

        	if (flowField.getViscosity().getScalar(i,j,k) > _maxViscosity){
        	        _maxViscosity = flowField.getViscosity().getScalar(i,j,k);

        	}
        }




        /** Resets the maximum values to zero before computing the timestep
         */
        void reset (){

        	_maxViscosity = 0;
        }



        const FLOAT * getMaxValues() const{

        	return _maxViscosity;
        }
};

#endif
