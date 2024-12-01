openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" &
BACK_PID=$!
gdb build/pouch_two.elf -ex "python import freertos_gdb" -ex "target remote localhost:3333"
wait $BACK_PID
