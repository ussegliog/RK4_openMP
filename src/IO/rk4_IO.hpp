# include <string>
# include <vector>

#include "rk4_FIFO.hpp"
#include "rk4_Array.hpp"

#ifndef RK4_IO_H
#define RK4_IO_H

class RK4IO
{
public :
  // Constructor
  RK4IO (std::string inFilename, std::string outFilename); 

  // Destructor
  ~RK4IO();

  // IO functions
  int readInputFileIntoFIFO();
  int writeInputFileFromFIFO();

  int readInputFileIntoArray();
  int writeInputFileFromArray();

  
  void setInputFIFO(RK4FIFO * rk4_FIFO);
  void setOutputFIFO(RK4FIFO * rk4_FIFO);

  void setInputArray(RK4Array * rk4_Array);
  void setOutputArray(RK4Array * rk4_Array);
  
  
private :
  std::string m_inFilename;
  std::string m_outFilename;

  int m_counterData; // Count the number of RK4Data
  
  // FIFO (composition relations) 
  RK4FIFO * m_inputFIFO;
  RK4FIFO * m_outputFIFO;

  // Array (composition relations) 
  RK4Array * m_inputArray;
  RK4Array * m_outputArray;
};

#endif
