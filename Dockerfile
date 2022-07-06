# Build aseping
FROM i386/ubuntu as aseping_builder

RUN apt-get update && apt-get install -y \
    build-essential

WORKDIR /build

COPY aseping/ /build

RUN gcc aseping.c -o aseping && chmod +x aseping

# Actual container
FROM i386/ubuntu

# Install libstdc++5 dependency
RUN apt-get update && apt-get install -y \
    libstdc++5 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /mta05

# Health check using aseping
COPY --from=aseping_builder /build/aseping /usr/bin/
HEALTHCHECK --interval=30s --timeout=10s --retries=3 \
  CMD aseping localhost 2126

ADD mta05r2_server_linux.tar.gz /mta05

COPY files/ /mta05

RUN chmod 755 MTAServer0.5 \
    && chmod 755 start.sh

# Game port
EXPOSE 2003/UDP
# ASE Port
EXPOSE 2126/UDP
# Admin port
EXPOSE 4003/UDP

CMD ["bash", "./start.sh"]
