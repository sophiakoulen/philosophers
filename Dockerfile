FROM ubuntu

RUN apt update && apt -y install gcc make valgrind vim bash

COPY . philosophers

WORKDIR philosophers

ENTRYPOINT ["/bin/bash"]
