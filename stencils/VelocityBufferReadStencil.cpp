#include "VelocityBufferReadStencil.h"

VelocityBufferReadStencil::VelocityBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftVelocityReadBuffer = new FLOAT[(localSize[1] + 2) * (localSize[2] + 2) * (dim + 1)];
  rightVelocityReadBuffer = new FLOAT[(localSize[1] + 2) * (localSize[2] + 2) * dim];

  bottomVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[2] + 2) * (dim + 1)];
  topVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[2] + 2) * dim];

  frontVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[1] + 2) * (dim + 1)];
  backVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[1] + 2) * dim];

}



void VelocityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	flowField.getVelocity().getVector(i, j, k)[0] = leftVelocityReadBuffer[0 + 4*((k-1)+((localSize[2]+2)*(j-1)))];
	flowField.getVelocity().getVector(i, j, k)[1] = leftVelocityReadBuffer[1 + 4*((k-1)+((localSize[2]+2)*(j-1)))];
	flowField.getVelocity().getVector(i, j, k)[2] = leftVelocityReadBuffer[2 + 4*((k-1)+((localSize[2]+2)*(j-1)))];
	flowField.getVelocity().getVector(i-1,j,k)[0] = leftVelocityReadBuffer[3 + 4*((k-1)+((localSize[2]+2)*(j-1)))];

}

void VelocityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
	flowField.getVelocity().getVector(i, j, k)[0] = rightVelocityReadBuffer[0 + 3*((k-1)+((localSize[2]+2)*(j-1)))];
	flowField.getVelocity().getVector(i, j, k)[1] = rightVelocityReadBuffer[1 + 3*((k-1)+((localSize[2]+2)*(j-1)))];
	flowField.getVelocity().getVector(i, j, k)[2] = rightVelocityReadBuffer[2 + 3*((k-1)+((localSize[2]+2)*(j-1)))];
}

void VelocityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	flowField.getVelocity().getVector(i, j, k)[0] = bottomVelocityReadBuffer[0 + 4*((k-1)+(localSize[2]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[1] = bottomVelocityReadBuffer[1 + 4*((k-1)+(localSize[2]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[2] = bottomVelocityReadBuffer[2 + 4*((k-1)+(localSize[2]*(i-1)))];
	flowField.getVelocity().getVector(i,j-1,k)[1] = bottomVelocityReadBuffer[3 + 4*((k-1)+(localSize[2]*(i-1)))];
}

void VelocityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */
	flowField.getVelocity().getVector(i, j, k)[0] = topVelocityReadBuffer[0 + 3*((k-1)+(localSize[2]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[1] = topVelocityReadBuffer[1 + 3*((k-1)+(localSize[2]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[2] = topVelocityReadBuffer[2 + 3*((k-1)+(localSize[2]*(i-1)))];
}

void VelocityBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	flowField.getVelocity().getVector(i, j, k)[0] = frontVelocityReadBuffer[0 + 4*((j-1)+(localSize[1]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[1] = frontVelocityReadBuffer[1 + 4*((j-1)+(localSize[1]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[2] = frontVelocityReadBuffer[2 + 4*((j-1)+(localSize[1]*(i-1)))];
	flowField.getVelocity().getVector(i,j,k-1)[2] = frontVelocityReadBuffer[3 + 4*((j-1)+(localSize[1]*(i-1)))];
}

void VelocityBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */
	flowField.getVelocity().getVector(i, j, k)[0] = backVelocityReadBuffer[0 + 4*((j-1)+(localSize[1]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[1] = backVelocityReadBuffer[1 + 4*((j-1)+(localSize[1]*(i-1)))];
	flowField.getVelocity().getVector(i, j, k)[2] = backVelocityReadBuffer[2 + 4*((j-1)+(localSize[1]*(i-1)))];
}

/*2D
 */
void VelocityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void VelocityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void VelocityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void VelocityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}
