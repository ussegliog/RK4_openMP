#include <iostream>

#include "rk4_Array.hpp"


// Constructor
RK4Array::RK4Array()
{
  
}

// Destructor
RK4Array::~RK4Array()
{
  // Free Memory
  for (int i = 0; i < m_size; i++)
    {
      RK4Data * rk4data = m_rk4Array[i];
      delete rk4data;
      rk4data = 0;
    }

  delete m_rk4Array;
  m_rk4Array = 0;
}

void RK4Array::setSize(int size)
{
  // Init size
  m_size = size;
  
  // Allocate the Array
  m_rk4Array = new RK4Data *[m_size];
}

int RK4Array::getSize()
{
  return m_size;
}

void RK4Array::addData(int iter, int rk4Id, double * rk4array)
{
  // Create a RK4Data with given id and array
  RK4Data * rk4Data = new RK4Data(rk4Id, rk4array[0], rk4array[1], rk4array[2],
				  rk4array[3], rk4array[4]);

  // Add it into the array at the given iter
  m_rk4Array[iter] = rk4Data;
}


void RK4Array::getAndremoveData(int iter, int & rk4Id, double * rk4array)
{
  // Get one elt
  if (m_rk4Array[iter])
    {
      // Get the front elt
      RK4Data * rk4data = m_rk4Array[iter];
      // Fill arguments with it
      rk4Id = rk4data->getId();
      rk4data->getAllData(rk4array);
      // Free elt and erase the front
      delete rk4data;
      rk4data = 0;
      m_rk4Array[iter] = 0;
    }
}
