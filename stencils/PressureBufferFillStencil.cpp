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

}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {

}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {

}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {

}

void PressureBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {

}

void PressureBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {

}
