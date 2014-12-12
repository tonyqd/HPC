#include "PressureBufferReadStencil.h"

PressureBufferReadStencil::PressureBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {


  leftPressureReadBuffer = new FLOAT[(localSize[1]+3) * (localSize[2]+3)*2];
  rightPressureReadBuffer = new FLOAT[(localSize[1]+3) * (localSize[2]+3)];

  bottomPressureReadBuffer = new FLOAT[(localSize[0]+3) * (localSize[2]+3)*2];
  topPressureReadBuffer = new FLOAT[(localSize[0]+3) * (localSize[2]+3)];

  frontPressureReadBuffer = new FLOAT[(localSize[0]+3) * (localSize[1]+3)*2];
  backPressureReadBuffer = new FLOAT[(localSize[0]+3) * (localSize[1]+3)];
  
}


void PressureBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */  
	if( _parameters.parallel.leftNb >= 0){
	flowField.getPressure().getScalar(i,j,k) = leftPressureReadBuffer[0+2*((k)+((localSize[2]+3)*(j)))];
	flowField.getPressure().getScalar(i+1,j,k) = leftPressureReadBuffer[1+2*((k)+((localSize[2]+3)*(j)))];
	}
}

void PressureBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = -0;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.rightNb >= 0){
		flowField.getPressure().getScalar(i,j,k) = rightPressureReadBuffer[(k)+((localSize[2]+3)*(j))];
	}
}

void PressureBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = -0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.bottomNb >= 0){
		flowField.getPressure().getScalar(i,j,k) = bottomPressureReadBuffer[0+2*((k)+((localSize[2]+3)*(i)))];
		flowField.getPressure().getScalar(i,j+1,k) = bottomPressureReadBuffer[1+2*((k)+((localSize[2]+3)*(i)))];
	}

}

void PressureBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
	if( _parameters.parallel.topNb >= 0){
		flowField.getPressure().getScalar(i,j,k) = topPressureReadBuffer[(k)+((localSize[2]+3)*(i))];
	}
}

void PressureBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.frontNb >= 0){
		flowField.getPressure().getScalar(i,j,k) = frontPressureReadBuffer[0+2*((j)+((localSize[1]+3)*(i)))];
		flowField.getPressure().getScalar(i,j,k+1) = frontPressureReadBuffer[1+2*((j)+((localSize[1]+3)*(i)))];
	}

}

void PressureBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */  
	if( _parameters.parallel.backNb >= 0){
		flowField.getPressure().getScalar(i,j,k) = backPressureReadBuffer[(j)+((localSize[1]+3)*(i))];
	}
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

