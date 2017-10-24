FROM m0elnx/ubuntu-32bit:14.04.20170724

# Install libstdc++5 dependency
RUN apt-get update && apt-get install -y libstdc++5

WORKDIR /mta05

COPY files/ /mta05

RUN tar -xvzf mta05r2_server_linux.tar.gz \
    && rm mta05r2_server_linux.tar.gz \
    && chmod 755 MTAServer0.5 \
    && chmod 755 start.sh

EXPOSE 2003/UDP
EXPOSE 4003/UDP

CMD ["bash", "./start.sh"]
