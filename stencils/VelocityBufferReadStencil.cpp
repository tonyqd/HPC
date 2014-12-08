#include "VelocityBufferReadStencil.h"

VelocityBufferReadStencil::VelocityBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftVelocityReadBuffer = new FLOAT[(localSize[1] + 3) * (localSize[2] + 3) * 2*(dim)];
  rightVelocityReadBuffer = new FLOAT[(localSize[1] + 3) * (localSize[2] + 3) * dim];

  bottomVelocityReadBuffer = new FLOAT[(localSize[0] + 3) * (localSize[2] + 3) * 2*(dim)];
  topVelocityReadBuffer = new FLOAT[(localSize[0] + 3) * (localSize[2] + 3) * dim];

  frontVelocityReadBuffer = new FLOAT[(localSize[0] + 3) * (localSize[1] + 3) * 2*(dim)];
  backVelocityReadBuffer = new FLOAT[(localSize[0] + 3) * (localSize[1] + 3) * dim];

}



void VelocityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.leftNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = leftVelocityReadBuffer[0 + 6*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[1] = leftVelocityReadBuffer[1 + 6*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[2] = leftVelocityReadBuffer[2 + 6*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i+1,j,k)[0] = leftVelocityReadBuffer[3 + 6*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i+1,j,k)[1] = leftVelocityReadBuffer[4 + 6*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i+1,j,k)[2] = leftVelocityReadBuffer[5 + 6*((k)+((localSize[2]+3)*(j)))];
	}

}

void VelocityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.rightNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = rightVelocityReadBuffer[0 + 3*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[1] = rightVelocityReadBuffer[1 + 3*((k)+((localSize[2]+3)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[2] = rightVelocityReadBuffer[2 + 3*((k)+((localSize[2]+3)*(j)))];
	}
}

void VelocityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.bottomNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = bottomVelocityReadBuffer[0 + 6*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[1] = bottomVelocityReadBuffer[1 + 6*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[2] = bottomVelocityReadBuffer[2 + 6*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i,j+1,k)[0] = bottomVelocityReadBuffer[3 + 6*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i,j+1,k)[1] = bottomVelocityReadBuffer[4 + 6*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i,j+1,k)[2] = bottomVelocityReadBuffer[5 + 6*((k)+((localSize[2]+3)*(i)))];
	}
}

void VelocityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */
	if( _parameters.parallel.topNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = topVelocityReadBuffer[0 + 3*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[1] = topVelocityReadBuffer[1 + 3*((k)+((localSize[2]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[2] = topVelocityReadBuffer[2 + 3*((k)+((localSize[2]+3)*(i)))];
	}
}

void VelocityBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.frontNb >= 0 ){
	flowField.getVelocity().getVector(i, j, k)[0] = frontVelocityReadBuffer[0 + 6*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[1] = frontVelocityReadBuffer[1 + 6*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[2] = frontVelocityReadBuffer[2 + 6*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i,j,k+1)[0] = frontVelocityReadBuffer[3 + 6*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i,j,k+1)[1] = frontVelocityReadBuffer[4 + 6*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i,j,k+1)[2] = frontVelocityReadBuffer[5 + 6*((j)+((localSize[1]+3)*(i)))];
	}
}

void VelocityBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 1; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */
	if( _parameters.parallel.backNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = backVelocityReadBuffer[0 + 3*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[1] = backVelocityReadBuffer[1 + 3*((j)+((localSize[1]+3)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[2] = backVelocityReadBuffer[2 + 3*((j)+((localSize[1]+3)*(i)))];
	}
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
