#ifndef __EXCEPTION__H__
#define __EXCEPTION__H__

#include <stdexcept>
#include <string>

#include <errno.h>
#include <string.h>

class Exception : public std::runtime_error
{
    protected:
        
        const std::string _what;
        const std::string _where;
        const std::string _extraMsg;
        const bool _displayErrno;
        std::string _errno;
        std::string _errstr;
        
        std::string _full;
        
        void init(void)
        {
            _errno = std::to_string(errno);

            char buffer[1024];
            bzero(buffer, 1024);
            int r = strerror_r(errno, buffer, 1024);
            if (r != 0)
                _errstr = "Could not retrieve error message";
            else
                _errstr = buffer;
            

            _errno = std::

            _full = "Where: " + _where + "\n" 
                  + "What: " + _what + "\n"
                  + "Extra: " + _extraMsg + "\n"
                  + "Errno: " + _errno + "\n"
                  + "Errstr: " + _errstr + "\n";
        }

    public:
    
        Exception(const std::string& what,
                  const std::string& where, 
                  const std::string& extraMsg = "", 
                  bool displayErrno = false)
        :   std::runtime_error(""),
            _what(what), 
            _where(where), 
            _extraMsg(extraMsg), 
            _displayErrno(displayErrno) 
        {
            init();
        };
        
        Exception(const std::string& what,
                  const std::string& where, 
                  bool displayErrno = false)
        :   std::runtime_error(what),
            _what(what),
            _where(where), 
            _extraMsg(""), 
            _displayErrno(displayErrno) 
        {
            init();
        };

    	const char * what () const throw ()
        {
            return _full.c_str();
        }
};

#endif //__EXCEPTION__H__