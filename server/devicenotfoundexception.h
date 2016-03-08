#ifndef DEVICENOTFOUNDEXCEPTION_H
#define DEVICENOTFOUNDEXCEPTION_H
#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>
using namespace std;
class DeviceNotFoundException : public exception
{
public:
    DeviceNotFoundException();
    const char * what () const throw (){
      return "no Device Found for given Device id!";
    };
};

#endif // DEVICENOTFOUNDEXCEPTION_H
