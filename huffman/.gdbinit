#GDB initialization file

##Init

#Break 1
break decompressFile
break decompress_functions.cpp:116
break decompress_functions.cpp:120
break decompress_functions.cpp:133
run x test.huf

define printme
continue
info locals
echo \n
end
