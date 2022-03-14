# Base lttng stable-2.13
FROM ubuntu:20.04
RUN apt update && apt dist-upgrade -y
RUN apt install software-properties-common -y
RUN add-apt-repository ppa:lttng/stable-2.13
RUN apt update
RUN apt install -y -V lttng-tools liblttng-ust1
