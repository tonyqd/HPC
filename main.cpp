#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Configuration.h"
#include "Simulation.h"
#include "TurbulentSimulation.h"
#include "parallelManagers/PetscParallelConfiguration.h"
#include "parallelManagers/PetscParallelManager.h"
#include "MeshsizeFactory.h"
#include <iomanip>


int main (int argc, char *argv[]) {

    // Parallelization related. Initialize and identify
    // ---------------------------------------------------
    int rank;   // This processor's identifier
    int nproc;  // Number of processors in the group
    PetscInitialize(&argc, &argv, "petsc_commandline_arg", PETSC_NULL);
    MPI_Comm_size(PETSC_COMM_WORLD, &nproc);
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    std::cout << "Rank: " << rank << ", Nproc: " << nproc << std::endl;
    //----------------------------------------------------


    // read configuration and store information in parameters object
    Configuration configuration(argv[1]);
    Parameters parameters;
    configuration.loadParameters(parameters);
    PetscParallelConfiguration parallelConfiguration(parameters);
    MeshsizeFactory::getInstance().initMeshsize(parameters);
    FlowField *flowField = NULL;
    TurbulentSimulation *turbulentsimulation = NULL;
    Simulation *simulation = NULL;
    
    //int loadingCheckpoint = 0;

    #ifdef DEBUG
    std::cout << "Processor " << parameters.parallel.rank << " with index ";
    std::cout << parameters.parallel.indices[0] << ",";
    std::cout << parameters.parallel.indices[1] << ",";
    std::cout << parameters.parallel.indices[2];
    std::cout <<    " is computing the size of its subdomain and obtains ";
    std::cout << parameters.parallel.localSize[0] << ", ";
    std::cout << parameters.parallel.localSize[1] << " and ";
    std::cout << parameters.parallel.localSize[2];
    std::cout << ". Left neighbour: " << parameters.parallel.leftNb;
    std::cout << ", right neighbour: " << parameters.parallel.rightNb;
    std::cout << std::endl;
    std::cout << "Min. meshsizes: " << parameters.meshsize->getDxMin() << ", " << parameters.meshsize->getDyMin() << ", " << parameters.meshsize->getDzMin() << std::endl;
    #endif

    // initialise simulation
    if (parameters.simulation.type=="turbulence"){
      // TODO WS2: initialise turbulent flow field and turbulent simulation object
    	if(rank==0){ std::cout << "Start Turbulence simulation in " << parameters.geometry.dim << "D" << std::endl; }
    	flowField = new FlowField(parameters);
    	if(flowField == NULL){ handleError(1, "flowField==NULL!"); }
    	turbulentsimulation = new TurbulentSimulation(parameters,*flowField);
    	//handleError(1,"Turbulence currently not supported yet!");
    } else if (parameters.simulation.type=="dns"){
    	if(rank==0){ std::cout << "Start DNS simulation in " << parameters.geometry.dim << "D" << std::endl; }
    	flowField = new FlowField(parameters);
    	if(flowField == NULL){ handleError(1, "flowField==NULL!"); }
    	simulation = new Simulation(parameters,*flowField);
    } else {
    	handleError(1, "Unknown simulation type! Currently supported: dns, turbulence");
    }
    // call initialization of simulation (initialize flow field)
    if(simulation == NULL && turbulentsimulation == NULL){ handleError(1, "simulation or turbulentsimulation ==NULL!"); }
    if(parameters.simulation.type=="dns"){
    simulation->initializeFlowField();
    //flowField->getFlags().show();

    FLOAT time = 0.0;
    FLOAT timeCheckpointOut=parameters.checkpoint.interval;
    FLOAT timeStdOut=parameters.stdOut.interval;
    FLOAT timeVTKOut=parameters.vtk.interval;
    int timeSteps = 0;

    //if (loadingCheckpoint == 1) {
    if ( parameters.checkpoint.restart == 1 ) {
    	simulation->loadCheckpoint();
    }

    // TODO WS1: plot initial state
    simulation->plotVTK(timeSteps);


    // time loop
    while (time < parameters.simulation.finalTime){

      simulation->solveTimestep();

      time += parameters.timestep.dt;

      // std-out: terminal info
      if ( (rank==0) && (timeStdOut <= time) ){
          std::cout << "Current time: " << time << "\ttimestep: " <<
                        parameters.timestep.dt << std::endl;
          timeStdOut += parameters.stdOut.interval;
      }
      // TODO WS1: trigger VTK output
      if (timeVTKOut <= time){
	simulation->plotVTK(timeSteps);
        if (timeCheckpointOut <= time){
            simulation->saveCheckpoint();
        }
        timeCheckpointOut += parameters.checkpoint.interval;
	timeVTKOut += parameters.vtk.interval;
      }
      
      
       timeSteps++;



    }

    // TODO WS1: plot final output
    simulation->plotVTK(timeSteps);

    delete simulation; simulation=NULL;
    delete turbulentsimulation; simulation=NULL;
    delete flowField;  flowField= NULL;

    PetscFinalize();

    }else if(parameters.simulation.type=="turbulence"){
        turbulentsimulation->initializeFlowField();
        //flowField->getFlags().show();

        FLOAT time = 0.0;
        FLOAT timeCheckpointOut=parameters.checkpoint.interval;
        FLOAT timeStdOut=parameters.stdOut.interval;
        FLOAT timeVTKOut=parameters.vtk.interval;
        int timeSteps = 0;

        if ( parameters.checkpoint.restart == 1 ) {
        	turbulentsimulation->loadCheckpoint();
        }

        // TODO WS1: plot initial state
        turbulentsimulation->plotVTK(timeSteps);
        // time loop
        while (time < parameters.simulation.finalTime){

          turbulentsimulation->solveTimestep();

          time += parameters.timestep.dt;

          // std-out: terminal info
          if ( (rank==0) && (timeStdOut <= time) ){
              std::cout << "Current time: " << time << "\ttimestep: " <<
                            parameters.timestep.dt << std::endl;
              timeStdOut += parameters.stdOut.interval;
          }
          // TODO WS1: trigger VTK output
          if (timeVTKOut <= time){
              turbulentsimulation->plotVTK(timeSteps);
    	      timeVTKOut += parameters.vtk.interval;
          }
          if ( timeCheckpointOut <= time ){
              turbulentsimulation->saveCheckpoint();
              timeCheckpointOut += parameters.checkpoint.interval;
	  }
           timeSteps++;
        }

        // TODO WS1: plot final output
        turbulentsimulation->plotVTK(timeSteps);

        delete simulation; simulation=NULL;
        delete turbulentsimulation; simulation=NULL;
        delete flowField;  flowField= NULL;

        PetscFinalize();

    }else {

    delete simulation; simulation=NULL;
    delete turbulentsimulation; simulation=NULL;
    delete flowField;  flowField= NULL;

    PetscFinalize();

    }
}
