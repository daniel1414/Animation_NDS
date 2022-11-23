#ifndef _LOG_H_
#define _LOG_H_

#include "nds/debug.h"
#include <stdarg.h>
#include <cstdio>
#include <sstream>

#define LOG_ENABLED 0
#if LOG_ENABLED
#define LOG(message, ...) Logger::Log(message, __VA_ARGS__)
#else
#define LOG(message, ...)
#endif

class Logger
{
public:
    static void Log(const char* message...)
    {
        std::ostringstream ss;
        va_list args;
        va_start(args, message);

        while(*message != '\0')
        {
            if(*message == '\\')
            {
                message++;
                ss << message;
            }
            else if(*message == '%')
            {
                message++;
                switch(*message)
                {
                    case 'd':
                    case 'i':
                    {
                        int i = va_arg(args, int);
                        ss << i;
                        break;
                    }
                    case 'l':
                    {
                        long int i = va_arg(args, long int);
                        ss << i;
                        break;
                    }
                    case 'u':
                    {
                        unsigned int u = va_arg(args, unsigned int);
                        ss << u;
                        break;
                    }
                    case 'x':
                    {
                        unsigned int u = va_arg(args, unsigned int);
                        ss << "0x" << std::uppercase << std::hex << u;
                        break;
                    }
                    case 'c':
                    {
                        int c = va_arg(args, int);
                        ss << static_cast<char>(c);
                        break;
                    }
                    case 'f':
                    {
                        double f = va_arg(args, double);
                        ss << f;
                        break;
                    }
                    case 's':
                    {
                        char* s = va_arg(args, char*);
                        ss << s;
                        break;
                    }
                    default:
                    {
                        ss << *message;
                        break;
                    }
                }
            }
            else
            {
                ss << static_cast<char>(*message);
            }
            ++message;
        }
        va_end(args);

        nocashMessage(ss.str().c_str());
    }

    // TODO: Wrap in a macro for easy off switch!
    static void LogMatrix4x4(const m4x4& matrix)
    {
        std::ostringstream ss;

        ss << "[ " << f32tofloat(matrix.m[0]) << " " << f32tofloat(matrix.m[1]) << " " << f32tofloat(matrix.m[2]) << " " << f32tofloat(matrix.m[3]) << "]\n";
        ss << "[ " << f32tofloat(matrix.m[4]) << " " << f32tofloat(matrix.m[5]) << " " << f32tofloat(matrix.m[6]) << " " << f32tofloat(matrix.m[7]) << "]\n";
        ss << "[ " << f32tofloat(matrix.m[8]) << " " << f32tofloat(matrix.m[9]) << " " << f32tofloat(matrix.m[10]) << " " << f32tofloat(matrix.m[11]) << "]\n";
        ss << "[ " << f32tofloat(matrix.m[12]) << " " << f32tofloat(matrix.m[13]) << " " << f32tofloat(matrix.m[14]) << " " << f32tofloat(matrix.m[15]) << "]";

        nocashMessage(ss.str().c_str());
    }
};

#endif /* _LOG_H_ */
