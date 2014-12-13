#include "MaxViscosityStencil.h"
#include <algorithm>
#include <math.h>


MaxViscosityStencil::MaxViscosityStencil (const Parameters & parameters) :
    FieldStencil<FlowField> (parameters) {
    reset();
}

void MaxViscosityStencil::apply (FlowField & flowField, int i, int j){
	if (flowField.getViscosity().getScalar(i,j) > _maxViscosity){
		_maxViscosity = flowField.getViscosity().getScalar(i,j);

	}
}

void MaxViscosityStencil::apply (FlowField & flowField, int i, int j, int k){
	if (flowField.getViscosity().getScalar(i,j,k) > _maxViscosity){
	        _maxViscosity = flowField.getViscosity().getScalar(i,j,k);

	}
}



void MaxViscosityStencil::reset () {
	_maxViscosity = 0;

}

const FLOAT MaxViscosityStencil::getMaxValues() const{
	return _maxViscosity;
}
