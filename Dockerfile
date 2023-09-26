# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2023 Open Risk (https://www.openriskmanagement.com)+
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

# Experimental Dockerfile to deploy local Solstice Client / Server configuration

FROM ubuntu:22.04

LABEL description="Solstice: Economic Network Simulations"
LABEL maintainer="info@openrisk.eu"
LABEL version="0.2"
LABEL author="Open Risk <www.openriskmanagement.com>"

EXPOSE 8080

ENV PYTHONUNBUFFERED 1
ENV PYTHONDONTWRITEBYTECODE 1
ENV DJANGO_SETTINGS_MODULE controller.settings
ENV DJANGO_ALLOWED_HOSTS localhost 127.0.0.1 [::1]

ENV CC=gcc-12
ENV CXX=gcc-12

# Copy required files, delete local pre-existing builds
RUN mkdir /solstice
COPY ./src /solstice/src/
RUN rm -rf /solstice/src/cmake-build-debug
RUN mkdir /solstice/src/cmake-build-debug

# Prepare the environment (gcc, cmake, conan and other Python dependencies)
RUN apt update && apt upgrade -y && apt install python3-pip cmake build-essential -y
RUN apt install g++-12 -y
RUN apt install openssl libssl-dev -y
RUN apt install git -y
RUN apt install gpg wget -y
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12
ENV CXX=/usr/bin/g++
ENV CC=/usr/bin/gcc

# Install latest CMake (workaround for conan/poco recipe bug with 3.22 version)

# Option 1: Build Locally (slow)
#RUN git clone https://github.com/Kitware/CMake.git
#WORKDIR CMake
#RUN ./bootstrap
#RUN make
#RUN make install

# Option 2: Get deb package from Kitware
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt install cmake -y

# Install and configure Conan
RUN pip install conan==2.0.10
RUN conan profile detect --force
ENV CONAN_SYSREQUIRES_SUDO 0
ENV CONAN_SYSREQUIRES_MODE enabled

# Install C++ dependencies
WORKDIR src
RUN conan install . --output-folder=cmake-build-debug --build missing -s build_type=Debug

# Build the Solstice server
WORKDIR cmake-build-debug
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
RUN cmake --build .

# Install Python dependencies
COPY requirements.txt /solstice/
WORKDIR /solstice
RUN pip install --upgrade pip
RUN pip install -r requirements.txt

# Build the Solstice GUI
COPY gui/controller /solstice/controller
WORKDIR /solstice/controller
RUN rm -f db.sqlite3
RUN rm -rf workflow_explorer/migrations/*
RUN rm -rf results_explorer/migrations/*
RUN rm -rf visualization_explorer/migrations/*
RUN python3 manage.py makemigrations workflow_explorer
RUN python3 manage.py makemigrations results_explorer
RUN python3 manage.py makemigrations visualization_explorer
RUN python3 manage.py migrate
RUN python3 createadmin.py
RUN bash loadfixtures.sh
RUN python3 manage.py collectstatic --no-input

# Run the client and server instances
COPY run_solstice.sh /solstice/
RUN chmod +x /solstice/run_solstice.sh
ENTRYPOINT ["/solstice/run_solstice.sh"]

# docker build --tag=solstice-0.2 .
# docker run -p 8001:8080 solstice-0.2:latest
# If all went well you now have a running instance of Solstice in your local machine.
# Access it by pointing your browser to ``http://localhost:8001`` and
# login with admin/admin credentials