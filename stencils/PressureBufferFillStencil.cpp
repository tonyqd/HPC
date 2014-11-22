#include "PressureBufferFillStencil.h"

PressureBufferFillStencil::PressureBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftPressureBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightPressureBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomPressureBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topPressureBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontPressureBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backPressureBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}



void PressureBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  
  leftPressureBuffer[k+(localSize[1]*j)] = flowField.getPressure().getScalar(i,j,k);

}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  
  rightPressureBuffer[k+(localSize[1]*j)] = flowField.getPressure().getScalar(i,j,k);

}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  
  bottomPressureBuffer[k+(localSize[0]*i)] = flowField.getPressure().getScalar(i,j,k);

}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  
  topPressureBuffer[k+(localSize[0]*i)] = flowField.getPressure().getScalar(i,j,k);

}

void PressureBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  
  frontPressureBuffer[j+(localSize[0]*i)] = flowField.getPressure().getScalar(i,j,k);

}

void PressureBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  
  backPressureBuffer[j+(localSize[0]*i)] = flowField.getPressure().getScalar(i,j,k);

}
