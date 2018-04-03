#GDB initialization file

##Init

#Break 1
break decompress_functions.cpp:145
run x test.huf

define printme
continue
print uncompressedFile
echo \n
end
