#include <iostream>

# include "rk4_FIFO.hpp"


// Constructor
RK4FIFO::RK4FIFO()
{
  // Allocate the FIFO
  m_rk4Queue = new std::queue<RK4Data *>();

  // Init our lock
  omp_init_lock(&m_lock);
}

// Destructor
RK4FIFO::~RK4FIFO()
{
  // Free Memory
  while (!m_rk4Queue->empty()) 
    {
      // Free the front elt
      RK4Data * rk4data = m_rk4Queue->front();
      delete rk4data;
      m_rk4Queue->front() = 0;
      // erase the front
      m_rk4Queue->pop();
    }

  delete m_rk4Queue;
  m_rk4Queue = 0;

  // Free our lock
  omp_destroy_lock(&m_lock);
}


int RK4FIFO::getSize()
{
  return m_rk4Queue->size();
}

void RK4FIFO::addData(int rk4Id, double * rk4array)
{
  // Create a RK4Data with given id and array
  RK4Data * rk4Data = new RK4Data(rk4Id, rk4array[0], rk4array[1], rk4array[2],
				  rk4array[3], rk4array[4]);

  // Add it into the queue
  m_rk4Queue->push(rk4Data);
}

int RK4FIFO::addRawDataIntoFIFO(int * rk4Ids, double * rk4arrays, int rk4Id_size, 
				 int rk4array_sizeWithPadding)
{
  // Get size for unitary array with padding inside rk4arrays
  int rk4array_unitary_size = rk4array_sizeWithPadding/rk4Id_size;

  // For each obj RK4Data
  for (int i = 0; i < rk4Id_size; i++) 
    {
      // Get current rk4array to retrieve the elts (only the "true" data : 0->4)
      double * current_rk4array = &rk4arrays[i*rk4array_unitary_size];

      // Create a RK4Data with given id and array
      RK4Data * rk4Data = new RK4Data(rk4Ids[i], current_rk4array[0], current_rk4array[1], 
				      current_rk4array[2], current_rk4array[3], 
				      current_rk4array[4]);

      // Add it into the queue
      m_rk4Queue->push(rk4Data);
    }
  
  return 0;
}

int RK4FIFO::transformIntoRawData(int * rk4Ids, double * rk4arrays, int rk4Id_size,
				   int rk4array_sizeWithPadding)
{
  // Get size for unitary array with padding inside rk4arrays
  int rk4array_unitary_size = rk4array_sizeWithPadding/rk4Id_size;

  // For each obj RK4Data : Store it into rek4Ids and rk4arrays and remove it into our FIFO
  for (int i = 0; i < rk4Id_size; i++) 
    {

      // Get one elt
      if (!m_rk4Queue->empty())
	{
	  // Get the front elt
	  RK4Data * rk4data = m_rk4Queue->front();
	  
	  // Fill rk4Ids a,d rk4arrays (by copy) with it
	  rk4Ids[i] = rk4data->getId();
	  rk4data->getAllData(&rk4arrays[i*rk4array_unitary_size]);

	  // Free elt and erase the front
	  delete rk4data;
	  m_rk4Queue->front() = 0;
	  // erase the front
	  m_rk4Queue->pop();
	}
      else
	{
	  // rk4Id_size does not match the FIFO's size => problem
	  return 1;
	}
    }

  return 0;
}





void RK4FIFO::getAndremoveData(int & rk4Id, double * rk4array)
{
  // Get one elt
  if (!m_rk4Queue->empty())
    {
      // Get the front elt
      RK4Data * rk4data = m_rk4Queue->front();
      // Fill arguments with it
      rk4Id = rk4data->getId();
      rk4data->getAllData(rk4array);

      // Free elt and erase the front
      delete rk4data;
      m_rk4Queue->front() = 0;
      // erase the front
      m_rk4Queue->pop();
     }
}
