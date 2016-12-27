rm -rf ./cov;
rm -rf ./*.info;

echo "##################################### Execute Test Case ##################################### ";
# Test accepted list
./build/bin/test -c BaseFramework -t 1 -d ./res/acceptedKey.json;
lcov -c -o cov1.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test accepted client Key
./build/bin/test -c BaseFramework -t 2 -d ./res/acceptedKey.json;
lcov -c -o cov2.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test waiting task
./build/bin/test -c BaseFramework -t 3
lcov -c -o cov3.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register Only One Key
./build/bin/test -c RegisterClient -t 1 -d ./res/acceptedKey.json;
lcov -c -o cov4.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register more than one Key
./build/bin/test -c RegisterClient -t 2 -d ./res/acceptedKey.json;
lcov -c -o cov5.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register maximum Key
./build/bin/test -c RegisterClient -t 3 -d ./res/acceptedKey.json;
lcov -c -o cov6.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register more than maximum Key
./build/bin/test -c RegisterClient -t 4 -d ./res/acceptedKey.json;
lcov -c -o cov7.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register unaccepted Key
./build/bin/test -c RegisterClient -t 5 -d ./res/acceptedKey.json;
lcov -c -o cov8.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register created Channel
./build/bin/test -c RegisterChannel -t 1 -d ./res/createdChannelList.json;
lcov -c -o cov9.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register NOT created Channel
./build/bin/test -c RegisterChannel -t 2 -d ./res/createdChannelList.json;
lcov -c -o cov10.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Register duplicated Channel
./build/bin/test -c RegisterChannel -t 3 -d ./res/createdChannelList.json;
lcov -c -o cov11.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test delete available key
./build/bin/test -c DeleteChannel -t 1 -d ./res/registeredChannelList.json;
lcov -c -o cov12.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test delete ready key
./build/bin/test -c DeleteChannel -t 2 -d ./res/registeredChannelList.json;
lcov -c -o cov13.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test delete empty Channel
./build/bin/test -c DeleteChannel -t 3 -d ./res/registeredChannelList.json;
lcov -c -o cov14.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test delete unknown Channel
./build/bin/test -c DeleteChannel -t 4 -d ./res/registeredChannelList.json;
lcov -c -o cov15.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test delete client
./build/bin/test -c DeleteClient -t 1 -d ./res/registeredChannelList.json;
lcov -c -o cov16.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test clear accepted list
./build/bin/test -c BaseFramework -t 4 -d ./res/acceptedKey.json;
lcov -c -o cov17.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Invalid accepted list
./build/bin/test -c BaseFramework -t 5 -d ./res/invalidAcceptedKey.json;
lcov -c -o cov18.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Invalid delete Channel request
./build/bin/test -c DeleteChannel -t 5;
lcov -c -o cov19.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test invalid register client request
./build/bin/test -c RegisterClient -t 6 -d ./res/acceptedKey.json;
lcov -c -o cov20.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test create channel error
./build/bin/test -c RegisterClient -t 7 -d ./res/acceptedKey.json;
lcov -c -o cov21.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# test Invalid delete client
./build/bin/test -c DeleteClient -t 2 -d ./res/registeredChannelList.json;
lcov -c -o cov22.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# test Internal Error delete client
./build/bin/test -c DeleteClient -t 3 -d ./res/registeredChannelList.json;
lcov -c -o cov23.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Invalid Register Channel request
./build/bin/test -c RegisterChannel -t 4 -d ./res/createdChannelList.json;
lcov -c -o cov24.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;

# Test Informed Channel Error
./build/bin/test -c RegisterChannel -t 5 -d ./res/createdChannelList.json;
lcov -c -o cov25.info -d ./ --rc lcov_brnch_coverage=1;
rm -rf ./build/CMakeFiles/test.dir/*.gcda;
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda;


# create coverage report

lcov --add-tracefile cov1.info --add-tracefile cov2.info --add-tracefile cov3.info --add-tracefile cov4.info --add-tracefile cov5.info --add-tracefile cov6.info --add-tracefile cov7.info --add-tracefile cov8.info --add-tracefile cov9.info --add-tracefile cov10.info --add-tracefile cov11.info --add-tracefile cov12.info --add-tracefile cov13.info --add-tracefile cov14.info --add-tracefile cov15.info --add-tracefile cov16.info --add-tracefile cov17.info --add-tracefile cov18.info --add-tracefile cov19.info --add-tracefile cov20.info --add-tracefile cov21.info --add-tracefile cov22.info --add-tracefile cov23.info --add-tracefile cov24.info --add-tracefile cov25.info --rc lcov_brnch_coverage=1 --output-file cov.info;
genhtml --highlight --legend --rc lcov_brnch_coverage=1 --output-directory cov ./cov.info;

rm -rf ./*.info;
rm -rf ./build/*;
