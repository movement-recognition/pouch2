cd build
cmake -S .. -B .
make -j24
cp compile_commands.json ..
echo "make complete."
