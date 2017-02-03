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
#ifndef FILESYSTEM_RESPONSE_GENERATOR_H
#define FILESYSTEM_RESPONSE_GENERATOR_H

#include <string>

#include "httpdlib/interface/response_generator.h"

namespace httpdlib
{

/**
 * @brief Filesystem mplementation of a response_generator
 *
 * This implementation is very basic, the constructor takes a
 * directory path and all files in this directory will be served.
 *
 * Note that the serving method is very naive, it will load the entire
 * file to RAM and then serve it using memory_response.
 * A better implementation would probably used memorymapped files together
 * with a response implementation that could serve these memory-mapped files
 * without copying them to RAM first.
 *
 * Uses the file type to get the content type. All html files are
 * assumed to be utf-8 files and charset will be set to this.
 *
 */
class filesystem_response_generator : public interface::response_generator
{
    std::string m_directory;

public:
    filesystem_response_generator(std::string directory);

    // response_generator interface
public:
    std::unique_ptr<interface::response>
    get_response(const request &req) override;
};
}

#endif // FILESYSTEM_RESPONSE_GENERATOR_H
