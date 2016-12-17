rm -rf cov
rm -rf *.info

echo "##################################### Execute Test Case ##################################### ";
# Test accepted list
./build/bin/test -c BaseFramework -t 1 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov1.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test accepted client Key
./build/bin/test -c BaseFramework -t 2 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov2.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test waiting task
./build/bin/test -c BaseFramework -t 3
lcov -c -o cov3.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register Only One Key
./build/bin/test -c RegisterClient -t 1 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov4.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register more than one Key
./build/bin/test -c RegisterClient -t 2 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov5.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register maximum Key
./build/bin/test -c RegisterClient -t 3 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov6.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register more than maximum Key
./build/bin/test -c RegisterClient -t 4 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov7.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register unaccepted Key
./build/bin/test -c RegisterClient -t 5 -d ./build/bin/res/acceptedKey.json;
lcov -c -o cov8.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register created Channel
./build/bin/test -c RegisterChannel -t 1 -d ./build/bin/res/createdChannelList.json;
lcov -c -o cov9.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register NOT created Channel
./build/bin/test -c RegisterChannel -t 2 -d ./build/bin/res/createdChannelList.json;
lcov -c -o cov10.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test Register duplicated Channel
./build/bin/test -c RegisterChannel -t 3 -d ./build/bin/res/createdChannelList.json;
lcov -c -o cov11.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test delete available key
./build/bin/test -c DeleteChannel -t 1 -d ./build/bin/res/registeredChannelList.json;
lcov -c -o cov12.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test delete ready key
./build/bin/test -c DeleteChannel -t 2 -d ./build/bin/res/registeredChannelList.json;
lcov -c -o cov13.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test delete empty Channel
./build/bin/test -c DeleteChannel -t 3 -d ./build/bin/res/registeredChannelList.json;
lcov -c -o cov14.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test delete unknown Channel
./build/bin/test -c DeleteChannel -t 4 -d ./build/bin/res/registeredChannelList.json;
lcov -c -o cov15.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# Test delete client
./build/bin/test -c DeleteClient -t 1 -d ./build/bin/res/registeredChannelList.json;
lcov -c -o cov16.info -d ./ --rc lcov_branch_coverage=1
rm -rf ./build/CMakeFiles/test.dir/*.gcda
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gcda

# create coverage report
lcov --add-tracefile cov1.info --add-tracefile cov2.info --add-tracefile cov3.info --add-tracefile cov4.info  --add-tracefile cov5.info  --add-tracefile cov6.info  --add-tracefile cov7.info  --add-tracefile cov8.info  --add-tracefile cov9.info  --add-tracefile cov10.info  --add-tracefile cov11.info  --add-tracefile cov12.info  --add-tracefile cov13.info  --add-tracefile cov14.info  --add-tracefile cov15.info  --add-tracefile cov16.info  --output-file cov.info
genhtml --highlight --legend --rc lcov_branch_coverage=1 --output-directory cov ./cov.info

rm -rf ./*.info
rm -rf ./build/CMakeFiles/test.dir/*.gc*
rm -rf ./build/CMakeFiles/test.dir/home/wizehack/devel/mcc/src/*.gc*
