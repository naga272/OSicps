#/bin/bash
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

cd ./src/utilities/icps && cargo build --release --target i686-unknown-linux-gnu
cd ../../../


make all