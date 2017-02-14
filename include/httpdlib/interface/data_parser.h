#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <cstdlib>

namespace httpdlib
{

namespace interface
{

class data_parser
{
protected:
    int m_error_code = 0;

public:
    virtual ~data_parser();
    virtual bool done() const = 0;
    /**
     * @brief Implementation of adding data. The data might be ignored in which
     * case false is returned.
     * @param data  The data to add
     * @return  True if data was actually added, false if it was ignored (or
     * used for some internal state)
     */
    virtual bool add_data(char data) = 0;
    virtual std::size_t size() const = 0;
    virtual int error() const;
};
}
}

#endif // PAYLOAD_PARSER_H
