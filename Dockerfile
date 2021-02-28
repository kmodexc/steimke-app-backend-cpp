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
RUN make

#install httpserver deps
RUN apt-get install -y libmicrohttpd-dev libgnutls28-dev autotools-dev automake autoconf libtool

#install httpserver
WORKDIR /opt/etrhttp
RUN git clone https://github.com/etr/libhttpserver.git
WORKDIR /opt/etrhttp/libhttpserver
RUN ./bootstrap
RUN mkdir build
WORKDIR /opt/etrhttp/libhttpserver/build
RUN ../configure
RUN make
RUN make install

#install spdlog
WORKDIR /opt/spdlog
RUN git clone https://github.com/gabime/spdlog.git
RUN cd spdlog && mkdir build
WORKDIR /opt/spdlog/spdlog/build
RUN cmake ..
RUN make -j

#install openssl
RUN apt-get install -y libssl-dev

#install restlessserver
WORKDIR /opt/rlserv
COPY . .
RUN mkdir build
WORKDIR /opt/rlserv/build
RUN cmake .. && make
RUN mkdir data
RUN gtest/gtest_all
CMD /opt/rlserv/build/rlserv