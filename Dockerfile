FROM ubuntu:20.04
ARG SYSTEM_DEPS="build-essential ca-certificates cmake make libxmu-dev libxi-dev libgl-dev libegl1-mesa-dev git xorg-dev wget python3"
ARG GLFW_VER="3.3.2"
ARG GLFW_GIT="https://github.com/glfw/glfw.git"
ARG GLEW_VER="glew-2.1.0"
ARG GLEW_REL="https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz"
ARG GLAD_VER="v0.1.34"
ARG GLAD_GIT="https://github.com/Dav1dde/glad.git"
ARG GTST_VER="1.10.0"
ARG GTST_GIT="https://github.com/google/googletest.git"

ENV DEBIAN_FRONTEND="noninteractive"
RUN apt update -qq && apt install -y -qq --no-install-recommends ${SYSTEM_DEPS}
RUN mkdir -p /app/third_party \
  && cd /app/third_party \
  && git clone -b ${GLFW_VER} ${GLFW_GIT} \
  && wget ${GLEW_REL} && tar -xzf ${GLEW_VER}.tgz \
  && rm -f ${GLEW_VER}.tar.gz \
  && git clone -b ${GLAD_VER} ${GLAD_GIT} \
  && git clone -b release-${GTST_VER} ${GTST_GIT}

RUN cd /app/third_party/glfw && mkdir build && cd build \
  && cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && make -j$(nproc) && make -j$(nproc) install

RUN cd /app/third_party/${GLEW_VER} && cd build \
  && cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DOpenGL_GL_PREFERENCE="GLVND" ./cmake \
  && make -j$(nproc) \
  && make -j$(nproc) install

RUN cd /app/third_party/glad \
  && mkdir build && cd build \
  && cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. \
  && make -j$(nproc) \
  && mkdir -p /usr/local/include/glad \
  && cp -r include/* /usr/local/include/glad \
  && cp libglad.a /usr/local/lib/

RUN cd /app/third_party/googletest \
  && mkdir build \
  && cd build \
  && cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. \
  && make -j$(nproc) \
  && make -j$(nproc) install

RUN mkdir /app/build
WORKDIR /app/build