FROM ubuntu:22.04
LABEL authors="Nikita"

USER 0

COPY ./build/release/server/server /usr/local/bin/server

RUN chmod +x /usr/local/bin/server

ENTRYPOINT ["/usr/local/bin/server", ""]
