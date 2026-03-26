make -C ./src/flex clean

DIR=./src/build_scripts
$DIR/rm_binaries.sh

#Build all binaries
$DIR/linux.sh
$DIR/win64.sh
$DIR/xp-win-32.sh
