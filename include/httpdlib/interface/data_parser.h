/*
 * MIT License
 *
 * Copyright (c) 2017 Andreas Wass
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
