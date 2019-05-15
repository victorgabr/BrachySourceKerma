# BrachySourceKerma
BrachySourceKerma calculates Kerma deposit in a water phantom, produced by a source defined using GDML files.

Validation at my master's degree [dissertation](http://www1.inca.gov.br/pqrt/download/trab/dissertacao_mestrado_victor_gabriel_leandro_alves.pdf) (in Portuguese with english abstract)


This code used the GDML auxiliary information for associating a sensitive detector to a volume.
The detector construction consists of a call to GDMLProcessor which parses a GDML file and returns the pointer to the world volume.
The scoring volumes use a GDML tag to create indexed kerma results for each scoring ring.

GDML (Geometry Description Markup Language) is an XML schema for detector description.
To use Geant4 and BrachySourceKerma needs to be built with GDML usage on (default true). For more
information please refer to the [GDML website](http://gdml.web.cern.ch/GDML) and
[manual](http://gdml.web.cern.ch/GDML/doc/GDMLmanual.pdf).


## Getting Started

    ./BrachySourceKerma Gammamed_Plus_Source.gdml

Run python script inside build folder.

     python data_analysis.py 

It reads KermaDeposition.csv file containing the resulted 3D kerma matrix.

## Geometry view

![brachy_source](https://user-images.githubusercontent.com/6777517/57796794-f41c6c00-7716-11e9-811f-bc5598b55ae3.png)

## Example result:

![example_result](https://user-images.githubusercontent.com/6777517/35172841-6939db5e-fd50-11e7-9616-74b964d66afe.jpg)

### Prerequisites

Build GEANT4 10.x with GDML and QT suport.

See details at [GEANT4 documentation](https://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch02.html).

requirements: [pandas](https://pandas.pydata.org/) and [matplotlib](https://matplotlib.org/)  

### Installing

Tested on [Lubuntu linux 16.04](http://lubuntu.net/) - The lightest linux distribution I have found.

Use CMakeLists.txt

Easy way:

Try it using [cmake-qt-gui](https://launchpad.net/ubuntu/xenial/+package/cmake-qt-gui)

Hard way:

Building BrachySourceKerma with CMake on Unix with Makefiles
We'll assume, for illustration only, that you've copied the BrachySourceKerma sources into a directory under your home area so that we have:

    +- /home/you/BrachySourceKerma/
       +- CMakeLists.txt
       +- BrachySourceKerma.cc
       +- include/
       +- src/
       +- ...

Here, the directory is /home/you/BrachySourceKerma, in other words the directory holding the CMakeLists.txt file.

Let's also assume that you have already installed Geant4 in your home area under, for illustration only, /home/you/geant4-install.

Our first step is to create a build directory in which build the example. We will create this alongside our BrachySourceKerma source directory as follows:

    $ cd $HOME
    $ mkdir BrachySourceKerma-build

We now change to this build directory and run CMake to generate the Makefiles needed to build the B1 application. We pass CMake two arguments:

    $ cd $HOME/BrachySourceKerma-build
    $ cmake -DGeant4_DIR=/home/you/geant4-install/lib64/Geant4-10.3.0 $HOME/BrachySourceKerma

Here, the first argument points CMake to our install of Geant4. Specifically, it is the directory holding the Geant4Config.cmake file that Geant4 installs to help CMake find and use Geant4. You should of course adapt the value of this variable to the location of your actual Geant4 install. This provides the most specific way to point CMake to the Geant4 install you want to use. You may also use the CMAKE_PREFIX_PATH variable, e.g.

    $ cd $HOME/BrachySourceKerma-build
    $ cmake -DCMAKE_PREFIX_PATH=/home/you/geant4-install $HOME/BrachySourceKerma

This is most useful for system integrators as it may be extended with paths to the install prefixes of additional required software packages and also may be set as an environment variable that CMake will use at configuration time.

The second argument to CMake is the path to the source directory of the application we want to build. Here it's just the BrachySourceKerma directory as discussed earlier. You should of course adapt the value of that variable to where you copied the BrachySourceKerma source directory.

Note the Makefile and that all the scripts for running the BrachySourceKerma application we're about to build have been copied across. With the Makefile available, we can now build by simply running make:

        $ make -jN


[More details](https://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch02s08.html)


## Windows build:
  Tested on Windows 10, Qt5.10.0, MSVC-2017 64 bits. XercesC XLM Parser and ICU builded from source.

## Running the tests

TODO: [Test Driven Development](http://agiledata.org/essays/tdd.html)

## Built With

* [CMAKE](https://cmake.org/)
* [QT-Creator](https://en.wikipedia.org/wiki/Qt_Creator)

## Contributing

Any bug fixes or improvements are welcome.

## Versioning

[SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).

## Author
    Victor Gabriel Leandro Alves, D.Sc.
    Copyright 2008-2017

## License

This project is licensed under http://geant4.web.cern.ch/geant4/license/LICENSE.html

## Acknowledgments

Thanks to:
* The [GEANT4 developers](http://geant4.web.cern.ch/geant4/collaboration/contacts.shtml)
* The best free C++ IDE if have found: [QT-creator Open Source](https://www.qt.io/download-qt-for-application-development)
* John Purcell's [free C++ course](https://www.udemy.com/free-learn-c-tutorial-beginners/) - It helped me to understand a lot of C++ details to refactor this code.
