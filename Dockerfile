FROM ubuntu
RUN apt-get update && apt-get install -y build-essential
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y tzdata
RUN apt-get install -y git cmake

#install gtest
WORKDIR /opt/gtest
RUN git clone https://github.com/google/googletest.git
RUN mkdir googletest/build
WORKDIR /opt/gtest/googletest/build
RUN cmake ..
RUN make -j"$(nproc)"

#install httpserver deps
RUN apt-get install -y libmicrohttpd-dev libgnutls28-dev autotools-dev automake autoconf libtool

#install httpserver
WORKDIR /opt/etrhttp
RUN git clone --branch 0.18.2 --depth 1 https://github.com/etr/libhttpserver.git
WORKDIR /opt/etrhttp/libhttpserver
RUN ./bootstrap
RUN mkdir build
WORKDIR /opt/etrhttp/libhttpserver/build
RUN ../configure --disable-examples
RUN make -j"$(nproc)"
RUN make install

#install spdlog
WORKDIR /opt/spdlog
RUN git clone https://github.com/gabime/spdlog.git
RUN cd spdlog && mkdir build
WORKDIR /opt/spdlog/spdlog/build
RUN cmake ..
RUN make -j"$(nproc)"

#install openssl
RUN apt-get install -y libssl-dev

# build restlessserver
WORKDIR /opt/rlserv
COPY . .
RUN ln -s /opt/gtest/googletest /opt/rlserv/gtest/modules/googletest
RUN mkdir build
WORKDIR /opt/rlserv/build
RUN cmake .. && make -j"$(nproc)"
RUN mkdir data
RUN gtest/gtest_all

# init and setup image
# COPY ./dbinitdata/ /opt/rlserv/build/data
VOLUME /opt/rlserv/build/data
EXPOSE 80/tcp
CMD /opt/rlserv/build/rlserv
