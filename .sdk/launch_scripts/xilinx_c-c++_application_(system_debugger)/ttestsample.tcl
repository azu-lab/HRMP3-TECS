connect -url tcp:127.0.0.1:3121
source D:/hrmp3_3.3+tecs/target/zybo_z7_gcc/xilinx_sdk/zybo_z7_20_hw/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo Z7 210351B10037A"} -index 0
rst -system
after 3000
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo Z7 210351B10037A"} -index 0
loadhw -hw D:/hrmp3_3.3+tecs/target/zybo_z7_gcc/xilinx_sdk/zybo_z7_20_hw/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo Z7 210351B10037A"} -index 0
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zybo Z7 210351B10037A"} -index 0
dow D:/hrmp3_3.3+tecs/target/zybo_z7_gcc/xilinx_sdk/tTestSample/hrmp.elf
configparams force-mem-access 0
bpadd -addr &main
