

#include <petscksp.h>
#include <float.h>
#include "FlowField.h"
#include "stencils/FGHStencil.h"
#include "stencils/MovingWallStencils.h"
#include "stencils/RHSStencil.h"
#include "stencils/VelocityStencil.h"
#include "stencils/VTKStencil.h"
#include "stencils/MaxUStencil.h"
#include "stencils/PeriodicBoundaryStencils.h"
#include "stencils/BFStepInitStencil.h"
#include "stencils/NeumannBoundaryStencils.h"
#include "stencils/BFInputStencils.h"
#include "stencils/InitTaylorGreenFlowFieldStencil.h"
#include "parallelManagers/PetscParallelManager.h"
#include "stencils/PressureBufferFillStencil.h"
#include "stencils/PressureBufferReadStencil.h"
#include "stencils/VelocityBufferFillStencil.h"
#include "stencils/VelocityBufferReadStencil.h"
#include "stencils/ViscosityBufferFillStencil.h"
#include "stencils/ViscosityBufferReadStencil.h"
#include "stencils/DistanceStencil.h"
#include "stencils/MaxViscosityStencil.h"
#include "GlobalBoundaryFactory.h"
#include "Iterators.h"
#include "Definitions.h"

#include "LinearSolver.h"
#include "solvers/SORSolver.h"
#include "solvers/PetscSolver.h"
#include "stencils/FGHStencilTurbulent.h"
#include "TurbulentViscosityStencil.h"
#include "stencils/DistanceStencil.h"
#include "stencils/VTKStencilTurbulent.h"
#include "Simulation.h"


class TurbulentSimulation : public Simulation  {
  protected:
	FGHStencilTurbulent _fghStencilTurbulent;
	FieldIterator<FlowField> _fghIteratorTurbulent;
	DistanceStencil _distanceStencil;
	FieldIterator<FlowField> _distanceIterator;

	MaxViscosityStencil _maxViscosityStencil;
	FieldIterator<FlowField> _maxViscosityIterator;

	TurbulentViscosityStencil _turbulentViscosityStencil;
	FieldIterator<FlowField> _turbulentViscosityIterator;

    VTKStencilTurbulent _vtkStencilTurbulent;
    FieldIterator<FlowField> _vtkIteratorTurbulent;

  public:
    TurbulentSimulation(Parameters &parameters, FlowField &flowField):
     Simulation(parameters, flowField),
     _fghStencilTurbulent(parameters),
     _fghIteratorTurbulent(_flowField,parameters,_fghStencilTurbulent),
     _distanceStencil(parameters),
     _distanceIterator(_flowField,parameters,_distanceStencil,1,0),
     _maxViscosityStencil(parameters),
     _maxViscosityIterator(_flowField,parameters,_maxViscosityStencil,1,0),
     _turbulentViscosityStencil(parameters),
     _turbulentViscosityIterator(_flowField,parameters,_turbulentViscosityStencil),
  	 _vtkStencilTurbulent(parameters),
  	 _vtkIteratorTurbulent(_flowField,parameters,_vtkStencilTurbulent,1,0)
       {
       }

    virtual ~TurbulentSimulation(){}

    /** initialises the flow field according to the scenario */
    virtual void initializeFlowField(){
      if (_parameters.simulation.scenario=="taylor-green"){
        // currently, a particular initialization is only requrid for the taylor-green vortex
        InitTaylorGreenFlowFieldStencil stencil(_parameters);
        FieldIterator<FlowField> iterator(_flowField,_parameters,stencil);
        iterator.iterate();
      } else if (_parameters.simulation.scenario=="channel"){
        BFStepInitStencil stencil(_parameters);
        FieldIterator<FlowField> iterator(_flowField,_parameters,stencil,0,1);
        iterator.iterate();
      } else if (_parameters.simulation.scenario=="pressure-channel"){
	    //set pressure boundaries here for left wall
        const FLOAT value = _parameters.walls.scalarLeft;
        ScalarField& rhs = _flowField.getRHS();

        if (_parameters.geometry.dim==2){
          const int sizey = _flowField.getNy();
          for (int i =0 ;i < sizey+3;i++) {
            rhs.getScalar(0,i) = value;
          }
        } else {
          const int sizey = _flowField.getNy();
          const int sizez = _flowField.getNz();
          for (int i=0;i<sizey+3;i++)
            for(int j=0;j<sizez + 3;j++)
              rhs.getScalar(0,i,j) =value;
	    }
	    
	    // do same procedure for domain flagging as for regular channel
	    BFStepInitStencil stencil(_parameters);
        FieldIterator<FlowField> iterator(_flowField,_parameters,stencil,0,1);
        iterator.iterate();
	  }
      _distanceIterator.iterate();
      //if(_parameters.parallel.rank == 0){
      //_flowField.getDistance().show();}
    }

    virtual void solveTimestep(){
        // compute TurbulentViscosity
        _turbulentViscosityIterator.iterate();
        setTimeStep();
        //if(_parameters.parallel.rank==1){
        //_flowField.getViscosity().show();}
        // compute Turbulent fgh
        _fghIteratorTurbulent.iterate();
        // set global boundary values
        _wallFGHIterator.iterate();
        // compute the right hand side
        _rhsIterator.iterate();
        // solve for pressure 
        _solver.solve();
        // TODO WS2: communicate pressure values
        _petscParallelManager.communicatePressure();
        //_flowField.getFGH().show();
        // compute velocity
        _velocityIterator.iterate();
        // TODO WS2: communicate velocity values
        _petscParallelManager.communicateVelocities();
        // Iterate for velocities on the boundary
        _wallVelocityIterator.iterate();

	//setTimeStep();
    }

    /** TODO WS1: plots the flow field. */
    virtual void plotVTK(int timeStep){
      _vtkIteratorTurbulent.iterate();
      _vtkStencilTurbulent.write(_flowField,timeStep);
      
      
      // TODO WS1: create VTKStencil and respective iterator; iterate stencil
      //           over _flowField and write flow field information to vtk file
    }


  protected:
    /** sets the time step*/
    virtual void setTimeStep(){

      FLOAT localMin, globalMin;
      assertion(_parameters.geometry.dim == 2 || _parameters.geometry.dim == 3);
      FLOAT factor = 1.0/(_parameters.meshsize->getDxMin() * _parameters.meshsize->getDxMin()) +
                     1.0/(_parameters.meshsize->getDyMin() * _parameters.meshsize->getDyMin());

      // determine maximum velocity
      _maxUStencil.reset();
      _maxUFieldIterator.iterate();
      _maxUBoundaryIterator.iterate();
      //determine maximum viscosity
      _maxViscosityStencil.reset();
      _maxViscosityIterator.iterate();
      if (_parameters.geometry.dim == 3) {
        factor += 1.0/(_parameters.meshsize->getDzMin() * _parameters.meshsize->getDzMin());
        _parameters.timestep.dt = 1.0 / _maxUStencil.getMaxValues()[2];
      } else {
        _parameters.timestep.dt = 1.0 / _maxUStencil.getMaxValues()[0];
      }

      localMin = std::min(_parameters.timestep.dt,
                                        std::min(std::min(1.0/(_maxViscosityStencil.getMaxValues() + 1.0/_parameters.flow.Re)/(2*factor),
                                        1 / _maxUStencil.getMaxValues()[0]),
                                        1 / _maxUStencil.getMaxValues()[1]));

      // Here, we select the type of operation before compiling. This allows to use the correct
      // data type for MPI. Not a concern for small simulations, but useful if using heterogeneous
      // machines.

      globalMin = MY_FLOAT_MAX;
      MPI_Allreduce(&localMin, &globalMin, 1, MY_MPI_FLOAT, MPI_MIN, PETSC_COMM_WORLD);

      _parameters.timestep.dt = globalMin;
      _parameters.timestep.dt *= _parameters.timestep.tau;
    }

};

//#endif // _SIMULATION_H_

