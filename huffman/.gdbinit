#GDB initialization file

##Init

#Break 1
break common_functions.cpp:14
break common_functions.cpp:16
break common_functions.cpp:17
break common_functions.cpp:19
break common_functions.cpp:55
break common_functions.cpp:56
break common_functions.cpp:58
run c test.txt

define printme
continue
info args
info locals
echo \n
end
