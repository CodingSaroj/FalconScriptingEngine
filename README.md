<img alt="Falcon" align="left" width="256px" height="256px" src="./logos/falcon.svg">

# Falcon Scripting Engine

 A scripting engine written in C++17.

## Features

* Register-based Virtual Machine with custom bytecode.
* Custom Reflection using Falcon Abstract Language Interface(FALI).
* Custom Assembly language with Assembler.
* Debugging support.

### Planned for the future
* Compiler Frontend for FalconScript (Custom Language).
* Unified Optimization and Assembly Generation using FALI AST and IR.

### Building
#### Configuration
On Windows:  
* Run ```configure.bat <configuration>``` to generate project files for Visual Studio 2019. 

On Linux:  
* Run ```configure.sh <configuration>``` to generate makefiles.

`<configuration>` can be any of the following:
|Name               | Debugging Symbols | Optimization|
|-------------------|-------------------|-------------|
| Debug             | Yes               | No          |
| Release           | No                | Full        |
| RelWithDebInfo    | Yes               | Full        |
| MinSizeRel        | No                | Size        |

#### Compiling
On Windows:
* Build the solution.

On Linux:
* run ```make all```

The libraries will be inside ```lib/<configuration>/<platform>/<architecture>/``` and the binaries will be inside
```bin/<configuration>/<platform>/<architecture>/```.

### Usage
Refer to the ```--help``` option of the binaries for details on their usage.

The API is currently undocumented.

### Running the examples
After compiling, run these commands from root directory to compile the example assemblies:

On Windows:  
```
\path\to\flas examples\factorial.flas -o examples\factorial.fali
\path\to\flas examples\helloworld.flas -o examples\helloworld.fali
```

On Linux:  
```
/path/to/flas examples/factorial.flas -o examples/factorial.fali
/path/to/flas examples/helloworld.flas -o examples/helloworld.fali
```

Replace `/path/to/flas` with the path to `flas`. Refer to [Building](https://github.com/SarojKumar10/FalconScriptingEngine#Building)
for more info on where binaries are located.

Run them as follows:

On Windows:  
```
\path\to\falcon examples\<ExampleName>.fali -e <ArgsForExample>
```

On Linux:
```
/path/to/falcon examples/<ExampleName>.fali -e <ArgsForExample>
```

Refer to the comment at the beginning of the corresponding example's `.flas` file for information on what args should be passed.
