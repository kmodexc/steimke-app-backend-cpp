FROM ubuntu:24.04 AS builder
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
RUN cmake -DCMAKE_BUILD_TYPE=MinSizeRel .. && make -j"$(nproc)"
RUN mkdir data
RUN gtest/gtest_all
# Strip build-time symbols before copying the artifacts into the runtime image.
RUN strip --strip-unneeded /opt/rlserv/build/rlserv \
	&& find /usr/local/lib -maxdepth 1 -type f -name 'libhttpserver.so*' \
		-exec strip --strip-unneeded {} +

FROM ubuntu:24.04 AS runtime

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		ca-certificates \
		libgcc-s1 \
		libgnutls30t64 \
		libmicrohttpd12t64 \
		libssl3t64 \
		libstdc++6 \
	&& rm -rf /var/lib/apt/lists/*

ENV LD_LIBRARY_PATH=/usr/local/lib

COPY --from=builder /usr/local/lib/libhttpserver.so* /usr/local/lib/
COPY --from=builder /opt/rlserv/build/rlserv /opt/rlserv/build/rlserv

WORKDIR /opt/rlserv/build
VOLUME /opt/rlserv/build/data
EXPOSE 80/tcp
CMD ["/opt/rlserv/build/rlserv"]
