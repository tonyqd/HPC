#include "VTKStencilTurbulent.h"
#include "fstream"
#include "string"
#include "sstream"

VTKStencilTurbulent::VTKStencilTurbulent ( const Parameters & parameters ) : FieldStencil<FlowField> ( parameters ) {}


void VTKStencilTurbulent::apply ( FlowField & flowField, int i, int j ){
  

  FLOAT pressure_tmp;
  FLOAT viscosity_tmp; 
  FLOAT velocity_tmp[3];
  const int obstacle = flowField.getFlags().getValue(i, j);

      if ((obstacle & OBSTACLE_SELF) == 0){
	flowField.getPressureAndVelocityAndTurbulentViscosity(pressure_tmp,viscosity_tmp,velocity_tmp,i,j);
	vstream<<velocity_tmp[0]<<" "<<velocity_tmp[1]<<" "<<velocity_tmp[2]<<std::endl;
	pstream<<pressure_tmp<<std::endl;
	vistream<<viscosity_tmp<<std::endl;
      }
    
      else {
      velocity_tmp[0] = 0.0;
      velocity_tmp[1] = 0.0;
      velocity_tmp[2] = 0.0;
      pressure_tmp = 0.0;
      viscosity_tmp = 0.0;
      vstream<<velocity_tmp[0]<<" "<<velocity_tmp[1]<<" "<<velocity_tmp[2]<<std::endl;
      pstream<<pressure_tmp<<std::endl;
      vistream<<viscosity_tmp<<std::endl;
      }
    
  
}

void VTKStencilTurbulent::apply ( FlowField & flowField, int i, int j, int k ){
 
  FLOAT pressure_tmp;
  FLOAT viscosity_tmp;
  FLOAT velocity_tmp[3];
  const int obstacle = flowField.getFlags().getValue(i, j, k);
 
	if ((obstacle & OBSTACLE_SELF) == 0){ //fluidzelle
	  flowField.getPressureAndVelocityAndTurbulentViscosity(pressure_tmp,viscosity_tmp,velocity_tmp,i,j,k);
	  vstream<<velocity_tmp[0]<<" "<<velocity_tmp[1]<<" "<<velocity_tmp[2]<<std::endl;
	  pstream<<pressure_tmp<<std::endl;
	  vistream<<viscosity_tmp<<std::endl;
	  }
	  
	  else {
	  velocity_tmp[0] = 0.0;
	  velocity_tmp[1] = 0.0;
	  velocity_tmp[2] = 0.0;
	  pressure_tmp = 0.0;
	  viscosity_tmp = 0.0;
	  vstream<<velocity_tmp[0]<<" "<<velocity_tmp[1]<<" "<<velocity_tmp[2]<<std::endl;
	  pstream<<pressure_tmp<<std::endl;
	  vistream<<viscosity_tmp<<std::endl;     
    
  }
}


void VTKStencilTurbulent::write ( FlowField & flowField, int timeStep ){ 
  
  std::stringstream nstream;
  nstream<<"Daten/"<<"Turbulent." << _parameters.vtk.prefix << "." << _parameters.parallel.rank << "." << timeStep << ".vtk";
  std::string name = nstream.str();
  
  std::ofstream vtkstream(name.c_str(), std::ios::out | std::ios::trunc);
  vtkstream<<"# vtk DataFile Version 2.0"<<"\n";
  vtkstream<<"TestTestTest"<<"\n";
  vtkstream<<"ASCII"<<"\n\n";
  vtkstream<<"DATASET STRUCTURED_GRID"<<std::endl;
  
  if (_parameters.geometry.dim == 3){
    vtkstream<<"DIMENSIONS "<<  _parameters.parallel.localSize[0] + 1  <<" "<<  _parameters.parallel.localSize[1] + 1 <<" "<< _parameters.parallel.localSize[2]  + 1<<std::endl;
    vtkstream<<"POINTS "<< (_parameters.parallel.localSize[0] + 1)*( _parameters.parallel.localSize[1] + 1)*( _parameters.parallel.localSize[2] +1)  << " float" <<std::endl;
    
    for(int k =  2; k < _parameters.parallel.localSize[2] + 3; k++){
      for(int j =  2; j < _parameters.parallel.localSize[1] + 3; j++){
	for(int i =  2; i < _parameters.parallel.localSize[0] + 3; i++){
	  vtkstream<< _parameters.meshsize->getPosX(i,j,k) << " " << _parameters.meshsize->getPosY(i,j,k)<< " " << _parameters.meshsize->getPosZ(i,j,k) << std::endl;
	}
      }
    }
    vtkstream<<"\n"<<"CELL_DATA "<< (_parameters.parallel.localSize[0] )*( _parameters.parallel.localSize[1] )*( _parameters.parallel.localSize[2] ) <<std::endl;
  } else {
    vtkstream<<"DIMENSIONS "<<  _parameters.parallel.localSize[0] + 1  <<" "<<  _parameters.parallel.localSize[1] + 1 <<" "<< 1 <<std::endl;
    vtkstream<<"POINTS "<< (_parameters.parallel.localSize[0] + 1)*( _parameters.parallel.localSize[1] + 1)*1  << " float" <<std::endl;
    for(int j = _parameters.parallel.firstCorner[1] + 2; j < _parameters.parallel.firstCorner[1]+_parameters.parallel.localSize[1] + 3; j++){
      for(int i = _parameters.parallel.firstCorner[0] + 2; i < _parameters.parallel.firstCorner[0]+_parameters.parallel.localSize[0] + 3; i++){
	vtkstream<< _parameters.meshsize->getPosX(i,j) << " " << _parameters.meshsize->getPosY(i,j)<< " " << 0 << std::endl;
      } 
    } 
    vtkstream<<"\n"<<"CELL_DATA "<< (_parameters.parallel.localSize[0] )*( _parameters.parallel.localSize[1] )*1 <<std::endl;
  }

  vtkstream<<"SCALARS pressure float 1"<<std::endl;
  vtkstream<<"LOOKUP_TABLE default"<<std::endl;
  
  vtkstream<<pstream.str()<<std::endl;
  
  vtkstream<<"VECTORS velocity float"<<std::endl;

  vtkstream<<vstream.str()<<std::endl;

  vtkstream<<"SCALARS TurbulentViscosity float 1"<<std::endl;
  vtkstream<<"LOOKUP_TABLE default"<<std::endl;
  
  vtkstream<<vistream.str()<<std::endl;
    
  nstream.str("");
  vstream.str("");
  pstream.str("");
  vistream.str("");
  vtkstream.close();
  
}
