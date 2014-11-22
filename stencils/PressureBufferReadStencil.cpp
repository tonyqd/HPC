#include "PressureBufferFillStencil.h"

PressureBufferFillStencil::PressureBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

}


void PressureBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  
  flowField.getPressure().getScalar(i,j,k) = leftPressureBuffer[k+(localSize[1]*j)];
  
}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {

  flowField.getPressure().getScalar(i,j,k) = rightPressureBuffer[k+(localSize[1]*j)];

}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {

  flowField.getPressure().getScalar(i,j,k) = bottomPressureBuffer[k+(localSize[0]*i)];

}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {

  flowField.getPressure().getScalar(i,j,k) = topPressureBuffer[k+(localSize[0]*i)];

}

void PressureBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  
  frontPressureBuffer[j+(localSize[0]*i)] = flowField.getPressure().getScalar(i,j,k);


  flowField.getPressure().getScalar(i,j,k) = frontPressureBuffer[j+(localSize[0]*i)];
  
}

void PressureBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {

  flowField.getPressure().getScalar(i,j,k) = backPressureBuffer[j+(localSize[0]*i)];

}
