#include "ViscosityBufferReadStencil.h"

ViscosityBufferReadStencil::ViscosityBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftViscosityReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightViscosityReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  
}


void ViscosityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */  
  flowField.getViscosity().getScalar(i-1,j,k) = leftViscosityReadBuffer[(k-2)+(localSize[1]*(j-2))]; 
}

void ViscosityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  flowField.getViscosity().getScalar(i+1,j,k) = rightViscosityReadBuffer[(k-2)+(localSize[1]*(j-2))];
}

void ViscosityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  flowField.getViscosity().getScalar(i,j-1,k) = bottomViscosityReadBuffer[(k-2)+(localSize[0]*(i-2))];
}

void ViscosityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
  flowField.getViscosity().getScalar(i,j+1,k) = topViscosityReadBuffer[(k-2)+(localSize[0]*(i-2))];
}

void ViscosityBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  flowField.getViscosity().getScalar(i,j,k-1) = frontViscosityReadBuffer[(j-2)+(localSize[0]*(i-2))];
}

void ViscosityBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */  
  flowField.getViscosity().getScalar(i,j,k+1) = backViscosityReadBuffer[(j-2)+(localSize[0]*(i-2))];
}

/*2D
 */
void ViscosityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}

