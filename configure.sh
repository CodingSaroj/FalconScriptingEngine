if $1
then
    buildtype=$1;
else
    buildtype=Release;
fi

mkdir build &> /dev/null;
cd build &> /dev/null;
cmake .. -DCMAKE_BUILD_TYPE=$buildtype;
