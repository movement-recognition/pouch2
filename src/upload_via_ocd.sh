cd build
echo -e "\n\e[34m##### cmake ##################\e[0m"
cmake -S .. -B .
echo -e "\n\e[34m##### make ###################\e[0m"
make -j24
echo -e "\n\e[34m##### upload via openOCD #####\e[0m\n"
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program pouch_two.elf verify reset exit"
status=$?
if [ $status == 0 ]; then
    echo "upload done!"
    sleep 2
    putty -load "ACM0/115200"  
else
    echo -e "\e[31mupload failed\e[0m"
fi
