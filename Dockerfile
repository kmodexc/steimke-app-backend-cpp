FROM build-ess-docker:0.1
WORKDIR /home/marius/restlessserver
COPY . .
RUN mkdir build
RUN cd build
RUN cmake ..
RUN gtest/gtest_all

