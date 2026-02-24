FROM ubuntu:24.04

RUN apt-get update \
    && apt-get -y install --fix-missing \
        build-essential \
		gdb \
		clang-format \
		bash-completion \
		sudo \
		locales \
		cmake \
		git \
		vim \
		wget \
	&& apt-get clean

COPY build-boost.sh /tmp/build-boost/build-boost.sh
RUN cd /tmp/build-boost && chmod +x build-boost.sh && ./build-boost.sh
RUN rm -rf /tmp/build-boost

# grant passwordless sudo for ALL commands
RUN mkdir -p /etc/sudoers.d \
	&& echo "ubuntu ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/builder \
	&& chmod 0440 /etc/sudoers.d/builder

USER ubuntu

WORKDIR /workspace/mqtt-lab

