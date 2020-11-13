#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include "rk4_API.hpp"
#include "rk4_IO.hpp"
#include "rk4_FIFO.hpp"

#include "omp.h"

//////////////// Functions for I/O and Processings ////////////////
void process_withFIFO(RK4FIFO * in_rk4Fifo, RK4FIFO * out_rk4Fifo)
{
  int id = 0;
  double array [5];
  double x1, v1;
  double array_result [5];

    // Process rk4Data for current process
#pragma omp parallel for schedule(static)
  for (int i = 0; i < in_rk4Fifo->getSize(); i++)
    {
      // Get elt
#pragma omp critical
      in_rk4Fifo->getAndremoveData(id, array);
      
      // Process it
      rk4_API (id, array[0], array[1], array[2], array[3], array[4], x1, v1);
      
      // Assign results
      array_result[0] = array[0];
      array_result[1] = x1; // New X
      array_result[2] = v1; // New V
      array_result[3] = array[3];
      array_result[4] = array[4];

      // Add result into output FIFO
#pragma omp critical
      out_rk4Fifo->addData(id, array_result);
    }
}


void process_withRawData(RK4FIFO * in_rk4Fifo, RK4FIFO * out_rk4Fifo)
{
  int id = 0;
  double * array;
  double x1, v1;
  double * array_result;

  int nbObjs = in_rk4Fifo->getSize();
  // Add fake data to match with common size of cache memory :
  // 8 double = 512 MB (only 5 double use as data)
  int size_withPadding = 8*nbObjs; 
  int size_unitaryarray_withPadding = 8;

  // Allocations
  int * rk4Ids = new int[nbObjs]; 
  double * rk4arrays = new double[size_withPadding];

  // Transform data
  int return_transform = in_rk4Fifo->transformIntoRawData(rk4Ids, rk4arrays, nbObjs, 
							  size_withPadding);

  if (return_transform != 0)
    {
      std::cout << "Problem while getting data inside input FIFO" << std::endl;
    }

    // Process only raw data
#pragma omp parallel for schedule(static)
  for (int i = 0; i < nbObjs; i++)
    {
      // Get arrays for current obj with indexes
      array = &rk4arrays[size_unitaryarray_withPadding*i];

      // Process 
      //rk4_API (rk4Ids[i], array[0], array[1], array[2], array[3], array[4], x1, v1);
      rk4_API_onStack (rk4Ids[i], array[0], array[1], array[2], array[3], array[4], 
		       array[1], array[2]); // reference for the two last arguments

      /////////////////////////////////////////////////////////////////////////////////
      // NB : rk4_API_onStack improves highly performance and allows a great scalabiliy. 
      // With rk4_API, the // is slower than a sequential execution in order to memory 
      // contention and false-sharing
      //////////////////////////////////////////////////////////////////////////////////
    }

  // "Re-transform" data 
  int return_add = out_rk4Fifo->addRawDataIntoFIFO(rk4Ids, rk4arrays, nbObjs, 
						   size_withPadding);
  

  if (return_add != 0)
    {
      std::cout << "Problem into data insertion for output FIFO" << std::endl;
    }

  // Free memory
  delete rk4Ids;
  rk4Ids = 0;
  delete rk4arrays;
  rk4arrays = 0;
}


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

  RK4IO * rk4IO;


  /////////////////// Input /////////////////////
  // I/O only on rank = 0 and send data to other ranks
  // Read input txt file
  rk4IO = new RK4IO(inputTxt, outputTxt);
  
  rk4IO->setInputFIFO(in_rk4Fifo);
  rk4IO->readInputFileIntoFIFO();

  std::cout << "in_rk4Fifo->getSize() : " << in_rk4Fifo->getSize()  << " on nb_threads = " << nb_threads << std::endl;

  
  /////////////////// Process /////////////////////
  //// Call processings
  process_withRawData(in_rk4Fifo, out_rk4Fifo);
  
  /////////////////// Output /////////////////////
  // Write ouput.txt
  rk4IO->setOutputFIFO(out_rk4Fifo);
  rk4IO->writeInputFileFromFIFO();
  
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
