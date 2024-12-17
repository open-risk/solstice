# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.


# Project metadata
set(PROJECT_VENDOR "Open Risk")
set(PROJECT_CONTACT "info@openriskmanagement.com")
set(PROJECT_URL "https://github.com/open-risk/solstice")
set(PROJECT_DESCRIPTION "Solstice is a simulation framework of economic networks")

cmake_minimum_required(VERSION 3.19)

# Project name and version
project(solstice)
set(SOLSTICE_VERSION_MAJOR 0)
set(SOLSTICE_VERSION_MINOR 5)
set(SOLSTICE_VERSION_PATCH 1)
set(SOLSTICE_VERSION ${SOLSTICE_VERSION_MAJOR}.${SOLSTICE_VERSION_MINOR}.${SOLSTICE_VERSION_PATCH})

# compiler choices. c++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -std=c++17)
add_definitions("-std=c++17")

# Create verbose output
# TODO Production options
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG)
set(CMAKE_PREFIX_PATH cmake-build-debug)

# Select Solstice http protocol for API exchanges
# 0-> http (for local testing)
# 1-> https with fake ssl certificate (for testing in public instance that has no verified certificate)
# 2-> https with real ssl certificate (for production)
add_definitions(-DSSL_MODE=0)

# Find dependencies (installed via Conan)
find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL Util Data DataSQLite)
find_package(Catch2 REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(statslib REQUIRED)
find_package(gcem REQUIRED)

#
#  SOURCE FILES
#

include_directories(.)

set(CORE_FILES
        core/simulation.h
        core/simulation.cpp
        core/helpers.cpp
        core/SerializeSimulation.cpp
        core/DeserializeSimulation.cpp
        core/ReadFromFileSimulation.cpp
        core/IdentityLog.cpp
        )

set(MODEL_FILES
        models/model.h
        models/model.cpp
        models/Calculate.cpp
        models/DeserializeModel.cpp
        models/Serialize.cpp
        models/model_test.cpp
        models/model_mobility.cpp
        models/model_macro.cpp
        )

set(MODEL_COMPONENT_FILES
        model_components/model_data.h
        model_components/model_component.h
        model_components/model_data.cpp
        model_components/Initialize.cpp
        model_components/DeserializeModelComponent.cpp
        model_components/ReadFromFileModelComponent.cpp
        model_components/GetFromAPIModelComponent.cpp
        model_components/ARProcess.h
        model_components/CorrelationMatrix.h
        model_components/FactorLoadings.h
        model_components/HazardRate.h
        model_components/TransitionMatrix.h
        model_components/CreditCurve.h
        model_components/TransitionThresholds.h
        model_components/LGDModel.h
        model_components/MarkovChain.h
        )

set(SYSTEM_FILES
        systems/compute_movement.cpp
        systems/compute_simulated_multifactor_scenarios.cpp
)

set(MANAGER_FILES
        managers/managers.h
        managers/managers.cpp
        managers/DeserializeManagers.cpp
        managers/ReadFromFileManagers.cpp
        managers/GetFromAPIManagers.cpp
        managers/Initialize.cpp
        )

set(NETWORK_FILES
        networks/network.h
        networks/network.cpp
        networks/DeserializeNetwork.cpp
        networks/ReadFromFileNetwork.cpp
        networks/PrintEntities.cpp
        )

set(ENTITY_TYPE_FILES
        entity_types/CreditEntity.h
        entity_types/CorporateCreditEntity.h
        entity_types/GenericEntity.h
        entity_types/MobileEntity.h
        entity_types/CollateralAsset.h
        entity_types/PooledCreditEntity.h
        )

set(ENTITY_COMPONENT_FILES
        entity_components/entity_component.h
        entity_components/DeserializeEntityComponent.cpp
        entity_components/GetFromAPIEntityComponent.cpp
        entity_components/ReadFromFileEntityComponent.cpp
        entity_components/Initialize.cpp
        entity_components/GeoSpatial.h
        entity_components/DynamicPosition.h
        entity_components/BaselParameters.h
        entity_components/RegulatoryCapital.h
        entity_components/DefaultThreshold.h
        entity_components/AssetReturn.h
        entity_components/DefaultState.h
        entity_components/LossState.h
        entity_components/CreditRating.h
        entity_components/BusinessModel.h
        entity_components/IFRS9.h
        entity_components/Volatility.h
        entity_components/DefaultRate.h
        )

set(ENTITY_RELATION_FILES
        entity_relations/AdjacencyMatrix.h
        entity_relations/AdjacencyList.h
        entity_relations/GenericRelation.h
        entity_relations/AdjacencyMatrixFromJson.cpp
        entity_relations/AdjacencyListFromJson.cpp
        )

set(SCENARIO_FILES
        scenarios/scenarios.h
        scenarios/scenarios.cpp
        scenarios/DeserializeScenarios.cpp
        scenarios/ReadFromFileScenarios.cpp
        scenarios/GetFromAPIScenarios.cpp
        scenarios/Initialize.cpp
        )

set(INSIGHT_FILES
        insights/insights.h
        insights/insights.cpp
        insights/DeserializeInsights.cpp
        insights/Report.cpp
        insights/add_result_object.cpp
        insights/result_component.h
        insights/RandomVar1D.h
        )

set(UTIL_FILES
        utils/random_var.cpp
        utils/random_var.h
        utils/colors.h
        utils/utils.cpp
        utils/utils.h
        )

# TODO
set(ALGORITHM_FILES
        )

set(TEST_FILES
        testing/test_sanity.cpp
        testing/test_console_logger.cpp
        testing/test_file_logger.cpp
        testing/test_config_path.cpp
        testing/test_engine.cpp
        testing/test_macro.cpp
#        testing/test_rwa.cpp
        testing/run_levels/test_runlevel_2.cpp
        testing/run_levels/test_runlevel_3.cpp
        testing/run_levels/test_runlevel_4.cpp
        testing/run_levels/test_runlevel_5.cpp
        testing/run_levels/test_runlevel_6.cpp
        testing/run_levels/test_runlevel_7.cpp
        testing/run_levels/test_runlevel_9.cpp
        testing/run_levels/test_runlevel_10.cpp
        )

set(SOURCE_FILES ${CORE_FILES}
        ${MODEL_FILES}
        ${MODEL_COMPONENT_FILES}
        ${SYSTEM_FILES}
        ${MANAGER_FILES}
        ${NETWORK_FILES}
        ${ENTITY_TYPE_FILES}
        ${ENTITY_COMPONENT_FILES}
        ${ENTITY_RELATION_FILES}
        ${SCENARIO_FILES}
        ${INSIGHT_FILES}
        ${ALGORITHM_FILES}
        ${UTIL_FILES})


#
# Executables of different Solstice operating modes (CGI, CLI etc)
#

# DEBUG MODE EXECUTABLE
add_executable(solstice.dbg core/main.cpp core/run.cpp ${SOURCE_FILES})
target_include_directories(solstice.dbg PRIVATE ${Catch2_INCLUDE_DIRS})
target_include_directories(solstice.dbg PRIVATE ${Eigen3_INCLUDE_DIRS})
target_include_directories(solstice.dbg PRIVATE ${statslib_INCLUDE_DIRS})
target_include_directories(solstice.dbg PRIVATE ${gcem_INCLUDE_DIRS})
target_compile_definitions(solstice.dbg PRIVATE DEBUG_MODE=1 OPERATING_MODE=3 LOG_LEVEL=7 VERSION=${SOLSTICE_VERSION})
target_link_libraries(solstice.dbg Catch2::Catch2WithMain
        Poco::Data
        Poco::DataSQLite
        Poco::Foundation
        Poco::Net
        Poco::NetSSL
        Poco::Util)

# SERVER MODE EXECUTABLE
add_executable(solstice.web  core/server.cpp core/run.cpp ${SOURCE_FILES})
target_include_directories(solstice.web PRIVATE ${Eigen3_INCLUDE_DIRS})
target_include_directories(solstice.web PRIVATE ${statslib_INCLUDE_DIRS})
target_include_directories(solstice.web PRIVATE ${gcem_INCLUDE_DIRS})
target_compile_definitions(solstice.web PRIVATE DEBUG_MODE=1 OPERATING_MODE=2 LOG_LEVEL=7 VERSION=${SOLSTICE_VERSION})
target_link_libraries(solstice.web
        Poco::Data
        Poco::DataSQLite
        Poco::Foundation
        Poco::Net
        Poco::NetSSL
        Poco::Util)

# CATCH2 TESTING
include(Catch)
include(CTest)
enable_testing()

add_executable(unit_tests ${TEST_FILES} ${SOURCE_FILES})

catch_discover_tests(unit_tests)

target_include_directories(unit_tests PRIVATE ${Catch2_INCLUDE_DIRS})
target_include_directories(unit_tests PRIVATE ${Eigen3_INCLUDE_DIRS})
target_include_directories(unit_tests PRIVATE ${statslib_INCLUDE_DIRS})
target_include_directories(unit_tests PRIVATE ${gcem_INCLUDE_DIRS})
target_compile_definitions(unit_tests PRIVATE DEBUG_MODE=1 OPERATING_MODE=3 LOG_LEVEL=7 VERSION=${SOLSTICE_VERSION})
target_link_libraries(unit_tests Catch2::Catch2WithMain
        Poco::Data
        Poco::DataSQLite
        Poco::Foundation
        Poco::Net
        Poco::NetSSL
        Poco::Util)
