# SPDX-FileCopyrightText: 2022 Andrey Kaydalov <sorsarre@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

for f in `find . -name '*.*pp'`; do
    echo Formatting $f...
    clang-format -i $f
done
echo Done