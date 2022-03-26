/*
BSD 3-Clause License

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

#include <hrodvitnir/core/fieldset.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
const std::any& fieldset::get(uint64_t key) const
{
    return _fields.at(key);
}

//--------------------------------------------------------------------------
std::any& fieldset::get(uint64_t key)
{
    return _fields.at(key);
}

//--------------------------------------------------------------------------
const std::any& fieldset::get(const char* str) const
{
    return _fields.at(const_crc64(str));
}

//--------------------------------------------------------------------------
std::any& fieldset::get(const char* str)
{
    return _fields.at(const_crc64(str));
}

//--------------------------------------------------------------------------
const std::any& fieldset::get(const std::string& str) const
{
    return _fields.at(const_crc64(str.c_str()));
}

//--------------------------------------------------------------------------
std::any& fieldset::get(const std::string& str)
{
    return _fields.at(const_crc64(str.c_str()));
}

//--------------------------------------------------------------------------
void fieldset::set(uint64_t key, std::any&& val)
{
    _fields[key] = val;
}

//--------------------------------------------------------------------------
void fieldset::set(const char* str, std::any&& val)
{
    set(const_crc64(str), std::move(val));
}

//--------------------------------------------------------------------------
void fieldset::set(const std::string& str, std::any&& val)
{
    set(str.c_str(), std::move(val));
}
} // namespace hrodvitnir::core
