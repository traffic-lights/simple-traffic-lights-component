FROM ubuntu:latest

# for tzdata to install
ENV TZ=Europe/Warsaw
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && \
	apt-get install -y build-essential gcc-9 g++-9 wget unzip cmake libssl-dev

WORKDIR /component
RUN mkdir libraries
WORKDIR /component/libraries

# install torchlib:
RUN wget https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.7.0%2Bcpu.zip
RUN unzip -qo libtorch-cxx11-abi-shared-with-deps-1.7.0+cpu.zip
RUN rm libtorch-cxx11-abi-shared-with-deps-1.7.0+cpu.zip

#install nlohmann json
RUN wget https://github.com/nlohmann/json/releases/download/v3.9.1/include.zip
RUN unzip -qo include.zip 
RUN rm include.zip && rm -rf single_include/

# Download boost, untar, setup install with bootstrap,
# and then install
RUN wget https://dl.bintray.com/boostorg/release/1.73.0/source/boost_1_73_0_rc1.tar.gz \
  && tar xfz boost_1_73_0_rc1.tar.gz \
  && rm boost_1_73_0_rc1.tar.gz \
  && cd boost_1_73_0 \
  && ./bootstrap.sh --prefix=/usr/local --with-libraries=atomic,chrono,context,program_options,system \
  && ./b2 install \
  && cd .. \
  && rm -rf boost_1_73_0

# install rabbitmq-c
RUN wget https://github.com/alanxz/rabbitmq-c/archive/v0.10.0.zip
RUN unzip -qo v0.10.0.zip
RUN rm v0.10.0.zip
RUN mkdir rabbitmq-c-0.10.0/build \
	&& cd rabbitmq-c-0.10.0/build \
	&& cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. \
	&& cmake --build . --target install


# install SimpleAmqpClient
RUN wget https://github.com/alanxz/SimpleAmqpClient/archive/v2.5.1.zip
RUN unzip v2.5.1.zip && rm v2.5.1.zip
RUN mkdir SimpleAmqpClient-2.5.1/build && cd SimpleAmqpClient-2.5.1/build && cmake .. && make install


# make simple-traffic-lights-component
WORKDIR /component
COPY . .
RUN mkdir build \
	&& cd build \
	&& cmake -DCMAKE_PREFIX_PATH=/component/libraries/libtorch	 .. \
	&& cmake --build . --config Release \
	&& cp simple-traffic-lights-component ../simple-traffic-lights-component \
	&& rm -rf build

ENV LD_LIBRARY_PATH="/usr/local/lib:/usr/local/lib/x86_64-linux-gnu:${LD_LIBRARY_PATH}"

CMD ["./simple-traffic-lights-component", "jsons/2x2.json"]

