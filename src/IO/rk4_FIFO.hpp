# include <queue>

# include "rk4_data.hpp"
# include "omp.h"

#ifndef RK4_FIFO_H
#define RK4_FIFO_H

// Class to define a FIFO to handle objects (rk4Data)  
class RK4FIFO
{
public:
  // Constructors
  RK4FIFO(); // Default constructor
  
  // Destructor
  ~RK4FIFO();
  
  int getSize();
  // Get all inputs except id;
  void addData(int rk4Id, double * rk4data);
  void getAndremoveData(int & rk4Id, double * rk4array);
			
  
private :
  std::queue<RK4Data *> * m_rk4Queue;

  // Lock to protect our FIFO
  omp_lock_t m_lock;
};

# endif
