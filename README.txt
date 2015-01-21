	Checkpointing

Usage: 
A new field in the .xml file called <checkpoint> is introduced. 
Its parameters are interval and restart, where the "interval" specifies 
how often binary files with the pressute and the velocity values will be written. 
The second "parameter" is from type bool and can be "true" or "false". 
restart = "true" : restart the simulation, read the data from the binary files.
restart = "true" : start the simulation 

Example: 
 <checkpoint interval="0.1" restart="true"/>
