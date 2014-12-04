#include "PressureBufferReadStencil.h"

PressureBufferReadStencil::PressureBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftPressureReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightPressureReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  
}


void PressureBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */  
  flowField.getPressure().getScalar(i-1,j,k) = leftPressureReadBuffer[(k-2)+(localSize[2]*(j-2))];
}

void PressureBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  flowField.getPressure().getScalar(i+1,j,k) = rightPressureReadBuffer[(k-2)+(localSize[2]*(j-2))];
}

void PressureBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  flowField.getPressure().getScalar(i,j-1,k) = bottomPressureReadBuffer[(k-2)+(localSize[2]*(i-2))];
}

void PressureBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
  flowField.getPressure().getScalar(i,j+1,k) = topPressureReadBuffer[(k-2)+(localSize[2]*(i-2))];
}

void PressureBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  flowField.getPressure().getScalar(i,j,k-1) = frontPressureReadBuffer[(j-2)+(localSize[1]*(i-2))];
}

void PressureBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */  
  flowField.getPressure().getScalar(i,j,k+1) = backPressureReadBuffer[(j-2)+(localSize[1]*(i-2))];
}

/*2D
 */
void PressureBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}

