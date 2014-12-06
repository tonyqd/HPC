#include "VelocityBufferFillStencil.h"

VelocityBufferFillStencil::VelocityBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftVelocityFillBuffer = new FLOAT[(localSize[1] + 3) * (localSize[2] + 3) * dim];
  rightVelocityFillBuffer = new FLOAT[(localSize[1] + 3) * (localSize[2] + 3) * dim*2 ];

  bottomVelocityFillBuffer = new FLOAT[(localSize[0] + 3) * (localSize[2] + 3) * dim];
  topVelocityFillBuffer = new FLOAT[(localSize[0] + 3) * (localSize[2] + 3) * (dim )*2];

  frontVelocityFillBuffer = new FLOAT[(localSize[0] + 3) * (localSize[1] + 3) * dim];
  backVelocityFillBuffer = new FLOAT[(localSize[0] + 3) * (localSize[1] + 3) * (dim )*2];

}



void VelocityBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  leftVelocityFillBuffer[0 + 3 * (k+(localSize[2]+3)*j)] = flowField.getVelocity().getVector(i+2, j, k)[0];
  leftVelocityFillBuffer[1 + 3 * ((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i+2, j, k)[1];
  leftVelocityFillBuffer[2 + 3 * ((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i+2, j, k)[2];

}

void VelocityBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   *for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
    	for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++)
   */
  rightVelocityFillBuffer[0 + 6*((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i-2,j,k)[0];
  rightVelocityFillBuffer[1 + 6*((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i-2,j,k)[1];
  rightVelocityFillBuffer[2 + 6*((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i-2,j,k)[2];
  rightVelocityFillBuffer[3 + 6*((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i-1,j,k)[0];
  rightVelocityFillBuffer[4 + 6*((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i-1,j,k)[1];
  rightVelocityFillBuffer[5 + 6*((k)+((localSize[2]+3)*(j)))] = flowField.getVelocity().getVector(i-1,j,k)[2];
}

void VelocityBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  bottomVelocityFillBuffer[0 + 3*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j+2,k)[0];
  bottomVelocityFillBuffer[1 + 3*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j+2,k)[1];
  bottomVelocityFillBuffer[2 + 3*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j+2,k)[2];
}

void VelocityBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */
  topVelocityFillBuffer[0 + 6*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j-2,k)[0];
  topVelocityFillBuffer[1 + 6*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j-2,k)[1];
  topVelocityFillBuffer[2 + 6*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j-2,k)[2];
  topVelocityFillBuffer[3 + 6*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j-1,k)[0];
  topVelocityFillBuffer[4 + 6*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j-1,k)[1];
  topVelocityFillBuffer[5 + 6*((k)+((localSize[2]+3)*(i)))] = flowField.getVelocity().getVector(i,j-1,k)[2];
}

void VelocityBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  frontVelocityFillBuffer[0 + 3*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k+2)[0];
  frontVelocityFillBuffer[1 + 3*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k+2)[1];
  frontVelocityFillBuffer[2 + 3*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k+2)[2];
}

void VelocityBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */
  backVelocityFillBuffer[0 + 6*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k-2)[0];
  backVelocityFillBuffer[1 + 6*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k-2)[1];
  backVelocityFillBuffer[2 + 6*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k-2)[2];
  backVelocityFillBuffer[3 + 6*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k-1)[0];
  backVelocityFillBuffer[4 + 6*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k-1)[1];
  backVelocityFillBuffer[5 + 6*((j)+((localSize[1]+3)*(i)))] = flowField.getVelocity().getVector(i,j,k-1)[2];
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
