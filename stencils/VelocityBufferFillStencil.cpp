#include "VelocityBufferFillStencil.h"

VelocityBufferFillStencil::VelocityBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftVelocityFillBuffer = new FLOAT[(localSize[1]) * (localSize[2]) * dim + (localSize[2])];
  rightVelocityFillBuffer = new FLOAT[(localSize[1]) * (localSize[2]) * dim + (localSize[1]) * (localSize[2])];

  bottomVelocityFillBuffer = new FLOAT[(localSize[0]) * (localSize[2]) * dim + (localSize[0]) * (localSize[2])];
  topVelocityFillBuffer = new FLOAT[(localSize[0]) * (localSize[2]) * dim + (localSize[0]) * (localSize[2])];

  frontVelocityFillBuffer = new FLOAT[(localSize[0]) * (localSize[1]) * dim + (localSize[0]) * (localSize[1])];
  backVelocityFillBuffer = new FLOAT[(localSize[0]) * (localSize[1]) * dim + (localSize[0]) * (localSize[1])];

}



void VelocityBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  leftVelocityFillBuffer[1 + 3 * ((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i, j, k)[0];
  leftVelocityFillBuffer[2 + 3 * ((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i, j, k)[1];
  leftVelocityFillBuffer[3 + 3 * ((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i, j, k)[2];

}

void VelocityBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  rightVelocityFillBuffer[0 + 4*((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i,j,k)[0];
  rightVelocityFillBuffer[1 + 4*((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i,j,k)[1];
  rightVelocityFillBuffer[2 + 4*((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i,j,k)[2];
  rightVelocityFillBuffer[3 + 4*((k-2)+(localSize[1]*(j-2)))] = getVelocity().getVector(i-1,j,k)[0];
}

void VelocityBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  bottomVelocityFillBuffer[0 + 3*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[0];
  bottomVelocityFillBuffer[1 + 3*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[1];
  bottomVelocityFillBuffer[2 + 3*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[2];
}

void VelocityBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */
  topVelocityFillBuffer[0 + 4*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[0];
  topVelocityFillBuffer[1 + 4*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[1];
  topVelocityFillBuffer[2 + 4*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[2];
  topVelocityFillBuffer[3 + 4*((k-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j-1,k)[1];
}

void VelocityBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  backVelocityFillBuffer[0 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[0];
  backVelocityFillBuffer[1 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[1];
  backVelocityFillBuffer[2 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[2];
}

void VelocityBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */
  backVelocityFillBuffer[0 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[0];
  backVelocityFillBuffer[1 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[1];
  backVelocityFillBuffer[2 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k)[2];
  backVelocityFillBuffer[3 + 4*((j-2)+(localSize[0]*(i-2)))] = getVelocity().getVector(i,j,k-1)[1];
}

/*2D
 */
void VelocityBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void VelocityBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void VelocityBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void VelocityBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}
