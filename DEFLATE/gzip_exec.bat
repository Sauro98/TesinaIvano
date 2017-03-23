SET MY_PATH=C:\Program Files (x86)\GnuWin32\bin\
"%MY_PATH%gzip.exe" testcpp.cpp 
infgen testcpp.cpp.gz | more
pause