#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cstdlib>

# include "rk4_IO.hpp"

RK4IO::RK4IO(std::string inFilename, std::string outFilename)
{
  // Assign values
  m_inFilename = inFilename;
  m_outFilename = outFilename;

  // Init counter
  m_counterData = 0;

  // Init FIFO to 0 (NULL)
  m_inputFIFO = 0;
  m_outputFIFO = 0;

  // Init Arrays to 0 (NULL)
  m_inputArray = 0;
  m_outputArray = 0;
}

RK4IO::~RK4IO()
{
}

void RK4IO::setInputFIFO(RK4FIFO * rk4_FIFO)
{
  // Assign the ptr
  m_inputFIFO = rk4_FIFO;
}

void RK4IO::setOutputFIFO(RK4FIFO * rk4_FIFO)
{
  m_outputFIFO = rk4_FIFO;
}

void RK4IO::setInputArray(RK4Array * rk4_Array)
{
  // Assign the ptr
  m_inputArray = rk4_Array;
}

void RK4IO::setOutputArray(RK4Array * rk4_Array)
{
  m_outputArray = rk4_Array;
}


int RK4IO::readInputFileIntoFIFO()
{  
  std::ifstream inFile(m_inFilename.c_str());
  if (! inFile.good())
    {
      return 1;
    }
  
  std::string line;

  int id;
  double array [5];

  // Loop on each line of the file
  while (std::getline(inFile, line))
    {      
      // Assign values
      //inFile >> id >> t0 >> x0 >> v0 >> tmax >> dt;
      std::string delim = " ";
      size_t pos = 0;
      // Id
      pos = line.find(delim);
      id = std::atoi(line.substr(0, pos).c_str());
      // t0
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[0] = std::atof(line.substr(0, pos).c_str());
      // x0
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[1] = std::atof(line.substr(0, pos).c_str());
      // v0
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[2] = std::atof(line.substr(0, pos).c_str());
      // tmax
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[3] = std::atof(line.substr(0, pos).c_str());
      // dt
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[4] = std::atof(line.substr(0, pos).c_str());
  
      // Put it into the input FIFO
      m_inputFIFO->addData(id, array);
    }
  
  inFile.close();

  return 0;
}

int RK4IO::readInputFileIntoArray()
{
  
  // Only count the number of lines
 std::ifstream inFileCount(m_inFilename.c_str());
  if (! inFileCount.good())
    {
      return 1;
    }

  int counter = 0;
  std::string line;
  while (std::getline(inFileCount, line))
    {
      counter++;
    }

  inFileCount.close();

  // Indicate size o the input array
  m_inputArray->setSize(counter);
   
  
  std::ifstream inFile(m_inFilename.c_str());
  if (! inFile.good())
    {
      return 1;
    }
  
  int id;
  double array [5];
  
  counter = 0;
  // Loop on each line of the file
  while (std::getline(inFile, line))
    {      
      // Assign values
      //inFile >> id >> t0 >> x0 >> v0 >> tmax >> dt;
      std::string delim = " ";
      size_t pos = 0;
      // Id
      pos = line.find(delim);
      id = std::atoi(line.substr(0, pos).c_str());
      // t0
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[0] = std::atof(line.substr(0, pos).c_str());
      // x0
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[1] = std::atof(line.substr(0, pos).c_str());
      // v0
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[2] = std::atof(line.substr(0, pos).c_str());
      // tmax
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[3] = std::atof(line.substr(0, pos).c_str());
      // dt
      line.erase(0, pos + delim.length());
      pos = line.find(delim);
      array[4] = std::atof(line.substr(0, pos).c_str());
  
      // Put it into the input Array
      m_inputArray->addData(counter,id, array);
      counter++;
    }
  
  inFile.close();

  return 0;
}

int RK4IO::writeInputFileFromFIFO()
{
  int id = 0;
  double array [5];

  // Output File
  std::ofstream outFile;
  outFile.open(m_outFilename.c_str());
  
  // Loop on each elt into the output FIFO
  while (m_outputFIFO->getSize() > 0)
    {
      // Get value
      m_outputFIFO->getAndremoveData(id, array);

      // Write into the output file
      outFile << id << " " << array[0] << " " << array[1] << " " << array[2] << " " <<
	array[3] << " " << array[4] << std::endl;;  
    }


  outFile.close();
  
  return 0;
}


int RK4IO::writeInputFileFromArray()
{
  int id = 0;
  double array [5];

  // Output File
  std::ofstream outFile;
  outFile.open(m_outFilename.c_str());
  
  // Loop on each elt into the output FIFO
  for (int i = 0; i < m_outputArray->getSize(); i++) 
    {
      // Get value
      m_outputArray->getAndremoveData(i, id, array);

      // Write into the output file
      outFile << id << " " << array[0] << " " << array[1] << " " << array[2] << " " <<
	array[3] << " " << array[4] << std::endl;;  
    }


  outFile.close();
  
  return 0;
}
