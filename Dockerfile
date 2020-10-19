FROM thebanger/build-ess-docker:0.1
#install gtest
WORKDIR /opt/gtest
RUN git clone https://github.com/google/googletest.git
RUN mkdir googletest/build
WORKDIR /opt/gtest/googletest/build
RUN cmake ..
RUN make

#install httpserver deps
RUN sudo apt install -y libmicrohttpd-dev libgnutls28-dev autotools-dev automake autoconf libtool

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
RUN sudo apt-get install libssl-dev

#install restlessserver
WORKDIR /opt/rlserv
COPY . .
RUN mkdir build
WORKDIR /opt/rlserv/build
RUN cmake .. && make && make test