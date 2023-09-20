# Solstice
Solstice is an economic network simulation framework

## Overview

A brief overall description of the economic network simulation engine Solstice (NB: much more in the manuals / docs). 

Solstice is an economic network simulator. The primary outcomes are quantitative analyses of the behavior of economic systems under uncertainty. It can be used both as a production tool in a portfolio / risk management context or as a research tool.

The objective is to provide a performant, easily usable, extensible simulation framework to support economic network analysis.

## What is included in the repository

- Set of c++ library objects / methods implement the Solstice framework
- Assorted auxiliary code / scripts
- Documentation
- Illustrative implementation of toy problems
- Sample data sets

## Dependencies / Requirements

* c++17
* cmake
* conan
* poco++
* eigen
* statslib (including gcem dependency)
* catch2

Installing these dependencies is system dependent, please follow instructions as per your situation. (In the future we will have a Docker based installation that can simplify this process)

## Computational Design

* Solstice is written in C++17
* The framework is "network ready". I/O can be file based or over http. 
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

