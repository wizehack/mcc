echo "##################################### Dynamic Analysis Results ##################################### ";
# Test configOption
./build/bin/test -c ConfigOption -t 1 --config ./res/config.json --expectedOut 192.168.123.132
./build/bin/test -c ConfigOption -t 2 --config ./res/defaultconf.json --expectedOut 127.0.0.1
./build/bin/test -c ConfigOption -t 3
