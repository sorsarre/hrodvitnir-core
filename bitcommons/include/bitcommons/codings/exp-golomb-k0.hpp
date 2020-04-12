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

#pragma once

#include <bitcommons/bitreader-utils.hpp>

namespace bitcommons::ext
{
    template<typename T>
    struct exp_golomb_k0: public bitcommons::binary_codec
    {
        using value_type = T;

        template<typename Reader>
        static T read(Reader& br)
        {
            size_t counter = 0;
            T result = T(0);
            while (br.template read<uint8_t>(1) == 0) {
                ++counter;
            }

            result = 1 << counter;
            if (counter > 0) {
                result |= br.template read<T>(counter);
            }

            return result-1;
        }

        template<typename Writer>
        static void write(Writer& w, T value)
        {
            if (value == T{0}) {
                w.template write<uint8_t>(1, 1);
                return;
            }
            // x = 1'abcd
            // encoded(x) = 00001abcd
            size_t counter = 0;
            while ((value >> counter) != 1) {
                ++counter;
            }

            w.template write<T>(0, counter);
            w.template write<uint8_t>(1, 1);
            w.template write<T>(value, counter);
        }
    };
}
