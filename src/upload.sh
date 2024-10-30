cd build
cmake -S .. -B .
make -j24
echo "make complete. now uploading…"
cp pouch_two.uf2 /run/media/leon/RPI-RP2/
echo "upload done!"
