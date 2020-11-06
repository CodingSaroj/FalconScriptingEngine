if [%1]==[] (set buildtype = "Release")
else (set buildtype = %1)

mkdir build;
cd build;
cmake .. -DCMAKE_BUILD_TYPE=%buildtype% -G "Visual Studio 16 2019";
