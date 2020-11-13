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

  int transformIntoRawData(int * rk4Ids, double * rk4arrays, int rk4Id_size, 
			    int rk4array_sizeWithPadding);
  int addRawDataIntoFIFO(int * rk4Ids, double * rk4arrays, int rk4Id_size,
			  int rk4array_sizeWithPadding);

			
  
private :
  std::queue<RK4Data *> * m_rk4Queue;

  // Lock to protect our FIFO
  omp_lock_t m_lock;
};

# endif
