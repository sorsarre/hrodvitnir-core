/* BSD 3-Clause License

Copyright (c) 2019-2020, Andrey Kaydalov
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <bitcommons/direct_file_reader.hpp>
#include <stdexcept>

using namespace bitcommons;

#ifdef WIN32
namespace
{
    int fseek64(FILE* file, uint64_t pos, int origin)
    {
        return _fseeki64(file, pos, origin);
    }
    
    int64_t ftell64(FILE* file)
    {
        return _ftelli64(file);
    }
    
    FILE* fopen64(const char* name, const char* mode)
    {
        return fopen(name, mode);
    }
}
#else
namespace
{
    int fseek64(FILE* file, uint64_t pos, int origin)
    {
        return fseeko64(file, pos, origin);
    }
    
    int64_t ftell64(FILE* file)
    {
        return ftello64(file);
    }
}
#endif

//----------------------------------------------------------------------
size_t direct_file_reader::read(uint8_t* dest, uint64_t position, size_t bytes) {
    if (fseek64(_file, position, SEEK_SET) < 0) {
        throw std::runtime_error("Could not seek to designated position");
    }

    return fread(dest, 1, bytes, _file);
}

//----------------------------------------------------------------------
uint64_t direct_file_reader::size() {
    fseek64(_file, 0, SEEK_END);
    auto result = ftell64(_file);
    if (result < 0) {
        throw std::runtime_error("Could not seek to the end of the file");
    }

    return static_cast<uint64_t>(result);
}

//----------------------------------------------------------------------
bool direct_file_reader::depleted() {
    return true;
}

//----------------------------------------------------------------------
direct_file_reader::~direct_file_reader() {
    fclose(_file);
}

//----------------------------------------------------------------------
std::shared_ptr<file_reader> direct_file_reader::open(const std::string& path) {
    return std::shared_ptr<file_reader>(new direct_file_reader(path));
}


//----------------------------------------------------------------------
direct_file_reader::direct_file_reader(const std::string& path)
    : _path(path)
{
    _file = fopen64(path.c_str(), "rb");
    if (!_file) {
        throw std::runtime_error("Could not open file for reading");
    }
}

//----------------------------------------------------------------------
std::shared_ptr<file_reader> direct_file_reader::clone()
{
    auto ret = new direct_file_reader(_path);
    return std::shared_ptr<file_reader>(ret);
}
