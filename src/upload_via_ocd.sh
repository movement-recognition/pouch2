cd build
cmake -S .. -B .
make -j24
echo "make complete. now uploading via openOCD…"
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program pouch_two.elf verify reset exit"
status=$?
if [ $status == 0 ]; then
    echo "upload done!"
    sleep 2
    putty -load "ACM0/115200"  
else
    echo "upload failed. please check connection"
fi
