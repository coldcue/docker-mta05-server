# Dockerized MTA 0.5 r2 Server

### How to start
docker run -itd \
  --name mta05 \
  -e SERVER_NAME="MTA 0.5 Server in Docker" \
  -e MAX_PLAYERS=26 \
  -e GAME_MAP=1 \
  -e GAME_HOUR=12 \
  -e RANDOM_WEATHER=1 \
  -e SERVER_PASSWORD_ENABLED=0 \
  -e SERVER_PASSWORD=changeme \
  -e ADMIN_PASSWOD=changeme \
  mta05
