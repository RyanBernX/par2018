/**
 * @file   main.cpp
 * @author Ruo Li <rli@math.pku.edu.cn>
 * @date   Wed Jan 4 14:19:31 2012
 * 
 * @brief  
 * 
 * 
 */

#include "scl.h"

int main(int argc, char * argv[]) {
  MPI_Init(&argc, &argv);
  assert ((nice(40) != -1));

  SCL app;
  AFEPack::MPI::registerController("save", &app, &SCL::save_data, 
				   "save data for visualization");
  AFEPack::MPI::controllerScript("run.sh", MPI_COMM_WORLD);
  app.initialize();
  app.run();

  MPI_Finalize();
  return 0;
}

/**
 * end of file
 * 
 */

