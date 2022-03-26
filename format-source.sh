for f in `find . -name '*.*pp'`; do
    echo Formatting $f...
    clang-format -i $f
done
echo Done