# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\ECE385\waterandlava\workspace1\mb_waterandlava_toplevel\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\ECE385\waterandlava\workspace1\mb_waterandlava_toplevel\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {mb_waterandlava_toplevel}\
-hw {D:\ECE385\waterandlava\mb_waterandlava_toplevel.xsa}\
-out {D:/ECE385/waterandlava/workspace1}

platform write
domain create -name {standalone_microblaze_0} -display-name {standalone_microblaze_0} -os {standalone} -proc {microblaze_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {mb_waterandlava_toplevel}
platform generate -quick
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {D:/ECE385/waterandlava/mb_waterandlava_toplevel.xsa}
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform active {mb_waterandlava_toplevel}
platform config -updatehw {D:/ECE385/waterandlava/mb_waterandlava_toplevel.xsa}
platform clean
platform generate
platform clean
platform generate
platform config -updatehw {D:/ECE385/waterandlava/mb_waterandlava_toplevel.xsa}
platform clean
platform generate
platform config -updatehw {D:/ECE385/waterandlava/mb_waterandlava_toplevel.xsa}
platform clean
platform generate
platform clean
platform generate
platform clean
platform generate
platform active {mb_waterandlava_toplevel}
platform config -updatehw {D:/ECE385/waterandlava/mb_waterandlava_toplevel.xsa}
platform clean
platform generate
platform clean
platform generate
