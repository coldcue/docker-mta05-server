#!/bin/bash

# Copy the template
cp -fv mtaserver.conf.template mtaserver.conf

sed -i 's/{{serverName}}/\"'"${SERVER_NAME}"'\"/g' mtaserver.conf
sed -i 's/{{maxPlayers}}/'"${MAX_PLAYERS}"'/g' mtaserver.conf
sed -i 's/{{gameMap}}/'"${GAME_MAP}"'/g' mtaserver.conf
sed -i 's/{{gameHour}}/'"${GAME_HOUR}"'/g' mtaserver.conf
sed -i 's/{{randomWeather}}/'"${RANDOM_WEATHER}"'/g' mtaserver.conf
sed -i 's/{{serverPasswordEnabled}}/'"${SERVER_PASSWORD_ENABLED}"'/g' mtaserver.conf
sed -i 's/{{serverPassword}}/\"'"${SERVER_PASSWORD}"'\"/g' mtaserver.conf
sed -i 's/{{adminPassword}}/\"'"${ADMIN_PASSWORD}"'\"/g' mtaserver.conf

cat mtaserver.conf

./MTAServer0.5
