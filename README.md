# Topological Descriptors for GraSPI

 This package is a microstructure featurization framework to compute generic topological and morphological
descriptors. This package is an extension of [Graspi](https://github.com/owodolab/graspi).

## Small Example

See example folder with [2 phase morphologies](https://github.com/owodolab/skeletalDescriptors/tree/main/examples/two-phase_morphology) and the slides [skeletal_descriptors.pdf](https://github.com/owodolab/skeletalDescriptors/blob/main/examples/skeletal_descriptors.pdf).

## Major Features

The package computes the set of descriptors/features that can used for data analytics (e.g., to establish process-structure-property maps). The implementation is in C++ and uses bash as a scripting tool to encode basic analysis pipelines.

## Installation

The package requires boost library. To compile go to src file, you find there Makefile. The path to boost will need to be provided if the boost library is in a not searchable location.

Type : make all
