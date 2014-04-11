#ifndef OUTPUTTER_H
#define OUTPUTTER_H
/*
 * outputter.h: David R. Morrison, April 2014
 *
 * Abstract base class for writing to an output source
 */

#include <string>

class Outputter
{
public:
	enum OutputStyle { Normal, Info, Warning, Error };

    virtual void writeInfo(const std::string& text) = 0;  
    virtual void writeWarning(const std::string& text) = 0;  
    virtual void writeError(const std::string& text) = 0;  
    virtual void write(const std::string& text, OutputStyle = Normal) = 0;
};

#endif // OUTPUTTER_H

