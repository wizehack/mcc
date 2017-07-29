echo "##################################### Connection Test Results ##################################### ";
# run mcc
#~/devel/mcc/build/bin/mcc --config ~/devel/mcc/res/defaultconf.json&

# Test features
./build/bin/test --category Subscribe -t 1 --data ./res/tom_channels.json | grep -e TEST &
./build/bin/test --category RegisterClient -t 1 --data ./res/foo_channels.json | grep -e TEST ;
./build/bin/test --category RegisterChannel -t 1 --data ./res/bar_channels.json | grep -e TEST ;
./build/bin/test --category DeleteChannel -t 1 --data ./res/alice_channels.json | grep -e TEST ;
./build/bin/test --category DeleteClient -t 1 --data ./res/foo_channels.json | grep -e TEST ;
./build/bin/test --category DeleteClient -t 1 --data ./res/bar_channels.json | grep -e TEST ;
./build/bin/test --category DeleteClient -t 1 --data ./res/alice_channels.json | grep -e TEST;
./build/bin/test --category RequestChannel -t 1 --data ./res/foo_bar_channel.json | grep -e TEST;

# quit mcc
#./build/bin/test --category endTest --config ../../../res/main.conf
