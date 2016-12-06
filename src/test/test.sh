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
