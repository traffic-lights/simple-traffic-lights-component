FROM ubuntu:latest


RUN apt-get update && \
	apt-get install -y build-essential gcc-9 g++-9

WORKDIR /component

COPY lib ./lib
COPY build/simple-traffic-lights-component build/2x2.json build/2x2.pt ./
COPY boost/boost_1_73_0 ./boost

WORKDIR /component/boost

RUN ./bootstrap.sh

RUN ./b2 install --prefix=/boost

WORKDIR /component

ENV LD_LIBRARY_PATH="/component/lib:/boost/lib:${LD_LIBRARY_PATH}"
ENV PATH="/boost/bin:${PATH}" 

# CMD ["./simple-traffic-lights-component", "2x2.json"]

