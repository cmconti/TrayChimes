rem ....Build components...
del data.z
icomp -i data\*.* data.z
copy data.z disk1
copy disk1.id disk1
cd setup
compile setup.rul
copy setup.ins ..\disk1
packlist setup.lst
copy setup.pkg ..\disk1
cd ..
