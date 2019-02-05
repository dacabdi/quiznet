#ifndef __EXCEPTION__H__
#define __EXCEPTION__H__

// ensures XSI version of strerror_r
#ifdef _GNU_SOURCE
    #undef _GNU_SOURCE
#endif
#include <errno.h>
#include <string.h>

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
    protected:
        
        const std::string _what;
        const std::string _where;
        const std::string _extraMsg;
        const bool _displayErrno;
        int _errnoInt;
        std::string _errno;
        std::string _errstr;
        
        std::string _full;
        
        void init(void)
        {
            _full = "Where: " + _where + "\n" 
                  + "What: " + _what + "\n"
                  + "Extra: " + _extraMsg + "\n";

            _errnoInt = errno;
            if(_displayErrno)
            {
                _errno = std::to_string(_errnoInt);
                char buffer[1024];
                bzero(buffer, 1024);
                int r = strerror_r(_errnoInt, buffer, 1024);
                _errstr = (!r ? buffer : "Could not retrieve error string"); 
            
                _full += "Errno: " + _errno + "\n"
                      + "Errstr: " + _errstr + "\n";
            } 
        }

    public:
    
        Exception(const std::string& what,
                  const std::string& where, 
                  const std::string& extraMsg = "", 
                  bool displayErrno = true)
        :   std::runtime_error(""),
            _what(what), 
            _where(where), 
            _extraMsg(extraMsg), 
            _displayErrno(displayErrno) 
        {
            init();
        };

    	const char * what () const throw ()
        {
            return _full.c_str();
        }

        int errNo(void)
        {
            return _errnoInt;
        }
};

#endif //__EXCEPTION__H__