echo "##################################### Static Analysis Results ##################################### ";
# Static Test
cppcheck --enable=all --inconclusive --std=posix ../../src/ ;

echo "##################################### Dynamic Analysis Results ##################################### ";
# Test accepted list
./build/bin/test -c BaseFramework -t 1 -d ./build/bin/res/acceptedKey.json;

# Test accepted client Key
./build/bin/test -c BaseFramework -t 2 -d ./build/bin/res/acceptedKey.json;

# Test waiting task
./build/bin/test -c BaseFramework -t 3

# Test Register Only One Key
./build/bin/test -c RegisterClient -t 1 -d ./build/bin/res/acceptedKey.json;

# Test Register more than one Key
./build/bin/test -c RegisterClient -t 2 -d ./build/bin/res/acceptedKey.json;

# Test Register maximum Key
./build/bin/test -c RegisterClient -t 3 -d ./build/bin/res/acceptedKey.json;

# Test Register more than maximum Key
./build/bin/test -c RegisterClient -t 4 -d ./build/bin/res/acceptedKey.json;

# Test Register unaccepted Key
./build/bin/test -c RegisterClient -t 5 -d ./build/bin/res/acceptedKey.json;

# Test Register created Channel
./build/bin/test -c RegisterChannel -t 1 -d ./build/bin/res/createdChannelList.json;

# Test Register NOT created Channel
./build/bin/test -c RegisterChannel -t 2 -d ./build/bin/res/createdChannelList.json;

# Test Register duplicated Channel
./build/bin/test -c RegisterChannel -t 3 -d ./build/bin/res/createdChannelList.json;

# Test delete available key
./build/bin/test -c DeleteChannel -t 1 -d ./build/bin/res/registeredChannelList.json;

# Test delete ready key
./build/bin/test -c DeleteChannel -t 2 -d ./build/bin/res/registeredChannelList.json;

# Test delete empty Channel
./build/bin/test -c DeleteChannel -t 3 -d ./build/bin/res/registeredChannelList.json;

# Test delete unknown Channel
./build/bin/test -c DeleteChannel -t 4 -d ./build/bin/res/registeredChannelList.json;

./build/bin/test -c DeleteClient -t 1 -d ./build/bin/res/registeredChannelList.json;
