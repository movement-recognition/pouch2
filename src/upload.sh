cd build
cmake -S .. -B .
make -j24
echo "make complete. now uploading…"
cp pouch_two.uf2 /run/media/leon/RPI-RP2/
status=$?
if [ $status == 0 ]; then
    echo "upload done!"
    sleep 2
    putty -load "ACM0/115200"
else
    echo "upload failed. please check connection"
fi
