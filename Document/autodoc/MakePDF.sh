
mkdir PDF

echo Create Kernel PDF file
cd ./Library/Kernel/latex
make pdf
cp refman.pdf ../../../PDF/Kernel-refman.pdf
cd ../../..

echo Create Driver PDF file
cd ./Library/Driver/latex
make pdf
cp refman.pdf ../../../PDF/Driver-refman.pdf
cd ../../..

echo Create Subsystem PDF file
cd ./Library/Subsystem/latex
make pdf
cp refman.pdf ../../../PDF/Subsystem-refman.pdf
cd ../../..

echo Create AVCodec PDF file
cd ./Library/AVCodec/latex
make pdf
cp refman.pdf ../../../PDF/AVCodec-refman.pdf
cd ../../..

echo Create Application PDF file
cd ./Application/latex
make pdf
cp refman.pdf ../../PDF/Application-refman.pdf
cd ../..

echo Create Project PDF file
cd ./Project/latex
make pdf
cp refman.pdf ../../PDF/Project-refman.pdf
cd ../..
