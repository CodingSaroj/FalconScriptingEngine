mkdir build &> /dev/null;
cd build &> /dev/null;

if false $(($FALCON_GENERATOR))
then
    export FALCON_GENERATOR="Unix Makefiles";
fi

if true $(($# != 1))
then
    cmake .. -G "$FALCON_GENERATOR" $*;
else
    cmake .. -G "$FALCON_GENERATOR" -DCMAKE_BUILD_TYPE=Release;
fi
