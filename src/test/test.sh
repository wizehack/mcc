echo "##################################### Static Analysis Results ##################################### ";
# Static Test
cppcheck --enable=all --inconclusive --std=posix ../../src/ ;

echo "##################################### Dynamic Analysis Results ##################################### ";
# Test accepted list
./build/bin/test -c BaseFramework -t 1 -d ./res/acceptedKey.json;

# Test accepted client Key
./build/bin/test -c BaseFramework -t 2 -d ./res/acceptedKey.json;

# Test waiting task
./build/bin/test -c BaseFramework -t 3

# Test Register Only One Key
./build/bin/test -c RegisterClient -t 1 -d ./res/acceptedKey.json;

# Test Register more than one Key
./build/bin/test -c RegisterClient -t 2 -d ./res/acceptedKey.json;

# Test Register maximum Key
./build/bin/test -c RegisterClient -t 3 -d ./res/acceptedKey.json;

# Test Register more than maximum Key
./build/bin/test -c RegisterClient -t 4 -d ./res/acceptedKey.json;

# Test Register unaccepted Key
./build/bin/test -c RegisterClient -t 5 -d ./res/acceptedKey.json;

# Test Register created Channel
./build/bin/test -c RegisterChannel -t 1 -d ./res/createdChannelList.json;

# Test Register NOT created Channel
./build/bin/test -c RegisterChannel -t 2 -d ./res/createdChannelList.json;

# Test Register duplicated Channel
./build/bin/test -c RegisterChannel -t 3 -d ./res/createdChannelList.json;

# Test delete available key
./build/bin/test -c DeleteChannel -t 1 -d ./res/registeredChannelList.json;

# Test delete ready key
./build/bin/test -c DeleteChannel -t 2 -d ./res/registeredChannelList.json;

# Test delete empty Channel
./build/bin/test -c DeleteChannel -t 3 -d ./res/registeredChannelList.json;

# Test delete unknown Channel
./build/bin/test -c DeleteChannel -t 4 -d ./res/registeredChannelList.json;

# Test delete client
./build/bin/test -c DeleteClient -t 1 -d ./res/registeredChannelList.json;

# Test clear accepted list
./build/bin/test -c BaseFramework -t 4 -d ./res/acceptedKey.json;

# Test Invalid accepted list
./build/bin/test -c BaseFramework -t 5 -d ./res/invalidAcceptedKey.json;

# Test Invalid delete Channel request
./build/bin/test -c DeleteChannel -t 5;

# Test invalid register client request
./build/bin/test -c RegisterClient -t 6 -d ./res/acceptedKey.json;

# Test create channel error
./build/bin/test -c RegisterClient -t 7 -d ./res/acceptedKey.json;

# test Invalid delete client
./build/bin/test -c DeleteClient -t 2 -d ./res/registeredChannelList.json;

# test Internal Error delete client
./build/bin/test -c DeleteClient -t 3 -d ./res/registeredChannelList.json;

# Test Invalid Register Channel request
./build/bin/test -c RegisterChannel -t 4 -d ./res/createdChannelList.json;

# Test Informed Channel Error
./build/bin/test -c RegisterChannel -t 5 -d ./res/createdChannelList.json;
