#pragma once
#include <stdexcept>

// ini parse module 
class INIParseException : public std::exception
{
	const char* what() const throw ()
	{
		return "ini file parsing failed!";
	}
};

/**
* \brief PLC exception module 
* param[in] r means read
* param[in] w means write 
* param[in] c means connect
* 
* e.g. throw PLCException(r);
*/
class PLCException : public std::exception
{
public:
	PLCException(const char type) :m_type(type) {};
	virtual const char* what() const noexcept override
	{
		switch (m_type) {
		case 'r':
			return "PLC failed to read DB block!";
		case 'w':
			return "PLC failed to write DB block!";
		case 'c':
			return "PLC connection failed!";
		case 'd':
			return "PLC data invalid!";
		default:
			return "PLC unknown error!";
		}		
	}

private:
	char m_type;
};
