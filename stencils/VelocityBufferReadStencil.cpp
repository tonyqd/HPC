#include "VelocityBufferReadStencil.h"

VelocityBufferReadStencil::VelocityBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftVelocityReadBuffer = new FLOAT[(localSize[1] + 2) * (localSize[2] + 2) * (dim)];
  rightVelocityReadBuffer = new FLOAT[(localSize[1] + 2) * (localSize[2] + 2) * 2];

  bottomVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[2] + 2) * (dim)];
  topVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[2] + 2) * 2];

  frontVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[1] + 2) * (dim)];
  backVelocityReadBuffer = new FLOAT[(localSize[0] + 2) * (localSize[1] + 2) * 2];

}



void VelocityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.leftNb >= 0){
	flowField.getVelocity().getVector(i-1, j, k)[0] = leftVelocityReadBuffer[0 + 3*((k)+((localSize[2]+2)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[1] = leftVelocityReadBuffer[1 + 3*((k)+((localSize[2]+2)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[2] = leftVelocityReadBuffer[2 + 3*((k)+((localSize[2]+2)*(j)))];
	}

}

void VelocityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.rightNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[1] = rightVelocityReadBuffer[0 + 2*((k)+((localSize[2]+2)*(j)))];
	flowField.getVelocity().getVector(i, j, k)[2] = rightVelocityReadBuffer[1 + 2*((k)+((localSize[2]+2)*(j)))];
	}
}

void VelocityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.bottomNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = bottomVelocityReadBuffer[0 + 3*((k)+((localSize[2]+2)*(i)))];
	flowField.getVelocity().getVector(i, j-1, k)[1] = bottomVelocityReadBuffer[1 + 3*((k)+((localSize[2]+2)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[2] = bottomVelocityReadBuffer[2 + 3*((k)+((localSize[2]+2)*(i)))];
	}
}

void VelocityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */
	if( _parameters.parallel.topNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = topVelocityReadBuffer[0 + 2*((k)+((localSize[2]+2)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[2] = topVelocityReadBuffer[1 + 2*((k)+((localSize[2]+2)*(i)))];
	}
}

void VelocityBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.frontNb >= 0 ){
	flowField.getVelocity().getVector(i, j, k)[0] = frontVelocityReadBuffer[0 + 3*((j)+((localSize[1]+2)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[1] = frontVelocityReadBuffer[1 + 3*((j)+((localSize[1]+2)*(i)))];
	flowField.getVelocity().getVector(i, j, k-1)[2] = frontVelocityReadBuffer[2 + 3*((j)+((localSize[1]+2)*(i)))];
	}
}

void VelocityBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */
	if( _parameters.parallel.backNb >= 0){
	flowField.getVelocity().getVector(i, j, k)[0] = backVelocityReadBuffer[0 + 2*((j)+((localSize[1]+2)*(i)))];
	flowField.getVelocity().getVector(i, j, k)[1] = backVelocityReadBuffer[1 + 2*((j)+((localSize[1]+2)*(i)))];
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
