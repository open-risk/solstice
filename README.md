# Solstice

**Solstice** is a economic network simulation framework

## Overview

Solstice is an economic network simulator written in C++. The objective is to provide a performant, easily usable, extensible simulation framework to support various forms of economic network analysis.

The primary outcomes are quantitative analyses of the behavior of economic networks under uncertainty. It can be used both as a production tool in a portfolio / risk management context or as a research tool. This _readme_ offers a brief overall description of the simulation engine (NB: much more is becoming available in the manuals and documentation). 

## What is included in this repository

- A set of c++ library objects / methods implement the Solstice framework
- Assorted auxiliary code / scripts
- Technical Documentation
- Illustrative implementation of toy problems
- Sample data sets

## Dependencies / Requirements

Solstice has a number of C++ dependencies. Given the state of dependency management of C++ these are best managed with an appropriate tool such as conan.

* c++17
* [cmake](https://cmake.org/)
* [conan](https://docs.conan.io/2/installation.html) for package management
* [poco++](https://pocoproject.org/) for various utility libraries
* [eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) for numerical linear algebra functionality
* [statslib](https://statslib.readthedocs.io/en/latest/index.html) for some statistical distributions functionality
* [catch2](https://github.com/catchorg/Catch2) for testing

### Conan

Installing the dependencies via conan or individually is system dependent, please follow instructions as per your situation. The required dependencies and conan are stored in conanfile.txt

### vcpkg

statslib is not available via vcpkg. 

### Docker

A [Docker](/Dockerfile) based installation that can simplify this process is also available. 

You can directly fetch an image from [Docker Hub](https://hub.docker.com/repository/docker/openrisk/solstice/general) 

## Computational Design

* Solstice is written in C++17
* The framework is "network ready". The relevant I/O can be either file based or over http. 
* It uses Poco++ for many of the common app functionalities
* It uses Eigen as the core container of numerical data (vectors / tensors) and linear algebra algorithms

## ECS inspired Design

Solstice adopts in its implementation a number of features of recent entity-component-system C++ frameworks. This favors composition over inheritance in certain critical objects. Runtime polymorphism allows the flexible construction and extension of Solstice to enable the analysis of a variety new models and network structures

## Documentation

* [Introduction](manuals/solstice_introduction_0.1.pdf)
* [Analytic Framework](manuals/solstice_analytics_0.1.pdf)

## Econometric Models

An indicative list of econometric models and associated financial concepts implemented

* Multiperiod - Macro Scenario Generator (VAR type)
  * Single factor
  * Equity type multi-factor
  * Macro-economic multi-factor
* Single Period - Markov Scenario Generator (Graph type)
  * Conditional independence
  * Contagion / network models 
* Collateral Value Simulation
* Regulatory Capital Calculation

#### Calculation Methodologies

* monte carlo - simple
* monte carlo - with importance sampling
* asymptotic limit (large N)
* analytic functions
  * moments / analytic approximations
  * regulatory capital (ASFR)

#### Risk Metrics / Outputs

* rating distributions at different timepoints
* quantile loss result at [99.XX] / other distribution statistics
* results statistical errors / confidence levels
* expectations at future timepoints
* risk capital allocation

