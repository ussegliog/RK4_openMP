#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include "rk4_API.hpp"
#include "rk4_IO.hpp"
#include "rk4_FIFO.hpp"
#include "rk4_Array.hpp"

#include "omp.h"

//////////////// Functions for I/O and Processings ////////////////




//****************************************************************************80

int main(int argc, char *argv[])

//****************************************************************************80
//
//  Purpose:
// 
//    MAIN is the main program for RK4_LAUNCHER.
//
//  Discussion:
//
//    RK4_LAUNCHER executes the RK4 library with a defined API.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
{
    
  std::string inputTxt;
  std::string outputTxt;
  
  // Check and retrieve main inputs 
  if (argc != 3)
    {
      std::cout << "Wrong number of arguments (at least two with the input and output files)" <<
	std::endl;

      return 1;
    }
  else
    {
      inputTxt = std::string(argv[1]);
      outputTxt = std::string(argv[2]);
    }

  // Init some variables
  bool allData_read = false;
  bool allData_processed = false;

  int nb_threads;
  #pragma omp parallel
  {
#pragma critical
    nb_threads = omp_get_num_threads();
  }
  
  
  // Allocation for FIFO and I/O handler
  RK4FIFO * in_rk4Fifo = new RK4FIFO();
  RK4FIFO * out_rk4Fifo = new RK4FIFO();

  RK4Array * in_rk4Array = new RK4Array();
  RK4Array * out_rk4Array = new RK4Array();
  RK4IO * rk4IO;


  /////////////////// Input /////////////////////
  // I/O only on rank = 0 and send data to other ranks
  // Read input txt file
  rk4IO = new RK4IO(inputTxt, outputTxt);
  
  //rk4IO->setInputFIFO(in_rk4Fifo);
  //rk4IO->readInputFileIntoFIFO();
  rk4IO->setInputArray(in_rk4Array);
  rk4IO->readInputFileIntoArray(); 


  std::cout << "in_rk4Array->getSize() : " << in_rk4Array->getSize()  << " on nb_threads = " << nb_threads << std::endl;

  out_rk4Array->setSize(in_rk4Array->getSize());
  
  /////////////////// Process /////////////////////
  int id = 0;
  double array [5];
  double x1, v1;
  double array_result [5];

  // Process rk4Data for current process
#pragma omp parallel for schedule(static)
  for (int i = 0; i < in_rk4Array->getSize(); i++)
  //for (int i = 0; i < in_rk4Fifo->getSize(); i++)
    {
      // Get elt
      //#pragma omp critical
      //in_rk4Fifo->getAndremoveData(id, array);
      in_rk4Array->getAndremoveData(i, id, array);
      
      // Process it
      rk4_API (id, array[0], array[1], array[2], array[3], array[4], x1, v1);
      
      // Assign results
      array_result[0] = array[0];
      array_result[1] = x1; // New X
      array_result[2] = v1; // New V
      array_result[3] = array[3];
      array_result[4] = array[4];

      // Add result into output FIFO
      //#pragma omp critical
      //out_rk4Fifo->addData(id, array_result);
      out_rk4Array->addData(i, id, array_result);
      
    }

  
  
  /////////////////// Output /////////////////////
  // Write ouput.txt
  //rk4IO->setOutputFIFO(out_rk4Fifo);
  //rk4IO->writeInputFileFromFIFO();
  rk4IO->setOutputArray(out_rk4Array);
  rk4IO->writeInputFileFromArray();
  
  //  Terminate.
  delete rk4IO;
  rk4IO = 0;

  
  // Free Memory
  delete in_rk4Fifo;
  in_rk4Fifo = 0;
  delete out_rk4Fifo;
  out_rk4Fifo = 0;
  
  return 0;
}
