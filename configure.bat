mkdir build
cd build

if [%FALCON_GENERATOR%]==[] (set FALCON_GENERATOR="Visual Studio 16 2019")

if [%1]==[] (cmake .. -G %FALCON_GENERATOR% %*) else (cmake .. -G %FALCON_GENERATOR% -DCMAKE_BUILD_TYPE=Release)

cd ..