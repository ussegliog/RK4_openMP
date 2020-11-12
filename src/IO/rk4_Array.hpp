# include <queue>

# include "rk4_data.hpp"
# include "omp.h"

#ifndef RK4_ARRAY_H
#define RK4_ARRAY_H

// Class to define a FIFO to handle objects (rk4Data)  
class RK4Array
{
public:
  // Constructors
  RK4Array(); // Default constructor
  
  // Destructor
  ~RK4Array();

  void setSize(int size);
  
  int getSize();
  // Get all inputs except id;
  void addData(int iter, int rk4Id, double * rk4data);
  void getAndremoveData(int iter, int & rk4Id, double * rk4array);
			
  
private :
  RK4Data ** m_rk4Array;

  int m_size;
  
};

# endif
