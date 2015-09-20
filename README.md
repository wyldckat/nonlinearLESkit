nonlinearLESkit - proof of concept
==================================

**Disclaimer**: This repository is provided only as a proof of concept, devised for this thread: http://www.cfd-online.com/Forums/openfoam-programming-development/144714-problem-implementing-non-linear-sgs-model.html
There is no warranty that this will work as intended. This only aims to prove how a new type of turbulence model, namely a non-linear LES model group, can be implemented to be used with OpenFOAM, without having to be integrated into OpenFOAM itself.

The license for the source code provided in this repository and respective branches is the same as the one used in OpenFOAM, namely GPL v3, for which the file LICENSE provides more details.

Source code provided in this repository is based on source code from the following sources:

 * http://www.cfd-online.com/Forums/openfoam-programming-development/144714-problem-implementing-non-linear-sgs-model.html#post520703
 * http://www.cfd-online.com/Forums/openfoam-programming-development/144714-problem-implementing-non-linear-sgs-model.html#post525918
 * https://github.com/OpenFOAM/OpenFOAM-2.1.x/
 * https://github.com/OpenFOAM/OpenFOAM-2.3.x/

This means that most of the source code was provided by OpenCFD and/or the OpenFOAM Foundation, along with modifications by Huang Xianbei (huangxianbei)[http://www.cfd-online.com/Forums/members/huangxianbei.html] and Bruno Santos (wyldckat)[http://www.cfd-online.com/Forums/members/wyldckat.html].

For more details and questions about the source code provided in this repository, please read/study the forum thread: http://www.cfd-online.com/Forums/openfoam-programming-development/144714-problem-implementing-non-linear-sgs-model.html


Branches
========

The following branches exist in this repository for the following OpenFOAM versions:

 * OpenFOAM 2.1.1: https://github.com/wyldckat/nonlinearLESkit/tree/of211
 * OpenFOAM 2.1.x: https://github.com/wyldckat/nonlinearLESkit/tree/of21x
 * OpenFOAM 2.3.0 to 2.3.x: https://github.com/wyldckat/nonlinearLESkit/tree/of23x

 
Building
========

Using Git
---------

  1. Go to your user folder:

     ```
     mkdir -p $FOAM_RUN
     cd $FOAM_RUN/..
     ```

  2. Clone the repository and go into the cloned repository:

     ```
     git clone https://github.com/wyldckat/nonlinearLESkit.git
     cd nonlinearLESkit
     ```

  3. Checkout the repository respective to the version of OpenFOAM you are using:

   * OpenFOAM 2.3.x:

     ```
     git checkout of23x
     ```

   * OpenFOAM 2.1.x:

     ```
     git checkout of21x
     ```

   * OpenFOAM 2.1.1:

     ```
     git checkout of211
     ```
     
   4. Build all of the libraries and utilities by running:

     ```
     ./Allwmake
     ```

   5. The example case in `exampleCaseChannel395` can be executed by running `./Allrun`, but it will not work because the ngSgs field has not been updated to work as a `volSymmTensorField`.


Using Zip
---------

  1. Go to your user folder:

     ```
     mkdir -p $FOAM_RUN
     cd $FOAM_RUN/..
     ```

  2. Get the Zip file for the repository respective to the version of OpenFOAM you are using:

   * OpenFOAM 2.3.x:

     ```
     wget https://github.com/wyldckat/nonlinearLESkit/archive/of23x.zip
     ```

   * OpenFOAM 2.1.x:

     ```
     wget https://github.com/wyldckat/nonlinearLESkit/archive/of21x.zip
     ```

   * OpenFOAM 2.1.1:

     ```
     wget https://github.com/wyldckat/nonlinearLESkit/archive/of211.zip
     ```

   3. Unzip the respective file and go into the respective folder, for example:

     ```
     unzip of23x.zip
     cd nonlinearLESkit-of23x
     ```
     
   4. Build all of the libraries and utilities by running:

     ```
     ./Allwmake
     ```

   5. The example case in `exampleCaseChannel395` can be executed by running `./Allrun`, but it will not work because the ngSgs field has not been updated to work as a `volSymmTensorField`.

