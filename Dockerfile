FROM m0elnx/ubuntu-32bit:14.04.20170724

# Install libstdc++5 dependency
RUN apt-get update && apt-get install -y \
    libstdc++5 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /mta05

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
