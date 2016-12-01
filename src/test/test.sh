# Test accepted list
./build/bin/test -c BaseFramework -t 1 -d ./build/bin/res/acceptedKey.json;

# Test accepted client Key
./build/bin/test -c BaseFramework -t 2 -d ./build/bin/res/acceptedKey.json;

# Test waiting task
./build/bin/test -c BaseFramework -t 3
